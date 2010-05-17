#include "instructions_util.hpp"

#include "builtin/symbol.hpp"
#include "builtin/tuple.hpp"
#include "builtin/global_cache_entry.hpp"
#include "inline_cache.hpp"

#include "llvm/jit_visit.hpp"

namespace rubinius {

  class JITTraceVisit : public JITVisit {

  public:

    JITTraceVisit(LLVMState* ls, JITMethodInfo& info, BlockMap& bm, llvm::BasicBlock* start)
      : JITVisit(ls, info, bm, start)
    {}

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

      //init_out_args();
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


    void visit_meta_send_op_lt(opcode name) {
      InlineCache* cache = reinterpret_cast<InlineCache*>(name);
      if(cache->classes_seen() == 0) {

        set_has_side_effects();

        Value* recv = stack_back(1);
        Value* arg =  stack_top();

        BasicBlock* fast = new_block("fast");
        BasicBlock* dispatch = new_block("dispatch");
        BasicBlock* cont = new_block("cont");

        check_fixnums(recv, arg, fast, dispatch);

        set_block(dispatch);
        Value* called_value = simple_send(ls_->symbol("<"), 1);
        check_for_exception_then(called_value, cont);

        set_block(fast);
        Value* cmp = b().CreateICmpSLT(recv, arg, "imm_cmp");
        Value* imm_value = b().CreateSelect(cmp,
																						constant(Qtrue), constant(Qfalse), "select_bool");

        b().CreateBr(cont);

        set_block(cont);
        PHINode* phi = b().CreatePHI(ObjType, "addition");
        phi->addIncoming(called_value, dispatch);
        phi->addIncoming(imm_value, fast);
        stack_remove(2);
        stack_push(phi);

      } else {
        visit_send_stack(name, 1);
      }
    }

    void visit_push_local(opcode which) {
      Value* idx2[] = {
        ConstantInt::get(ls_->Int32Ty, 0),
        ConstantInt::get(ls_->Int32Ty, offset::vars_tuple),
        ConstantInt::get(ls_->Int32Ty, which)
      };
      Value* pos = b().CreateGEP(vars_, idx2, idx2+3, "local_pos");
			Value* tmp = b().CreateLoad(pos, "local");

      stack_push(tmp);
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
