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
      info()->add_return_value(Constant::getNullValue(ObjType), bb);
      b().CreateBr(info()->return_pad());

      /////////////

      set_block(cont);

    }

		Value* get_field(Value* val, int which) {
			return b().CreateConstGEP2_32(val, 0, which);
		}

		void do_traced_send(opcode which, opcode args){

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

			BasicBlock* cur = current_block();


			// Generate hard return
			set_block(info()->return_pad());
			b().CreateRet(info()->return_phi());
			set_block(cur);


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

			info()->set_stack(parent_info->stack());

			Value* prev_call_frame = parent_info->call_frame();
			prev_call_frame->setName("prev_call_frame");
			info()->set_previous(prev_call_frame);

			info()->set_args(out_args_);

			info()->set_out_args(info()->root_info()->pre_allocated_args[cur_trace_node_->id]);

			init_out_args();

			Value* cfstk = info()->root_info()->pre_allocated_stacks[cur_trace_node_->id];

			Value* var_mem = info()->root_info()->pre_allocated_vars[cur_trace_node_->id];

			check_arity();

			call_frame_ = b().CreateBitCast(
        cfstk,
        PointerType::getUnqual(cf_type), "call_frame");

			info()->set_call_frame(call_frame_);

			// Value* stk = b().CreateConstGEP1_32(cfstk, sizeof(CallFrame) / sizeof(Object*), "stack");
			// info()->set_stack(stk);

			Value* vars = b().CreateBitCast(
        var_mem,
        PointerType::getUnqual(stack_vars_type), "vars");

			info()->set_variables(vars);

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
			if(!use_full_scope_) flags |= CallFrame::cClosedScope;

			b().CreateStore(
        ConstantInt::get(ls_->Int32Ty, flags),
        get_field(call_frame_, offset::cf_flags));

			// ip
			b().CreateStore(
        ConstantInt::get(ls_->Int32Ty, 0),
        get_field(call_frame_, offset::cf_ip));

			// scope
			b().CreateStore(vars, get_field(call_frame_, offset::cf_scope));

//			nil_stack(info()->vmm->stack_size, constant(Qnil, obj_type));

			import_args();

			stack_remove(args + 1);

			// b().CreateBr(body);
			// b().SetInsertPoint(body);
		}


		void do_traced_return(){
			method_info_ = info()->parent_info();
			vm_ = info()->vm();
			call_frame_ = info()->call_frame();
			// Info has changed, setup out_args stuff again.
			init_out_args();

		}


		void import_args() {
			Value* vm_obj = info()->vm();
			Value* arg_obj = info()->args();
			setup_scope();

			// Import the arguments
			Value* offset = b().CreateConstGEP2_32(arg_obj, 0, offset::args_ary, "arg_ary_pos");

			Value* arg_ary = b().CreateLoad(offset, "arg_ary");

			// If there are a precise number of args, easy.
			if(info()->vmm->required_args == info()->vmm->total_args) {
				for(int i = 0; i < info()->vmm->required_args; i++) {
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


				// Otherwise, we must loop in the generate code because we don't know
				// how many they've actually passed in.
			} else {
				Value* loop_i = info()->counter();

				BasicBlock* top = BasicBlock::Create(ls_->ctx(), "arg_loop_top", info()->function());
				BasicBlock* body = BasicBlock::Create(ls_->ctx(), "arg_loop_body", info()->function());
				BasicBlock* after = BasicBlock::Create(ls_->ctx(), "arg_loop_cont", info()->function());

				Value* limit;

				// Because of a splat, there can be more args given than
				// vmm->total_args, so we need to use vmm->total_args as a max.
				if(info()->vmm->splat_position >= 0) {
					Value* static_total = ConstantInt::get(ls_->Int32Ty, info()->vmm->total_args);

					limit = b().CreateSelect(
            b().CreateICmpSLT(static_total, info()->arg_total()),
            static_total,
            info()->arg_total());
				} else {
					// Because of arity checks, arg_total is less than or equal
					// to vmm->total_args.
					limit = info()->arg_total();
				}

				b().CreateStore(ConstantInt::get(ls_->Int32Ty, 0), loop_i);
				b().CreateBr(top);

				b().SetInsertPoint(top);

				// now at the top of block, check if we should continue...
				Value* loop_val = b().CreateLoad(loop_i, "loop_val");
				Value* cmp = b().CreateICmpSLT(loop_val, limit, "loop_test");

				b().CreateCondBr(cmp, body, after);

				// Now, the body
				b().SetInsertPoint(body);

				Value* arg_val_offset =
					b().CreateGEP(arg_ary, loop_val, "arg_val_offset");

				Value* arg_val = b().CreateLoad(arg_val_offset, "arg_val");

				Value* idx2[] = {
					ConstantInt::get(ls_->Int32Ty, 0),
					ConstantInt::get(ls_->Int32Ty, offset::vars_tuple),
					loop_val
				};

				Value* pos = b().CreateGEP(info()->variables(), idx2, idx2+3, "var_pos");

				b().CreateStore(arg_val, pos);

				Value* plus_one = b().CreateAdd(loop_val,
																				ConstantInt::get(ls_->Int32Ty, 1), "add");
				b().CreateStore(plus_one, loop_i);

				b().CreateBr(top);

				b().SetInsertPoint(after);
			}

			// Setup the splat.
			if(info()->vmm->splat_position >= 0) {
				Signature sig(ls_, "Object");
				sig << "VM";
				sig << "Arguments";
				sig << ls_->Int32Ty;

				Value* call_args[] = {
					vm_obj,
					arg_obj,
					ConstantInt::get(ls_->Int32Ty, info()->vmm->total_args)
				};

				Function* func = sig.function("rbx_construct_splat");
				func->setOnlyReadsMemory(true);
				func->setDoesNotThrow(true);

				CallInst* splat_val = sig.call("rbx_construct_splat", call_args, 3, "splat_val", b());

				splat_val->setOnlyReadsMemory(true);
				splat_val->setDoesNotThrow(true);

				Value* idx3[] = {
					ConstantInt::get(ls_->Int32Ty, 0),
					ConstantInt::get(ls_->Int32Ty, offset::vars_tuple),
					ConstantInt::get(ls_->Int32Ty, info()->vmm->splat_position)
				};

				Value* pos = b().CreateGEP(info()->variables(), idx3, idx3+3, "splat_pos");
				b().CreateStore(splat_val, pos);
			}
		}

		void check_arity() {
			Value* vm_obj = vm_;
			Value* dis_obj = info()->msg();
			Value* arg_obj = info()->args();

			Value* total_offset = b().CreateConstGEP2_32(arg_obj, 0,
																									 offset::args_total, "total_pos");
			Value* total = b().CreateLoad(total_offset, "arg.total");

			// For others to use.
			info()->set_arg_total(total);

			BasicBlock* arg_error = BasicBlock::Create(ls_->ctx(), "arg_error", info()->function());
			BasicBlock* cont = BasicBlock::Create(ls_->ctx(), "import_args", info()->function());

			// Check arguments
			//
			// if there is a splat..
			if(info()->vmm->splat_position >= 0) {
				if(info()->vmm->required_args > 0) {
					// Make sure we got at least the required args
					Value* cmp = b().CreateICmpSLT(total,
																				 ConstantInt::get(ls_->Int32Ty, info()->vmm->required_args), "arg_cmp");
					b().CreateCondBr(cmp, arg_error, cont);
				} else {
					// Only splat or optionals, no handling!
					b().CreateBr(cont);
				}

				// No splat, a precise number of args
			} else if(info()->vmm->required_args == info()->vmm->total_args) {
				// Make sure we got the exact number of arguments
				Value* cmp = b().CreateICmpNE(total,
																			ConstantInt::get(ls_->Int32Ty, info()->vmm->required_args), "arg_cmp");
				b().CreateCondBr(cmp, arg_error, cont);

				// No splat, with optionals
			} else {
				Value* c1 = b().CreateICmpSLT(total,
																			ConstantInt::get(ls_->Int32Ty, info()->vmm->required_args), "arg_cmp");
				Value* c2 = b().CreateICmpSGT(total,
																			ConstantInt::get(ls_->Int32Ty, info()->vmm->total_args), "arg_cmp");

				Value* cmp = b().CreateOr(c1, c2, "arg_combine");
				b().CreateCondBr(cmp, arg_error, cont);
			}

			b().SetInsertPoint(arg_error);

			// Call our arg_error helper
			Signature sig(ls_, "Object");

			sig << "VM";
			sig << "CallFrame";
			sig << "Dispatch";
			sig << "Arguments";
			sig << ls_->Int32Ty;


			Value* call_args[] = {
				vm_obj,
				info()->previous(),
				dis_obj,
				arg_obj,
				ConstantInt::get(ls_->Int32Ty, info()->vmm->required_args)
			};


			Value* val = sig.call("rbx_arg_error", call_args, 5, "ret", b());
			return_value(val);

			// Switch to using continuation
			b().SetInsertPoint(cont);
		}

		void return_value(Value* ret, BasicBlock* cont = 0) {
			info()->add_return_value(ret, b().GetInsertBlock());
			b().CreateBr(info()->return_pad());
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



    void visit_send_method(opcode which) {
      visit_send_stack(which, 0);
    }

    void visit_send_stack(opcode which, opcode args) {
			if(cur_trace_node_->traced_send){
				do_traced_send(which, args);
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

				//dump_int(3);

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
