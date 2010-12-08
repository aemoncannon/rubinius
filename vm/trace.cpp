#include "vm.hpp"
#include "call_frame.hpp"
#include "vmmethod.hpp"
#include "trace.hpp"
#include "builtin/iseq.hpp"
#include "builtin/symbol.hpp"
#include "vm/builtin/compiledmethod.hpp"
#include "trace.hpp"
#include "utilities.hpp"

#include <iostream>
#include <sstream>

#ifdef ENABLE_LLVM
#include "llvm/jit_visit.hpp"
#include "llvm/jit.hpp"
#endif


namespace rubinius {


  TraceNode::TraceNode(STATE, int depth, int trace_pc, opcode op, int pc, int sp, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame)
    : 
    nested_trace(NULL),
    nested_executor(NULL),
		branch_tbl_offset(0),
    op(op),
    pc(pc),
    sp(sp),
		stck_effect(0),
		pc_effect(0),
    call_frame(call_frame),
		cm(state),
		target_cm(state),
		block_cm(state),
    target_klass(state),
    ip_ptr(ip_ptr),
    prev(NULL),
    next(NULL),
    traced_send(false),
    traced_yield(false),
    active_send(NULL),
    parent_send(NULL),
    trace_pc(trace_pc),
    call_depth(depth),
    jump_taken(false),
    exit_counter(0),
    side_exit_pc(-1),
		numargs(0),
		arg1(0),
		arg2(0)
  {
		cm.set(call_frame->cm);
		for(int i = 0; i < BRANCH_TBL_SIZE; i++) {
			branches[i] = NULL;
			branch_keys[i] = -1;
		}
  }

  std::string TraceNode::cm_name(STATE){
    std::string result = cm->name()->c_str(state);
    return result;
  }

  std::string TraceNode::op_name(){
    return InstructionSequence::get_instruction_name(op);
  }

  int TraceNode::send_arg_count(){
    if(op == InstructionSequence::insn_send_method) return 0;
    else return arg2;
  }

  std::string TraceNode::graph_node_name(STATE) {
    assert(next);
    std::stringstream s;
    s << "\"" << cm_name(state) << "-" << pc << " " << op_name() << "\"";
    std::string result = s.str();
    return result;
  }

	bool TraceNode::bump_exit_hotness(){
		if(exit_counter != Trace::COUNTER_DISABLED){
			exit_counter++;
			return exit_counter > Trace::BRANCH_RECORD_THRESHOLD;
		}
		return false;
	}

	void TraceNode::disable_counter(){
		exit_counter = Trace::COUNTER_DISABLED;
	}

  void TraceNode::pretty_print(STATE, std::ostream& out) {
		std::string op_nm = InstructionSequence::get_instruction_name(op);
    if(state != NULL){
      out << trace_pc << "(" << pc << ") " << cm->name()->c_str(state) << "  " << op_nm << ", ln=" << cm->line(state, pc)  << ", sp=" << sp;
    }
    else{
      out << trace_pc << "  ___" << "  " << op_nm << ", pc=" << pc << ", sp=" << sp;
    }

    if(numargs > 0) out << ", arg1=" << arg1;
    if(numargs > 1) out << ", arg2=" << arg2;
    if(active_send){
      out << ", active_send(";
      out << active_send->trace_pc;
      out << ")";
    }
    if(parent_send){
      out << ", parent_send(";
      out << parent_send->trace_pc;
      out << ")";
    }
  }


  // Standard constructor for initializing a new trace
  Trace* Trace::newTrace(opcode op, int pc, int sp, void** const ip_ptr, STATE, VMMethod* const vmm, CallFrame* const call_frame, Object** stack_ptr)
  {
		Trace* trace = new Trace();
		trace->add(op, pc, sp, ip_ptr, state, vmm, call_frame, stack_ptr);
		trace->anchor = trace->head;
		return trace;
  }


  // Simple null constructor used for creating branch
  // and nested traces
  Trace::Trace()  : 
    executor(NULL)
    ,expected_exit_ip(-1)
    ,anchor(NULL)
    ,head(NULL)
    ,entry(NULL)
    ,jitted_bytes(-1)
    ,entry_sp(-1)
    ,parent(NULL)
    ,parent_node(NULL)
    ,is_nested_trace(false)
    ,is_branch_trace(false)
    ,length(0)
    ,trace_pc_counter(0)
  {}


  Trace* Trace::create_branch_at(TraceNode* exit_node){
    Trace* branch = new Trace();
    branch->parent = this;
    branch->parent_node = exit_node;

    TraceNode* anchor = new TraceNode(*(this->anchor));
    branch->anchor = anchor;

    branch->expected_exit_ip = -1;
    branch->is_branch_trace = true;

    // Entry and head will be initialized
    // on the first call to add(...)
    branch->entry = NULL;
    branch->head = NULL;

    return branch;
  }

  Trace::Status Trace::add_nested_trace_call(Trace* nested_trace, 
																						 int nested_exit_pc, int pc, int sp, 
																						 void** const ip_ptr, 
																						 STATE, VMMethod* const vmm, 
																						 CallFrame* const call_frame,
																						 Object** stack_ptr){

    this->add(InstructionSequence::insn_nested_trace, pc, sp, ip_ptr, state, 
							vmm, call_frame, stack_ptr);
    TraceNode* adapter_node = this->head;

    adapter_node->nested_executor = nested_trace->executor;

    // Create a copy, modified to suit the context of the nesting
    Trace* nested = new Trace(*nested_trace);
    nested->is_nested_trace = true;
    nested->parent = this;
    nested->parent_node = adapter_node;
    nested->expected_exit_ip = nested_exit_pc;
    adapter_node->nested_trace = nested;

    return TRACE_OK;
  }


  Trace::Status Trace::add(opcode op, int pc, int sp, void** const ip_ptr, STATE, VMMethod* const vmm, CallFrame* const call_frame, Object** stack_ptr){

    if(length >= MAX_TRACE_LENGTH){
      DEBUGLN("Canceling record due to exceeded max trace length of " << MAX_TRACE_LENGTH);
      return TRACE_CANCEL;
    }

		TraceNode* prev = head;
		TraceNode* active_send = NULL;
		TraceNode* parent_send = NULL;
		int call_depth = 0;

		int side_exit_pc = pc;
		Class* target_klass = NULL;
		CompiledMethod* target_cm = NULL;
		CompiledMethod* block_cm = NULL;

		int arg1 = 0;
		int arg2 = 0;
		int trace_pc = assign_trace_pc(pc, call_frame);
		int numargs = 0;
		int stck_effect = 0;
		int pc_effect = 0;
		bool is_term = false;

		// Inherit properties from previous
		// node.
		if(prev){
			active_send = prev->active_send;
			parent_send = prev->parent_send;
			call_depth = prev->call_depth;
		}

		switch(op){
		case InstructionSequence::insn_push_int: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_push_self: {
			numargs = 0;
			break;
		}
		case InstructionSequence::insn_set_literal: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_push_literal: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_goto: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			if(arg1 == anchor->pc && 
				 call_depth == anchor->call_depth &&
				 call_frame->cm == anchor->cm.get()){
				is_term = true;
			}
			break;
		}
		case InstructionSequence::insn_goto_if_false: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_goto_if_true: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_ret: {
			if(call_depth == anchor->call_depth && 
				 call_frame->cm == anchor->cm.get()){
				DEBUGLN("Canceling record due to return from home frame.");
				return TRACE_CANCEL;
			}
			numargs = 0;
			break;
		}
		case InstructionSequence::insn_swap_stack: {
			numargs = 0;
			break;
		}
		case InstructionSequence::insn_dup_top: {
			numargs = 0;
			break;
		}
		case InstructionSequence::insn_dup_many: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_pop: {
			numargs = 0;
			break;
		}
		case InstructionSequence::insn_pop_many: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_rotate: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_move_down: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_set_local: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_push_local: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_push_local_depth: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			arg2 = (intptr_t)(*(ip_ptr + 2));
			numargs = 2;
			break;
		}
		case InstructionSequence::insn_set_local_depth: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			arg2 = (intptr_t)(*(ip_ptr + 2));
			numargs = 2;
			break;
		}
		case InstructionSequence::insn_passed_arg: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_setup_unwind: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			arg2 = (intptr_t)(*(ip_ptr + 2));
			numargs = 2;
			break;
		}

		case InstructionSequence::insn_make_array: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}

		case InstructionSequence::insn_set_ivar: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_push_ivar: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_push_const: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_set_const: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_set_const_at: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_find_const: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_push_cpath_top: {
			numargs = 0;
			break;
		}
		case InstructionSequence::insn_push_const_fast: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			arg2 = (intptr_t)(*(ip_ptr + 2));
			numargs = 2;
			break;
		}
		case InstructionSequence::insn_set_call_flags: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_send_method: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			stck_effect = -1;
			pc_effect = 2;

			int send_args = 0;
			Object* recv = *(stack_ptr - send_args);
			target_klass = recv->lookup_begin(state);
			break;
		}
		case InstructionSequence::insn_send_stack: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			arg2 = (intptr_t)(*(ip_ptr + 2));
			numargs = 2;
			stck_effect = -arg2 - 1;
			pc_effect = 3;

			int send_args = (intptr_t)(*(ip_ptr + 2));
			Object* recv = *(stack_ptr - send_args);
			target_klass = recv->lookup_begin(state);
			break;
		}
		case InstructionSequence::insn_send_stack_with_block: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			arg2 = (intptr_t)(*(ip_ptr + 2));
			numargs = 2;
			stck_effect = -arg2 - 2;
			pc_effect = 3;
			
			int send_args = (intptr_t)(*(ip_ptr + 2));
			Object* recv = *(stack_ptr - send_args);
			target_klass = recv->lookup_begin(state);
			break;
		}
		case InstructionSequence::insn_send_stack_with_splat: {
			DEBUGLN("Canceling record due to send_stack_with_splat.");
			return TRACE_CANCEL;
			// arg1 = (intptr_t)(*(ip_ptr + 1));
			// arg2 = (intptr_t)(*(ip_ptr + 2));
			// numargs = 2;
			// stck_effect = -arg2 - 3;
			// pc_effect = 3;

			// int send_args = (intptr_t)(*(ip_ptr + 2));
			// Object* recv = *(stack_ptr - send_args);
			// target_klass = recv->lookup_begin(state);
			// break;
		}
		case InstructionSequence::insn_send_super_stack_with_block: {
			DEBUGLN("Canceling record due send super stack with block.");
			return TRACE_CANCEL;
			// arg1 = (intptr_t)(*(ip_ptr + 1));
			// arg2 = (intptr_t)(*(ip_ptr + 2));
			// numargs = 2;
			break;
		}
		case InstructionSequence::insn_send_super_stack_with_splat: {
			DEBUGLN("Canceling record due to send super stack with splat.");
			return TRACE_CANCEL;
			// arg1 = (intptr_t)(*(ip_ptr + 1));
			// arg2 = (intptr_t)(*(ip_ptr + 2));
			// numargs = 2;
			break;
		}
		case InstructionSequence::insn_push_block: {
			numargs = 0;
			break;
		}
		case InstructionSequence::insn_passed_blockarg: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_create_block: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_yield_stack: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			stck_effect = -arg1;
			pc_effect = 2;

			Object* t1 = call_frame->scope->block();
			if(BlockEnvironment *env = try_as<BlockEnvironment>(t1)) {
				CompiledMethod* cm = env->method();
				block_cm = cm;
			}
			else if(t1->nil_p()) {
				DEBUGLN("Canceling record due to yield to nil block.");
				return TRACE_CANCEL;
			} 
			else{
				DEBUGLN("Canceling record due to yield to non-static block.");
				return TRACE_CANCEL;
			}
			break;
		}
		case InstructionSequence::insn_yield_splat: {
			DEBUGLN("Canceling record due to yield splat.");
			return TRACE_CANCEL;
			// arg1 = (intptr_t)(*(ip_ptr + 1));
			// numargs = 1;
			// stck_effect = -arg1 - 1;
			// pc_effect = 2;

			// Object* t1 = call_frame->scope->block();
			// if(BlockEnvironment *env = try_as<BlockEnvironment>(t1)) {
			// 	CompiledMethod* cm = env->method();
			// 	block_cm = cm;
			// }
			// else if(t1->nil_p()) {
			// 	DEBUGLN("Canceling record due to yield to nil block.");
			// 	return TRACE_CANCEL;
			// } 
			// else{
			// 	DEBUGLN("Canceling record due to yield to non-static block.");
			// 	return TRACE_CANCEL;
			// }
			//break;
		}
		case InstructionSequence::insn_string_append: {
			numargs = 0;
			break;
		}
		case InstructionSequence::insn_string_build: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_check_serial: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			arg2 = (intptr_t)(*(ip_ptr + 2));
			numargs = 2;
			break;
		}
		case InstructionSequence::insn_check_serial_private: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			arg2 = (intptr_t)(*(ip_ptr + 2));
			numargs = 2;
			break;
		}
		case InstructionSequence::insn_push_my_field: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_store_my_field: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}

		case InstructionSequence::insn_meta_send_op_plus:
		case InstructionSequence::insn_meta_send_op_minus:
		case InstructionSequence::insn_meta_send_op_equal:
		case InstructionSequence::insn_meta_send_op_lt:
		case InstructionSequence::insn_meta_send_op_gt:
		case InstructionSequence::insn_meta_send_op_tequal: {
			// These optimization instructions should not be encountered!
			// (disabled in compiler)
			assert(1 == 2);
			break;
		}

		case InstructionSequence::insn_meta_send_call: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			arg2 = (intptr_t)(*(ip_ptr + 2));
			numargs = 2;
			break;
		}
		case InstructionSequence::insn_push_my_offset: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_zsuper: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_push_stack_local: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_set_stack_local: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			numargs = 1;
			break;
		}
		case InstructionSequence::insn_invoke_primitive: {
			arg1 = (intptr_t)(*(ip_ptr + 1));
			arg2 = (intptr_t)(*(ip_ptr + 2));
			numargs = 2;
			break;
		}

		case InstructionSequence::insn_raise_return: 
		case InstructionSequence::insn_raise_break: 
		case InstructionSequence::insn_raise_exc: 
		case InstructionSequence::insn_ensure_return: 
		case InstructionSequence::insn_reraise: {
			DEBUGLN("Canceling record due to exception condition.");
			return TRACE_CANCEL;
		}
		case InstructionSequence::insn_noop:{
			return TRACE_OK;
		}
		case InstructionSequence::insn_allow_private:
		case InstructionSequence::insn_cast_for_single_block_arg:
		case InstructionSequence::insn_cast_for_multi_block_arg: 
		case InstructionSequence::insn_cast_for_splat_block_arg:
		case InstructionSequence::insn_push_current_exception: 
		case InstructionSequence::insn_clear_exception: 
		case InstructionSequence::insn_push_exception_state: 
		case InstructionSequence::insn_restore_exception_state: 
		case InstructionSequence::insn_push_nil:
		case InstructionSequence::insn_push_true: 
		case InstructionSequence::insn_push_false:
		case InstructionSequence::insn_cast_array:
		case InstructionSequence::insn_shift_array: 
		case InstructionSequence::insn_pop_unwind: 
		case InstructionSequence::insn_string_dup: 
		case InstructionSequence::insn_push_scope: 
		case InstructionSequence::insn_add_scope: 
		case InstructionSequence::insn_push_variables: 
		case InstructionSequence::insn_check_interrupts: 
		case InstructionSequence::insn_yield_debugger: 
		case InstructionSequence::insn_is_nil: 
		case InstructionSequence::insn_kind_of: 
		case InstructionSequence::insn_instance_of: 
		case InstructionSequence::insn_meta_push_neg_1: 
		case InstructionSequence::insn_meta_push_0: 
		case InstructionSequence::insn_meta_push_1: 
		case InstructionSequence::insn_meta_push_2: 
		case InstructionSequence::insn_push_block_arg:
		case InstructionSequence::insn_push_undef:
		case InstructionSequence::insn_push_has_block:
		case InstructionSequence::insn_push_proc:
		case InstructionSequence::insn_nested_trace:
		case InstructionSequence::insn_check_frozen:
		case InstructionSequence::insn_cast_multi_value: {
			numargs = 0;
			break;
		}
		}

		// If we've entered a new call_frame..
		if(prev && prev->call_frame && prev->call_frame != call_frame){
			if(prev->op == InstructionSequence::insn_ret){
				active_send = prev->parent_send;
				if(prev->parent_send){
					parent_send = prev->parent_send->active_send;
				}
				call_depth -= 1;
			}

			else if(prev->op == InstructionSequence::insn_send_stack ||
							prev->op == InstructionSequence::insn_send_stack_with_splat ||
							prev->op == InstructionSequence::insn_send_stack_with_block ||
							prev->op == InstructionSequence::insn_send_method ||
							prev->op == InstructionSequence::insn_yield_stack ||
							prev->op == InstructionSequence::insn_yield_splat
							){
				if(prev->op == InstructionSequence::insn_yield_stack ||
					 prev->op == InstructionSequence::insn_yield_splat){
					prev->traced_yield = true;
				}
				else{
					prev->traced_send = true;
				}
				prev->target_cm.set(call_frame->cm);

				parent_send = prev->active_send;
				active_send = prev;
				call_depth += 1;
			}
		}

		else{ // In the same callframe as last node..
			if(prev && (prev->op == InstructionSequence::insn_goto_if_true ||
									prev->op == InstructionSequence::insn_goto_if_false)){
				if(pc == prev->arg1){
					prev->jump_taken = true;
					prev->side_exit_pc = prev->pc + 2;
				}
				else{
					prev->side_exit_pc = prev->arg1;
				}
			}
		}

		head = new TraceNode(state, call_depth, trace_pc, op, pc, 
												 sp, ip_ptr, vmm, call_frame);
		head->active_send = active_send;
		head->parent_send = parent_send;
		if(target_klass) head->target_klass.set(target_klass);
		if(target_cm) head->target_cm.set(target_cm);
		if(block_cm) head->block_cm.set(block_cm);
		head->prev = prev;
		head->arg1 = arg1;
		head->arg2 = arg2;
		head->numargs = numargs;
		head->side_exit_pc = side_exit_pc;
		head->stck_effect = stck_effect;
		head->pc_effect = pc_effect;

		// Support lazy creation of 
		// initial node - for branch 
		// traces.
		if(prev == NULL){
			entry = head;
			entry_sp = sp;
			if(parent_node != NULL){
				entry->call_depth = parent_node->call_depth;
			}
		}
		else{
			prev->next = head;
		}

		length++;

		if(is_term){
			head->next = anchor;
			head = anchor;
			return TRACE_FINISHED;
		}
		else{
			return TRACE_OK;
		}
	}


	void Trace::compile(STATE) {
		LLVMState* ls = LLVMState::get(state);
		ls->compile_soon(state, this);
	}


	void Trace::store() {
		if(is_branch()){
			int key;
			// TODO - will we lookup branches by class?
			// if(entry->traced_send){
			// 	key = entry->target_klass.get();
			// 	assert(key);
			// 	DEBUGLN("Storing branch at class: " << key); 
			// }
			// else {
			key = entry->pc;
			DEBUGLN("Storing branch at pc: " << entry->pc); 
			// }
			int offset = parent_node->branch_tbl_offset;
			parent_node->branch_keys[offset] = key;
			parent_node->branches[offset] = this;
			parent_node->branch_tbl_offset = (parent_node->branch_tbl_offset + 1) % BRANCH_TBL_SIZE;
		}
		else{
			DEBUGLN("Storing trace at pc: " << entry->pc); 
			VMMethod* vmm = entry->cm.get()->backend_method();
			vmm->add_trace_at(this, entry->pc);
		}
	}

	int Trace::assign_trace_pc(int pc, CallFrame* call_frame){
		trace_pc_counter += 1;
		std::map<int,int>& cf_map = trace_pc_map[call_frame];
		cf_map[pc] = trace_pc_counter;
		return trace_pc_counter;
	}

	int Trace::get_trace_pc(int pc, CallFrame* call_frame){
		std::map<int,int>& cf_map = trace_pc_map[call_frame];
		std::map<int,int>::iterator found = cf_map.find(pc);
		if(found != cf_map.end()){
			return (*found).second;
		}
		else{
			return -1;
		}
	}

	string Trace::trace_name(){
		return string("_TRACE_");
	}

	void Trace::pretty_print(STATE, std::ostream& out) {
		out << "[" << "\n";
		TraceIterator it = iter();
		while(it.has_next()){
			TraceNode* node = it.next();
			for(int i=0; i < node->call_depth + 5;i++) out << "  ";
			node->pretty_print(state, out);
			out << "\n";
		}
		out << "]" << "\n";
	}

	std::string Trace::to_graph_data(STATE) {
		std::stringstream s;
		TraceIterator it = iter();
		while(it.has_next()){
			TraceNode* node = it.next();
			s << node->graph_node_name(state);
			s << " -> ";
			s << node->next->graph_node_name(state);
			s << ";\n";

			for(int i = 0; i < BRANCH_TBL_SIZE; i++){
				Trace* t = node->branches[i];
				if(t != NULL){
					s << node->graph_node_name(state);
					s << " -> ";
					s << t->entry->graph_node_name(state);
					s << ";\n";
					s << t->to_graph_data(state);
				}
			}

		}
		std::string result = s.str();
		return result;
	}

	void Trace::dump_to_graph(STATE) {
		std::string s = to_graph_data(state);
		state->write_trace_graph_output(s);
	}


	TraceIterator::TraceIterator(Trace* const trace)
		: trace(trace),
			cur(NULL){}

	TraceNode* TraceIterator::next(){
		if(cur == NULL){
			cur = trace->entry;
			return cur;
		}
		cur = cur->next;
		return cur;
	}

	bool TraceIterator::has_next(){
		return cur == NULL || (cur->next != NULL && cur->next != trace->anchor);
	}

}
