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
    branch_trace(NULL),
    branch_executor(&missing_branch_handler),
    nested_trace(NULL),
    nested_executor(&missing_branch_handler),
    op(op),
    pc(pc),
    sp(sp),
    call_frame(call_frame),
    cm(call_frame->cm),
    send_cm(NULL),
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
  }

  std::string TraceNode::cm_name(STATE){
    std::string result = cm->name()->c_str(state);
    return result;
  }

  std::string TraceNode::op_name(){
    return InstructionSequence::get_instruction_name(op);
  }

  int TraceNode::arg_count(){
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


  int missing_branch_handler(STATE, CallFrame* call_frame, Trace* trace, Trace* exit_trace, TraceNode* exit_node, int run_mode){
    TRACK_TIME(IN_EXIT_TIMER);
    DEBUGLN("No branch to continue on. Exiting."); 

    // Maybe start recording a branch trace...
    if(exit_node->bump_exit_hotness()){
      DEBUGLN("Exit node at " << exit_node->pc << " got hot! Recording branch...");
      state->recording_trace = exit_trace->create_branch_at(exit_node);
      exit_node->clear_hotness();
    }

    // Bail to uncommon if we've stacked up call_frames before the exit.
    // Or if a nested trace exited unexpectedly (we don't know _where_ it
    // ended up)...
    if(call_frame->is_traced_frame() 
       // Why is this necessary?
       || run_mode == Trace::RUN_MODE_NESTED || run_mode == Trace::RUN_MODE_RECORD_NESTED){

      VMMethod* vmm = call_frame->cm->backend_method();
      VMMethod::uncommon_interpreter(state, vmm, call_frame);
    }
    // Otherwise, just return directly to caller...
    TRACK_TIME(ON_TRACE_TIMER);
    return -1;
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
					     void** const ip_ptr, VMMethod* const vmm, 
					     CallFrame* const call_frame){

    this->add(InstructionSequence::insn_nested_trace, pc, sp, ip_ptr, vmm, call_frame);
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


  Trace::Status Trace::add(opcode op, int pc, int sp, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame){

    if(this->is_branch() && head == NULL){
      // Possible the side-exit jumps straight to the anchor...
      if(pc == anchor->pc && call_frame->cm == anchor->cm){
	DEBUGLN("Recording branch directly to anchor.");
	head = anchor;
	entry = head;
	return TRACE_FINISHED;
      }
      head = new TraceNode(0, 0, op, pc, sp, ip_ptr, vmm, call_frame);
      entry = head;
      pc_base_counter = 0;
      expected_exit_ip = -1;
      entry_sp = sp;
      return TRACE_OK;
    }

    if(pc == anchor->pc && call_frame->cm == anchor->cm){
      head->next = anchor;
      head = anchor;
      return TRACE_FINISHED;
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
    else if(op == InstructionSequence::insn_ret && call_frame == anchor->call_frame){
      DEBUGLN("Canceling record due to return from home frame.");
      return TRACE_CANCEL;
    }
    else{
      TraceNode* prev = head;
      TraceNode* active_send = prev->active_send;
      TraceNode* parent_send = prev->parent_send;
      CompiledMethod* cm = call_frame->cm;
      int pc_base = prev->pc_base;
      int call_depth = prev->call_depth;
      int side_exit_pc = -1;

      if(prev->call_frame != call_frame){
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
	if(prev->op == InstructionSequence::insn_goto_if_true ||
	   prev->op == InstructionSequence::insn_goto_if_false){

	  if(pc == prev->interp_jump_target()){
	    prev->jump_taken = true;
	    side_exit_pc = prev->pc + 2;
	  }
	  else{
	    side_exit_pc = prev->interp_jump_target();
	  }

	}
      }

      head = new TraceNode(call_depth, pc_base, op, pc, sp, ip_ptr, vmm, call_frame);
      head->active_send = active_send;
      head->parent_send = parent_send;
      head->side_exit_pc = side_exit_pc;
      head->prev = prev;
      prev->next = head;

      return TRACE_OK;
    }
  }


  void Trace::compile(STATE) {
    LLVMState* ls = LLVMState::get(state);
    ls->compile_trace(state, this);
  }

  void Trace::store() {
    VMMethod* vmm = entry->cm->backend_method();
    vmm->traces[entry->pc] = this;
    if(is_branch()){
      parent_node->branch_trace = this;
      assert(this->executor);
      parent_node->branch_executor = this->executor;
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
      if(node->branch_trace != NULL){
	s << node->graph_node_name(state);
	s << " -> ";
	s << node->branch_trace->entry->graph_node_name(state);
	s << ";\n";
	s << node->branch_trace->to_graph_data(state);
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
