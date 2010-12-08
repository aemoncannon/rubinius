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
	class String;
  class Class;
  class StackVariables;
  class Symbol;
  class JITVisit;
  class TraceInfo;
  class TraceNode;
  class Trace;
  class TraceIterator;


	static const int BRANCH_TBL_SIZE = 3;

  typedef uintptr_t opcode;
  typedef int (*trace_executor)(VM*, CallFrame*, Trace*, Trace*, TraceNode*, int);

  class TraceNode {
  public:
    Trace* nested_trace;
    trace_executor nested_executor;
    Trace* branches[BRANCH_TBL_SIZE];
		int branch_keys[BRANCH_TBL_SIZE];
    int branch_tbl_offset;
    opcode op;
    int pc;
    int sp;
    int stck_effect;
    int pc_effect;
    CallFrame* call_frame;
    TypedRoot<CompiledMethod*> cm;
    TypedRoot<CompiledMethod*> target_cm;
    TypedRoot<CompiledMethod*> block_cm;
		TypedRoot<Class*> target_klass;
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
    bool jump_taken;
    int exit_counter;
    int side_exit_pc;
    int numargs;
    intptr_t arg1;
    intptr_t arg2;

    TraceNode(STATE, int depth, int pc_base, opcode op, int pc, int sp, void** ip_ptr, VMMethod* vmm, CallFrame* call_frame);

    void pretty_print(STATE, std::ostream& out);

    std::string graph_node_name(STATE);

    int send_arg_count();

    bool is_on_home_call_frame(){
      return active_send == NULL;
    }

    int interp_jump_target(){
      return arg1 - pc_base;
    }

    int exit_to_pc(){
      return side_exit_pc;
    }

    bool bump_exit_hotness();

    void disable_counter();

    std::string cm_name(STATE);
    std::string op_name();

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
    trace_executor executor;
    int expected_exit_ip;
    TraceNode* anchor;
    TraceNode* head;
    TraceNode* entry;
    size_t jitted_bytes;
    int pc_base_counter;
    int entry_sp;
    Trace* parent;
    TraceNode* parent_node;
    bool is_nested_trace;
    bool is_branch_trace;
    int length;


    static const int RUN_MODE_NORM = 0;
    static const int RUN_MODE_NESTED = 1;
    static const int RUN_MODE_RECORD_NESTED = 2;

    static const int RETURN_SIDE_EXITED = -1;
    static const int RETURN_OK = 0;
    static const int RETURN_NESTED_OK = 1;

    static const int MAX_TRACE_LENGTH = 300;
    static const int RECORD_THRESHOLD = 30;
    static const int BRANCH_RECORD_THRESHOLD = 3;
		static const int COUNTER_DISABLED = -1;
		

    enum Status { TRACE_CANCEL, TRACE_OK, TRACE_FINISHED };

    Trace(opcode op, int pc, int sp, void** ip_ptr, VMMethod* vmm, CallFrame* call_frame);

    Trace();

		// Standard constructor for initializing a new trace
		static Trace* newTrace(opcode op, int pc, int sp, void** ip_ptr, 
													 STATE, VMMethod* vmm, CallFrame* call_frame, 
													 Object** stack_ptr);

    Status add(opcode op, int pc, int sp, void** ip_ptr, STATE, VMMethod* vmm, CallFrame* call_frame, Object** stack_ptr);

    Trace* create_branch_at(TraceNode* exit_node);

    Status add_nested_trace_call(Trace* trace, int nested_exit_pc, int pc, int sp, void** ip_ptr, STATE, VMMethod* vmm, CallFrame* call_frame, Object** stack_ptr);

    void pretty_print(STATE, std::ostream& out);

    string trace_name();

    void compile(STATE);

    std::string to_graph_data(STATE);
    void dump_to_graph(STATE);

    void store();

    CompiledMethod* entry_cm(){
      return entry->cm.get();
    }

    TraceIterator iter(){
      return TraceIterator(this);
    }

    bool is_branch(){
      return is_branch_trace;
    }

    bool is_nested(){
      return is_nested_trace;
    }

    int init_ip(){
      return entry->pc;
    }

    bool parent_of(Trace* trace){
      Trace* t = trace->parent;
      while(t != NULL){
				if(t == this) return true;
				t = t->parent;
      }
      return false;
    }

    Trace* ultimate_parent(){
      Trace* t = this;
      while(t->parent != NULL){
				t = t->parent;
      }
      return t;
    }

    void set_jitted(size_t bytes, void* impl) {
      jitted_bytes = bytes;
      executor = reinterpret_cast<trace_executor>(impl);
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
