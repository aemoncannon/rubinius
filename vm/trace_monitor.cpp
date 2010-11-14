#include "vm.hpp"
#include "call_frame.hpp"
#include "vmmethod.hpp"
#include "trace.hpp"
#include "builtin/iseq.hpp"
#include "builtin/symbol.hpp"
#include "vm/builtin/compiledmethod.hpp"
#include "trace.hpp"
#include "trace_monitor.hpp"
#include "utilities.hpp"

#include <iostream>
#include <sstream>

#ifdef ENABLE_LLVM
#include "llvm/jit_visit.hpp"
#include "llvm/jit.hpp"
#endif


namespace rubinius {


  TraceMonitor::TraceMonitor(VM* state)
    : 
		state(state),
    recording_trace(NULL)
	{}


	void TraceMonitor::cancel_trace_recording(){
		delete recording_trace; 
		recording_trace = NULL;
	}


	Trace::Status TraceMonitor::record_op(opcode op, int pc, int sp, void** const ip_ptr, STATE, VMMethod* const vmm, CallFrame* const call_frame, Object** stack_ptr){
		assert(recording_trace);
		Trace::Status s = recording_trace->add(op, pc, sp, ip_ptr, state, vmm, call_frame, stack_ptr);
    if(s == Trace::TRACE_FINISHED){
      DEBUGLN("Trace Recorded.\n--------------------------\n"); 
      IF_DEBUG(recording_trace->pretty_print(state, std::cout));
      TRACK_TIME(TRACE_COMPILER_TIMER);
      recording_trace->compile(state);
      TRACK_TIME(TRACE_SETUP_TIMER);
      recording_trace->store();
      IF_DEBUG(recording_trace->ultimate_parent()->dump_to_graph(state));
      recording_trace = NULL; 
    } 
    else if(s == Trace::TRACE_CANCEL){
			cancel_trace_recording();
    }
		return s;
	}

	int TraceMonitor::record_nested_trace(opcode op, int pc, int sp, void** const ip_ptr, STATE, VMMethod* const vmm, CallFrame* const call_frame, Object** stack_ptr){
		assert(recording_trace);
    Trace* nested_trace = vmm->traces[pc];
    DEBUGLN("Running nested trace while recording.\n"); 
    TRACK_TIME(ON_TRACE_TIMER);
    int result = nested_trace->executor(state, call_frame, 
																				nested_trace, NULL, NULL, 
																				Trace::RUN_MODE_RECORD_NESTED); 
    TRACK_TIME(TRACE_SETUP_TIMER);

    /* If result is -1, the nested trace must have bailed into */ 
    /* uncommon interpreter, we consider this recording invalidated.  */ 
    if(result == Trace::RETURN_SIDE_EXITED){
      DEBUGLN("Failed to record nested trace, throwing away recording\n"); 
			cancel_trace_recording();
    }
		else {
      /* Otherwise, we know that the */ 
      /* trace exited politely and we've successfully recorded a call to  */ 
      /* a nested trace. */
      DEBUGLN("Polite exit, saving nested trace..\n");
			int trace_exit_pc = call_frame->ip();
      recording_trace->add_nested_trace_call(nested_trace, trace_exit_pc,
																						 pc, sp, ip_ptr, state,
																						 vmm, call_frame, stack_ptr);
    }
		return result;
	}



	int TraceMonitor::run_trace(opcode op, int pc, int sp, void** const ip_ptr, STATE, VMMethod* const vmm, CallFrame* const call_frame, Object** stack_ptr){

    Trace* trace = vmm->traces[pc];
    assert(trace);
    DEBUGLN("\nRunning trace at " << pc);
    TRACK_TIME(ON_TRACE_TIMER);
    assert(trace->executor);

    int result = trace->executor(
			state, call_frame, trace, NULL, NULL, Trace::RUN_MODE_NORM); 

    TRACK_TIME(TRACE_SETUP_TIMER);

		DEBUGLN("Run finished.");
		DEBUGLN("Resuming at: " << call_frame->ip());
		IF_DEBUG(call_frame->dump());

		return result;
	}
	

}
		
