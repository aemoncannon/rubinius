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
    {}


		void at_trace_node(TraceNode* node){
			cur_trace_node_ = node;
		}

    void initialize() {
			info()->init_return_pad();

      BasicBlock* start = b().GetInsertBlock();

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

      info()->add_return_value(Constant::getNullValue(ObjType), current_block());
      b().CreateBr(info()->return_pad());

      set_block(ret_raise_val);
      Value* crv = f.clear_raise_value.call(&vm_, 1, "crv", b());
      if(use_full_scope_) flush_scope_to_heap(vars_);

      info()->add_return_value(crv, current_block());
      b().CreateBr(info()->return_pad());

      set_block(start);

      ip_pos_ = b().CreateConstGEP2_32(call_frame_, 0, offset::cf_ip, "ip_pos");

      global_serial_pos = b().CreateIntToPtr(
				ConstantInt::get(ls_->IntPtrTy, (intptr_t)ls_->shared().global_serial_address()),
				PointerType::getUnqual(ls_->IntPtrTy), "cast_to_intptr");

      init_out_args();
    }

    void visit_nested_trace() {

			Signature sig(ls_, ls_->VoidTy);
			sig << "VM";
			sig << "CallFrame";
			sig << ObjArrayTy;
			sig << "StackVariables";
			sig << ls_->Int32Ty;

			Value* call_args[] = {
				info()->vm(),
				info()->call_frame(),
				stack_ptr(),
				info()->variables(),
				ConstantInt::get(ls_->Int32Ty, cur_trace_node_->pc)
			};

			sig.call("rbx_call_trace", call_args, 5, "", b());

		}

    void visit_goto(opcode ip) {
			// Skip useless unconditional jumps (artifacts of
			// tracing).
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

      BasicBlock* loop_exit_stub = new_block("loop_exit_stub");

      b().CreateCondBr(cmp, loop_exit_stub, cont);

      set_block(loop_exit_stub);
			flush_ip(cur_trace_node_->interp_jump_target());
			flush_stack();
      info()->add_return_value(Constant::getNullValue(ObjType), loop_exit_stub);
      b().CreateBr(info()->return_pad());

      set_block(cont);
    }

		Value* get_field(Value* val, int which) {
			return b().CreateConstGEP2_32(val, 0, which);
		}


		void do_traced_return(){
			method_info_ = info()->parent_info();
			vm_ = info()->vm();
			call_frame_ = info()->call_frame();
			vars_ = info()->variables();
			// Info has changed, setup out_args stuff again.
			init_out_args();
		}

    void flush_ip() {
			flush_ip(current_ip_);
    }

    void flush_ip(int ip) {
			Value* call_frame = info()->root_info()->call_frame();
      Value* pos = b().CreateConstGEP2_32(call_frame, 0, offset::cf_ip, "ip_pos");
			// Note, pos points to an offset in the root call_frame for this trace,
			// not any of the subsequent call_frames created for traced sends.
      b().CreateStore(ConstantInt::get(ls_->Int32Ty, ip), pos);
    }

    void flush_stack() {
			Value* stk = b().CreateBitCast(stack_ptr(), ObjArrayTy, "obj_ary_type");
			Value* call_frame = info()->root_info()->call_frame();
			Value* pos = b().CreateBitCast(get_field(call_frame, offset::cf_flush_stk),
																		 PointerType::getUnqual(ObjArrayTy));
			b().CreateStore(stk, pos);
    }


    void visit_send_method(opcode which) {
      visit_send_stack(which, 0);
    }


#include "vm/llvm/jit_trace_send.hpp"
    void visit_send_stack(opcode which, opcode args) {
			if(cur_trace_node_->traced_send){
				do_traced_send(which, args, false);
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

    void visit_send_stack_with_block(opcode which, opcode args) {
			if(cur_trace_node_->traced_send){
				do_traced_send(which, args, true);
			}
			else{
				set_has_side_effects();

				InlineCache* cache = reinterpret_cast<InlineCache*>(which);

				bool has_literal_block = (current_block_ >= 0);
				bool block_on_stack = !has_literal_block;

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
				do_traced_yield_stack(count);
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
				do_traced_return();
			}
			else{
				info()->add_return_value(stack_top(), current_block());
				b().CreateBr(info()->return_pad());
			}
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




	};
}
