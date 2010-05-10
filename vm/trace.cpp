#include "call_frame.hpp"
#include "vmmethod.hpp"
#include "trace.hpp"

namespace rubinius {


	TraceNode::TraceNode(opcode op, void** ip_ptr, VMMethod* const vmm, CallFrame* const call_frame)
		: op(op)
	{

	}

	Trace::Trace(opcode op, void** ip_ptr, VMMethod* const vmm, CallFrame* const call_frame){
		anchor = new TraceNode(op, ip_ptr, vmm, call_frame);
		head = anchor;
	}


	void Trace::add(opcode op, void** ip_ptr, VMMethod* const vmm, CallFrame* const call_frame){
		TraceNode* tmp = new TraceNode(op, ip_ptr, vmm, call_frame);
		tmp->prev = head;
		head->next = tmp;
		head = tmp;
	}

	void Trace::pretty_print(std::ostream& out) {
		out << "[" << "\n";
		TraceNode* tmp = anchor;
		while(tmp != NULL){
			out << "(" << tmp->op << ")" << "\n";
			tmp = tmp->next;
		}
		out << "]" << "\n";
	}



}
