#ifndef RBX_LLVM_JIT_HPP
#define RBX_LLVM_JIT_HPP

#include <stdint.h>
#include <unistd.h>

#include <llvm/Module.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Function.h>
#include <llvm/Module.h>
#include <llvm/Instructions.h>
#include <llvm/Support/IRBuilder.h>
#include <llvm/ModuleProvider.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/CodeGen/MachineCodeInfo.h>
#include <llvm/Pass.h>
#include <llvm/PassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/LLVMContext.h>

#include "vm.hpp"
#include "vmmethod.hpp"
#include "configuration.hpp"

#include "gc/managed.hpp"
#include "gc/write_barrier.hpp"

#include "builtin/block_environment.hpp"

#include "instruments/timing.hpp"
#include "object_utils.hpp"

namespace rubinius {

  namespace jit {
    class Builder;
    class Context;
  }

  class InlinePolicy;
  class JITMethodInfo;

  class JITInlineBlock {
    llvm::PHINode* block_break_result_;
    llvm::BasicBlock* block_break_loc_;
    VMMethod* code_;
    TypedRoot<CompiledMethod*> method_;
    JITMethodInfo* scope_;
    int which_;
    bool created_object_;

  public:
    JITInlineBlock(LLVMState* ls, llvm::PHINode* phi, llvm::BasicBlock* brk,
                   CompiledMethod* cm, VMMethod* code,
                   JITMethodInfo* scope, int which);

    llvm::PHINode* block_break_result() {
      return block_break_result_;
    }

    llvm::BasicBlock* block_break_loc() {
      return block_break_loc_;
    }

    VMMethod* code() {
      return code_;
    }

    CompiledMethod* method() {
      return method_.get();
    }

    JITMethodInfo* creation_scope() {
      return scope_;
    }

    int which() {
      return which_;
    }

    bool created_object_p() {
      return created_object_;
    }

    void set_created_object() {
      created_object_ = true;
    }
  };

  class JITMethodInfo {
    jit::Context& context_;
    llvm::Function* function_;
    llvm::BasicBlock* entry_;
    llvm::Value* call_frame_;
    llvm::Value* stack_;
    llvm::Value* vm_;
    llvm::Value* args_;
    llvm::Value* unwinds_;
    llvm::Value* trace_;
    llvm::Value* block_env_;
    llvm::Value* top_scope_;
    llvm::Value* variables_;
    llvm::Value* previous_;
    llvm::Value* profiling_entry_;
    llvm::Value* out_args_;
    llvm::Value* counter_;
    llvm::Value* msg_;
    llvm::Value* arg_total_;
    llvm::Value* trace_result_;
    llvm::Value* trace_run_mode_;
    int saved_sp_;
    int saved_last_sp_;

    JITMethodInfo* parent_info_;
    JITMethodInfo* creator_info_;

    bool use_full_scope_;

    bool traced_block_supplied_;

    JITInlineBlock* inline_block_;
    JITInlineBlock* block_info_;

    TypedRoot<CompiledMethod*> method_;

    llvm::BasicBlock* return_pad_;
    llvm::PHINode* return_phi_;
    llvm::BasicBlock* trace_exit_pad_;

  public:
    VMMethod* vmm;
    bool is_block;
    llvm::BasicBlock* inline_return;
    llvm::Value* return_value;
    InlinePolicy* inline_policy;
    llvm::BasicBlock* fin_block;
    int called_args;
    std::vector<llvm::Value*>* stack_args;

    typedef std::map<int, llvm::Value*> ValMap;

    ValMap pre_allocated_args;
    ValMap pre_allocated_call_frames;
    ValMap pre_allocated_vars;
    ValMap pre_allocated_unwinds;

    llvm::PHINode* trace_node_phi;
    llvm::PHINode* exit_ip_phi;
    llvm::PHINode* exit_sp_phi;
    llvm::PHINode* exit_cf_phi;


    JITMethodInfo* root;

  public:
    JITMethodInfo(jit::Context& ctx, CompiledMethod* cm, VMMethod* v,
                  JITMethodInfo* parent = 0);

    jit::Context& context() {
      return context_;
    }

    void set_function(llvm::Function* func);

    llvm::Function* function() {
      return function_;
    }

    void set_vm(llvm::Value* vm) {
      vm_ = vm;
    }

    llvm::Value* vm() {
      return vm_;
    }

    void set_args(llvm::Value* args) {
      args_ = args;
    }

    llvm::Value* args() {
      return args_;
    }

    void set_trace(llvm::Value* trace) {
      trace_ = trace;
    }

    llvm::Value* trace() {
      return trace_;
    }

		void set_unwinds(llvm::Value* unwinds) {
      unwinds_ = unwinds;
    }

    llvm::Value* unwinds() {
      return unwinds_;
    }

    llvm::BasicBlock* trace_exit_pad() {
      return root_info()->trace_exit_pad_;
    }

    void set_block_env(llvm::Value* env) {
      block_env_ = env;
    }

    llvm::Value* block_env() {
      return block_env_;
    }

    void set_top_scope(llvm::Value* env) {
      top_scope_ = env;
    }

    llvm::Value* top_scope() {
      return top_scope_;
    }

    void set_arg_total(llvm::Value* arg_total) {
      arg_total_ = arg_total;
    }

    llvm::Value* arg_total() {
      return arg_total_;
    }

    void set_saved_sp(int sp) {
      saved_sp_ = sp;
    }

    int saved_sp() {
      return saved_sp_;
    }

    void set_saved_last_sp(int sp) {
      saved_last_sp_ = sp;
    }

    int saved_last_sp() {
      return saved_last_sp_;
    }

    void set_previous(llvm::Value* prev) {
      previous_ = prev;
    }

    llvm::Value* previous() {
      return previous_;
    }

    void set_msg(llvm::Value* msg) {
      msg_ = msg;
    }

    llvm::Value* msg() {
      return msg_;
    }

    void set_profiling_entry(llvm::Value* val) {
      profiling_entry_ = val;
    }

    llvm::Value* profiling_entry() {
      return profiling_entry_;
    }

    void set_entry(llvm::BasicBlock* entry) {
      entry_ = entry;
    }

    llvm::BasicBlock* entry() {
      return entry_;
    }

    void set_call_frame(llvm::Value* val) {
      call_frame_ = val;
    }

    llvm::Value* call_frame() {
      return call_frame_;
    }

    void set_trace_run_mode(llvm::Value* val) {
      trace_run_mode_ = val;
    }

    llvm::Value* trace_run_mode() {
      return trace_run_mode_;
    }

    void set_stack(llvm::Value* val) {
      stack_ = val;
    }

    llvm::Value* stack() {
      return stack_;
    }

    void set_variables(llvm::Value* vars) {
      variables_ = vars;
    }

    llvm::Value* variables() {
      return variables_;
    }

    CompiledMethod* method() {
      return method_.get();
    }

    void set_method(CompiledMethod* meth) {
      return method_.set(meth);
    }

    VMMethod* vm_method() {
      return vmm;
    }

    llvm::BasicBlock* return_pad() {
      return return_pad_;
    }

    llvm::PHINode* return_phi() {
      return return_phi_;
    }

    void add_return_value(llvm::Value* val, llvm::BasicBlock* block) {
      return_phi_->addIncoming(val, block);
    }

    void init_return_pad();

    void init_trace_exit_pad();

    void set_parent_info(JITMethodInfo* info) {
      parent_info_ = info;
      args_ = info->out_args();
      init_globals(info);
    }

    void init_globals(JITMethodInfo* info){
      vm_ = info->vm();
      counter_ = info->counter();
      root = info->root_info();
      trace_ = info->trace();
      return_phi_ = info->return_phi();
      return_pad_ = info->return_pad();
      trace_exit_pad_ = info->trace_exit_pad();
      trace_run_mode_ = info->trace_run_mode();
      set_function(info->function());
    }

    llvm::Value* parent_call_frame() {
      if(parent_info_) {
        return parent_info_->call_frame();
      }

      return 0;
    }

    JITMethodInfo* root_info() {
      return root;
    }

    JITMethodInfo* parent_info() {
      return parent_info_;
    }

    bool for_inlined_method() {
      return parent_info_ != 0;
    }

    JITMethodInfo* creator_info() {
      return creator_info_;
    }

    void set_creator_info(JITMethodInfo* creator_info) {
      creator_info_ = creator_info;
    }

    JITMethodInfo* home_info() {
      JITMethodInfo* nfo = creator_info_;
      if(!nfo) return 0;

      while(JITMethodInfo* nxt = nfo->creator_info()) {
        nfo = nxt;
      }

      return nfo;
    }

    bool traced_block_supplied() {
      return traced_block_supplied_;
    }

    void set_traced_block_supplied(bool val) {
      traced_block_supplied_ = val;
    }

    JITInlineBlock* inline_block() {
      return inline_block_;
    }

    void set_inline_block(JITInlineBlock* bi) {
      inline_block_ = bi;
    }

    void clear_inline_block() {
      inline_block_ = 0;
    }

    void set_block_info(JITInlineBlock* block) {
      block_info_ = block;
    }

    JITInlineBlock* block_info() {
      return block_info_;
    }

    llvm::BasicBlock* block_break_loc() {
      return block_info_->block_break_loc();
    }

    llvm::PHINode* block_break_result() {
      return block_info_->block_break_result();
    }

    void set_out_args(llvm::Value* out_args) {
      out_args_ = out_args;
    }

    llvm::Value* out_args() {
      return out_args_;
    }

    bool use_full_scope() {
      return use_full_scope_;
    }

    void set_use_full_scope() {
      use_full_scope_ = true;
    }

    llvm::Value* counter() {
      return counter_;
    }

    void set_counter(llvm::Value* counter) {
      counter_ = counter;
    }

    llvm::AllocaInst* create_alloca(const llvm::Type* type, llvm::Value* size = 0,
                                    const llvm::Twine& name = "");

  };

  struct JITBasicBlock {
    llvm::BasicBlock* block;
    llvm::BasicBlock* prologue;
    JITBasicBlock* exception_handler;
    int sp;
    int start_ip;
    int end_ip;
    bool reachable;
    bool landing_pad;

  public:
    JITBasicBlock()
      : block(0)
      , prologue(0)
      , exception_handler(0)
      , sp(-1)
      , start_ip(0)
      , end_ip(0)
      , reachable(false)
      , landing_pad(false)
    {}

    llvm::BasicBlock* entry() {
      if(prologue) return prologue;
      return block;
    }
  };

  typedef std::map<int, JITBasicBlock> BlockMap;

  enum JitDebug {
    cSimple = 1,
    cOptimized = 2,
    cMachineCode = 4
  };

  class BlockEnvironment;

  class BackgroundCompilerThread;

  class LLVMState : public ManagedThread {
    llvm::LLVMContext& ctx_;
    llvm::Module* module_;
    llvm::ExistingModuleProvider* mp_;
    llvm::ExecutionEngine* engine_;
    llvm::FunctionPassManager* passes_;

    const llvm::Type* object_;
    Configuration& config_;

    BackgroundCompilerThread* background_thread_;
    GlobalLock& global_lock_;
    SymbolTable& symbols_;

    int jitted_methods_;
    int queued_methods_;
    int accessors_inlined_;
    int uncommons_taken_;

    SharedState& shared_;
    bool include_profiling_;
    llvm::GlobalVariable* profiling_;

    int code_bytes_;

    std::ostream* log_;

    gc::WriteBarrier write_barrier_;

  public:

    uint64_t time_spent;

    const llvm::Type* VoidTy;

    const llvm::Type* Int1Ty;
    const llvm::Type* Int8Ty;
    const llvm::Type* Int16Ty;
    const llvm::Type* Int32Ty;
    const llvm::Type* Int64Ty;
    const llvm::Type* IntPtrTy;

    const llvm::Type* FloatTy;
    const llvm::Type* DoubleTy;

    const llvm::Type* Int8PtrTy;

    const llvm::Type* ObjType;
    const llvm::Type* ObjArrayTy;

    static LLVMState* get(STATE);
    static void shutdown(STATE);
    static void on_fork(STATE);
    static void pause(STATE);
    static void unpause(STATE);

    LLVMState(STATE);
    ~LLVMState();

    void add_internal_functions();

    int jit_dump_code() {
      return config_.jit_dump_code;
    }

    bool debug_p();

    Configuration& config() {
      return config_;
    }

    GlobalLock& global_lock() {
      return global_lock_;
    }

    llvm::GlobalVariable* profiling() {
      return profiling_;
    }

    bool include_profiling() {
      return include_profiling_;
    }

    llvm::Module* module() { return module_; }
    llvm::ExecutionEngine* engine() { return engine_; }
    llvm::FunctionPassManager* passes() { return passes_; }
    const llvm::Type* object() { return object_; }

    int jitted_methods() {
      return jitted_methods_;
    }

    int queued_methods() {
      return queued_methods_;
    }

    int add_jitted_method() {
      return ++jitted_methods_;
    }

    int code_bytes() {
      return code_bytes_;
    }

    void add_code_bytes(int bytes) {
      code_bytes_ += bytes;
    }

    void add_accessor_inlined() {
      accessors_inlined_++;
    }

    int accessors_inlined() {
      return accessors_inlined_;
    }

    void add_uncommons_taken() {
      uncommons_taken_++;
    }

    int uncommons_taken() {
      return uncommons_taken_;
    }

    SharedState& shared() { return shared_; }

    llvm::LLVMContext& ctx() { return ctx_; }

    std::ostream& log() {
      return *log_;
    }

    gc::WriteBarrier* write_barrier() {
      return &write_barrier_;
    }

    const llvm::Type* ptr_type(std::string name);
    const llvm::Type* type(std::string name);

    void compile_soon(STATE, CompiledMethod* cm, BlockEnvironment* block=0);
    void compile_soon(STATE, Trace* cm);
    void remove(llvm::Function* func);

    CompiledMethod* find_candidate(CompiledMethod* start, CallFrame* call_frame);
    void compile_callframe(STATE, CompiledMethod* start, CallFrame* call_frame,
                           int primitive = -1);


    Symbol* symbol(const char* sym);
    const char* symbol_cstr(const Symbol* sym);

    void shutdown_i();
    void on_fork_i();
    void pause_i();
    void unpause_i();

    static void show_machine_code(void* impl, size_t bytes);
  };


  class Signature {
  protected:
    LLVMState* ls_;
    std::vector<const llvm::Type*> types_;
    const llvm::Type* ret_type_;

  public:
    Signature(LLVMState* ls, const llvm::Type* rt)
      : ls_(ls)
      , ret_type_(rt)
    {}

    Signature(LLVMState* ls, const char* rt)
      : ls_(ls)
      , ret_type_(ls->ptr_type(rt))
    {}

    std::vector<const llvm::Type*>& types() {
      return types_;
    }

    Signature& operator<<(const char* name) {
      types_.push_back(ls_->ptr_type(name));

      return *this;
    }

    Signature& operator<<(const llvm::Type* type) {
      types_.push_back(type);

      return *this;
    }

    llvm::FunctionType* type() {
      return llvm::FunctionType::get(ret_type_, types_, false);
    }

    operator llvm::FunctionType*() { return type(); }

    llvm::Function* function(const char* name) {
      return llvm::cast<llvm::Function>(ls_->module()->getOrInsertFunction(name, type()));
    }

    void setDoesNotCapture(const char* name, int which) {
      function(name)->setDoesNotCapture(which, true);
    }

    llvm::CallInst* call(const char* name, llvm::Value** start, int size,
			 const char* inst_name, llvm::BasicBlock* block) {
      return llvm::CallInst::Create(function(name), start, start+size, inst_name, block);
    }

    llvm::CallInst* call(const char* name, llvm::Value** start, int size,
			 const char* inst_name, llvm::IRBuilder<>& builder) {
      return builder.CreateCall(function(name), start, start+size, inst_name);
    }

    llvm::CallInst* call(const char* name, std::vector<llvm::Value*> args,
			 const char* inst_name, llvm::IRBuilder<>& builder) {
      return builder.CreateCall(function(name), args.begin(), args.end(), inst_name);
    }

  };

  class BackgroundCompileRequest {
    Trace* trace_;

  public:
    BackgroundCompileRequest(STATE, Trace* trace)
      : trace_(trace)
    {}

    Trace* trace() {
      return trace_;
    }

    bool is_trace() {
      return trace_ != NULL;
    }
  };

}

#endif
