#ifndef RBX_TRACE_HPP
#define RBX_TRACE_HPP


using namespace std;

namespace llvm {
	class Function;
}

namespace rubinius {

	struct JITBasicBlock;


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

	typedef uintptr_t opcode;
  typedef map<int, JITBasicBlock> BlockMap;
  typedef Object* (*trace_executor)(VM*, CallFrame*, Object**, StackVariables*, TraceInfo*);


	class TraceNode {
	public:
		opcode op;
		int pc;
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

		int total_size;
		int numargs;
		intptr_t arg1;
		intptr_t arg2;


		TraceNode(int depth, int pc_base, opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void pretty_print(STATE, std::ostream& out);

		int interp_jump_target(){
			return arg1 - pc_base;
		}

	};


	class Trace {
	public:
		TraceNode* anchor;
		TraceNode* head;
		llvm::Function* llvm_function;
		size_t jitted_bytes;
		void*  jitted_impl;
		trace_executor  executor;
		int pc_base_counter;
		int expected_exit_ip;

		enum Status { TRACE_CANCEL, TRACE_OK, TRACE_FINISHED };

		Trace(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		Status add(opcode op, int pc, void** const ip_ptr, VMMethod* const vmm, CallFrame* const call_frame);

		void pretty_print(STATE, std::ostream& out);

		string trace_name();

		void compile(STATE);

		CompiledMethod* anchor_cm(){
			return anchor->cm;
		}

		int init_ip(){
			return anchor->pc;
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


		// class Walker {
		// 		JITVisit& v_;
		// 		BlockMap& map_;

		// 	public:
		// 		Walker(JITVisit& v, BlockMap& map)
		// 			: v_(v)
		// 			, map_(map)
		// 		{}

		// 		void call(OpcodeIterator& iter) {
		// 			v_.dispatch(iter.stream(), iter.ip());

		// 			if(v_.b().GetInsertBlock()->getTerminator() == NULL) {
		// 				BlockMap::iterator i = map_.find(iter.next_ip());
		// 				if(i != map_.end()) {
		// 					v_.b().CreateBr(i->second.block);
		// 				}
		// 			}
		// 		}
		// 	};


		template <typename T>
		void walk(T& walker) {
			TraceNode* tmp = anchor;
			while(tmp != NULL){
				walker.call(this, tmp);
				tmp = tmp->next;
				if(tmp == anchor) break;
			}
		}


	};



}

#endif
