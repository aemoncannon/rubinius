#ifdef ENABLE_LLVM

#include "llvm/jit_trace.hpp"
#include "call_frame.hpp"
#include "vmmethod.hpp"
#include "trace.hpp"
#include "utilities.hpp"

#include "llvm/jit_trace_visit.hpp"
#include "llvm/control_flow.hpp"
#include "llvm/cfg.hpp"

#include <llvm/Analysis/CaptureTracking.h>

namespace rubinius {

  namespace jit {

    TraceBuilder::TraceBuilder(LLVMState* ls, Trace* trace, JITMethodInfo* i)
      : ls_(ls)
      , vmm_(i->vmm)
      , builder_(ls->ctx())
      , use_full_scope_(false)
      , import_args_(0)
      , method_body_(0)
      , trace(trace)
      , info_(i)  

    {
      llvm::Module* mod = ls->module();
      cf_type = mod->getTypeByName("struct.rubinius::CallFrame");
      vars_type = mod->getTypeByName("struct.rubinius::VariableScope");
      stack_vars_type = mod->getTypeByName("struct.rubinius::StackVariables");
      obj_type = ls->ptr_type("Object");
      obj_ary_type = PointerType::getUnqual(obj_type);
    }

    void TraceBuilder::setup() {
      std::vector<const Type*> ftypes;
      ftypes.push_back(ls_->ptr_type("VM"));
      ftypes.push_back(ls_->ptr_type("CallFrame"));
      ftypes.push_back(ls_->ptr_type("Trace"));
      ftypes.push_back(ls_->ptr_type("Trace"));
      ftypes.push_back(ls_->ptr_type("TraceNode"));
      ftypes.push_back(ls_->Int32Ty);

      FunctionType* ft = FunctionType::get(ls_->Int32Ty, ftypes, false);

      std::string name = trace->trace_name();

      func = Function::Create(ft, GlobalValue::ExternalLinkage,
															name.c_str(), ls_->module());

      Function::arg_iterator ai = func->arg_begin();
      vm =   ai++; 
      vm->setName("state");

      // For the trace, this is the current and active CallFrame
      call_frame = ai++; 
      call_frame->setName("call_frame");

      Value* trace = ai++;
      trace->setName("trace");

      // Skip exit trace
      ai++;
      // Skip exit node
      ai++;

      // Get the trace run-mode
      Value* run_mode = ai++;

      BasicBlock* block = BasicBlock::Create(ls_->ctx(), "entry", func);
      builder_.SetInsertPoint(block);

      info_->set_function(func);
      info_->set_vm(vm);
      info_->set_call_frame(call_frame);
      info_->set_trace(trace);
      info_->set_trace_run_mode(run_mode);
      info_->set_entry(block);

      BasicBlock* body = BasicBlock::Create(ls_->ctx(), "method_body", func);
      method_body_ = body;

      pass_one(body);

      // Not sure what this is for..
      valid_flag = b().CreateAlloca(ls_->Int1Ty, 0, "valid_flag");
			
      //stack
      Value* stk_base = get_field(call_frame, offset::cf_stk);
      stk_base = b().CreateBitCast(stk_base, obj_ary_type, "obj_ary_type");
      info_->set_stack(stk_base);

      //unwinds
      Value* unwinds_pos = get_field(call_frame, offset::cf_unwinds);
      Value* unwinds = b().CreateLoad(unwinds_pos, "unwinds");
      unwinds = b().CreateBitCast(unwinds, ls_->ptr_type("UnwindInfo"), "unwinds");
      info_->set_unwinds(unwinds);

      //vars
      Value* vars_pos = get_field(call_frame, offset::cf_scope);
      Value* vars = b().CreateLoad(vars_pos, "vars");
      vars = b().CreateBitCast(vars, ls_->ptr_type("StackVariables"), "vars");
      info_->set_variables(vars);

      Value* args_pos = get_field(call_frame, offset::cf_arguments);
      Value* args = b().CreateLoad(args_pos, "args");
      args = b().CreateBitCast(vars, ls_->ptr_type("Arguments"), "args");
      info_->set_args(vars);

      info_->set_out_args(b().CreateAlloca(ls_->type("Arguments"), 0, "out_args"));

      // ip
      Value* ip_mem = get_field(call_frame, offset::cf_ip);
      b().CreateStore(ConstantInt::get(ls_->Int32Ty, 0), ip_mem);

      // Allocate a shared counter, used for looping when initializing stack
      // and locals at each send
      info_->set_counter(b().CreateAlloca(ls_->Int32Ty, 0, "counter_alloca"));

      allocate_call_structures();

      b().CreateBr(body);
      b().SetInsertPoint(body);
    }


    void TraceBuilder::allocate_call_structures(){
      const llvm::Type* obj_type = ls_->ptr_type("Object");

      TraceIterator it = trace->iter();
      while(it.has_next()){
				TraceNode* node = it.next();

				if(node->traced_send || node->traced_yield){

					info_->pre_allocated_args[node->trace_pc] = 
						b().CreateAlloca(ls_->type("Arguments"), 0, "out_args");

					CompiledMethod* cm = node->target_cm.get();
					assert(cm != Qundef);

					info_->pre_allocated_call_frames[node->trace_pc] = 
						b().CreateAlloca(obj_type,
														 ConstantInt::get(ls_->Int32Ty,
																							(sizeof(CallFrame) / sizeof(Object*)) + 
																							cm->stack_size()->to_native()),
														 "cfstk");

					info_->pre_allocated_vars[node->trace_pc] = 
						b().CreateAlloca(obj_type,
														 ConstantInt::get(ls_->Int32Ty,
																							(sizeof(StackVariables) / sizeof(Object*)) + 
																							cm->number_of_locals()),
														 "var_mem");

					info_->pre_allocated_unwinds[node->trace_pc] = 
						b().CreateAlloca(ls_->type("UnwindInfo"), 
														 ConstantInt::get(ls_->Int32Ty, kMaxUnwindInfos),
														 "unwind_infos");
				}
      }
    }


    Value* TraceBuilder::get_field(Value* val, int which) {
      return b().CreateConstGEP2_32(val, 0, which);
    }



    class Walker {
      JITTraceVisit& v_;
      BlockMap& map_;

    public:
      Walker(JITTraceVisit& v, BlockMap& map)
				: v_(v)
				, map_(map)
      {}

      void call(Trace* trace, TraceNode* node){

				// DEBUGLN("\n\n\nCompiling node: ");
				// DEBUGLN("sp is: " << v_.sp());
				// IF_DEBUG(node->pretty_print(VM::current_state(), std::cout));
				// DEBUGLN("\nsp is: " << v_.sp());
				trace->dispatch(v_, node);

				// if(v_.b().GetInsertBlock()->getTerminator() == NULL) {
				// 	std::cout << "No terminator at: " << node->pc << "\n";
				// 	if(node->next != NULL) {
				// 		BlockMap::iterator i = map_.find(node->next->pc);
				// 		if(i != map_.end()) {
				// 			std::cout << "Making terminator: " << node->next->pc << "\n";
				// 			v_.b().CreateBr(i->second.block);
				// 		}
				// 	}
				//}


      }
    };


    bool TraceBuilder::generate_body() {

      JITTraceVisit visitor(ls_, trace, info_, block_map_, b().GetInsertBlock());
      visitor.set_called_args(0);
      visitor.set_valid_flag(valid_flag);
      if(use_full_scope_) visitor.use_full_scope();
      visitor.initialize();

      Walker walker(visitor, block_map_);

      try {
				trace->walk(walker);
      } catch(JITVisit::Unsupported &e) {
				std::cout << "ERROR: Unsupported operation.";
				return false;
      }

      visitor.visit_end();

      info_->return_pad()->moveAfter(visitor.current_block());
      info_->fin_block = visitor.current_block();

      return true;
    }

    void TraceBuilder::generate_hard_return() {
      b().SetInsertPoint(info_->return_pad());
      b().CreateRet(info_->return_phi());
    }


    class TracePassOne : public VisitInstructions<TracePassOne> {
      LLVMState* ls_;
      BlockMap& map_;
      Function* function_;
      opcode current_ip_;
      int force_break_;
      bool creates_blocks_;
      int number_of_sends_;
      bool loops_;
      int sp_;
      JITBasicBlock* current_block_;
      bool calls_evalish_;

      Symbol* s_eval_;
      Symbol* s_binding_;
      Symbol* s_class_eval_;
      Symbol* s_module_eval_;

      TraceNode* cur_trace_node_;

      std::list<JITBasicBlock*> exception_handlers_;
      //			CFGCalculator& cfg_;

    public:

      TracePassOne(LLVMState* ls, BlockMap& map, int init_ip, Function* func, BasicBlock* start)
				: ls_(ls)
				, map_(map)
				, function_(func)
				, current_ip_(init_ip)
				, force_break_(false)
				, creates_blocks_(false)
				, number_of_sends_(0)
				, loops_(false)
				, sp_(-1)
				, calls_evalish_(false)
					//				, cfg_(cfg)  Aemon fix this!
      {
				JITBasicBlock& jbb = map_[init_ip];
				jbb.reachable = true;
				jbb.block = start;

				current_block_ = &jbb;

				s_eval_ = ls->symbol("eval");
				s_binding_ = ls->symbol("binding");
				s_class_eval_ = ls->symbol("class_eval");
				s_module_eval_ = ls->symbol("module_eval");

				// By default, there is no handler.
				exception_handlers_.push_back(0);
      }

      void at_trace_node(TraceNode* node){
				cur_trace_node_ = node;
      }

      bool calls_evalish() {
				return calls_evalish_;
      }

      bool creates_blocks() {
				return creates_blocks_;
      }

      int number_of_sends() {
				return number_of_sends_;
      }

      bool loops_p() {
				return loops_;
      }

      void at_ip(int ip) {
				current_ip_ = ip;

				// If this is a new block, reset sp here
				/*
					BlockMap::iterator i = map_.find(ip);
					if(i != map_.end()) {
					sp_ = i->second.sp;
					}
				*/
      }

      void debug_node() {
				for(int i=0; i < cur_trace_node_->call_depth;i++) std::cout << "  ";
				cur_trace_node_->pretty_print(NULL, std::cout);
				std::cout << " ";
      }

      const static int cUnknown = -10;
      const static bool cDebugStack = false;

#include "gen/instruction_effects.hpp"

      bool before(opcode op, opcode arg1=0, opcode arg2=0) {
				BlockMap::iterator i = map_.find(current_ip_);
				if(i != map_.end()) {
					//					std::cout << "Found block at " << current_ip_ << "\n";
					if(i->second.sp == cUnknown) {
						if(cDebugStack) {
							debug_node();
							std::cout << current_ip_ << ": " << sp_ << " (inherit)\n";
						}
						i->second.sp = sp_;
					} else {
						sp_ = i->second.sp;
						if(cDebugStack) {
							debug_node();
							std::cout << current_ip_ << ": " << sp_ << " (reset)\n";
						}
					}

					current_block_ = &i->second;
				} 
				else {
					if(force_break_) {
						if(cDebugStack) {
							debug_node();
							std::cout << current_ip_ << ": dead\n";
						}
						return false;
					}

					if(cDebugStack) {
						debug_node();
						std::cout << current_ip_ << ": " << sp_ << "\n";
					}
				}

				// Update current_block everytime. When current_block changes,
				// previous current blocks will thereby contain their real end_ip
				current_block_->end_ip = current_ip_;

				force_break_ = false;
				if(sp_ != cUnknown) {
					sp_ += stack_difference(op, arg1, arg2);

					//Decrementing at traced ret, to pop off self
					if(op == InstructionSequence::insn_ret && cur_trace_node_->active_send){
						if(cDebugStack) std::cout << "Fixing!" << "\n";
						sp_--;
					}
				}

				return true;
      }

      JITBasicBlock* break_at(opcode ip) {
				if(cDebugStack) {
					std::cout << "Breaking at " << ip << "\n";
				}
				BlockMap::iterator i = map_.find(ip);
				if(i == map_.end()) {
					std::ostringstream ss;
					ss << "ip" << ip;
					JITBasicBlock& jbb = map_[ip];
					jbb.block = BasicBlock::Create(ls_->ctx(), ss.str().c_str(), function_);
					jbb.start_ip = ip;
					jbb.sp = sp_;

					// Aemon fix this!
					/*CFGBlock* cfg_block = cfg_.find_block(ip);
						assert(cfg_block);

						if(CFGBlock* handler = cfg_block->exception_handler()) {
						BlockMap::iterator hi = map_.find(handler->start_ip());
						assert(hi != map_.end());

						jbb.exception_handler = &hi->second;
						}*/


					// Assign the new block the current handler. This works
					// because code creates now blocks always within the
					// scope of the current handler and it's illegal for code
					// to generate a goto across a handler boundary
					if(exception_handlers_.size() > 0) {
						jbb.exception_handler = exception_handlers_.back();
					}

					if(ip < current_ip_) {
						jbb.end_ip = current_ip_;
					}

					if(cDebugStack) {
						std::cout << "patch " << ip << ": " << jbb.sp << "\n";
					}
					return &jbb;
				} else {
					if(cDebugStack) {
						std::cout << "Verifying that " << i->second.sp << " == " << sp_ << "\n";
					}
					//assert(i->second.sp == sp_);
					return &(i->second);
				}
      }

      void next_ip_too() {
				force_break_ = true;
      }

      void visit_goto(opcode which) {
				if(current_ip_ < which) loops_ = true;

				break_at(which);
				next_ip_too();
      }

      void visit_goto_if_true(opcode which) {
				if(current_ip_ < which) loops_ = true;

				break_at(which);
				break_at(current_ip_ + 2);
      }

      void visit_goto_if_false(opcode which) {
				if(current_ip_ < which) loops_ = true;

				break_at(which);
				break_at(current_ip_ + 2);
      }

      void visit_goto_if_defined(opcode which) {
				if(current_ip_ < which) loops_ = true;

				break_at(which);
				break_at(current_ip_ + 2);
      }

      void visit_setup_unwind(opcode which, opcode type) {}

      void visit_ret() {}

      void visit_raise_return() {
				next_ip_too();
      }

      void visit_raise_break() {
				next_ip_too();
      }

      void visit_ensure_return() {
				next_ip_too();
      }

      void visit_reraise() {
				next_ip_too();
      }

      void visit_raise_exc() {
				next_ip_too();
      }

      void visit_create_block(opcode which) {
				creates_blocks_ = true;
      }

      void check_for_eval(opcode which) {
				InlineCache* ic = reinterpret_cast<InlineCache*>(which);
				if(ic->name == s_eval_ ||
					 ic->name == s_binding_ ||
					 ic->name == s_class_eval_ ||
					 ic->name == s_module_eval_) {
					calls_evalish_ = true;
				}
      }

      void visit_send_stack(opcode which, opcode args) {
				check_for_eval(which);
				number_of_sends_++;
      }

      void visit_send_method(opcode which) {
				number_of_sends_++;
      }

      void visit_send_stack_with_block(opcode which, opcode args) {
				number_of_sends_++;
      }

      void visit_send_stack_with_splat(opcode which, opcode args) {
				check_for_eval(which);
				number_of_sends_++;
      }

      void visit_send_super_stack_with_block(opcode which, opcode args) {
				number_of_sends_++;
      }

      void visit_send_super_stack_with_splat(opcode which, opcode args) {
				number_of_sends_++;
      }

      void visit_zsuper(opcode which) {
				// HACK. zsuper accesses scope.
				calls_evalish_ = true;
				number_of_sends_++;
      }
    };



    class PassOneWalker {
      TracePassOne& v_;
      BlockMap& map_;

    public:
      PassOneWalker(TracePassOne& v, BlockMap& map)
				: v_(v)
				, map_(map)
      {}

      void call(Trace* trace, TraceNode* node){
				trace->dispatch(v_, node);
      }
    };

    void TraceBuilder::pass_one(BasicBlock* body) {

      // Pass 1, detect BasicBlock boundaries
      TracePassOne finder(ls_, block_map_, trace->init_trace_pc(), func, body);
      PassOneWalker walker(finder, block_map_);
      trace->walk(walker);

      if(finder.creates_blocks() || finder.calls_evalish()) {
				info_->set_use_full_scope();
				use_full_scope_ = true;
      }

      number_of_sends_ = finder.number_of_sends();
      loops_ = finder.loops_p();
    }


  }
}

#endif
