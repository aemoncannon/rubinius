void emit_traced_yield_stack(opcode args) {

	if(cur_trace_node_->op == InstructionSequence::insn_yield_splat){
		setup_out_args(args);
		stack_remove(args + 1);
	}
	else{
		setup_out_args(args);
		stack_remove(args);
	}
	
	CompiledMethod* cm = cur_trace_node_->target_cm.get();
	assert(cm);
	VMMethod* vmm = cm->backend_method();
	jit::Context& ctx = info()->context();
	JITMethodInfo* parent_info = info();
	parent_info->set_saved_sp(sp_);
	parent_info->set_saved_last_sp(last_sp_);

	JITMethodInfo* new_info = new JITMethodInfo(ctx, cm, vmm);
	new_info->is_block = true;
	new_info->set_parent_info(parent_info);
	method_info_ = new_info;
	sp_ = -1;
	last_sp_ = -1;

	llvm::Module* mod = ls_->module();
	const llvm::Type* cf_type = mod->getTypeByName("struct.rubinius::CallFrame");
	const llvm::Type* stack_vars_type = mod->getTypeByName("struct.rubinius::StackVariables");

	vm_ = parent_info->vm();
	vm_->setName("vm");
	info()->set_vm(vm_);

	Value* block_env = b().CreateLoad(
		b().CreateConstGEP2_32(parent_info->variables(), 0, offset::vars_block),
		"env");
	block_env = b().CreateBitCast(block_env,
																ls_->ptr_type("BlockEnvironment"),
																"block_env");

	info()->set_block_env(block_env);

	Value* prev_call_frame = parent_info->call_frame();
	prev_call_frame->setName("prev_call_frame");
	info()->set_previous(prev_call_frame);

	args_ = parent_info->out_args();
	info()->set_args(args_);

	out_args_ = info()->root_info()->pre_allocated_args[cur_trace_node_->trace_pc];
	out_args_ = b().CreateBitCast(out_args_, ls_->ptr_type("Arguments"), "casted out_args");
	info()->set_out_args(out_args_);
	init_out_args();

	Value* cfstk = info()->root_info()->pre_allocated_call_frames[cur_trace_node_->trace_pc];
	call_frame_ = b().CreateBitCast(
		cfstk,
		PointerType::getUnqual(cf_type), "call_frame");
	info()->set_call_frame(call_frame_);

	stack_ = b().CreateConstGEP1_32(
 		cfstk, sizeof(CallFrame) / sizeof(Object*), "stack");
	info()->set_stack(stack_);

	Value* unwinds = info()->root_info()->pre_allocated_unwinds[
		cur_trace_node_->trace_pc];
	info()->set_unwinds(unwinds);
	b().CreateStore(unwinds, get_field(call_frame_, offset::cf_unwinds));
	b().CreateStore(int32(0), get_field(call_frame_, offset::cf_current_unwind));

	Value* var_mem = info()->root_info()->pre_allocated_vars[cur_trace_node_->trace_pc];
	vars_ = b().CreateBitCast(
		var_mem,
		PointerType::getUnqual(stack_vars_type), "vars");
	info()->set_variables(vars_);

	initialize_yield_frame(info()->vmm->stack_size);

	setup_yield_scope();

	const llvm::Type* obj_type = ls_->ptr_type("Object");
  nil_stack(info()->vmm->stack_size, constant(Qnil, obj_type));
}



void setup_yield_scope() {

	llvm::Module* mod = ls_->module();
	const llvm::Type* vars_type = mod->getTypeByName("struct.rubinius::VariableScope");
	const llvm::Type* obj_type = ls_->ptr_type("Object");

	b().CreateStore(ConstantExpr::getNullValue(PointerType::getUnqual(vars_type)),
									get_field(info()->variables(), offset::vars_on_heap));

	Value* self = b().CreateLoad(get_field(info()->parent_info()->variables(), 
																				 offset::vars_self),
															 "invocation.self");

	b().CreateStore(self, get_field(info()->variables(), offset::vars_self));

	Value* inv_mod = b().CreateLoad(get_field(info()->parent_info()->variables(),
																						offset::vars_module),
																	"invocation.module");

	Value* creation_mod = b().CreateLoad(
		get_field(info()->block_env(), offset::blockenv_module),
		"env.module");

	Value* module = b().CreateSelect(
		b().CreateICmpNE(inv_mod, ConstantExpr::getNullValue(inv_mod->getType())),
		inv_mod,
		creation_mod);

	b().CreateStore(module, get_field(info()->variables(), offset::vars_module));

	Value* blk = b().CreateLoad(get_field(info()->top_scope(), offset::varscope_block),
															"args.block");
	b().CreateStore(blk, get_field(info()->variables(), offset::vars_block));


	// We don't use top_scope here because of nested blocks. Parent MUST be
	// the scope the block was created in, not the top scope for depth
	// variables to work.
	Value* be_scope = b().CreateLoad(
		get_field(info()->block_env(), offset::blockenv_scope),
		"env.scope");

	b().CreateStore(be_scope, get_field(info()->variables(), offset::vars_parent));
	b().CreateStore(constant(Qnil, obj_type), get_field(info()->variables(), offset::vars_last_match));

	nil_locals();
}


void initialize_yield_frame(int stack_size) {

	Value* method = b().CreateLoad(get_field(info()->block_env(), offset::blockenv_method),
																 "env.method");

	// static_scope
	Value* ss = b().CreateLoad(get_field(method, offset::cm_static_scope), 
														 "invocation.static_scope");

	// previous
	b().CreateStore(info()->previous(), get_field(info()->call_frame(), offset::cf_previous));

	b().CreateStore(ss, get_field(info()->call_frame(), offset::cf_static_scope));

	// arguments
	b().CreateStore(info()->args(), get_field(info()->call_frame(), offset::cf_arguments));

	// msg
	b().CreateStore(Constant::getNullValue(ls_->Int8PtrTy),
									get_field(info()->call_frame(), offset::cf_msg));


	// cm
	Value* cm_gep = get_field(info()->call_frame(), offset::cf_cm);
	b().CreateStore(method, cm_gep);

	// flags
	Value* inv_flags = ConstantInt::get(ls_->Int32Ty, 0);
	//b().CreateLoad(get_field(block_inv, offset::blockinv_flags),
  //																		"invocation.flags");

	int block_flags = CallFrame::cCustomStaticScope |
		CallFrame::cMultipleScopes |
		CallFrame::cBlock |
		CallFrame::cTracedFrame;


	if(!use_full_scope_) block_flags |= CallFrame::cClosedScope;

	Value* flags = b().CreateOr(inv_flags,
															ConstantInt::get(ls_->Int32Ty, block_flags), "flags");
	b().CreateStore(flags, get_field(info()->call_frame(), offset::cf_flags));



	// scope
	b().CreateStore(info()->variables(), get_field(info()->call_frame(), offset::cf_scope));

	// top_scope
	Value* top_scope = b().CreateLoad(
		get_field(info()->block_env(), offset::blockenv_top_scope),
		"env.top_scope");
	info()->set_top_scope(top_scope);

	b().CreateStore(info()->top_scope(), get_field(info()->call_frame(), offset::cf_top_scope));


}











