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

	class TraceMonitor {
	public:
		VM* state;
		Trace* trace;


#define	SIMPLE_CANCEL_CHECKS()   if(this->trace == NULL) {		\
			return Trace::TRACE_CANCEL;																				\
		}																																		\
		else if(this->trace->length >= Trace::MAX_TRACE_LENGTH) { \
			DEBUGLN("Canceling record due to return from home frame.");				\
			this->cancel_trace_recording();																		\
			return Trace::TRACE_CANCEL;																				\
		} 


#define RECORD_PARAMS opcode op,int pc,int sp,void** const ip_ptr,STATE, \
			VMMethod* const vmm,CallFrame* const call_frame,Object** stack_ptr


		TraceMonitor(VM* state);


		Trace::Status record_op(RECORD_PARAMS);


		void cancel_trace_recording();

		void finish_trace();

		void start_recording();


		bool is_recording() { 
			return this->trace != NULL;
		}

		int start_recording_nested_trace(RECORD_PARAMS);

		int run_trace(RECORD_PARAMS);


	};


}

#endif
