#include "instructions_util.hpp"

#include "builtin/symbol.hpp"
#include "builtin/tuple.hpp"
#include "builtin/global_cache_entry.hpp"
#include "inline_cache.hpp"

#include "llvm/jit_visit.hpp"
#include "trace.hpp"

namespace rubinius {

  class JITTraceVisit : public JITVisit {

		Trace* trace_;
		TraceNode* cur_trace_node_;

  public:

    JITTraceVisit(LLVMState* ls, Trace* trace, JITMethodInfo* info, BlockMap& bm, llvm::BasicBlock* start)
      : JITVisit(ls, info, bm, start)
      , trace_(trace)
    {
			sp_ = trace->entry_sp;
			last_sp_ = trace->entry_sp;
		}


		void at_trace_node(TraceNode* node){
			cur_trace_node_ = node;
		}

    void at_ip(int ip) {
      BlockMap::iterator i = block_map_.find(ip);
      if(i != block_map_.end()) {
        JITBasicBlock& jbb = i->second;
        current_jbb_ = &jbb;

				// Might need this stuff if we ever have branches in our
				// traces..
				
        // if(BasicBlock* next = jbb.block) {
        //   if(!b().GetInsertBlock()->getTerminator()) {
				// 		std::cout << "ACK! creating BR!!" << "\n";
        //     b().CreateBr(next);
        //   }
        //   next->moveAfter(b().GetInsertBlock());
        //   set_block(next);
        // }
        // if(jbb.sp != -10) set_sp(jbb.sp);
      }

      remember_sp();

      current_ip_ = ip;
    }


    void initialize() {

			info()->init_return_pad();
			info()->init_trace_exit_pad();

      BasicBlock* start = current_block();


			set_block(info()->trace_exit_pad());

			emit_trace_exit_pad();

			set_block(start);

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

			return_value(Constant::getNullValue(ObjType));

      set_block(ret_raise_val);
      Value* crv = f.clear_raise_value.call(&vm_, 1, "crv", b());
      if(use_full_scope_) flush_scope_to_heap(vars_);

			return_value(crv);

      set_block(start);

      ip_pos_ = b().CreateConstGEP2_32(call_frame_, 0, offset::cf_ip, "ip_pos");

      global_serial_pos = b().CreateIntToPtr(
				ConstantInt::get(ls_->IntPtrTy, (intptr_t)ls_->shared().global_serial_address()),
				PointerType::getUnqual(ls_->IntPtrTy), "cast_to_intptr");

      init_out_args();

    }

    void emit_trace_exit_pad() {
			// Emit code for entering the uncommon interpreter.
			// This is used in exit stubs, when the trace bails for some reason.
			// Inputs to the exit pad
			
			Value* trace_ip = info()->root_info()->trace_ip_phi;
			Value* actual_exit_ip = info()->root_info()->exit_ip_phi;
			Value* exit_cf = info()->root_info()->exit_cf_phi;


			Value* recording_pos = get_field(info()->trace_info(), offset::trace_info_recording);
			Value* recording = b().CreateIntCast(b().CreateLoad(recording_pos, "recording"), ls_->Int1Ty, "recording");
			Value* not_recording = b().CreateNot(recording, "not_recording");

			Value* nested_pos = get_field(info()->trace_info(), offset::trace_info_nested);
			Value* nested = b().CreateIntCast(b().CreateLoad(nested_pos, "nested"), ls_->Int1Ty, "nested");
			Value* not_nested = b().CreateNot(nested, "not_nested");

			Value* entry_cf_pos = get_field(info()->trace_info(), offset::trace_info_entry_cf);
			Value* entry_call_frame = b().CreateLoad(entry_cf_pos, "save_entry_call_frame");

			Value* exp_exit_ip_pos = get_field(info()->trace_info(), offset::trace_info_expected_exit_ip);
			Value* expected_exit_ip = b().CreateLoad(exp_exit_ip_pos, "save_expected_exit_ip");

			Value* nestable_pos = get_field(info()->trace_info(), offset::trace_info_nestable);
			Value* exit_ip_pos = get_field(info()->trace_info(), offset::trace_info_exit_ip);
			Value* exit_trace_pc_pos = get_field(info()->trace_info(), offset::trace_info_exit_trace_pc);

      Value* ip_cmp = b().CreateICmpEQ(actual_exit_ip, expected_exit_ip, "exiting_at_expected_ip_p");
      Value* cf_cmp = b().CreateICmpEQ(entry_call_frame, exit_cf, "at_expected_call_frame_p");

			// Store information about exit into TraceInfo
			b().CreateStore(actual_exit_ip, exit_ip_pos);
      b().CreateStore(ConstantInt::get(ls_->Int32Ty, 1), nestable_pos);
			// So we know where in the trace we exited (useful for debugging)...
      b().CreateStore(trace_ip, exit_trace_pc_pos);

      BasicBlock* cont = new_block("continue");
      BasicBlock* exit = new_block("exit");

			// If we are recording, and the current call_frame is this trace's home call_frame,
			// return directly to the caller of the trace - setting nestable to true. Result will
			// be that this trace will be recorded as a nested trace.
      Value* anded = b().CreateAnd(recording, cf_cmp, "and");
      b().CreateCondBr(anded, exit, cont);
      set_block(exit);
			return_value(constant(Qnil));
      set_block(cont);

			// If we are not recording, and the current call_frame is this trace's home call_frame,
			// exit directly to the caller of the trace - setting nestable to true (this informs caller
			// that the trace exited politely, so interpreter doesn't have to pop itself)
      cont = new_block("continue");
      exit = new_block("exit");
      anded = b().CreateAnd(not_recording, cf_cmp, "and");
      anded = b().CreateAnd(anded, not_nested, "and");
      b().CreateCondBr(anded, exit, cont);
			set_block(exit);
			return_value(constant(Qnil));
      set_block(cont);

			// If we are not recording, and this was a nested trace, and the current call_frame is 
			// this trace's home call_frame, and the ip we are exiting to is the ip that the calling trace was expecting,
			// exit directly to the caller of the trace - setting nestable to true  (this informs caller
			// that the trace exited politely, so parent trace doesn't have to pop itself)
      cont = new_block("continue");
      exit = new_block("exit");
      anded = b().CreateAnd(not_recording, cf_cmp, "and");
      anded = b().CreateAnd(anded, nested, "and");
      anded = b().CreateAnd(anded, ip_cmp, "and");
      b().CreateCondBr(anded, exit, cont);
			set_block(exit);
			return_value(constant(Qnil));


			// Otherwise, we exit to the uncommon interpreter,
			// setting 'nestable' to false so the caller of this trace will know that
			// this trace run did not exit politely.
      set_block(cont);
      b().CreateStore(ConstantInt::get(ls_->Int32Ty, 0), nestable_pos);


			Signature sig(ls_, "Object");
			sig << "VM";
			sig << "CallFrame";

			Value* call_args[] = { info()->vm(), exit_cf};
			Value* ret = sig.call("rbx_continue_uncommon", call_args, 2, "", b());
			return_value(ret);

    }



    void visit_nested_trace() {
			
			// Save away current trace info
			Value* exp_exit_ip_pos = get_field(info()->trace_info(), offset::trace_info_expected_exit_ip);
			Value* save_expected_exit_ip = b().CreateLoad(exp_exit_ip_pos, "save_expected_exit_ip");

			Value* entry_cf_pos = get_field(info()->trace_info(), offset::trace_info_entry_cf);
			Value* save_entry_call_frame = b().CreateLoad(entry_cf_pos, "save_entry_call_frame");

			Value* nested_pos = get_field(info()->trace_info(), offset::trace_info_nested);
			Value* save_nested = b().CreateLoad(nested_pos, "save_nested");

			// Write new trace info for this nested trace
			// The ip where the trace should exit, if all goes as planned
			b().CreateStore(ConstantInt::get(ls_->Int32Ty, cur_trace_node_->nested_trace->expected_exit_ip), exp_exit_ip_pos);

			// The active callframe. 'home' call_frame from the nested trace's perspective.
      b().CreateStore(info()->call_frame(), entry_cf_pos);

			// Yes, this trace invocation is 'nested' (being called from a parent trace)
      b().CreateStore(ConstantInt::get(ls_->Int32Ty, 1), nested_pos);


			// Flush info just in case nested trace side-exits. 
			// Have to do this before we call, since we don't know if we'll need it...
			flush_call_stack();

			// Call the nested trace
			Signature sig(ls_, "Object");
			sig << "VM";
			sig << "CallFrame";
			sig << ls_->Int32Ty;
			sig << "TraceInfo";
			Value* call_args[] = {
				info()->vm(),
				info()->call_frame(),
				ConstantInt::get(ls_->Int32Ty, cur_trace_node_->pc),
				info()->trace_info()
			};
			Value* ret = sig.call("rbx_call_trace", call_args, 4, "", b());

			// Restore the trace-info for the parent trace
      b().CreateStore(save_expected_exit_ip, exp_exit_ip_pos);
      b().CreateStore(save_entry_call_frame, entry_cf_pos);
      b().CreateStore(save_nested, nested_pos);

			// Check if the result of the nested trace is 'nestable'. That is, check if
			// the trace run exited in a polite way.
			Value* nestable_pos = get_field(info()->trace_info(), offset::trace_info_nestable);
			Value* nestable = b().CreateIntCast(b().CreateLoad(nestable_pos, ""), ls_->Int1Ty, "nestable");
			Value* not_nestable = b().CreateNot(nestable, "not_nestable");

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


    void visit_goto(opcode ip) {

			// Skip useless unconditional jumps (not needed in
			// trace).
			if(cur_trace_node_->next->trace_pc == (int)ip &&
				 cur_trace_node_->next != trace_->anchor){
				return;
			}
				
			BasicBlock* bb = block_map_[ip].block;
			assert(bb);
			b().CreateBr(bb);
			set_block(new_block("continue"));
    }

   
    void visit_goto_if_false(opcode ip) {
      Value* cond = stack_pop();
      Value* i = b().CreatePtrToInt(
				cond, ls_->IntPtrTy, "as_int");
      Value* anded = b().CreateAnd(
				i,
				ConstantInt::get(ls_->IntPtrTy, FALSE_MASK), 
				"and");
      Value* cmp = b().CreateICmpEQ(
				anded,
				ConstantInt::get(ls_->IntPtrTy, cFalse), 
				"is_true");

      BasicBlock* cont = new_block("continue");
      BasicBlock* exit_stub = new_block("exit_stub");

			if(cur_trace_node_->jump_taken){
				b().CreateCondBr(cmp, cont, exit_stub);
			}
			else{
				b().CreateCondBr(cmp, exit_stub, cont);
			}
			set_block(exit_stub);
			exit_trace(cur_trace_node_->interp_jump_target());
			set_block(cont);
    }

    void visit_goto_if_true(opcode ip) {
      Value* cond = stack_pop();
      Value* i = b().CreatePtrToInt(
				cond, ls_->IntPtrTy, "as_int");
      Value* anded = b().CreateAnd(
				i,
				ConstantInt::get(ls_->IntPtrTy, FALSE_MASK), "and");

      Value* cmp = b().CreateICmpNE(
				anded,
				ConstantInt::get(ls_->IntPtrTy, cFalse), "is_true");

			BasicBlock* cont = new_block("continue");
      BasicBlock* exit_stub = new_block("exit_stub");

			if(cur_trace_node_->jump_taken){
				b().CreateCondBr(cmp, cont, exit_stub);
			}
			else{
				b().CreateCondBr(cmp, exit_stub, cont);
			}
			set_block(exit_stub);
			exit_trace(cur_trace_node_->interp_jump_target());
			set_block(cont);
    }

    void visit_setup_unwind(opcode where, opcode type) {
      BasicBlock* code;

      JITBasicBlock& jbb = block_map_[where];
      jbb.landing_pad = true;
      assert(jbb.block);

      if(type == cRescue) {
        // Add a prologue block that checks if we should handle this
        // exception.

        BasicBlock* orig = current_block();
        code = new_block("is_exception");
        set_block(code);

        // std::vector<const Type*> types;
        // types.push_back(VMTy);

        // FunctionType* ft = FunctionType::get(ls_->Int1Ty, types, false);
        // Function* func = cast<Function>(
				// 	module_->getOrInsertFunction("rbx_raising_exception", ft));

        // Value* call_args[] = { vm_ };

				
				// Check if the raised condition is an exception..
        // Value* isit = b().CreateCall(func, call_args, call_args+1, "rae");

				BasicBlock* exit_stub = new_block("exit_stub");
				set_block(exit_stub);
				exit_trace(where);
				set_block(code);

				
        // If it's not an exception, we're going to want to pass to 
				// a surrounding handler. If no such handler exists, we bail.
        // BasicBlock* next = 0;
        // if(has_exception_handler()) {
        //   next = exception_handler();
        // } else {
        //   next = exit_stub;
        // }

				// Now, if it was an exception, run the rescue clause. Otherwise,
				// pass to surrounding handler or bail.
        // b().CreateCondBr(isit, jbb.block, next);

				// Currently we're being babies and just bailing out at any 
				// exceptional condition. 

				// See todo.org for what we should be doing.

				b().CreateBr(exit_stub);


        set_block(orig);

        // Now, change jbb to point to code, so anyone branching there hits
        // the check first.
        jbb.prologue = code;
      } else {
        code = jbb.block;
      }
    }


		Value* get_field(Value* val, int which) {
			return b().CreateConstGEP2_32(val, 0, which);
		}

		void emit_traced_return(){

			Value* ret_val = stack_top();

			method_info_ = info()->parent_info();
			vm_ = info()->vm();
			call_frame_ = info()->call_frame();
			vars_ = info()->variables();
			stack_ = info()->stack();
			sp_ = info()->saved_sp();
			last_sp_ = info()->saved_last_sp();

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

		void return_value(Value* ret) {
			info()->add_return_value(ret, current_block());
			b().CreateBr(info()->return_pad());
		}

		void exit_trace(int next_ip){
			BasicBlock* cur = current_block();
			info()->root_info()->exit_ip_phi->addIncoming(int32(cur_trace_node_->pc), cur);
			info()->root_info()->trace_ip_phi->addIncoming(int32(cur_trace_node_->trace_pc), 
																										 cur);
			info()->root_info()->exit_cf_phi->addIncoming(info()->call_frame(), cur);


			// Flush ip and sp of active current frame
			Value* cf = info()->call_frame();
			Value* next_ip_val = ConstantInt::get(ls_->Int32Ty, next_ip);
			Value* next_ip_pos = get_field(cf, offset::cf_ip);
			b().CreateStore(next_ip_val, next_ip_pos);
			Value* stckp = ConstantInt::get(ls_->Int32Ty, sp());
			Value* exit_sp_pos = get_field(cf, offset::cf_sp);
      b().CreateStore(stckp, exit_sp_pos);

			flush_call_stack();

			b().CreateBr(info()->trace_exit_pad());
		}


		void flush_call_stack(){
				std::cout << "\n\nFLUSHING CALL STACK\n" << endl;
			// Flush ip and sp of any stacked frames
			TraceNode* node = cur_trace_node_->active_send;
			while(node != NULL){
				std::cout << "NODE_DUMP{" << endl;
				node->pretty_print(VM::current_state(), std::cout);
				std::cout << "}" << endl;

				assert(node->traced_send || node->traced_yield);
				Value* cf = NULL;
				TraceNode* cf_creator_node = node->active_send;
				if(cf_creator_node != NULL){
					cf = info()->root_info()->pre_allocated_call_frames[cf_creator_node->trace_pc];	
				}
				else{
					cf = info()->root_info()->call_frame();
				}

				cf = b().CreateBitCast(cf, CallFrameTy, "call_frame");
				assert(cf);

				Value* stckp = NULL;
				Value* next_ip = NULL;

				if(node->op == InstructionSequence::insn_send_stack){
					stckp = ConstantInt::get(ls_->Int32Ty, node->sp - node->arg2 - 1);
					next_ip = ConstantInt::get(ls_->Int32Ty, node->pc + 3);
				}
				else if(node->op == InstructionSequence::insn_send_stack_with_block){
					stckp = ConstantInt::get(ls_->Int32Ty, node->sp - node->arg2 - 2);
					next_ip = ConstantInt::get(ls_->Int32Ty, node->pc + 3);
				}
				else if(node->op == InstructionSequence::insn_yield_stack){
					stckp = ConstantInt::get(ls_->Int32Ty, node->sp - node->arg1);
					next_ip = ConstantInt::get(ls_->Int32Ty, node->pc + 2);
				}
				else if(node->op == InstructionSequence::insn_send_method){
					stckp = ConstantInt::get(ls_->Int32Ty, node->sp - 1);
					next_ip = ConstantInt::get(ls_->Int32Ty, node->pc + 2);
				}
				else{
					stckp = ConstantInt::get(ls_->Int32Ty, node->sp - node->arg2 - 1);
					next_ip = ConstantInt::get(ls_->Int32Ty, node->pc + 3);
				}

				Value* next_ip_pos = get_field(cf, offset::cf_ip);
				b().CreateStore(next_ip, next_ip_pos);

				Value* exit_sp_pos = get_field(cf, offset::cf_sp);
				b().CreateStore(stckp, exit_sp_pos);

				node = node->active_send;
			}
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
				emit_traced_send(which, args, false);
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

				emit_traced_send(which, args, true);

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
			if(cur_trace_node_->traced_yield){
				emit_traced_yield_stack(count);
			}
			else{
				set_has_side_effects();
				Value* vars = vars_;
				if(JITMethodInfo* home = info()->home_info()) {
					vars = home->variables();
				}
				Signature sig(ls_, ObjType);
				sig << VMTy;
				sig << CallFrameTy;
				sig << "Object";
				sig << ls_->Int32Ty;
				sig << ObjArrayTy;
				Value* block_obj = b().CreateLoad(
					b().CreateConstGEP2_32(vars, 0, offset::vars_block),
					"block");

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

		void visit_ret() {
			if(use_full_scope_) flush_scope_to_heap(info()->variables());
			if(cur_trace_node_->active_send){
				emit_traced_return();
			}
			else{
				return_value(stack_top());
			}
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


		void visit_meta_send_op_plus(opcode name) {
			InlineCache* cache = reinterpret_cast<InlineCache*>(name);
			if(cache->classes_seen() == 0) {
				set_has_side_effects();
				Value* recv = stack_back(1);
				Value* arg =  stack_top();

				BasicBlock* fast = new_block("fast");
				BasicBlock* dispatch = new_block("dispatch");
				BasicBlock* tagnow = new_block("tagnow");
				BasicBlock* cont = new_block("cont");

				check_fixnums(recv, arg, fast, dispatch);

				set_block(dispatch);

				Value* called_value = simple_send(ls_->symbol("+"), 1);

				check_for_exception_then(called_value, cont);

				set_block(fast);

				std::vector<const Type*> types;
				types.push_back(FixnumTy);
				types.push_back(FixnumTy);

				std::vector<const Type*> struct_types;
				struct_types.push_back(FixnumTy);
				struct_types.push_back(ls_->Int1Ty);

				StructType* st = StructType::get(ls_->ctx(), struct_types);

				FunctionType* ft = FunctionType::get(st, types, false);
				Function* func = cast<Function>(
					module_->getOrInsertFunction(ADD_WITH_OVERFLOW, ft));

				Value* recv_int = tag_strip(recv);
				Value* arg_int = tag_strip(arg);
				Value* call_args[] = { recv_int, arg_int };
				Value* res = b().CreateCall(func, call_args, call_args+2, "add.overflow");

				Value* sum = b().CreateExtractValue(res, 0, "sum");
				Value* dof = b().CreateExtractValue(res, 1, "did_overflow");

				b().CreateCondBr(dof, dispatch, tagnow);

				set_block(tagnow);


				Value* imm_value = fixnum_tag(sum);

				b().CreateBr(cont);

				set_block(cont);

				PHINode* phi = b().CreatePHI(ObjType, "addition");
				phi->addIncoming(called_value, dispatch);
				phi->addIncoming(imm_value, tagnow);


				stack_remove(2);
				stack_push(phi);
			} else {
				visit_send_stack(name, 1);
			}
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




	};
}
