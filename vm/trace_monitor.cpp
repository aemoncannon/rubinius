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


	Trace::Status TraceMonitor::record_op(RECORD_PARAMS){
		SIMPLE_CANCEL_CHECKS();
		Trace::Status s = this->trace->add(
			op, pc, sp, ip_ptr, 
			state, vmm, call_frame, stack_ptr);
		if(s == Trace::TRACE_FINISHED){
			finish_trace();
		}
		return s;
	}

	void TraceMonitor::finish_trace(){
		DEBUGLN("Trace Recorded.\n--------------------------\n"); 
		IF_DEBUG(this->trace->pretty_print(state, std::cout));
		TRACK_TIME(TRACE_COMPILER_TIMER);
		this->trace->compile(state);
		TRACK_TIME(TRACE_SETUP_TIMER);
		this->trace->store();
		IF_DEBUG(this->trace->ultimate_parent()->dump_to_graph(state));
	}

	void TraceMonitor::cancel_trace_recording(){
		if(this->trace != NULL){
			delete this->trace;
			this->trace = NULL;
		}
	}

	void TraceMonitor::start_recording(){
		DEBUGLN("Initing new trace!");
		this->trace = new Trace();
	}

	TraceMonitor::TraceMonitor(VM* state)
		: 
		state(state),
		trace(NULL)
	{}


	int TraceMonitor::start_recording_nested_trace(RECORD_PARAMS){
		assert(this->trace);
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
			this->trace->add_nested_trace_call(nested_trace, trace_exit_pc,
																				 pc, sp, ip_ptr, state,
																				 vmm, call_frame, stack_ptr);
		}
		return result;
	}



	int TraceMonitor::run_trace(RECORD_PARAMS){

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
		
