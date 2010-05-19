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
		  ip_ptr(ip_ptr),
		  prev(NULL),
		  next(NULL),
			traced_send(false)
	{
#include "vm/gen/instruction_trace_record.hpp"
	}

	void TraceNode::pretty_print(STATE, std::ostream& out) {
		out << cm->name()->c_str(state) << " - " << pc  <<  ": ";
		out << InstructionSequence::get_instruction_name(op);
		out << " ";
		if(numargs > 0) out << arg1;
		if(numargs > 1) out << ", " << arg2;
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
