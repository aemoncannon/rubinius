
void emit_traced_send(opcode which, opcode args, bool with_block){

	const llvm::Type* obj_type = ls_->ptr_type("Object");

	if(with_block){
		// copy things from caller stack into args
		setup_out_args_with_block(args);
		// pop things off caller stack
		stack_remove(args + 2);
	}
	else{
		// copy things from caller stack into args
		setup_out_args(args);
		// pop things off caller stack
		stack_remove(args + 1);
	}

	CompiledMethod* cm = cur_trace_node_->send_cm;
	VMMethod* vmm = cm->backend_method();
	jit::Context& ctx = info()->context();
	JITMethodInfo* parent_info = info();
	parent_info->set_saved_sp(sp_);
	parent_info->set_saved_last_sp(last_sp_);
	JITMethodInfo* new_info = new JITMethodInfo(ctx, cm, vmm);
	new_info->is_block = false;
	new_info->set_parent_info(parent_info);
	method_info_ = new_info;
	sp_ = -1;
	last_sp_ = -1;

	new_info->set_traced_block_supplied(with_block);

	llvm::Module* mod = ls_->module();
	const llvm::Type* cf_type = mod->getTypeByName("struct.rubinius::CallFrame");
	const llvm::Type* stack_vars_type = mod->getTypeByName("struct.rubinius::StackVariables");

	InlineCache* cache = reinterpret_cast<InlineCache*>(which);
	Value* msg = b().CreateIntToPtr(
		ConstantInt::get(ls_->IntPtrTy, reinterpret_cast<uintptr_t>(cache)),
		ls_->ptr_type("Dispatch"), "cast_to_ptr");

	info()->set_msg(msg);

	vm_ = parent_info->vm();
	vm_->setName("vm");
	info()->set_vm(vm_);


	Value* prev_call_frame = parent_info->call_frame();
	prev_call_frame->setName("prev_call_frame");
	info()->set_previous(prev_call_frame);

	args_ = parent_info->out_args();
	info()->set_args(args_);

	out_args_ = info()->root_info()->pre_allocated_args[cur_trace_node_->trace_pc];
	out_args_ = b().CreateBitCast(out_args_, ls_->ptr_type("Arguments"), "casted out_args");
	info()->set_out_args(out_args_);
	init_out_args();

	Value* cfstk = info()->root_info()->pre_allocated_call_frames[
		cur_trace_node_->trace_pc];
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

	// Pasting code from initialize_frame
	Value* exec = b().CreateLoad(get_field(info()->msg(), 2), "msg.exec");
	Value* cm_gep = get_field(call_frame_, offset::cf_cm);
	Value* method = b().CreateBitCast(
		exec, cast<PointerType>(cm_gep->getType())->getElementType(), "cm");


	// previous
	b().CreateStore(info()->previous(), get_field(call_frame_, offset::cf_previous));

	// arguments
	b().CreateStore(info()->args(), get_field(call_frame_, offset::cf_arguments));

	// msg
	b().CreateStore(
		b().CreatePointerCast(info()->msg(), ls_->Int8PtrTy),
		get_field(call_frame_, offset::cf_msg));

	// cm
	b().CreateStore(method, cm_gep);

	// flags
	int flags = CallFrame::cJITed;
	flags |= CallFrame::cTracedFrame;
	if(!use_full_scope_) flags |= CallFrame::cClosedScope;

	b().CreateStore(
		ConstantInt::get(ls_->Int32Ty, flags),
		get_field(call_frame_, offset::cf_flags));


	// scope
	b().CreateStore(vars_, get_field(call_frame_, offset::cf_scope));

  nil_stack(info()->vmm->stack_size, constant(Qnil, obj_type));

	import_args();

	// b().CreateBr(body);
	// b().SetInsertPoint(body);
}


void import_args() {
	Value* arg_obj = info()->args();

	setup_scope();

	// Import the arguments
	Value* offset = b().CreateConstGEP2_32(arg_obj, 0, offset::args_ary, "arg_ary_pos");
	Value* arg_ary = b().CreateLoad(offset, "arg_ary");

	// As opposed to method jit, number of args is always known statically.
	// Currently, splats invalidate traces, so don't worry about those.
	for(int i = 0; i < cur_trace_node_->send_arg_count(); i++) {
		Value* int_pos = ConstantInt::get(ls_->Int32Ty, i);

		Value* arg_val_offset = b().CreateConstGEP1_32(arg_ary, i, "arg_val_offset");

		Value* arg_val = b().CreateLoad(arg_val_offset, "arg_val");

		Value* idx2[] = {
			ConstantInt::get(ls_->Int32Ty, 0),
			ConstantInt::get(ls_->Int32Ty, offset::vars_tuple),
			int_pos
		};

		Value* pos = b().CreateGEP(info()->variables(), idx2, idx2+3, "var_pos");

		b().CreateStore(arg_val, pos);

	}

}


void setup_scope() {

	llvm::Module* mod = ls_->module();
	const llvm::Type* vars_type = mod->getTypeByName("struct.rubinius::VariableScope");
	const llvm::Type* obj_type = ls_->ptr_type("Object");
	Value* heap_null = ConstantExpr::getNullValue(PointerType::getUnqual(vars_type));
	Value* heap_pos = get_field(info()->variables(), offset::vars_on_heap);

	b().CreateStore(heap_null, heap_pos);

	Value* self = b().CreateLoad(get_field(info()->args(), offset::args_recv),
															 "args.recv");
	b().CreateStore(self, get_field(info()->variables(), offset::vars_self));

	Value* module = b().CreateLoad(get_field(info()->msg(), offset::msg_module),
																 "msg.module");

	b().CreateStore(module, get_field(info()->variables(), offset::vars_module));

	Value* blk = b().CreateLoad(get_field(info()->args(), offset::args_block),
															"args.block");
	b().CreateStore(blk, get_field(info()->variables(), offset::vars_block));

	b().CreateStore(Constant::getNullValue(ls_->ptr_type("VariableScope")),
									get_field(info()->variables(), offset::vars_parent));
	b().CreateStore(constant(Qnil, obj_type), get_field(info()->variables(), offset::vars_last_match));

	nil_locals();
}

void nil_stack(int size, Value* nil) {

	if(size == 0) return;
	// Stack size 5 or less, do 5 stores in a row rather than
	// the loop.
	if(size <= 5) {
		for(int i = 0; i < size; i++) {
			b().CreateStore(nil, b().CreateConstGEP1_32(info()->stack(), i, "stack_pos"));
		}
		return;
	}

	Value* max = ConstantInt::get(ls_->Int32Ty, size);
	Value* one = ConstantInt::get(ls_->Int32Ty, 1);

	BasicBlock* top = BasicBlock::Create(ls_->ctx(), "stack_nil", info()->function());
	BasicBlock* cont = BasicBlock::Create(ls_->ctx(), "bottom", info()->function());

	b().CreateStore(ConstantInt::get(ls_->Int32Ty, 0), info()->counter());

	b().CreateBr(top);

	b().SetInsertPoint(top);

	Value* cur = b().CreateLoad(info()->counter(), "counter");
	b().CreateStore(nil, b().CreateGEP(info()->stack(), cur, "stack_pos"));

	Value* added = b().CreateAdd(cur, one, "added");
	b().CreateStore(added, info()->counter());

	Value* cmp = b().CreateICmpEQ(added, max, "loop_check");
	b().CreateCondBr(cmp, cont, top);

	b().SetInsertPoint(cont);
}


void nil_locals() {
	const llvm::Type* obj_type = ls_->ptr_type("Object");
	Value* nil = constant(Qnil, obj_type);
	int size = info()->vmm->number_of_locals;


	if(size == 0) return;
	// Stack size 5 or less, do 5 stores in a row rather than
	// the loop.
	if(size <= 5) {
		for(int i = 0; i < size; i++) {
			Value* idx[] = {
				ConstantInt::get(ls_->Int32Ty, 0),
				ConstantInt::get(ls_->Int32Ty, offset::vars_tuple),
				ConstantInt::get(ls_->Int32Ty, i)
			};

			Value* gep = b().CreateGEP(info()->variables(), idx, idx+3, "local_pos");
			b().CreateStore(nil, gep);
		}
		return;
	}


	Value* max = ConstantInt::get(ls_->Int32Ty, size);
	Value* one = ConstantInt::get(ls_->Int32Ty, 1);

	BasicBlock* top = BasicBlock::Create(ls_->ctx(), "locals_nil", info()->function());
	BasicBlock* cont = BasicBlock::Create(ls_->ctx(), "bottom", info()->function());

	b().CreateStore(ConstantInt::get(ls_->Int32Ty, 0), info()->counter());

	b().CreateBr(top);

	b().SetInsertPoint(top);


	Value* cur = b().CreateLoad(info()->counter(), "counter");
	Value* idx[] = {
		ConstantInt::get(ls_->Int32Ty, 0),
		ConstantInt::get(ls_->Int32Ty, offset::vars_tuple),
		cur
	};

	Value* gep = b().CreateGEP(info()->variables(), idx, idx+3, "local_pos");
	b().CreateStore(nil, gep);


	Value* added = b().CreateAdd(cur, one, "added");
	b().CreateStore(added, info()->counter());

	Value* cmp = b().CreateICmpEQ(added, max, "loop_check");
	b().CreateCondBr(cmp, cont, top);


	b().SetInsertPoint(cont);
}


