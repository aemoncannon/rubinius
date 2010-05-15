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
		void** ip_ptr;
		TraceNode* prev;
		TraceNode* next;
		int numargs;
		intptr_t arg1;
		intptr_t arg2;


		TraceNode(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void pretty_print(STATE, std::ostream& out);

	};


    class Trace {
    public:
		TraceNode* anchor;
		TraceNode* head;
		llvm::Function* llvm_function;
		size_t jitted_bytes;
		void*  jitted_impl;

		Trace(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		bool add(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void pretty_print(STATE, std::ostream& out);

	};



}

#endif
