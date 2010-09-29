#include "builtin/object.hpp"
#include "builtin/array.hpp"
#include "builtin/autoload.hpp"
#include "builtin/block_environment.hpp"
#include "builtin/class.hpp"
#include "builtin/compiledmethod.hpp"
#include "builtin/exception.hpp"
#include "builtin/fixnum.hpp"
#include "builtin/string.hpp"
#include "builtin/symbol.hpp"
#include "builtin/taskprobe.hpp"
#include "builtin/tuple.hpp"
#include "builtin/iseq.hpp"
#include "builtin/staticscope.hpp"
#include "builtin/nativemethod.hpp"
#include "builtin/lookuptable.hpp"
#include "builtin/proc.hpp"
#include "builtin/thread.hpp"
#include "builtin/system.hpp"
#include "builtin/global_cache_entry.hpp"
#include "builtin/iseq.hpp"

#include "call_frame.hpp"
#include "trace_info.hpp"

#include "objectmemory.hpp"
#include "arguments.hpp"
#include "dispatch.hpp"
#include "instructions.hpp"
#include "instruments/profiler.hpp"

#include "helpers.hpp"
#include "inline_cache.hpp"

#include "vm/gen/instruction_defines.hpp"

#define interp_assert(code) if(!(code)) { Exception::internal_error(state, call_frame, "assertion failed: " #code); RUN_EXCEPTION(); }

using namespace rubinius;

#define STACK_PTR stack_ptr

/** We have to use the local here we need to evaluate val before we alter
 * the stack. The reason is evaluating val might throw an exception. The
 * old code used an undefined behavior, this forces the order. */
#define stack_push(val) ({ Object* __stack_v = (val); *++STACK_PTR = __stack_v; })
#define stack_pop() *STACK_PTR--
#define stack_set_top(val) *STACK_PTR = (val)

#define USE_JUMP_TABLE

#define stack_top() *STACK_PTR
#define stack_back(count) *(STACK_PTR - count)
#define stack_clear(count) STACK_PTR -= count

#define stack_position(where) (STACK_PTR = call_frame->stk + where)
#define stack_calculate_sp() (STACK_PTR - call_frame->stk)

#define stack_local(which) call_frame->stk[vmm->stack_size - which - 1]

#define next_int ((opcode)(stream[call_frame->inc_ip()]))

#define both_fixnum_p(_p1, _p2) ((uintptr_t)(_p1) & (uintptr_t)(_p2) & TAG_FIXNUM)

extern "C" {
  Object* send_slowly(STATE, VMMethod* vmm, InterpreterCallFrame* const call_frame,
                      Symbol* name, Object** stk_pos, size_t args);

#define HANDLE_EXCEPTION(val) if(val == NULL) goto exception
#define RUN_EXCEPTION() goto exception

#define SET_CALL_FLAGS(val) is.call_flags = (val)
#define CALL_FLAGS() is.call_flags

#define SET_ALLOW_PRIVATE(val) is.allow_private = (val)
#define ALLOW_PRIVATE() is.allow_private

#define stack_back_position(count) (STACK_PTR - (count - 1))

  Object* send_slowly(STATE, VMMethod* vmm, InterpreterCallFrame* const call_frame,
                      Symbol* name, Object** stk_pos, size_t count)
  {
    Object* recv = stk_pos[0];
    Arguments args(recv, count, stk_pos+1);
    Dispatch dis(name);

    return dis.send(state, call_frame, args);
  }
}


Object* VMMethod::interpreter(STATE,
                              VMMethod* const vmm,
                              InterpreterCallFrame* const call_frame)
{

#include "vm/gen/instruction_locations.hpp"

  if(unlikely(state == 0)) {
    VMMethod::instructions = const_cast<void**>(insn_locations);
    return NULL;
  }

  InterpreterState is;

#ifdef X86_ESI_SPEEDUP
  register void** ip_ptr asm ("esi") = vmm->addresses;
#else
  register void** ip_ptr = vmm->addresses;
#endif

  Object** stack_ptr = call_frame->stk - 1;

  int current_unwind = 0;
  UnwindInfo unwinds[kMaxUnwindInfos];

  if(!state->check_stack(call_frame, &state)) return NULL;

  if(unlikely(state->interrupts.check)) {
    state->interrupts.checked();
    if(state->interrupts.perform_gc) {
      state->interrupts.perform_gc = false;
      state->collect_maybe(call_frame);
    }
  }

  if(unlikely(state->check_local_interrupts)) {
    if(!state->process_async(call_frame)) return NULL;
  }

	opcode op;
	int cur_ip;
	int sp;


	goto continue_to_run;


 run_trace:
	{
		TraceInfo ti; 
		ti.entry_call_frame = call_frame; 
		ti.recording = false; 
		ti.nested = false; 
		call_frame->dump();
		std::cout << "Running trace." << endl; 
		Object* ret = vmm->traces[cur_ip]->executor(state, call_frame, &ti); 
		std::cout << "Run finished." << endl; 
		/* If traceinfo answers false to nestable, the trace must have bailed into */ 
		/* uncommon interpreter, which will have already finished  */ 
		/* interpreting this invocation, so we pop this frame. */ 
		if(!(ti.nestable)){
			std::cout << "Exit at trace_pc: " << ti.exit_trace_pc << endl; 
			return ret; 
		} 
		std::cout << "Polite exit." << endl; 
		/* Otherwise, we know that the */ 
		/* trace exited politely, and we can keep rolling with the */ 
		/* same call_frame. */ 
		std::cout << "Resuming at: " << call_frame->ip() << endl; 
		ip_ptr = vmm->addresses + call_frame->ip(); 
		stack_ptr = call_frame->stk + call_frame->sp();
		goto continue_to_run; 
	}

 record_op:
	{
		sp = stack_ptr - call_frame->stk;
		Trace::Status s = state->recording_trace->add(op, cur_ip, sp, ip_ptr, vmm, call_frame); 
		if(s == Trace::TRACE_FINISHED){
			std::cout << "Trace finished.\n"; 
			state->recording_trace->compile(state); 
			std::cout << "Trace Compiled.\n"; 
			state->recording_trace->pretty_print(state, std::cout); 
			vmm->traces[cur_ip] = state->recording_trace; 
			state->recording_trace = NULL; 
		}																																
		else if(s == Trace::TRACE_CANCEL){
			delete state->recording_trace; 
			state->recording_trace = NULL;																
		}
		goto **ip_ptr++;
	}

 record_nested_trace:
	{
		/* Add a virtual op that will cause call of nested trace to be emitted */ 
		sp = stack_ptr - call_frame->stk;			 
		state->recording_trace->add(
			InstructionSequence::insn_nested_trace, cur_ip, sp, ip_ptr, vmm, call_frame); 
		TraceInfo ti; 
		ti.entry_call_frame = call_frame; 
		ti.recording = true; 
		std::cout << "Running nested trace while recording.\n"; 
		Object* ret = vmm->traces[cur_ip]->executor(state, call_frame, &ti); 
		/* If traceinfo answers false to nestable, the nested trace must have bailed into */ 
		/* uncommon interpreter, we consider this recording invalidated.  */ 
		/* Pop the frame  */ 
		if(!(ti.nestable)){
			std::cout << "Exit at trace_pc: " << ti.exit_trace_pc << "\n"; 
			std::cout << "Failed to record nested trace, throwing away recording\n"; 
			delete state->recording_trace; 
			state->recording_trace = NULL;																
			return ret; 
		}																																
		std::cout << "Polite exit.\n"; 
		/* Otherwise, we know that the */ 
		/* trace exited politely, and A) we can keep rolling with the */ 
		/* same call_frame, B) we've successfully recorded a call to  */ 
		/* a nested trace. */ 
		vmm->traces[cur_ip]->expected_exit_ip = ti.exit_ip; 
		ip_ptr = vmm->addresses + call_frame->ip(); 
		stack_ptr = call_frame->stk + call_frame->sp();
		goto continue_to_run; 
	}

	

 continue_to_run:
  try {

#undef DISPATCH
//#define DISPATCH goto **ip_ptr++;
#define DISPATCH  cur_ip = ip_ptr - vmm->addresses;											\
	  op = vmm->opcodes[cur_ip];																					\
		if(state->tracing_enabled) {																				\
			if(state->recording_trace == NULL && vmm->traces[cur_ip] != NULL){ \
				/*Not currently recording. Hit an ip with a stored trace...*/		\
				goto run_trace;																									\
			}																																	\
			else if(state->recording_trace != NULL && vmm->traces[cur_ip] != NULL){ \
				/*Recording. Hit an ip with a stored trace...*/									\
				goto record_nested_trace;																				\
			}																																	\
			else if(state->recording_trace != NULL){													\
				/* Normal recording...*/																				\
				goto record_op;																									\
			}																																	\
			else if(op == InstructionSequence::insn_goto){										\
				/* Check for backward gotos, increment corresponding counter.*/	\
				intptr_t location = (intptr_t)(*(ip_ptr + 1));									\
				if(location < cur_ip){																					\
					vmm->trace_counters[location]++;															\
				}																																\
			}																																	\
			else{																															\
				/* Start recording after threshold is hit..*/										\
				if(vmm->trace_counters[cur_ip] > 50){														\
					std::cout << "Start recording trace.\n";											\
					sp = stack_ptr - call_frame->stk;													\
					state->recording_trace = new Trace(op, cur_ip, sp, ip_ptr, vmm, call_frame); \
				}																																\
			}																																	\
		}																																		\
		goto **ip_ptr++;

#undef next_int
#define next_int ((opcode)(*ip_ptr++))

#define cache_ip(which) ip_ptr = vmm->addresses + which
#define flush_ip() call_frame->calculate_ip(ip_ptr)

#include "vm/gen/instruction_implementations.hpp"

	} catch(TypeError& e) {
		flush_ip();
		Exception* exc =
			Exception::make_type_error(state, e.type, e.object, e.reason);
		exc->locations(state, System::vm_backtrace(state, Fixnum::from(0), call_frame));

		state->thread_state()->raise_exception(exc);
		call_frame->scope->flush_to_heap(state);
		return NULL;
	} catch(const RubyException& exc) {
		exc.exception->locations(state,
														 System::vm_backtrace(state, Fixnum::from(0), call_frame));
		state->thread_state()->raise_exception(exc.exception);
		return NULL;
	}

// There is no reason to be here. Either the bytecode loop exits,
// or it jumps to exception;
	abort();

// If control finds it's way down here, there is an exception.
 exception:
	ThreadState* th = state->thread_state();
//
	switch(th->raise_reason()) {
	case cException:
		if(current_unwind > 0) {
			UnwindInfo* info = &unwinds[--current_unwind];
			stack_position(info->stack_depth);
			call_frame->set_ip(info->target_ip);
			cache_ip(info->target_ip);
			goto continue_to_run;
		} else {
			call_frame->scope->flush_to_heap(state);
			return NULL;
		}

	case cBreak:
		// If we're trying to break to here, we're done!
		if(th->destination_scope() == call_frame->scope->on_heap()) {
			stack_push(th->raise_value());
			th->clear_break();
			goto continue_to_run;
			// Don't return here, because we want to loop back to the top
			// and keep running this method.
		}

		// Otherwise, fall through and run the unwinds
	case cReturn:
	case cCatchThrow:
		// Otherwise, we're doing a long return/break unwind through
		// here. We need to run ensure blocks.
		while(current_unwind > 0) {
			UnwindInfo* info = &unwinds[--current_unwind];
			if(info->for_ensure()) {
				stack_position(info->stack_depth);
				call_frame->set_ip(info->target_ip);
				cache_ip(info->target_ip);

				// Don't reset ep here, we're still handling the return/break.
				goto continue_to_run;
			}
		}

		// Ok, no ensures to run.
		if(th->raise_reason() == cReturn) {
			call_frame->scope->flush_to_heap(state);

			// If we're trying to return to here, we're done!
			if(th->destination_scope() == call_frame->scope->on_heap()) {
				Object* val = th->raise_value();
				th->clear_return();
				return val;
			} else {
				// Give control of this exception to the caller.
				return NULL;
			}

		} else { // Not for us!
			call_frame->scope->flush_to_heap(state);
			// Give control of this exception to the caller.
			return NULL;
		}

	case cExit:
		call_frame->scope->flush_to_heap(state);
		return NULL;
	default:
		break;
	} // switch

	std::cout << "bug!\n";
	call_frame->print_backtrace(state);
	abort();
	return NULL;
}

Object* VMMethod::uncommon_interpreter(STATE,
                                       VMMethod* const vmm_,
                                       CallFrame* const call_frame_)
{

#include "vm/gen/instruction_locations.hpp"
	
	VMMethod* vmm = vmm_;
	CallFrame* call_frame = call_frame_;
  opcode* stream = vmm->opcodes;
  InterpreterState is;
  Object** stack_ptr = call_frame->stk + call_frame->sp();

  int current_unwind = 0;
  UnwindInfo unwinds[kMaxUnwindInfos];

  if(!state->check_stack(call_frame, &state)) return NULL;

  if(unlikely(state->interrupts.check)) {
    state->interrupts.checked();
    if(state->interrupts.perform_gc) {
      state->interrupts.perform_gc = false;
      state->collect_maybe(call_frame);
    }
  }

  if(unlikely(state->check_local_interrupts)) {
    if(!state->process_async(call_frame)) return NULL;
  }

  std::cout << "Entering uncommon at " << call_frame->cm->name()->c_str(state) << endl;
  std::cout << "Ip at " << call_frame->ip() << endl;
  std::cout << "Stack at " << call_frame->sp() << endl;
	opcode op;

 continue_to_run:
  try {

#undef DISPATCH
#define DISPATCH op = stream[call_frame->inc_ip()];	\
    if(op == InstructionSequence::insn_ret &&				\
			 call_frame->is_traced_frame()) {							\
			call_frame = call_frame->previous;						\
			stack_ptr = call_frame->stk + call_frame->sp(); \
			std::cout << "Ip at " << call_frame->ip() << endl;		\
			std::cout << "Stack at " << call_frame->sp() << endl; \
			vmm = call_frame->cm->backend_method();				\
			stream = vmm->opcodes;												\
			goto continue_to_run;													\
		}																								\
		goto *insn_locations[op];

#undef next_int
#undef cache_ip
#undef flush_ip

#define next_int ((opcode)(stream[call_frame->inc_ip()]))
#define cache_ip(which)
#define flush_ip()

#include "vm/gen/instruction_implementations.hpp"

	} catch(TypeError& e) {
		flush_ip();
		Exception* exc =
			Exception::make_type_error(state, e.type, e.object, e.reason);
		exc->locations(state, System::vm_backtrace(state, Fixnum::from(0), call_frame));

		state->thread_state()->raise_exception(exc);
		call_frame->scope->flush_to_heap(state);
		return NULL;
	} catch(const RubyException& exc) {
		exc.exception->locations(state,
														 System::vm_backtrace(state, Fixnum::from(0), call_frame));
		state->thread_state()->raise_exception(exc.exception);
		return NULL;
	}

// No reason to be here!
	abort();

 exception:
	ThreadState* th = state->thread_state();
//
	switch(th->raise_reason()) {
	case cException:
		if(current_unwind > 0) {
			UnwindInfo* info = &unwinds[--current_unwind];
			stack_position(info->stack_depth);
			call_frame->set_ip(info->target_ip);
			cache_ip(info->target_ip);
			goto continue_to_run;
		} else {
			call_frame->scope->flush_to_heap(state);
			return NULL;
		}

	case cBreak:
		// If we're trying to break to here, we're done!
		if(th->destination_scope() == call_frame->scope->on_heap()) {
			stack_push(th->raise_value());
			th->clear_break();
			goto continue_to_run;
			// Don't return here, because we want to loop back to the top
			// and keep running this method.
		}

		// Otherwise, fall through and run the unwinds
	case cReturn:
	case cCatchThrow:
		// Otherwise, we're doing a long return/break unwind through
		// here. We need to run ensure blocks.
		while(current_unwind > 0) {
			UnwindInfo* info = &unwinds[--current_unwind];
			if(info->for_ensure()) {
				stack_position(info->stack_depth);
				call_frame->set_ip(info->target_ip);
				cache_ip(info->target_ip);

				// Don't reset ep here, we're still handling the return/break.
				goto continue_to_run;
			}
		}

		// Ok, no ensures to run.
		if(th->raise_reason() == cReturn) {
			call_frame->scope->flush_to_heap(state);

			// If we're trying to return to here, we're done!
			if(th->destination_scope() == call_frame->scope->on_heap()) {
				Object* val = th->raise_value();
				th->clear_return();
				return val;
			} else {
				// Give control of this exception to the caller.
				return NULL;
			}

		} else { // It's cBreak thats not for us!
			call_frame->scope->flush_to_heap(state);
			// Give control of this exception to the caller.
			return NULL;
		}

	case cExit:
		call_frame->scope->flush_to_heap(state);
		return NULL;
	default:
		break;
	} // switch

	std::cout << "bug!\n";
	call_frame->print_backtrace(state);
	abort();
	return NULL;
}


/* The debugger interpreter loop is used to run a method when a breakpoint
 * has been set. It has additional overhead, since it needs to inspect
 * each opcode for the breakpoint flag. It is installed on the VMMethod when
 * a breakpoint is set on compiled method.
 */
Object* VMMethod::debugger_interpreter(STATE,
                                       VMMethod* const vmm,
                                       InterpreterCallFrame* const call_frame)
{

#include "vm/gen/instruction_locations.hpp"

  opcode* stream = vmm->opcodes;
  InterpreterState is;

  int current_unwind = 0;
  UnwindInfo unwinds[kMaxUnwindInfos];

  opcode op;

  // TODO: ug, cut and paste of the whole interpreter above. Needs to be fast,
  // maybe could use a function template?
  //
  // The only thing different is the DISPATCH macro, to check for debugging
  // instructions.

  Object** stack_ptr = call_frame->stk - 1;

  if(!state->check_stack(call_frame, &state)) return NULL;

  if(unlikely(state->interrupts.check)) {
    state->interrupts.checked();
    if(state->interrupts.perform_gc) {
      state->interrupts.perform_gc = false;
      state->collect_maybe(call_frame);
    }
  }

  if(unlikely(state->check_local_interrupts)) {
    if(!state->process_async(call_frame)) return NULL;
  }

 continue_to_run:
  try {

#undef DISPATCH
#define DISPATCH op = stream[call_frame->inc_ip()]; \
    if(unlikely(op & cBreakpoint)) {								\
      call_frame->dec_ip();													\
      Helpers::yield_debugger(state, call_frame);		\
      call_frame->inc_ip();													\
      op &= 0x00ffffff;															\
    }																								\
    goto *insn_locations[op];

#undef next_int
#undef cache_ip
#undef flush_ip

#define next_int ((opcode)(stream[call_frame->inc_ip()]))
#define cache_ip(which)
#define flush_ip()

#include "vm/gen/instruction_implementations.hpp"

  } catch(TypeError& e) {
    flush_ip();
    Exception* exc =
      Exception::make_type_error(state, e.type, e.object, e.reason);
    exc->locations(state, System::vm_backtrace(state, Fixnum::from(0), call_frame));

    state->thread_state()->raise_exception(exc);
    call_frame->scope->flush_to_heap(state);
    return NULL;
  } catch(const RubyException& exc) {
    exc.exception->locations(state,
														 System::vm_backtrace(state, Fixnum::from(0), call_frame));
    state->thread_state()->raise_exception(exc.exception);
    return NULL;
  }

  // no reason to be here!
  abort();

  // If control finds it's way down here, there is an exception.
 exception:
  ThreadState* th = state->thread_state();
  //
  switch(th->raise_reason()) {
  case cException:
    if(current_unwind > 0) {
      UnwindInfo* info = &unwinds[--current_unwind];
      stack_position(info->stack_depth);
      call_frame->set_ip(info->target_ip);
      cache_ip(info->target_ip);
      goto continue_to_run;
    } else {
      call_frame->scope->flush_to_heap(state);
      return NULL;
    }

  case cBreak:
    // If we're trying to break to here, we're done!
    if(th->destination_scope() == call_frame->scope->on_heap()) {
      stack_push(th->raise_value());
      th->clear_break();
      goto continue_to_run;
      // Don't return here, because we want to loop back to the top
      // and keep running this method.
    }

    // Otherwise, fall through and run the unwinds
  case cReturn:
  case cCatchThrow:
    // Otherwise, we're doing a long return/break unwind through
    // here. We need to run ensure blocks.
    while(current_unwind > 0) {
      UnwindInfo* info = &unwinds[--current_unwind];
      stack_position(info->stack_depth);

      if(info->for_ensure()) {
        stack_position(info->stack_depth);
        call_frame->set_ip(info->target_ip);
        cache_ip(info->target_ip);

        // Don't reset ep here, we're still handling the return/break.
        goto continue_to_run;
      }
    }

    // Ok, no ensures to run.
    if(th->raise_reason() == cReturn) {
      call_frame->scope->flush_to_heap(state);

      // If we're trying to return to here, we're done!
      if(th->destination_scope() == call_frame->scope->on_heap()) {
        Object* val = th->raise_value();
        th->clear_return();
        return val;
      } else {
        // Give control of this exception to the caller.
        return NULL;
      }

    } else { // It's cBreak thats not for us!
      call_frame->scope->flush_to_heap(state);
      // Give control of this exception to the caller.
      return NULL;
    }

  case cExit:
    call_frame->scope->flush_to_heap(state);
    return NULL;
  default:
    break;
  } // switch

  std::cout << "bug!\n";
  call_frame->print_backtrace(state);
  abort();
  return NULL;
}
