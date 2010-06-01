

void do_traced_yield_stack(opcode args){

	llvm::Module* mod = ls_->module();
	const llvm::Type* cf_type = mod->getTypeByName("struct.rubinius::CallFrame");
	const llvm::Type* stack_vars_type = mod->getTypeByName("struct.rubinius::StackVariables");


	// Emit setup code for new call.
	// Stores into args Values.
	setup_out_args(args);

	CompiledMethod* cm = cur_trace_node_->send_cm;
	VMMethod* vmm = cm->backend_method();
	jit::Context ctx(ls_);
	JITMethodInfo* new_info = new JITMethodInfo(ctx, cm, vmm);
	JITMethodInfo* parent_info = info();
	new_info->is_block = false;
	new_info->set_parent_info(parent_info);
	method_info_ = new_info;


	Value* block_env = b().CreateLoad(
		b().CreateConstGEP2_32(parent_info->variables(), 0, offset::vars_block),
		"env");

	BasicBlock* cur = current_block();

	vm_ = parent_info->vm();
	vm_->setName("vm");
	info()->set_vm(vm_);

	info()->set_stack(parent_info->stack());

	Value* prev_call_frame = parent_info->call_frame();
	prev_call_frame->setName("prev_call_frame");
	info()->set_previous(prev_call_frame);

	info()->set_args(out_args_);
	info()->set_out_args(info()->root_info()->pre_allocated_args[cur_trace_node_->trace_pc]);
	init_out_args();

	Value* cfstk = info()->root_info()->pre_allocated_stacks[cur_trace_node_->trace_pc];

	Value* var_mem = info()->root_info()->pre_allocated_vars[cur_trace_node_->trace_pc];

	check_arity();

	call_frame_ = b().CreateBitCast(
		cfstk,
		PointerType::getUnqual(cf_type), "call_frame");

	info()->set_call_frame(call_frame_);

	// Value* stk = b().CreateConstGEP1_32(cfstk, sizeof(CallFrame) / sizeof(Object*), "stack");
	// info()->set_stack(stk);

	vars_ = b().CreateBitCast(
		var_mem,
		PointerType::getUnqual(stack_vars_type), "vars");
	info()->set_variables(vars_);

	// Pasting code from initialize_frame

	// previous
	b().CreateStore(info()->previous(), get_field(call_frame_, offset::cf_previous));

	// arguments
	b().CreateStore(info()->args(), get_field(call_frame_, offset::cf_arguments));


	ip_pos_ = b().CreateConstGEP2_32(call_frame_, 0, offset::cf_ip, "ip_pos");



	// ip
	b().CreateStore(
		ConstantInt::get(ls_->Int32Ty, 0),
		get_field(call_frame_, offset::cf_ip));

	// scope
	b().CreateStore(vars_, get_field(call_frame_, offset::cf_scope));

	import_args();

	stack_remove(args);

}
