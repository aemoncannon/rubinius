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

      set_block(bb);
      info().add_return_value(Constant::getNullValue(ObjType), bb);
      b().CreateBr(info().return_pad());

      set_block(cont);
    }


  };
}
