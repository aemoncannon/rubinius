#ifndef RBX_TRACE_INFO_HPP
#define RBX_TRACE_INFO_HPP

using namespace std;

namespace rubinius {

	class VM;
	class VMMethod;
	class CallFrame;
	class CompiledMethod;
	class Object;
	class StackVariables;
	class Symbol;
	class JITVisit;

	class TraceInfo {
	public:
		int exit_ip;
		Object** exit_stack;
		CallFrame* exit_call_frame;
		CallFrame* entry_call_frame;
		int expected_exit_ip;
		int nestable;
		int recording;
		int nested;
		int next_ip;
		int exit_trace_pc;


		TraceInfo();
	};

}

#endif
