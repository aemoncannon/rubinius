#ifndef RBX_TRACE_HPP
#define RBX_TRACE_HPP


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
	class StackVariables;
	class Symbol;
	class JITVisit;
	class TraceInfo;
	class Trace;
	class TraceIterator;

	typedef uintptr_t opcode;
  typedef int (*trace_executor)(VM*, CallFrame*, TraceInfo*);

	class TraceNode {
	public:
		opcode op;
		int pc;
		int sp;
		CallFrame* const call_frame;
		CompiledMethod* const cm;
		CompiledMethod* send_cm;
		void** ip_ptr;
		TraceNode* prev;
		TraceNode* next;
		bool traced_send;
		bool traced_yield;
		TraceNode* active_send;
		TraceNode* parent_send;
		int trace_pc;
		int pc_base;
		int call_depth;
		Trace* nested_trace;
		bool jump_taken;
		int exit_counter;

		int total_size;
		int numargs;
		intptr_t arg1;
		intptr_t arg2;


		TraceNode(int depth, int pc_base, opcode op, int pc, int sp, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void pretty_print(STATE, std::ostream& out);

		int interp_jump_target(){
			return arg1 - pc_base;
		}

		bool bump_exit_hotness(){
			exit_counter++;
			return exit_counter > 50;
		}

		void clear_hotness(){
			exit_counter = 0;
		}

	};


	class TraceIterator {
		Trace* trace;
		TraceNode* cur;
	public:
		TraceIterator(Trace* const trace);
		TraceNode* next();
		bool has_next();
	};


	class Trace {
	public:
		std::map<int, TraceNode*> node_map;
		TraceNode* anchor;
		TraceNode* head;
		TraceNode* entry;
		llvm::Function* llvm_function;
		size_t jitted_bytes;
		void*  jitted_impl;
		trace_executor  executor;
		int pc_base_counter;
		int expected_exit_ip;
		int entry_sp;
		Trace* parent;

		enum Status { TRACE_CANCEL, TRACE_OK, TRACE_FINISHED };

		Trace(opcode op, int pc, int sp, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		Trace(Trace* parent);

		Status add(opcode op, int pc, int sp, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void pretty_print(STATE, std::ostream& out);

		string trace_name();

		void compile(STATE);

		void store();

		CompiledMethod* entry_cm(){
			return entry->cm;
		}

		TraceIterator iter(){
			return TraceIterator(this);
		}

		bool is_branch(){
			return parent != NULL;
		}

		int init_ip(){
			return entry->pc;
		}

		TraceNode* node_at(int trace_pc){
			return node_map[trace_pc];
		}

		bool parent_of(Trace* trace){
			Trace* t = trace->parent;
			while(t != NULL){
				if(t == this) return true;
				t = t->parent;
			}
			return false;
		}

		void set_jitted(llvm::Function* func, size_t bytes, void* impl) {
			llvm_function = func;
			jitted_impl = impl;
			jitted_bytes = bytes;
			executor = reinterpret_cast<trace_executor>(jitted_impl);
		}

		template <typename T>
		void dispatch(T& v, TraceNode* node){
			opcode op = node->op;
			intptr_t arg1 = node->arg1;
			intptr_t arg2 = node->arg2;
			v.at_trace_node(node);
			v.at_ip(node->trace_pc);

			switch(op) {
#define HANDLE_INST0(code, name)									\
				case code:																\
					if(v.before(op)) { v.visit_ ## name();}	\
					break;

#define HANDLE_INST1(code, name)														\
				case code:																					\
					if(v.before(op, arg1)) { v.visit_ ## name(arg1);}	\
					break;

#define HANDLE_INST2(code, name)																				\
				case code:																											\
					if(v.before(op, arg1, arg2)) { v.visit_ ## name(arg1, arg2);}	\
					break;
			
#include "vm/gen/instruction_visitors.hpp"

#undef HANDLE_INST0
#undef HANDLE_INST1
#undef HANDLE_INST2

			}
		}


		template <typename T>
		void walk(T& walker) {
			TraceIterator it = iter();
			while(it.has_next()){
				TraceNode* node = it.next();
				walker.call(this, node);
			}
		}


	};



}

#endif
