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
		bool home_exit;
	};

}

#endif
