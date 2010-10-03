#include "vm.hpp"
#include "call_frame.hpp"
#include "vmmethod.hpp"
#include "trace.hpp"
#include "builtin/iseq.hpp"
#include "builtin/symbol.hpp"
#include "vm/builtin/compiledmethod.hpp"
#include "trace.hpp"


#ifdef ENABLE_LLVM
#include "llvm/jit_visit.hpp"
#include "llvm/jit.hpp"
#endif


namespace rubinius {


	TraceNode::TraceNode(int depth, int pc_base, opcode op, int pc, int sp, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame)
		: op(op),
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
			nested_trace(NULL),
			jump_taken(false)

	{
#include "vm/gen/instruction_trace_record.hpp"
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

	Trace::Trace(opcode op, int pc, int sp, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame){
		anchor = new TraceNode(0, 0, op, pc, sp, ip_ptr, vmm, call_frame);
		head = anchor;
		entry = anchor;
		pc_base_counter = 0;
		expected_exit_ip = -1;
		entry_sp = sp;
		parent = NULL;
	}

	Trace::Trace(Trace* parent){
		this->parent = parent;
		anchor = parent->anchor;
		entry = NULL;
		head = NULL;
	}


	Trace::Status Trace::add(opcode op, int pc, int sp, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame){

		if(this->is_branch() && head == NULL){
			// Possible the side-exit jumps straight to the anchor...
			if(pc == anchor->pc && call_frame->cm == anchor->cm){
				std::cout << "Recording branch directly to anchor.\n";
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
			std::cout << "Canceling record due to exception condition.\n";
			return TRACE_CANCEL;
		}
		else if(op == InstructionSequence::insn_ret && call_frame == anchor->call_frame){
			std::cout << "Canceling record due to return from home frame.\n";
			return TRACE_CANCEL;
		}
		else{
			TraceNode* prev = head;
			TraceNode* active_send = prev->active_send;
			TraceNode* parent_send = prev->parent_send;
			CompiledMethod* cm = call_frame->cm;
			int pc_base = prev->pc_base;
			int call_depth = prev->call_depth;

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
						pc_base = anchor->pc_base;
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
					}
				}
			}

			head = new TraceNode(call_depth, pc_base, op, pc, sp, ip_ptr, vmm, call_frame);
			head->active_send = active_send;
			head->parent_send = parent_send;
			head->prev = prev;
			prev->next = head;
			node_map[head->trace_pc] = head;

			if(op == InstructionSequence::insn_nested_trace){
				head->nested_trace = vmm->traces[pc];
			}

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
	}

	string Trace::trace_name(){
		return string("_TRACE_");
	}

	void Trace::pretty_print(STATE, std::ostream& out) {
		out << "[" << "\n";
		TraceIterator it = iter();
		while(it.has_next()){
			TraceNode* node = it.next();
			for(int i=0; i < node->call_depth;i++) out << "  ";
			node->pretty_print(state, out);
			out << "\n";
		}
		out << "]" << "\n";
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
