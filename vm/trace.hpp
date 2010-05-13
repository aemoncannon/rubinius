#ifndef RBX_TRACE_HPP
#define RBX_TRACE_HPP


namespace llvm {
  class Function;
}

namespace rubinius {

	typedef uintptr_t opcode;

	class VM;
	class VMMethod;
	class CallFrame;
	class CompiledMethod;
	class Symbol;

	class TraceNode {
    public:
		opcode op;
		int pc;
		CompiledMethod* cm;
		int numargs;
		intptr_t arg1;
		intptr_t arg2;
		TraceNode* prev;
		TraceNode* next;


		TraceNode(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void pretty_print(STATE, std::ostream& out);

	};


    class Trace {
    public:
		TraceNode* anchor;
		TraceNode* head;
		llvm::Function* llvm_function_;
		size_t jitted_bytes_;
		void*  jitted_impl_;

		Trace(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void add(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void pretty_print(STATE, std::ostream& out);

	};



}

#endif
