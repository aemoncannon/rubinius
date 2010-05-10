#ifndef RBX_TRACE_HPP
#define RBX_TRACE_HPP

namespace rubinius {

	typedef uintptr_t opcode;

	class VMMethod;
	class CallFrame;



	class TraceNode {
    public:
		opcode op;
		intptr_t operand1;
		intptr_t operand2;
		TraceNode* prev;
		TraceNode* next;

		TraceNode(opcode op, void** ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

	};


    class Trace {
    public:
		TraceNode* anchor;
		TraceNode* head;

		Trace(opcode op, void** ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void add(opcode op, void** ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void pretty_print(std::ostream& out);


	};



}

#endif
