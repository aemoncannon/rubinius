#include "vm.hpp"
#include "call_frame.hpp"
#include "vmmethod.hpp"
#include "trace.hpp"
#include "builtin/iseq.hpp"
#include "builtin/symbol.hpp"
#include "vm/builtin/compiledmethod.hpp"

#ifdef ENABLE_LLVM
#include "llvm/jit.hpp"
#endif


namespace rubinius {


	TraceNode::TraceNode(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame)
		: op(op),
		  pc(pc),
		  cm(call_frame->cm),
		  ip_ptr(ip_ptr),
		  prev(NULL),
		  next(NULL)
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

    std:string trace_name(){
		TraceNode* tmp = anchor;
		return std:string("_TRACE_") + tmp->pc;
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



}
