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


	TraceNode::TraceNode(int depth, int pc_base, opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame)
		: op(op),
		  pc(pc),
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
			nested_trace(NULL)

	{
#include "vm/gen/instruction_trace_record.hpp"
	}

	void TraceNode::pretty_print(STATE, std::ostream& out) {
		if(state != NULL){
			out << cm->name()->c_str(state) << " - " << trace_pc  <<  "(" << pc << "): ";
		}
		else{
			out << "____" << " - " << trace_pc  <<  "(" << pc << "): ";
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

	Trace::Trace(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame){
		anchor = new TraceNode(0, 0, op, pc, ip_ptr, vmm, call_frame);
		head = anchor;
		pc_base_counter = 0;
		expected_exit_ip = -1;
	}


	Trace::Status Trace::add(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame){
		if(pc == anchor->pc && call_frame->cm == anchor->cm){
			head->next = anchor;
			head = anchor;
			return TRACE_FINISHED;
		}
		else if(op == InstructionSequence::insn_ret && call_frame == anchor->call_frame){
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

			head = new TraceNode(call_depth, pc_base, op, pc, ip_ptr, vmm, call_frame);
			head->active_send = active_send;
			head->parent_send = parent_send;
			head->prev = prev;
			prev->next = head;

			return TRACE_OK;
		}
	}


	void Trace::compile(STATE) {
		LLVMState* ls = LLVMState::get(state);
		ls->compile_trace(state, this);
	}

	string Trace::trace_name(){
		return string("_TRACE_");
	}

	void Trace::pretty_print(STATE, std::ostream& out) {
		out << "[" << "\n";
		TraceNode* tmp = anchor;
		while(tmp != NULL){
			for(int i=0; i < tmp->call_depth;i++) out << "  ";
			tmp->pretty_print(state, out);
			out << "\n";
			tmp = tmp->next;
			if(tmp == anchor) break;
		}
		out << "]" << "\n";
	}





	// while(!work_list_.empty()) {
	// 	int ip = work_list_.back();
	// 	work_list_.pop_back();

	// 	iter.switch_to(ip);

	// 	while(seen_[iter.ip()] == 0) {
	// 		seen_[iter.ip()] = 1;

	// 		each.call(iter);

	// 		if(iter.goto_p()) {
	// 			opcode target = iter.goto_target();
	// 			assert(target >= 0 && target < vmm_->total);

	// 			add_section(target);

	// 			// Non-terminating goto's stop the current block and queue the code
	// 			// right after them.
	// 			if(!iter.terminator_p() && iter.next_p()) add_section(iter.next_ip());
	// 			break;
	// 		}

	// 		if(iter.terminator_p()) break;

	// 		if(!iter.next_p()) break;
	// 		iter.advance();
	// 	}
	//		}




}
