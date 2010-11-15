#ifndef RBX_TRACE_MONITOR_HPP
#define RBX_TRACE_MONITOR_HPP


using namespace std;

namespace llvm {
  class Function;
}

namespace rubinius {


  class VM;
  class VMMethod;
  class CallFrame;
  class CompiledMethod;
  class Object;
  class Class;
  class StackVariables;
  class Symbol;
  class JITVisit;
  class TraceInfo;
  class TraceNode;
  class Trace;
  class TraceIterator;

  typedef uintptr_t opcode;

  class TraceMonitorBase  {
	public:

		VM* state;
		Trace* recording_trace;

		TraceMonitorBase(VM* state);

		Trace::Status record_op(opcode op, int pc, int sp, 
														void** const ip_ptr, STATE, VMMethod* const vmm, 
														CallFrame* const call_frame, Object** stack_ptr);

#include "vm/gen/trace_monitor_record_ops.hpp"

		void cancel_trace_recording();

	};

	class TraceMonitor : public TraceMonitorBase{
	public:

		TraceMonitor(VM* state);

		void start_recording();

		bool is_recording() { 
			return this->recording_trace != NULL;
		}

		int start_recording_nested_trace(opcode op, int pc, int sp, 
																		 void** const ip_ptr, STATE, 
																		 VMMethod* const vmm, CallFrame* const call_frame, 
																		 Object** stack_ptr);
		int run_trace(opcode op, int pc, int sp, 
									void** const ip_ptr, STATE, VMMethod* const vmm, 
									CallFrame* const call_frame, Object** stack_ptr);


	};


}

#endif
