#include "instructions_util.hpp"

#include "builtin/symbol.hpp"
#include "builtin/tuple.hpp"
#include "builtin/global_cache_entry.hpp"
#include "inline_cache.hpp"
#include "utilities.hpp"

#include "llvm/jit_visit.hpp"
#include "trace.hpp"

namespace rubinius {

  class JITTraceVisit : public JITVisit {

    Trace* trace_;
    TraceNode* cur_trace_node_;
    bool emitted_exit_;

  public:

    JITTraceVisit(LLVMState* ls, Trace* trace, JITMethodInfo* info, BlockMap& bm, llvm::BasicBlock* start)
      : JITVisit(ls, info, bm, start)
      , trace_(trace)
    {
      sp_ = trace->entry_sp;
      last_sp_ = trace->entry_sp;
      emitted_exit_ = false;

      vm_ = info->vm();
      call_frame_ = info->call_frame();
      vars_ = info->variables();
      stack_ = info->stack();
      args_ = info->args();
      out_args_ = info->out_args();
    }


    void at_trace_node(TraceNode* node){
      cur_trace_node_ = node;
    }


    void at_ip(int ip) {
      BlockMap::iterator i = block_map_.find(ip);

      if(i != block_map_.end()) {
        JITBasicBlock& jbb = i->second;
        current_jbb_ = &jbb;
      }

      remember_sp();

      current_ip_ = ip;
    }


    void initialize() {

      info()->init_return_pad();

      BasicBlock* start = current_block();
      set_block(start);

      TRACK_TIME_ON_TRACE(ON_TRACE_TIMER);

      bail_out_ = new_block("bail_out");

      Value* call_args[] = {
        vm_,
        call_frame_
      };

      set_block(bail_out_);

      Value* isit = f.return_to_here.call(call_args, 2, "rth", b());

      BasicBlock* ret_raise_val = new_block("ret_raise_val");
      bail_out_fast_ = new_block("ret_null");

      start->moveAfter(bail_out_fast_);

      b().CreateCondBr(isit, ret_raise_val, bail_out_fast_);

      set_block(bail_out_fast_);
      if(use_full_scope_) {
        flush_scope_to_heap(vars_);
      }

      dump_int(666);
      return_value(int32(-1));

      set_block(ret_raise_val);
      //      Value* crv = f.clear_raise_value.call(&vm_, 1, "crv", b());
      if(use_full_scope_) flush_scope_to_heap(vars_);

      dump_int(6667);
      return_value(int32(-1));

      set_block(start);

      ip_pos_ = b().CreateConstGEP2_32(call_frame_, 0, offset::cf_ip, "ip_pos");

      global_serial_pos = b().CreateIntToPtr(
				ConstantInt::get(ls_->IntPtrTy, (intptr_t)ls_->shared().global_serial_address()),
				PointerType::getUnqual(ls_->IntPtrTy), "cast_to_intptr");

      init_out_args();

    }

    virtual void check_for_exception_then(Value* val, BasicBlock* cont) {
      Value* null = Constant::getNullValue(ObjType);
      Value* null_p = b().CreateICmpEQ(val, null, "null_check");

      BasicBlock* current = current_block();
      BasicBlock* exit_stub = new_block("exit_stub");

			b().CreateCondBr(null_p, exit_stub, cont);

      set_block(exit_stub);
      exit_trace_on_exception(cur_trace_node_->pc);
      set_block(current);
    }


		void guard_class_change(Value* object, Class* klass){
      BasicBlock* exit_stub = new_block("exit_stub");

			check_class(object, klass, exit_stub);
			BasicBlock* current = current_block();

      set_block(exit_stub);
      exit_trace_at_fork(cur_trace_node_->pc);

      set_block(current);
		}

		void guard_block_change(Value* object, CompiledMethod* block_cm){
      BasicBlock* exit_stub = new_block("exit_stub");
			Value* block_env = b().CreateBitCast(object, ls_->ptr_type("BlockEnvironment"),"block_env");
			Value* cm = load_field(block_env, offset::blockenv_method, "env.method");
			Value* existing_cm = constant(block_cm, ls_->ptr_type("CompiledMethod"));
      Value* cmp = b().CreateICmpEQ(cm, existing_cm, "same block");
			verify_guard(cmp, exit_stub);
			BasicBlock* current = current_block();

      set_block(exit_stub);
      exit_trace_at_fork(cur_trace_node_->pc);

      set_block(current);
		}


    void ensure_trace_exit_pad(){
      if(!emitted_exit_){
				BasicBlock* cur = current_block();
				info()->root_info()->init_trace_exit_pad();
				set_block(info()->trace_exit_pad());
				emit_trace_exit_pad();
				emitted_exit_ = true;
				set_block(cur);
      }
    }

    void emit_trace_exit_pad() {

      // Emit code for entering the uncommon interpreter.
      // This is used in exit stubs, when the trace bails for some reason.
      // Inputs to the exit pad
      Value* exit_trace_node = info()->root_info()->trace_node_phi;
      Value* exit_pc = info()->root_info()->exit_ip_phi;
      Value* exit_sp = info()->root_info()->exit_sp_phi;
      Value* exit_cf = info()->root_info()->exit_cf_phi;

      // Do fast checks to see if we are a nested trace that is
      // finishing, or if we are otherwise exiting in a polite way,
      // and should simply return to caller.

      Value* recording = b().CreateICmpEQ(info()->trace_run_mode(), int32(2), "is recording");
      Value* not_recording = b().CreateNot(recording, "not_recording");
      Value* nested = b().CreateICmpEQ(info()->trace_run_mode(), int32(1), "is nested");
      Value* not_nested = b().CreateNot(recording, "not_nested");
      Value* entry_call_frame = info()->root_info()->call_frame();
      Value* trace = info()->trace();
      Value* expected_exit_ip = load_field(trace, offset::trace_expected_exit_ip, "expected exit ip");

      Value* ip_cmp = b().CreateICmpEQ(exit_pc, expected_exit_ip, "exiting_at_expected_ip_p");
      Value* cf_cmp = b().CreateICmpEQ(entry_call_frame, exit_cf, "at_expected_call_frame_p");

      BasicBlock* cont = new_block("continue");
      BasicBlock* exit = new_block("exit");

      // If we are recording (which makes this trace a nested trace candidate by definition), 
      // and the current call_frame is this trace's home call_frame,
      // return 1 directly to the caller of the trace. Result will
      // be that this trace will be recorded as a nested trace.
      Value* anded = b().CreateAnd(recording, cf_cmp, "and");
      b().CreateCondBr(anded, exit, cont);
      set_block(exit);
			flush_call_frame(exit_cf, exit_pc, exit_sp);
      return_value(int32(Trace::RETURN_NESTED_OK));
      set_block(cont);

      // If we are not recording, and this was a nested trace, and the current call_frame is 
      // this trace's home call_frame, and the ip we are exiting to is the ip that the calling trace was expecting,
      // exit directly to the caller of the trace -  (this informs caller
      // that the trace exited politely, so parent trace doesn't have to pop itself)
      cont = new_block("continue");
      exit = new_block("exit");
      anded = b().CreateAnd(not_recording, cf_cmp, "and");
      anded = b().CreateAnd(anded, nested, "and");
      anded = b().CreateAnd(anded, ip_cmp, "and");
      b().CreateCondBr(anded, exit, cont);
      set_block(exit);
			flush_call_frame(exit_cf, exit_pc, exit_sp);
      return_value(int32(Trace::RETURN_NESTED_OK));
      set_block(cont);

      // If we are not recording, and this trace is not being run as a nested trace, and  current call_frame 
      // is this trace's home call_frame, and we exited on the expected ip, return 0 to the caller of the trace
      cont = new_block("continue");
      exit = new_block("exit");
      anded = b().CreateAnd(not_recording, cf_cmp, "and");
      anded = b().CreateAnd(anded, not_nested, "and");
      anded = b().CreateAnd(anded, ip_cmp, "and");
      b().CreateCondBr(anded, exit, cont);
      set_block(exit);
			flush_call_frame(exit_cf, exit_pc, exit_sp);
      return_value(int32(Trace::RETURN_OK));
      set_block(cont);

      std::vector<const Type*> types;
      types.push_back(VMTy);
      types.push_back(CallFrameTy);
      types.push_back(ls_->ptr_type("Trace"));
      types.push_back(ls_->ptr_type("TraceNode"));
      types.push_back(ls_->Int32Ty);

      FunctionType* ft = FunctionType::get(ls_->Int32Ty, types, false);
      Function* func = cast<Function>(module_->getOrInsertFunction("rbx_side_exit", ft));
      Value* bail_call_args[] = {
				info()->vm(),
				exit_cf,
				info()->trace(),
				exit_trace_node,
				info()->trace_run_mode()
      };
      return_value(b().CreateCall(func, bail_call_args, bail_call_args + 5, "call side_exit"));
    }

    void return_value(Value* ret) {
      info()->add_return_value(ret, current_block());
      b().CreateBr(info()->return_pad());
    }

    void skip_to_anchor() {
      BasicBlock* bb = block_map_[trace_->anchor->trace_pc].block;
      assert(bb);
      b().CreateBr(bb);
      set_block(new_block("continue"));
    }

    void exit_trace_at_fork(int next_ip){
			DEBUGLN("Emitting exit from " << cur_trace_node_->pc << " to " << next_ip);
      TRACK_TIME_ON_TRACE(IN_EXIT_TIMER);
      ensure_trace_exit_pad();

      Value* exit_trace_node = constant(cur_trace_node_, ls_->ptr_type("TraceNode"));
			Value* exit_pc = int32(next_ip);
      Value* exit_sp = int32(cur_trace_node_->sp);
      Value* exit_cf = info()->call_frame();

			Value* branches_array = get_field(exit_trace_node, offset::trace_node_branches);
			Value* branch_trace_pos = b().CreateConstGEP2_32(branches_array, 0, 0, "branch_trace_pos");
			Value* branch_trace = b().CreateLoad(branch_trace_pos, "branch_trace");

      Value* branch_found_p = b().CreateICmpNE(branch_trace, constant(0, ls_->ptr_type("Trace")), "bailed_p");
      BasicBlock* no_branch_b = new_block("no_branch_b");
      BasicBlock* run_branch_b = new_block("run_branch_b");

      b().CreateCondBr(branch_found_p, run_branch_b, no_branch_b);

      set_block(run_branch_b);
			call_branch_trace(branch_trace, exit_trace_node, exit_cf);

      set_block(no_branch_b);
      info()->root_info()->exit_ip_phi->addIncoming(exit_pc, current_block());
      info()->root_info()->exit_sp_phi->addIncoming(exit_sp, current_block());
      info()->root_info()->trace_node_phi->addIncoming(exit_trace_node,current_block());
      info()->root_info()->exit_cf_phi->addIncoming(exit_cf, current_block());
      b().CreateBr(info()->trace_exit_pad());
    }

    void exit_trace_on_exception(int next_ip){
			DEBUGLN("Emitting exit from " << cur_trace_node_->pc << " to " << next_ip);
      TRACK_TIME_ON_TRACE(IN_EXIT_TIMER);
      ensure_trace_exit_pad();

      Value* exit_trace_node = constant(cur_trace_node_, ls_->ptr_type("TraceNode"));
			Value* exit_pc = int32(next_ip);
      Value* exit_sp = int32(cur_trace_node_->sp);
      Value* exit_cf = info()->call_frame();

      info()->root_info()->exit_ip_phi->addIncoming(exit_pc, current_block());
      info()->root_info()->exit_sp_phi->addIncoming(exit_sp, current_block());
      info()->root_info()->trace_node_phi->addIncoming(exit_trace_node,current_block());
      info()->root_info()->exit_cf_phi->addIncoming(exit_cf, current_block());
      b().CreateBr(info()->trace_exit_pad());
    }

		void call_branch_trace(Value* branch_trace, Value* exit_trace_node, Value* exit_cf){

      Value* executor = load_field(branch_trace, 
																	 offset::trace_executor, 
																	 "executor");

      Value* call_args[] = {
				info()->vm(),
				exit_cf,
				branch_trace,
				info()->trace(),
				exit_trace_node,
				int32(Trace::RUN_MODE_NORM)
      };
      Value* ret = b().CreateCall(executor, call_args, call_args + 6, "call_branch_trace");

      // Did the branch-trace bail? Parent trace should collapse, too.
      Value* bailed_p = b().CreateICmpEQ(ret, int32(Trace::RETURN_SIDE_EXITED), 
																				 "bailed_p");

      BasicBlock* cont = new_block("continue");
      BasicBlock* collapse_b = new_block("collapse_b");
      b().CreateCondBr(bailed_p, collapse_b, cont);

      set_block(collapse_b);
      return_value(ret);
      set_block(cont);

      if(!(trace_->is_branch())){
				// If we got to here, that means that a branch of this trace was
				// run successfully, which by definition must have looped
				// back to the anchor.
				TRACK_TIME_ON_TRACE(ON_TRACE_TIMER);
				skip_to_anchor();
      }
      else{
				// If we got to here, that means that a branch trace called
				// from a branch trace was run successfully. 
				// We return immediately, which hands control back up to the 
				// parent trace, which will (per above) jump to the anchor
				TRACK_TIME_ON_TRACE(ON_TRACE_TIMER);
				return_value(int32(Trace::RETURN_OK));
      }

		}

    void flush_call_frame(Value* cf, Value* pc, Value* sp){
			store_field(cf, offset::cf_ip, pc);
			store_field(cf, offset::cf_sp, sp);
    }


    void visit_nested_trace() {
      TRACK_TIME_ON_TRACE(IN_EXIT_TIMER);

      // Otherwise look up a branch for this location (this needs to be faster)
      Value* exit_trace_node = constant(cur_trace_node_, ls_->ptr_type("TraceNode"));
      Value* nested_trace = load_field(exit_trace_node,
																			 offset::trace_node_nested_trace, 
																			 "nested executor");
      Value* executor = load_field(exit_trace_node, 
																	 offset::trace_node_nested_executor, 
																	 "nested executor");

      Value* call_args[] = {
				info()->vm(),
				info()->call_frame(),
				nested_trace,
				info()->trace(),
				exit_trace_node,
				int32(Trace::RUN_MODE_NESTED)
      };
      Value* ret = b().CreateCall(executor, call_args, call_args+6, "call_nested_trace");

      TRACK_TIME_ON_TRACE(IN_EXIT_TIMER);

      Value* not_nestable = b().CreateICmpEQ(ret, int32(Trace::RETURN_SIDE_EXITED), 
																						 "nestable_p");

      BasicBlock* cont = new_block("continue");
      BasicBlock* not_nestable_b = new_block("not_nestable");
      b().CreateCondBr(not_nestable, not_nestable_b, cont);

      //  The nested trace must have bailed into the uncommon interpreter and has 
      //  already popped its home call_frame.  This trace is no longer relevant.
      set_block(not_nestable_b);
      return_value(ret);

      // The nested trace exited on the expected_ip, on its home callframe.
      // Continue on our merry way.
      set_block(cont);
    }

    void visit_check_interrupts() {
      std::vector<const Type*> types;

      types.push_back(VMTy);
      types.push_back(CallFrameTy);

      FunctionType* ft = FunctionType::get(ObjType, types, false);
      Function* func = cast<Function>(
				module_->getOrInsertFunction("rbx_check_interrupts", ft));

      func->setDoesNotCapture(0, true);
      func->setDoesNotCapture(1, true);
      func->setDoesNotCapture(2, true);

      flush();

      Value* call_args[] = {
        vm_,
        call_frame_
      };

      Value* ret = b().CreateCall(func, call_args, call_args+2, "ci");
      check_for_exception(ret);
    }  


    void visit_goto(opcode ip) {

      // Omit unconditional jump if all it does is hop 
      // to the next trace node. (unless the next node is
      // the anchor, in which case we do want to jump because
      // the anchor is emitted first, not last.)
      if(cur_trace_node_->next->trace_pc == (int)ip &&
				 cur_trace_node_->next != trace_->anchor){
				return;
      }

      // Omit unconditional jump if this is a branch trace
      // and we're at the last node before the terminator.
      if(trace_->is_branch() && cur_trace_node_->next == trace_->anchor){
				return;
      }

      BasicBlock* bb = block_map_[ip].block;
      assert(bb);
      b().CreateBr(bb);
      set_block(new_block("continue"));
    }


    void visit_end() {
      if(trace_->is_branch()){
				return_value(int32(Trace::RETURN_OK));
      }
    }

    void visit_goto_if_false(opcode ip) {
			emit_goto_if(ip, false);
		}

    void visit_goto_if_true(opcode ip) {
			emit_goto_if(ip, true);
		}
   
    void emit_goto_if(opcode ip, bool affirmative) {
      Value* cond = stack_pop();
      Value* i = b().CreatePtrToInt(
				cond, ls_->IntPtrTy, "as_int");
      Value* anded = b().CreateAnd(
				i,
				ConstantInt::get(ls_->IntPtrTy, FALSE_MASK), 
				"and");

      Value* cmp;

			if(affirmative){
				cmp = b().CreateICmpNE(
					anded,
					ConstantInt::get(ls_->IntPtrTy, cFalse), "is_true");
			}
			else{
				cmp = b().CreateICmpEQ(
					anded,
					ConstantInt::get(ls_->IntPtrTy, cFalse), "is_true");
			}

      BasicBlock* cont = new_block("continue");
      BasicBlock* exit_stub = new_block("exit_stub");

      int exit_to_pc;
      if(cur_trace_node_->jump_taken){
				b().CreateCondBr(cmp, cont, exit_stub);
				exit_to_pc = cur_trace_node_->pc + 1 + cur_trace_node_->numargs;
      }
      else{
				b().CreateCondBr(cmp, exit_stub, cont);
				exit_to_pc = cur_trace_node_->interp_jump_target();
      }
      set_block(exit_stub);
      exit_trace_at_fork(exit_to_pc);
      set_block(cont);
    }

    void visit_set_local(opcode which) {
      Value* idx2[] = {
        ConstantInt::get(ls_->Int32Ty, 0),
        ConstantInt::get(ls_->Int32Ty, offset::vars_tuple),
        ConstantInt::get(ls_->Int32Ty, which)
      };
      Value* pos = b().CreateGEP(vars_, idx2, idx2+3, "local_pos");
      Value* val = stack_top();
      b().CreateStore(val, pos);
    }

    void visit_setup_unwind(opcode where, opcode type){
			Value* cur_unwind = load_field(
				info()->call_frame(), offset::cf_current_unwind, "current_unwind");
			Value* next_unwind = b().CreateAdd(cur_unwind, int32(1), "add_one");
			store_field(info()->call_frame(), offset::cf_current_unwind, next_unwind);

			Value* unwind_array = b().CreateConstGEP1_32(info()->unwinds(), 0, "unwind_array");
			Value* unwind = b().CreateGEP(unwind_array, next_unwind, "local_pos");
			store_field(unwind, offset::unwind_info_target_ip, int32(where));
			store_field(unwind, offset::unwind_info_stack_depth, int32(cur_trace_node_->sp));
			store_field(unwind, offset::unwind_info_type, int32(type));
    }

    void visit_pop_unwind() {
			Value* cur_unwind = load_field(
				info()->call_frame(), offset::cf_current_unwind, "current_unwind");
			Value* next_unwind = b().CreateSub(cur_unwind, int32(1), "subtract_one");
			store_field(info()->call_frame(), offset::cf_current_unwind, next_unwind);
		}

    void emit_traced_return(){

      Value* ret_val = stack_top();

      // parent_info exists if this call_frame
      // was activated on this trace
      if(info()->parent_info()){
				method_info_ = info()->parent_info();
				vm_ = info()->vm();
				args_ = info()->args();
				call_frame_ = info()->call_frame();
				vars_ = info()->variables();
				stack_ = info()->stack();
				sp_ = info()->saved_sp();
				last_sp_ = info()->saved_last_sp();
				out_args_ = info()->out_args();
      }
      else{
				// Otherwise, we're returning from
				// a call_frame that wasn't created on
				// this trace. We need to grab all the
				// info by other means.

				vm_ = info()->vm();

				// Hmmmm, shouln't do this unless there's def. a block
				Value* prev_block_env_pos = get_field(info()->variables(), offset::vars_block);
				Value* block_env = b().CreateLoad(prev_block_env_pos, "previous_block_env");
				block_env = b().CreateBitCast(block_env, ls_->ptr_type("BlockEnvironment"),"block_env");

				call_frame_ = load_field(info()->call_frame(), 
																 offset::cf_previous, "previous_cf");
				vars_ = load_field(call_frame_, offset::cf_scope, "vars");
				args_ = load_field(call_frame_, offset::cf_arguments, "args");
				Value* stk_base = get_field(call_frame_, offset::cf_stk);
				stack_ = b().CreateBitCast(stk_base, ObjArrayTy, "obj_ary_type");

				Value* unwinds = load_field(info()->call_frame(), 
																		offset::cf_unwinds, "unwinds");

				out_args_ = b().CreateAlloca(ls_->type("Arguments"), 0, "out_args");

				JITMethodInfo* prev_info = info();

				// Trace should never end on a return..
				assert(cur_trace_node_->next); 

				CompiledMethod* cm = cur_trace_node_->next->cm.get();
				VMMethod* vmm = cm->backend_method();
				method_info_ = new JITMethodInfo(info()->context(), cm, vmm);
				info()->is_block = false; // <---- this will be wrong if returning to a block...
				info()->set_block_env(block_env);
				info()->init_globals(prev_info);
				info()->context().set_root(info()->root_info());
				info()->set_call_frame(call_frame_);
				info()->set_variables(vars_);
				info()->set_stack(stack_);
				info()->set_vm(vm_);
				info()->set_args(args_);
				info()->set_out_args(out_args_);
				info()->set_unwinds(unwinds);

				assert(cur_trace_node_->next);
				// Leave room for return value..
				sp_ = cur_trace_node_->next->sp - 1; 

				last_sp_ = sp_;
      }

      stack_push(ret_val);

      // Info has changed, setup out_args stuff again.
      init_out_args();
    }

    void flush_ip() {
      flush_ip(current_ip_);
    }

    void flush_ip(int ip) {
      Value* call_frame = info()->call_frame();
      Value* pos = b().CreateConstGEP2_32(call_frame, 0, offset::cf_ip, "ip_pos");
      b().CreateStore(ConstantInt::get(ls_->Int32Ty, ip), pos);
    }

    void visit_push_has_block() {
      // We're in a traced method, so we know if there is a block staticly.
      if(info()->traced_block_supplied()) {
				stack_push(constant(Qtrue));
      }
      else{
				stack_push(constant(Qfalse));
      }
    }

    void visit_push_self() {
      stack_push(get_self());
    }

    void visit_send_method(opcode which) {
      visit_send_stack(which, 0);
    }

#include "vm/llvm/jit_trace_send.hpp"
    void visit_send_stack(opcode which, opcode args) {
      if(cur_trace_node_->traced_send){
				guard_class_change(stack_back(args), cur_trace_node_->target_klass.get());
				emit_traced_send(which, args);
      }
      else{
				InlineCache* cache = reinterpret_cast<InlineCache*>(which);
				set_has_side_effects();
				Value* ret = inline_cache_send(args, cache);
				stack_remove(args + 1);
				check_for_exception(ret);
				stack_push(ret);
				allow_private_ = false;
      }
    }


    void emit_create_block(opcode which) {
      // if we're inside an inlined method that has a block
      // visible, that means that we've note yet emited the code to
      // actually create the block for this inlined block.
      //
      // But, because we're about to create a block here, it might
      // want to yield (ie, inlining Enumerable#find on an Array, but
      // not inlining the call to each inside find).
      //
      // So at this point, we have to create the block object
      // for this parent block.

      //emit_delayed_create_block();

      std::vector<const Type*> types;
      types.push_back(VMTy);
      types.push_back(CallFrameTy);
      types.push_back(ls_->Int32Ty);

      FunctionType* ft = FunctionType::get(ObjType, types, false);
      Function* func = cast<Function>(
				module_->getOrInsertFunction("rbx_create_block", ft));

      Value* call_args[] = {
				vm_,
				call_frame_,
				ConstantInt::get(ls_->Int32Ty, which)
      };

      stack_set_top(b().CreateCall(func, call_args, call_args+3, "create_block"));
    }

    void visit_send_stack_with_block(opcode which, opcode args) {

      InlineCache* cache = reinterpret_cast<InlineCache*>(which);
      bool has_literal_block = (current_block_ >= 0);
      bool block_on_stack = !has_literal_block;

      if(cur_trace_node_->traced_send){
				if(!block_on_stack) {
					emit_create_block(current_block_);
				}
				emit_traced_send(which, args);
      }
      else{
				set_has_side_effects();

				// Detect a literal block being created and passed here.
				if(!block_on_stack) {
					emit_create_block(current_block_);
				}

				Value* ret = block_send(cache, args, allow_private_);
				stack_remove(args + 2);
				check_for_return(ret);
				allow_private_ = false;

				// Clear the current block
				current_block_ = -1;
      }
    }


#include "vm/llvm/jit_trace_yield.hpp"
    void visit_yield_stack(opcode count) {

			set_has_side_effects();
			Value* vars = vars_;
			if(JITMethodInfo* home = info()->home_info()) {
				vars = home->variables();
			}

			Value* block_obj = b().CreateLoad(
				b().CreateConstGEP2_32(vars, 0, offset::vars_block),
				"block");

      if(cur_trace_node_->traced_yield){
				CompiledMethod* cm = cur_trace_node_->target_cm.get();
				assert(cm);
				guard_block_change(block_obj, cm);
				emit_traced_yield(count);
      }
      else{
				Signature sig(ls_, ObjType);
				sig << VMTy;
				sig << CallFrameTy;
				sig << "Object";
				sig << ls_->Int32Ty;
				sig << ObjArrayTy;
				Value* call_args[] = {
					vm_,
					call_frame_,
					block_obj,
					ConstantInt::get(ls_->Int32Ty, count),
					stack_objects(count)
				};
				flush_ip();
				Value* val = sig.call("rbx_yield_stack", call_args, 5, "ys", b());
				stack_remove(count);
				check_for_exception(val);
				stack_push(val);
      }
    }


    void visit_yield_splat(opcode count) {
			set_has_side_effects();
			Value* vars = vars_;
			if(JITMethodInfo* home = info()->home_info()) {
				vars = home->variables();
			}

			Value* block_obj = b().CreateLoad(
				b().CreateConstGEP2_32(vars, 0, offset::vars_block),
				"block");

      if(cur_trace_node_->traced_yield){
				CompiledMethod* cm = cur_trace_node_->target_cm.get();
				assert(cm);
				guard_block_change(block_obj, cm);
				emit_traced_yield(count);
      }
      else{
				Signature sig(ls_, ObjType);
				sig << VMTy;
				sig << CallFrameTy;
				sig << "Object";
				sig << ls_->Int32Ty;
				sig << ObjArrayTy;
				Value* call_args[] = {
					vm_,
					call_frame_,
					block_obj,
					ConstantInt::get(ls_->Int32Ty, count),
					stack_objects(count + 1)
				};
				flush_ip();
				Value* val = sig.call("rbx_yield_splat", call_args, 5, "ys", b());
				stack_remove(count + 1);
				check_for_exception(val);
				stack_push(val);
      }
    }

    void visit_ret() {
      if(use_full_scope_) flush_scope_to_heap(info()->variables());
      emit_traced_return();
    }

    void visit_push_local(opcode which) {
      Value* idx2[] = {
				ConstantInt::get(ls_->Int32Ty, 0),
				ConstantInt::get(ls_->Int32Ty, offset::vars_tuple),
				ConstantInt::get(ls_->Int32Ty, which)
      };
      Value* pos = b().CreateGEP(vars_, idx2, idx2+3, "local_pos");
      stack_push(b().CreateLoad(pos, "local"));
    }

    void print_debug(){
      Signature sig(ls_, ls_->VoidTy);
      Value* call_args[] = {};
      sig.call("rbx_print_debug", call_args, 0, "", b());
    }

    void dump_vm_state(){
      Signature sig(ls_, ls_->VoidTy);
      sig << "VM";
      sig << "CallFrame";

      Value* call_args[] = {
				vm_,
				call_frame_
      };
      sig.call("rbx_show_state", call_args, 2, "", b());
    }

    void dump_obj(Value* val){
      Signature sig(ls_, ls_->VoidTy);
      sig << "VM";
      sig << "Object";

      Value* call_args[] = {
				vm_,
				val
      };
      sig.call("rbx_show_obj", call_args, 2, "", b());
    }

    void dump_vars(Value* val){
      Signature sig(ls_, ls_->VoidTy);
      sig << "VM";
      sig << "CallFrame";
      sig << "StackVariables";

      Value* call_args[] = {
				vm_,
				call_frame_,
				val
      };

      sig.call("rbx_show_vars", call_args, 3, "", b());
    }

    void track_time(int timer){
      Signature sig(ls_, ls_->VoidTy);
      sig << "VM";
      sig << ls_->Int32Ty;
      Value* call_args[] = {
				vm_,
				int32(timer)
      };
      sig.call("rbx_track_time", call_args, 2, "", b());
    }

    void dump_local(opcode which){
      Value* idx2[] = {
				ConstantInt::get(ls_->Int32Ty, 0),
				ConstantInt::get(ls_->Int32Ty, offset::vars_tuple),
				ConstantInt::get(ls_->Int32Ty, which)
      };
      Value* pos = b().CreateGEP(vars_, idx2, idx2+3, "local_pos");
      dump_obj(b().CreateLoad(pos, "local"));
    }

    void dump_int(int i){

      Value* val = ConstantInt::get(ls_->Int32Ty, i);

      Signature sig(ls_, ls_->VoidTy);
      sig << ls_->Int32Ty;

      Value* call_args[] = {
				val
      };
      sig.call("rbx_show_int", call_args, 1, "", b());
    }

    void dump_int1(Value* val){
      Signature sig(ls_, ls_->VoidTy);
      sig << ls_->Int1Ty;
      Value* call_args[] = {
				val
      };
      sig.call("rbx_show_int", call_args, 1, "", b());
    }

    void dump_int32(Value* val){
      Signature sig(ls_, ls_->VoidTy);
      sig << ls_->Int32Ty;
      Value* call_args[] = {
				val
      };
      sig.call("rbx_show_int", call_args, 1, "", b());
    }



    // void dump_str(char* str){

    // 	vector<const Type*> arg_types;
    // 	arg_types.push_back(PointerType::get(Type::SByteTy));
    // 	Function* f = module_.getOrInsertFunction(
    // 		"printf",
    // 		FunctionType::get(Type::IntTy, arg_types, true));

    // 	Constant* constStr = ConstantArray::get(str);
    // 	GlobalVariable* gv = new GlobalVariable(
    // 		constStr->getType(), 
    // 		true, GlobalValue::InternalLinkage, 
    // 		constStr, "", module_);

    // 	std::vector<Constant*> geplist;
    // 	geplist.push_back(ConstantUInt::get(Type::UIntTy,0));
    // 	geplist.push_back(ConstantUInt::get(Type::UIntTy,0));
    // 	Constant* gep = ConstantExpr::getGetElementPtr(gv,geplist);

    // 	std::vector<Value*> args;
    // 	args.push_back(gep);

    // 	llvm::CallInst::Create(f, args.begin(), args.end(), "call_printf", b());
    // }



  };
}
