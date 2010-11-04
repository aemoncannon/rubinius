#include "vm.hpp"
#include "call_frame.hpp"
#include "vmmethod.hpp"
#include "trace.hpp"
#include "builtin/iseq.hpp"
#include "builtin/symbol.hpp"
#include "vm/builtin/compiledmethod.hpp"
#include "trace.hpp"
#include "trace_info.hpp"
#include "utilities.hpp"

#include <iostream>
#include <sstream>

#ifdef ENABLE_LLVM
#include "llvm/jit_visit.hpp"
#include "llvm/jit.hpp"
#endif


namespace rubinius {


  TraceNode::TraceNode(int depth, int pc_base, opcode op, int pc, int sp, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame)
    : 
    nested_trace(NULL),
    nested_executor(NULL),
		branch_tbl_offset(0),
    op(op),
    pc(pc),
    sp(sp),
    call_frame(call_frame),
    cm(call_frame->cm),
    send_cm(NULL),
    target_klass(NULL),
    ip_ptr(ip_ptr),
    prev(NULL),
    next(NULL),
    traced_send(false),
    traced_yield(false),
    active_send(NULL),
    parent_send(NULL),
    trace_pc(0),
    pc_base(pc_base),
    call_depth(depth),
    jump_taken(false),
    exit_counter(0),
    side_exit_pc(-1)

  {
#include "vm/gen/instruction_trace_record.hpp"
		for(int i = 0; i < BRANCH_TBL_SIZE; i++) {
			branches[i] = NULL;
			branch_keys[i] = NULL;
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
    else if(op == InstructionSequence::insn_send_stack) return arg2;
    else return arg2;
  }

  std::string TraceNode::graph_node_name(STATE) {
    assert(next);
    std::stringstream s;
    s << "\"" << cm_name(state) << "-" << pc << " " << op_name() << "\"";
    std::string result = s.str();
    return result;
  }

  void TraceNode::pretty_print(STATE, std::ostream& out) {
    if(state != NULL){
      out << cm->name()->c_str(state) << " - " << trace_pc  <<  "(" << pc << ") " << "sp=" << sp << ": ";
    }
    else{
      out << "____" << " - " << trace_pc  <<  "(" << pc << ") " << "sp=" << sp << ": ";
    }
    out << InstructionSequence::get_instruction_name(op);
    out << " ";
    if(numargs > 0) out << arg1;
    if(numargs > 1) out << ", " << arg2;
    if(active_send){
      out << " : active_send(";
      out << active_send->trace_pc;
      out << ")";
    }
    if(parent_send){
      out << " : parent(";
      out << parent_send->trace_pc;
      out << ")";
    }
  }



  // Standard constructor for initializing a new trace
  Trace::Trace(opcode op, int pc, int sp, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame) : 
    executor(NULL)
    ,expected_exit_ip(-1)
    ,anchor(NULL)
    ,head(NULL) 
    ,entry(NULL) 
    ,jitted_bytes(-1) 
    ,pc_base_counter(0) 
    ,entry_sp(sp)
    ,parent(NULL)
    ,parent_node(NULL)
    ,is_nested_trace(false)
    ,is_branch_trace(false)
    ,length(0)
  {
    anchor = new TraceNode(0, 0, op, pc, sp, ip_ptr, vmm, call_frame);
    head = anchor;
    entry = anchor;
    entry_sp = sp;
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
    ,pc_base_counter(0) 
    ,entry_sp(-1)
    ,parent(NULL)
    ,parent_node(NULL)
    ,is_nested_trace(false)
    ,is_branch_trace(false)
    ,length(0)
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

    if(pc == anchor->pc && call_frame->cm == anchor->cm && 
       head->op == InstructionSequence::insn_goto){
      head->next = anchor;
      head = anchor;
      return TRACE_FINISHED;
    }
    else if(length >= MAX_TRACE_LENGTH){
      DEBUGLN("Canceling record due to exceeded max trace length of " << MAX_TRACE_LENGTH);
      return TRACE_CANCEL;
    }
    else if(op == InstructionSequence::insn_raise_exc || 
						op == InstructionSequence::insn_raise_return ||
						op == InstructionSequence::insn_raise_break ||
						op == InstructionSequence::insn_reraise){
      DEBUGLN("Canceling record due to exception condition.");
      return TRACE_CANCEL;
    }
    else if(op == InstructionSequence::insn_send_stack_with_splat){
      DEBUGLN("Canceling record due to splat.");
      return TRACE_CANCEL;
    }
    else if(op == InstructionSequence::insn_ret && call_frame->cm == anchor->cm){
      DEBUGLN("Canceling record due to return from home frame.");
      return TRACE_CANCEL;
    }
    else{
      TraceNode* prev = head;
			TraceNode* active_send = NULL;
			TraceNode* parent_send = NULL;
			int pc_base = 0;
			int call_depth = 0;
			if(prev){
				active_send = prev->active_send;
				parent_send = prev->parent_send;
				pc_base = prev->pc_base;
				call_depth = prev->call_depth;
			}
      CompiledMethod* cm = call_frame->cm;
      int side_exit_pc = pc;

			// Remember the target class for sends

			Class* target_klass = NULL;
			if(op == InstructionSequence::insn_send_stack || 
				 op == InstructionSequence::insn_send_method || 
				 op == InstructionSequence::insn_send_stack_with_block){

				int send_args = -1;
				if(op == InstructionSequence::insn_send_stack) 
					send_args = (intptr_t)(*(ip_ptr + 2));
				else if(op == InstructionSequence::insn_send_method) 
					send_args = 0;
				else if(op == InstructionSequence::insn_send_stack_with_block) 
					send_args = (intptr_t)(*(ip_ptr + 2));
				assert(send_args > -1);

				Object* recv = *(stack_ptr - send_args);
				assert(recv);
				target_klass = recv->lookup_begin(state);
				assert(target_klass);
			}

      if(prev && prev->call_frame && prev->call_frame != call_frame){
				if(prev->op == InstructionSequence::insn_ret){
					active_send = prev->parent_send;
					if(prev->parent_send){
						parent_send = prev->parent_send->active_send;
					}
					if(prev->active_send){
						pc_base = prev->active_send->pc_base;
					}
					else{
						pc_base_counter += prev->cm->backend_method()->total;
						pc_base = pc_base_counter;
					}
					call_depth -= 1;
				}
				else if(prev->op == InstructionSequence::insn_send_stack ||
								prev->op == InstructionSequence::insn_send_method ||
								prev->op == InstructionSequence::insn_send_stack_with_block ||
								prev->op == InstructionSequence::insn_yield_stack){

					pc_base_counter += prev->cm->backend_method()->total;
					pc_base = pc_base_counter;

					if(prev->op == InstructionSequence::insn_yield_stack){
						prev->traced_yield = true;
					}
					else if(prev->op == InstructionSequence::insn_send_stack_with_block){
						prev->traced_send = true;
					}
					else{
						prev->traced_send = true;
					}
					prev->send_cm = cm;

					parent_send = prev->active_send;
					active_send = prev;
					call_depth += 1;
				}
      }
      else{ // In the same callframe as last node..
				if(prev &&
					 (prev->op == InstructionSequence::insn_goto_if_true ||
						prev->op == InstructionSequence::insn_goto_if_false)){

					if(pc == prev->interp_jump_target()){
						prev->jump_taken = true;
						prev->side_exit_pc = prev->pc + 2;
					}
					else{
						prev->side_exit_pc = prev->interp_jump_target();
					}

				}
      }


      length++;

			if(this->is_branch() && head == NULL){
				head = new TraceNode(0, 0, op, pc, sp, ip_ptr, vmm, call_frame);
				head->side_exit_pc = side_exit_pc;
				head->target_klass = target_klass;
				entry = head;
				pc_base_counter = 0;
				expected_exit_ip = -1;
				entry_sp = sp;
				return TRACE_OK;
			}
			else{
				head = new TraceNode(call_depth, pc_base, op, pc, sp, ip_ptr, vmm, call_frame);
				head->active_send = active_send;
				head->parent_send = parent_send;
				head->side_exit_pc = side_exit_pc;
				head->target_klass = target_klass;
				head->prev = prev;
				prev->next = head;
				return TRACE_OK;
			}
    }
  }


  void Trace::compile(STATE) {
    LLVMState* ls = LLVMState::get(state);
    ls->compile_trace(state, this);
  }


  void Trace::store() {
    if(is_branch()){
			void* key;
			if(entry->traced_send){
				key = entry->target_klass;
				assert(key);
				DEBUGLN("Storing branch at class: " << key); 
			}
			else {
				key = (void*)entry->pc;
				DEBUGLN("Storing branch at pc: " << entry->pc); 
			}
			int offset = parent_node->branch_tbl_offset;
			parent_node->branch_keys[offset] = key;
			parent_node->branches[offset] = this;
			parent_node->branch_tbl_offset = (parent_node->branch_tbl_offset + 1) % BRANCH_TBL_SIZE;
    }
		else{
			DEBUGLN("Storing trace at pc: " << entry->pc); 
			VMMethod* vmm = entry->cm->backend_method();
			vmm->traces[entry->pc] = this;
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
