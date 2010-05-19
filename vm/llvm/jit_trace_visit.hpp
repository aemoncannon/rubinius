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

    JITTraceVisit(LLVMState* ls, Trace* trace, JITMethodInfo& info, BlockMap& bm, llvm::BasicBlock* start)
      : JITVisit(ls, info, bm, start)
      , trace_(trace)
    {}


		void at_trace_node(TraceNode* node){
			cur_trace_node_ = node;
		}

    void initialize() {
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

      info().add_return_value(Constant::getNullValue(ObjType), current_block());
      b().CreateBr(info().return_pad());

      set_block(ret_raise_val);
      Value* crv = f.clear_raise_value.call(&vm_, 1, "crv", b());
      if(use_full_scope_) flush_scope_to_heap(vars_);

      info().add_return_value(crv, current_block());
      b().CreateBr(info().return_pad());

      set_block(start);

      ip_pos_ = b().CreateConstGEP2_32(call_frame_, 0, offset::cf_ip, "ip_pos");

      global_serial_pos = b().CreateIntToPtr(
				ConstantInt::get(ls_->IntPtrTy, (intptr_t)ls_->shared().global_serial_address()),
				PointerType::getUnqual(ls_->IntPtrTy), "cast_to_intptr");

      init_out_args();
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
      BasicBlock* bb = block_map_[ip].block;
      assert(bb);
      b().CreateCondBr(cmp, bb, cont);


      ////////////////////
			// Trace exit stub!

      set_block(bb);
			print_debug();
      info().add_return_value(Constant::getNullValue(ObjType), bb);
      b().CreateBr(info().return_pad());

      /////////////

      set_block(cont);

    }

    void visit_send_method(opcode which) {
      visit_send_stack(which, 0);
    }

    void visit_send_stack(opcode which, opcode args) {
			if(cur_trace_node_->traced_send){
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


	};
}
