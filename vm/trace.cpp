#include "vm.hpp"
#include "call_frame.hpp"
#include "vmmethod.hpp"
#include "trace.hpp"
#include "builtin/iseq.hpp"
#include "builtin/symbol.hpp"
#include "vm/builtin/compiledmethod.hpp"
#include "trace.hpp"


#ifdef ENABLE_LLVM
#include "llvm/jit_visit.hpp"
#include "llvm/jit.hpp"
#endif


namespace rubinius {


	TraceNode::TraceNode(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame)
		: op(op),
		  pc(pc),
		  cm(call_frame->cm),
		  send_cm(NULL),
		  ip_ptr(ip_ptr),
		  prev(NULL),
		  next(NULL),
			traced_send(false),
			active_send(NULL),
			parent_send(NULL)
	{
#include "vm/gen/instruction_trace_record.hpp"
	}

	void TraceNode::pretty_print(STATE, std::ostream& out) {
		out << cm->name()->c_str(state) << " - " << pc  <<  ": ";
		out << InstructionSequence::get_instruction_name(op);
		out << " ";
		if(numargs > 0) out << arg1;
		if(numargs > 1) out << ", " << arg2;
		if(active_send){
			out << " : active(";
			active_send->pretty_print(state, out);
			out << ")";
		}
		if(parent_send){
			out << " : parent(";
			parent_send->pretty_print(state, out);
			out << ")";
		}
	}

	Trace::Trace(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame){
		anchor = new TraceNode(op, pc, ip_ptr, vmm, call_frame);
		head = anchor;
	}


	bool Trace::add(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame){
		if(ip_ptr == anchor->ip_ptr){
			head->next = anchor;
			head = anchor;
			return true;
		}
		else{
			TraceNode* tmp = new TraceNode(op, pc, ip_ptr, vmm, call_frame);
			tmp->prev = head;
			head->next = tmp;
			head = tmp;

			opcode prev_op = head->prev->op;

			if(head->prev){
				head->active_send = head->prev->active_send;
				head->parent_send = head->prev->parent_send;				
			}

			if(head->prev && head->cm != head->prev->cm){
				if(prev_op == InstructionSequence::insn_ret){
					if(head->prev->parent_send){
						head->active_send = head->prev->parent_send;
						head->parent_send = head->active_send->parent_send;
					}
					else{
						head->active_send = NULL;
						head->parent_send = NULL;
					}
				}
				else if(prev_op == InstructionSequence::insn_send_stack ||
								prev_op == InstructionSequence::insn_send_method){
					// The cm has changed and the previous
					// op was a send. We must be tracing into a call.
					head->prev->traced_send = true;
					head->prev->send_cm = head->cm;
					head->parent_send = head->prev->active_send;
					head->active_send = head->prev;
				}

			}


			return false;
		}
	}

	void Trace::compile(STATE) {
		LLVMState* ls = LLVMState::get(state);
		ls->compile_trace(state, this);
	}

	string Trace::trace_name(){
		return string("_TRACE_");
	}

	void Trace::pretty_print(STATE, std::ostream& out) {
		out << "[" << "\n";
		TraceNode* tmp = anchor;
		while(tmp != NULL){
			tmp->pretty_print(state, out);
			out << "\n";
			tmp = tmp->next;
			if(tmp == anchor) break;
		}
		out << "]" << "\n";
	}





	// while(!work_list_.empty()) {
	// 	int ip = work_list_.back();
	// 	work_list_.pop_back();

	// 	iter.switch_to(ip);

	// 	while(seen_[iter.ip()] == 0) {
	// 		seen_[iter.ip()] = 1;

	// 		each.call(iter);

	// 		if(iter.goto_p()) {
	// 			opcode target = iter.goto_target();
	// 			assert(target >= 0 && target < vmm_->total);

	// 			add_section(target);

	// 			// Non-terminating goto's stop the current block and queue the code
	// 			// right after them.
	// 			if(!iter.terminator_p() && iter.next_p()) add_section(iter.next_ip());
	// 			break;
	// 		}

	// 		if(iter.terminator_p()) break;

	// 		if(!iter.next_p()) break;
	// 		iter.advance();
	// 	}
	//		}




}
