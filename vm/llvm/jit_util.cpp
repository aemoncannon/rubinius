#ifdef ENABLE_LLVM

#include "vm.hpp"
#include "objectmemory.hpp"
#include "llvm/jit.hpp"
#include "call_frame.hpp"
#include "trace.hpp"
#include "builtin/object.hpp"
#include "builtin/symbol.hpp"
#include "builtin/system.hpp"
#include "builtin/class.hpp"
#include "builtin/string.hpp"
#include "builtin/block_environment.hpp"
#include "builtin/staticscope.hpp"
#include "builtin/proc.hpp"
#include "builtin/autoload.hpp"
#include "builtin/global_cache_entry.hpp"
#include "builtin/iseq.hpp"
#include "builtin/memorypointer.hpp"
#include "builtin/integer.hpp"
#include "builtin/float.hpp"

#include "instruments/profiler.hpp"

#include "helpers.hpp"
#include "utilities.hpp"

#include "arguments.hpp"
#include "dispatch.hpp"
#include "lookup_data.hpp"
#include "inline_cache.hpp"

#include <cstdarg>

#define both_fixnum_p(_p1, _p2) ((uintptr_t)(_p1) & (uintptr_t)(_p2) & TAG_FIXNUM)

using namespace rubinius;

extern "C" {

  void rbx_show_int(int i)
  {
    std::cout << "DEBUG: " << i << "\n";
  }

  void rbx_show_vars(STATE, CallFrame* call_frame, StackVariables* vars)
  {
    CompiledMethod* cm = call_frame->cm;
    VMMethod* vmm = cm->backend_method();
    int number_of_locals = vmm->number_of_locals;
    for(int i = 0; i < number_of_locals; i++){
      Object* obj = vars->get_local(i);
      String* s = obj->to_s(state);
      std::cout << "  " << i << ": " << s->c_str() << "\n";
    }
  }

  void rbx_show_obj(STATE, Object* obj)
  {
		std::cout << "Dumping Object* " << obj;
    if(obj == NULL){
      std::cout << "NULL Object pointer.\n";
    }
    else{
      obj->type_info(state)->show(state, obj, 1);
    }
  }


  void rbx_show_state(STATE, CallFrame* call_frame)
  {
    Object** stk = call_frame->stk;
    StackVariables* vars = call_frame->scope;
    CompiledMethod* cm = call_frame->cm;
    VMMethod* vmm = cm->backend_method();
    int stack_size = vmm->stack_size;
    int number_of_locals = vmm->number_of_locals;
    std::cout << "---------------------" << "\n";
    std::cout << "stack size = " << stack_size << "\n";
    std::cout << "number of locals = " << number_of_locals << "\n";
    std::cout << "Stack: " << "\n";

    for(int i = 0; i < stack_size; i++){
      Object* obj = stk[i];
      String* s = obj->to_s(state);
      std::cout << "  " << i << ": " << s->c_str() << "\n";
    }
    std::cout << "Locals: " << "\n";

    for(int i = 0; i < number_of_locals; i++){
      Object* obj = vars->get_local(i);
      String* s = obj->to_s(state);
      std::cout << "  " << i << ": " << s->c_str() << "\n";
    }
    std::cout << "---------------------" << "\n";

  }

  void rbx_print_debug()
  {
    std::cout << "trace debug!" << "\n";
  }



  Object* rbx_write_barrier(STATE, Object* obj, Object* val) {
    if(obj->zone() == UnspecifiedZone) return val;
    state->om->write_barrier(obj, val);
    return val;
  }

  Object* rbx_check_frozen(STATE, CallFrame* call_frame, Object* obj) {
    if(obj->reference_p() && obj->is_frozen_p()) {
      Exception::frozen_error(state, call_frame);
      return NULL;
    }

    return obj;
  }

  void rbx_begin_profiling(STATE, void* data, Dispatch& msg, Arguments& args,
                           CompiledMethod* cm)
  {
    // Use placement new to stick the class into data, which is on the callers
    // stack.
    new(data) profiler::MethodEntry(state, msg, args, cm);
  }

  void rbx_end_profiling(profiler::MethodEntry* entry) {
    entry->~MethodEntry();
  }

  Object* rbx_simple_send(STATE, CallFrame* call_frame, Symbol* name,
                          int count, Object** args) {
    Object* recv = args[0];
    Arguments out_args(recv, count, args+1);
    Dispatch dis(name);

    return dis.send(state, call_frame, out_args);
  }

  Object* rbx_simple_send_private(STATE, CallFrame* call_frame, Symbol* name,
                                  int count, Object** args) {
    Object* recv = args[0];
    Arguments out_args(recv, count, args+1);
    LookupData lookup(recv, recv->lookup_begin(state), true);
    Dispatch dis(name);

    return dis.send(state, call_frame, lookup, out_args);
  }

  Object* rbx_splat_send(STATE, CallFrame* call_frame, Symbol* name,
												 int count, Object** args) {
    Object* recv = args[0];
    Arguments out_args(recv, args[count+2], count, args+1);
    Dispatch dis(name);

    if(Array* ary = try_as<Array>(args[count+1])) {
      out_args.append(state, ary);
    }

    return dis.send(state, call_frame, out_args);
  }

  Object* rbx_splat_send_private(STATE, CallFrame* call_frame, Symbol* name,
																 int count, Object** args) {
    Object* recv = args[0];
    Arguments out_args(recv, args[count+2], count, args+1);
    LookupData lookup(recv, recv->lookup_begin(state), true);
    Dispatch dis(name);

    if(Array* ary = try_as<Array>(args[count+1])) {
      out_args.append(state, ary);
    }

    return dis.send(state, call_frame, lookup, out_args);
  }

  Object* rbx_super_send(STATE, CallFrame* call_frame, Symbol* name,
												 int count, Object** args) {
    Object* recv = call_frame->self();
    Arguments out_args(recv, args[count], count, args);
    LookupData lookup(recv, call_frame->module()->superclass(), true);
    Dispatch dis(name);

    return dis.send(state, call_frame, lookup, out_args);
  }

  Object* rbx_super_splat_send(STATE, CallFrame* call_frame, Symbol* name,
															 int count, Object** args) {
    Object* recv = call_frame->self();
    Arguments out_args(recv, args[count+1], count, args);
    LookupData lookup(recv, call_frame->module()->superclass(), true);
    Dispatch dis(name);

    out_args.append(state, as<Array>(args[count]));

    return dis.send(state, call_frame, lookup, out_args);
  }

  Object* rbx_zsuper_send(STATE, CallFrame* call_frame, Symbol* name, Object* block) {
    Object* const recv = call_frame->self();

    VariableScope* scope = call_frame->method_scope(state);
    assert(scope);

    VMMethod* v = scope->method()->backend_method();
    Object* splat_obj = 0;
    Array* splat = 0;

    size_t arg_count = v->total_args;

    if(v->splat_position >= 0) {
      splat_obj = scope->get_local(state, v->splat_position);
      splat = try_as<Array>(splat_obj);
      if(splat) {
        arg_count += splat->size();
      } else {
        arg_count++;
      }
    }

    Tuple* tup = Tuple::create(state, arg_count);
    for(int i = 0; i < v->total_args; i++) {
      tup->put(state, i, scope->get_local(state, i));
    }

    if(splat) {
      for(size_t i = 0; i < splat->size(); i++) {
        tup->put(state, i + v->total_args, splat->get(state, i));
      }
    } else if(splat_obj) {
      tup->put(state, v->total_args, splat_obj);
    }

    Arguments out_args(recv, block, arg_count, 0);
    out_args.use_tuple(tup, arg_count);

    LookupData lookup(recv, call_frame->module()->superclass(), true);
    Dispatch dis(name);

    return dis.send(state, call_frame, lookup, out_args, eSuper);
  }

  Object* rbx_arg_error(STATE, CallFrame* call_frame, Dispatch& msg, Arguments& args,
                        int required) {
    Exception* exc =
      Exception::make_argument_error(state, required, args.total(), msg.name);
    exc->locations(state, System::vm_backtrace(state, Fixnum::from(0), call_frame));
    state->thread_state()->raise_exception(exc);

    return NULL;
  }

  Object* rbx_string_dup(STATE, CallFrame* call_frame, Object* obj) {
    try {
      return as<String>(obj)->string_dup(state);
    } catch(TypeError& e) {
      Exception* exc =
        Exception::make_type_error(state, e.type, e.object, e.reason);
      exc->locations(state, System::vm_backtrace(state, Fixnum::from(0), call_frame));

      state->thread_state()->raise_exception(exc);
      return NULL;
    }
  }

  Object* rbx_create_block(STATE, CallFrame* call_frame, int index) {
    Object* _lit = call_frame->cm->literals()->at(state, index);
    CompiledMethod* cm = as<CompiledMethod>(_lit);

    // TODO: We don't need to be doing this everytime.
    if(cm->scope()->nil_p()) {
      cm->scope(state, call_frame->static_scope());
    }

    VMMethod* vmm = call_frame->cm->backend_method();
    return BlockEnvironment::under_call_frame(state, cm, vmm,
                                              call_frame, index);
  }

  Object* rbx_create_block_multi(STATE, CompiledMethod* cm, int index, int count, ...) {
    va_list ap;

    CallFrame* closest = 0;
    VariableScope* top = 0;
    VariableScope* parent = 0;

    va_start(ap, count);
    for(int i = 0; i < count; i++) {
      closest = va_arg(ap, CallFrame*);
      closest->scope->set_parent(parent);
      parent = closest->promote_scope(state);

      if(!top) {
        top = parent;
      } else {
        closest->flags |= CallFrame::cMultipleScopes;
        closest->top_scope_ = top;
      }
    }
    va_end(ap);

    // TODO: We don't need to be doing this everytime.
    cm->scope(state, closest->static_scope());

    VMMethod* vmm = closest->cm->backend_method();
    return BlockEnvironment::under_call_frame(state, cm, vmm, closest, index);
  }

  Object* rbx_promote_variables(STATE, CallFrame* call_frame) {
    return call_frame->promote_scope(state);
  }

  Object* rbx_construct_splat(STATE, Arguments& args, size_t total) {
    if(args.total() > total) {
      size_t splat_size = args.total() - total;
      Array* ary = Array::create(state, splat_size);

      for(size_t i = 0, n = total; i < splat_size; i++, n++) {
        ary->set(state, i, args.get_argument(n));
      }
      return ary;
    } else {
      return Array::create(state, 0);
    }
  }

  Object* rbx_cast_array(STATE, CallFrame* call_frame, Object* top) {
    if(Tuple* tup = try_as<Tuple>(top)) {
      return Array::from_tuple(state, tup);
    } else if(kind_of<Array>(top)) {
      return top;
    }

    // coerce
    Object* recv = G(array);
    Arguments args(recv, 1, &top);
    Dispatch dis(G(sym_coerce_into_array));

    return dis.send(state, call_frame, args);
  }

  Object* rbx_cast_multi_value(STATE, CallFrame* call_frame, Object* top) {
    if(kind_of<Array>(top)) return top;
    return Array::to_ary(state, top, call_frame);
  }

  Object* rbx_add_scope(STATE, CallFrame* call_frame, Object* top) {
    Module* mod = as<Module>(top);
    StaticScope* scope = StaticScope::create(state);
    scope->module(state, mod);
    scope->parent(state, call_frame->static_scope());
    call_frame->cm->scope(state, scope);
    // call_frame->static_scope_ = scope;

    return Qnil;
  }

  Object* rbx_cast_for_splat_block_arg(STATE, Arguments& args) {
    Array* ary = Array::create(state, args.total());
    for(size_t i = 0; i < args.total(); i++) {
      ary->set(state, i, args.get_argument(i));
    }

    return ary;
  }

  Object* rbx_cast_for_multi_block_arg(STATE, Arguments& args) {
    /* If there is only one argument and that thing is an array... */
    if(args.total() == 1 && kind_of<Array>(args.get_argument(0))) {
      return args.get_argument(0);
    }

    Array* ary = Array::create(state, args.total());
    for(size_t i = 0; i < args.total(); i++) {
      ary->set(state, i, args.get_argument(i));
    }

    return ary;
  }

  Object* rbx_cast_for_multi_block_arg_varargs(STATE, int count, ...) {
    va_list ap;

    /* If there is only one argument and that thing is an array... */
    if(count == 1) {
      va_start(ap, count);

      Object* first = va_arg(ap, Object*);
      if(!kind_of<Array>(first)) {
        Array* ary = Array::create(state, 1);
        ary->set(state, 0, first);
        first = ary;
      }

      va_end(ap);
      return first;
    }

    Array* ary = Array::create(state, count);

    va_start(ap, count);
    for(int i = 0; i < count; i++) {
      ary->set(state, i, va_arg(ap, Object*));
    }
    va_end(ap);

    return ary;
  }

  Object* rbx_cast_for_single_block_arg(STATE, Arguments& args) {
    int k = args.total();
    if(k == 0) {
      return Qnil;
    } else if(k == 1) {
      return args.get_argument(0);
    }

    Array* ary = Array::create(state, k);
    for(int i = 0; i < k; i++) {
      ary->set(state, i, args.get_argument(i));
    }

    return ary;
  }

  Object* rbx_check_serial(STATE, CallFrame* call_frame, InlineCache* cache,
                           int serial, Object* recv)
  {
    if(cache->update_and_validate(state, call_frame, recv) &&
       cache->method->serial()->to_native() == serial) {
      return Qtrue;
    }

    return Qfalse;
  }

  Object* rbx_check_serial_private(STATE, CallFrame* call_frame, InlineCache* cache,
																	 int serial, Object* recv)
  {
    if(cache->update_and_validate(state, call_frame, recv) &&
       cache->method->serial()->to_native() == serial) {
      return Qtrue;
    }

    return Qfalse;
  }

  Object* rbx_find_const(STATE, CallFrame* call_frame, int index, Object* top) {
    bool found;
    Module* under = as<Module>(top);
    Symbol* sym = as<Symbol>(call_frame->cm->literals()->at(state, index));
    Object* res = Helpers::const_get_under(state, under, sym, &found);

    if(!found) {
      res = Helpers::const_missing_under(state, under, sym, call_frame);
    } else if(Autoload* autoload = try_as<Autoload>(res)) {
      res = autoload->resolve(state, call_frame);
    }

    return res;
  }

  Object* rbx_instance_of(STATE, Object* top, Object* b1) {
    Class* cls = as<Class>(b1);
    if(top->class_object(state) == cls) return Qtrue;
    return Qfalse;
  }

  Object* rbx_kind_of(STATE, Object* top, Object* b1) {
    return top->kind_of_p(state, b1) ? Qtrue : Qfalse;
  }

  Object* rbx_make_array(STATE, int count, Object** args) {
    Array* ary = Array::create(state, count);
    for(int i = 0; i < count; i++) {
      ary->set(state, i, args[i]);
    }

    return ary;
  }

  Object* rbx_create_array(STATE, int count, ...) {
    va_list ap;

    Array* ary = Array::create(state, count);

    va_start(ap, count);
    for(int i = 0; i < count; i++) {
      Object* obj = va_arg(ap, Object*);
      ary->set(state, i, obj);
    }

    va_end(ap);

    return ary;
  }

  Object* rbx_meta_send_call(STATE, CallFrame* call_frame, int count, Object** args) {
    Object* t1 = args[0];

    Arguments out_args(Qnil, count, args+1);

    if(BlockEnvironment *env = try_as<BlockEnvironment>(t1)) {
      return env->call(state, call_frame, out_args);
    } else if(Proc* proc = try_as<Proc>(t1)) {
      return proc->call(state, call_frame, out_args);
    }

    return rbx_simple_send(state, call_frame, G(sym_call), count, args);
  }

  Object* rbx_yield_stack(STATE, CallFrame* call_frame, Object* block,
                          int count, Object** args)
  {
    Arguments out_args(block, count, args);

    if(BlockEnvironment *env = try_as<BlockEnvironment>(block)) {
      return env->call(state, call_frame, out_args);
    } else if(Proc* proc = try_as<Proc>(block)) {
      return proc->yield(state, call_frame, out_args);
    } else if(block->nil_p()) {
      state->thread_state()->raise_exception(Exception::make_lje(state, call_frame));
      return NULL;
    }

    Dispatch dis(G(sym_call));

    return dis.send(state, call_frame, out_args);
  }

  Object* rbx_yield_splat(STATE, CallFrame* call_frame, Object* block,
                          int count, Object** stk)
  {
    Object* ary = stk[count];

    Arguments args(block, count, stk);

    if(!ary->nil_p()) {
      args.append(state, as<Array>(ary));
    }

    if(BlockEnvironment *env = try_as<BlockEnvironment>(block)) {
      return env->call(state, call_frame, args);
    } else if(Proc* proc = try_as<Proc>(block)) {
      return proc->yield(state, call_frame, args);
    } else if(block->nil_p()) {
      state->thread_state()->raise_exception(Exception::make_lje(state, call_frame));
      return NULL;
    }

    Dispatch dis(G(sym_call));
    return dis.send(state, call_frame, args);
  }

  Object* rbx_meta_send_op_gt(STATE, CallFrame* call_frame, Object** stk) {
    Object* t1 = stk[0];
    Object* t2 = stk[1];
    if(both_fixnum_p(t1, t2)) {
      native_int j = as<Integer>(t1)->to_native();
      native_int k = as<Integer>(t2)->to_native();
      return (j > k) ? Qtrue : Qfalse;
    }

    return rbx_simple_send(state, call_frame, G(sym_gt), 1, stk);
  }

  Object* rbx_meta_send_op_minus(STATE, CallFrame* call_frame, Object** stk) {
    Object* left =  stk[0];
    Object* right = stk[1];

    if(both_fixnum_p(left, right)) {
      return reinterpret_cast<Fixnum*>(left)->sub(state,
																									reinterpret_cast<Fixnum*>(right));

    }

    return rbx_simple_send(state, call_frame, G(sym_minus), 1, stk);
  }

  Object* rbx_meta_send_op_nequal(STATE, CallFrame* call_frame, Object** stk) {
    Object* t1 = stk[0];
    Object* t2 = stk[1];
    /* If both are not references, compare them directly. */
    if(!t1->reference_p() && !t2->reference_p()) {
      return (t1 == t2) ? Qfalse : Qtrue;
    }

    return rbx_simple_send(state, call_frame, G(sym_nequal), 1, stk);
  }

  Object* rbx_meta_send_op_tequal(STATE, CallFrame* call_frame, Object** stk) {
    Object* t1 = stk[0];
    Object* t2 = stk[1];
    /* If both are fixnums, or both are symbols, compare the ops directly. */
    if((t1->fixnum_p() && t2->fixnum_p()) || (t1->symbol_p() && t2->symbol_p())) {
      return (t1 == t2) ? Qfalse : Qtrue;
    }

    return rbx_simple_send(state, call_frame, G(sym_tequal), 1, stk);
  }

  Object* rbx_passed_arg(STATE, Arguments& args, int index) {
    return (index < (int)args.total()) ? Qtrue : Qfalse;
  }

  // TODO remove this and use passed_arg
  Object* rbx_passed_blockarg(STATE, Arguments& args, int index) {
    return (index == (int)args.total()) ? Qtrue : Qfalse;
  }

  Object* rbx_push_const(STATE, CallFrame* call_frame, Symbol* sym) {
    bool found;
    Object* res = Helpers::const_get(state, call_frame, sym, &found);

    if(!found) {
      res = Helpers::const_missing(state, sym, call_frame);
    } else if(Autoload* autoload = try_as<Autoload>(res)) {
      res = autoload->resolve(state, call_frame);
    }

    return res;
  }

  Object* rbx_push_const_fast(STATE, CallFrame* call_frame, Symbol* sym,
                              int association_index) {
    bool found;
    Object* res = 0;

    Object* val = call_frame->cm->literals()->at(state, association_index);

    // See if the cache is present, if so, validate it and use the value
    GlobalCacheEntry* cache;
    if((cache = try_as<GlobalCacheEntry>(val)) != NULL) {
      if(cache->valid_p(state)) {
        res = cache->value();
      } else {
        res = Helpers::const_get(state, call_frame, sym, &found);
        if(found) cache->update(state, res);
      }
    } else {
      res = Helpers::const_get(state, call_frame, sym, &found);
      if(found) {
        cache = GlobalCacheEntry::create(state, res);
        call_frame->cm->literals()->put(state, association_index, cache);
      } else {
        res = Helpers::const_missing(state, sym, call_frame);
      }
    }

    if(!res) return NULL;

    if(Autoload* autoload = try_as<Autoload>(res)) {
      res = autoload->resolve(state, call_frame);
      if(cache && res) {
        cache->update(state, res);
      }
    }

    return res;
  }

  Object* rbx_set_local_depth(STATE, CallFrame* call_frame, Object* top,
                              int depth, int index) {
    if(depth == 0) {
      call_frame->scope->set_local(index, top);
    } else {
      VariableScope* scope = call_frame->scope->parent();
      for(int j = 1; j < depth; j++) {
        scope = scope->parent();
      }
      scope->set_local(state, index, top);
    }

    return top;
  }

  Object* rbx_set_local_from(STATE, CallFrame* call_frame, Object* top,
                             int depth, int index) {
    VariableScope* scope = call_frame->scope->parent();

    for(int j = 1; j < depth; j++) {
      scope = scope->parent();
    }

    scope->set_local(state, index, top);

    return top;
  }

  Object* rbx_push_local_depth(STATE, CallFrame* call_frame,
															 int depth, int index) {
    if(depth == 0) {
      return call_frame->scope->get_local(index);
    } else {
      VariableScope* scope = call_frame->scope->parent();

      for(int j = 1; j < depth; j++) {
        scope = scope->parent();
      }

      return scope->get_local(index);
    }
  }

  Object* rbx_push_local_from(STATE, CallFrame* call_frame,
                              int depth, int index) {
    VariableScope* scope = call_frame->scope->parent();

    for(int j = 1; j < depth; j++) {
      scope = scope->parent();
    }

    return scope->get_local(index);
  }

  Object* rbx_prologue_check(STATE, CallFrame* call_frame) {
    if(!state->check_stack(call_frame, &state)) return NULL;
    if(!state->check_async(call_frame)) return NULL;
    if(!state->interrupts.check) return Qtrue;

    state->interrupts.checked();

    if(state->interrupts.perform_gc) {
      state->interrupts.perform_gc = false;
      state->collect_maybe(call_frame);
    }

    if(state->interrupts.timer) {
      state->interrupts.timer = false;
      state->set_call_frame(call_frame);
      state->global_lock().yield();
    }

    return Qtrue;
  }

  Object* rbx_check_interrupts(STATE, CallFrame* call_frame) {
    if(unlikely(state->interrupts.check)) {
      state->interrupts.checked();

      if(state->interrupts.perform_gc) {
        state->interrupts.perform_gc = true;
        state->collect_maybe(call_frame);
      }

      if(state->interrupts.timer) {
        state->interrupts.timer = false;
        state->set_call_frame(call_frame);
        state->global_lock().yield();
      }
    }
    if(!state->check_async(call_frame)) return NULL;
    return Qtrue;
  }

  bool rbx_return_to_here(STATE, CallFrame* call_frame) {
    ThreadState* th = state->thread_state();
    if(th->raise_reason() != cReturn) return false;
    if(th->destination_scope() == call_frame->scope->on_heap()) return true;
    return false;
  }

  bool rbx_break_to_here(STATE, CallFrame* call_frame) {
    ThreadState* th = state->thread_state();
    if(th->raise_reason() != cBreak) return false;
    if(th->destination_scope() == call_frame->scope->on_heap()) return true;
    return false;
  }

  Object* rbx_clear_raise_value(STATE) {
    Object* val = state->thread_state()->raise_value();
    state->thread_state()->clear_return();
    return val;
  }

  bool rbx_raising_exception(STATE) {
    return state->thread_state()->raise_reason() == cException;
  }

  Object* rbx_current_exception(STATE) {
    return state->thread_state()->current_exception();
  }

  Object* rbx_clear_exception(STATE) {
    state->thread_state()->clear_raise();
    return Qnil;
  }

  Object* rbx_push_exception_state(STATE) {
    return state->thread_state()->state_as_object(state);
  }

  Object* rbx_restore_exception_state(STATE, CallFrame* call_frame, Object* top) {
    if(top->nil_p()) {
      state->thread_state()->clear();
    } else {
      state->thread_state()->set_state(state, top);
    }

    return Qnil;
  }

  Object* rbx_push_system_object(STATE, int which) {
    switch(which) {
    case 0:
      return G(object);
    default:
      return Qnil;
    }
  }

  Object* rbx_push_ivar(STATE, Object* self, Symbol* name) {
    return self->get_ivar(state, name);
  }

  Object* rbx_set_ivar(STATE, Object* self, Symbol* name, Object* val) {
    return self->set_ivar(state, name, val);
  }

  Object* rbx_push_my_field(STATE, Object* self, int which) {
    return self->get_field(state, which);
  }

  Object* rbx_set_my_field(STATE, Object* self, int which, Object* val) {
    self->set_field(state, which, val);
    return val;
  }

  Object* rbx_set_const(STATE, CallFrame* call_frame, Symbol* name, Object* val) {
    call_frame->static_scope()->module()->set_const(state, name, val);
    return val;
  }

  Object* rbx_set_const_at(STATE, Symbol* name, Object* mod, Object* val) {
    // TODO if the as<> fails, the process will abort().
    as<Module>(mod)->set_const(state, name, val);
    return val;
  }

  Object* rbx_set_literal(STATE, CallFrame* call_frame, int which, Object* val) {
    call_frame->cm->literals()->put(state, which, val);
    return Qnil;
  }

  Object* rbx_shift_array(STATE, Object** loc) {
    Array* array = as<Array>(*loc);
    size_t size = (size_t)array->size();

    if(size == 0) return Qnil;

    size_t j = size - 1;
    Object* shifted_value = array->get(state, 0);
    Array* smaller_array = Array::create(state, j);

    for(size_t i = 0; i < j; i++) {
      smaller_array->set(state, i, array->get(state, i+1));
    }

    *loc = smaller_array;
    return shifted_value;
  }

  Object* rbx_string_append(STATE, Object* left, Object* right) {
    // TODO Don't use as<>, since it will throw a C++ exception
    // into JITd code.
    return as<String>(left)->append(state, as<String>(right));
  }

  Object* rbx_string_build(STATE, CallFrame* call_frame, int count, Object** parts) {
    size_t size = 0;

    // Figure out the total size
    for(int i = 0; i < count; i++) {
      Object* obj = parts[i];
      String* str = try_as<String>(obj);

      if(str) {
        size += str->size();
      } else {
        // This isn't how MRI does this. If sub isn't a String, it converts the
        // the original object via any_to_s, not the bad value returned from #to_s.
        // This quite a bit harder to implement in rubinius atm, so i'm opting for
        // this way instead.

        str = obj->to_s(state, false);
        size += str->size();

        parts[i] = str;
      }
    }

    String* str = String::create(state, 0, size);
    uint8_t* pos = str->byte_address();

    for(int i = 0; i < count; i++) {
      // We can force here because we've typed check them above.
      String* sub = force_as<String>(parts[i]);
      memcpy(pos, sub->byte_address(), sub->size());
      pos += sub->size();
    }

    return str;
  }

  Object* rbx_raise_return(STATE, CallFrame* call_frame, Object* top) {
    if(!(call_frame->flags & CallFrame::cIsLambda) &&
       !call_frame->scope_still_valid(call_frame->scope->parent())) {
      Exception* exc = Exception::make_exception(state, G(jump_error), "unexpected return");
      exc->locations(state, System::vm_backtrace(state, Fixnum::from(0), call_frame));
      state->thread_state()->raise_exception(exc);
    } else {
      state->thread_state()->raise_return(top, call_frame->top_scope(state));
    }

    return Qnil;
  }

  Object* rbx_ensure_return(STATE, CallFrame* call_frame, Object* top) {
    state->thread_state()->raise_return(top, call_frame->promote_scope(state));
    return Qnil;
  }

  Object* rbx_raise_break(STATE, CallFrame* call_frame, Object* top) {
    state->thread_state()->raise_break(top, call_frame->scope->parent());
    return Qnil;
  }

  bool rbx_check_class(STATE, Object* obj, int id) {
    return obj->lookup_begin(state)->class_id() == id;
  }

  Object* rbx_get_ivar(STATE, Object* self, Symbol* name) {
    return self->get_ivar(state, name);
  }

  void rbx_set_table_ivar(STATE, Object* self, Symbol* name, Object* val) {
    self->set_table_ivar(state, name, val);
  }

  
  int rbx_side_exit(STATE, CallFrame* call_frame, Trace* exit_trace, TraceNode* exit_node, int run_mode){

 		assert(exit_node);

    TRACK_TIME(IN_EXIT_TIMER);
    DEBUGLN("No branch to continue on. Exiting from:");
		IF_DEBUG(exit_node->pretty_print(state, std::cout));
		DEBUGLN("\n");
    IF_DEBUG(call_frame->dump());

    DEBUGLN("Is traced frame? " << call_frame->is_traced_frame());

		int next_pc = exit_node->exit_to_pc();
    DEBUGLN("Next pc: " << next_pc);

 		// Flush the interpreter stack and pc information
 		// out to the call stack.

		call_frame->set_ip(next_pc);
 		call_frame->set_sp(exit_node->sp);
 
 		CallFrame* cf = call_frame->previous;
 		TraceNode* activator = exit_node->active_send;
		Trace* cur_trace = exit_trace;

		int num_flushed = 0;
 		while(true){
			DEBUGLN("Walking to call_frame: " << cf);
			DEBUGLN("Walking to activator: " << activator);

			// Ummmm this should be static..

 			if(activator == NULL) {
				// Bridge gaps between traces (nested/branches)
				while(cur_trace->parent_node != NULL){
					activator = cur_trace->parent_node->active_send;
					if(activator == NULL) {
						cur_trace = cur_trace->parent;
						continue;
					}
					else{
						cur_trace = cur_trace->parent;
						break;
					}
				}
				if(activator == NULL) break;
			}

			assert(cf);
			assert(activator->traced_send || activator->traced_yield);

			DEBUGLN("Flushing to call_frame:");
			IF_DEBUG(call_frame->dump());
			cf->set_ip(activator->pc + activator->pc_effect);
			cf->set_sp(activator->sp + activator->stck_effect);
			num_flushed++;
 
			activator = activator->active_send;
			cf = cf->previous;
		}

		ThreadState* th = state->thread_state();
		// Maybe start recording a branch trace...
		if(th->raise_reason() == cNone && exit_node->bump_exit_hotness()){
			DEBUGLN("Exit node at " << exit_node->pc << " got hot! Recording branch...");
			state->recording_trace = exit_trace->create_branch_at(exit_node);
			exit_node->disable_counter();
		}

		// Bail to uncommon if we've stacked up call_frames before the exit.
		if(num_flushed > 0){

		// Or if a nested trace exited unexpectedly (we don't know _where_ it
		// ended up)...
			 // Why is this necessary?
//			 || run_mode == Trace::RUN_MODE_NESTED || run_mode == Trace::RUN_MODE_RECORD_NESTED){

			VMMethod* vmm = call_frame->cm->backend_method();
			VMMethod::uncommon_interpreter(state, vmm, call_frame);
		}

		// Otherwise, just return directly to caller...
		TRACK_TIME(ON_TRACE_TIMER);
		return Trace::RETURN_SIDE_EXITED;
	}





	void rbx_track_time(STATE, int timer) {
		state->start_trace_timer(timer);
	}

	Object* rbx_restart_interp(STATE, CallFrame* call_frame, Dispatch& msg, Arguments& args) {
		return VMMethod::execute(state, call_frame, msg, args);
	}

	Object* rbx_flush_scope(STATE, StackVariables* vars) {
		vars->flush_to_heap(state);
		return Qnil;
	}

	// FFI helpers
	native_int rbx_ffi_to_int(STATE, Object* obj, bool* valid) {
		if(Integer* i = try_as<Integer>(obj)) {
			*valid = true;
			return i->to_native();
		}

		Exception* exc =
			Exception::make_type_error(state, Fixnum::type, obj, "invalid type for FFI");
		state->thread_state()->raise_exception(exc);

		*valid = false;
		return 0;
	}

	float rbx_ffi_to_float(STATE, Object* obj, bool* valid) {
		if(Float* i = try_as<Float>(obj)) {
			*valid = true;
			return i->val;
		}

		Exception* exc =
			Exception::make_type_error(state, Float::type, obj, "invalid type for FFI");
		state->thread_state()->raise_exception(exc);

		*valid = false;
		return 0.0;
	}

	double rbx_ffi_to_double(STATE, Object* obj, bool* valid) {
		if(Float* i = try_as<Float>(obj)) {
			*valid = true;
			return i->val;
		}

		Exception* exc =
			Exception::make_type_error(state, Float::type, obj, "invalid type for FFI");
		state->thread_state()->raise_exception(exc);

		*valid = false;
		return 0.0;
	}

	uint64_t rbx_ffi_to_int64(STATE, Object* obj, bool* valid) {
		if(Integer* i = try_as<Integer>(obj)) {
			*valid = true;
			return i->to_long_long();
		}

		Exception* exc =
			Exception::make_type_error(state, Fixnum::type, obj, "invalid type for FFI");
		state->thread_state()->raise_exception(exc);

		*valid = false;
		return 0ULL;
	}

	void* rbx_ffi_to_ptr(STATE, Object* obj, bool* valid) {
		if(MemoryPointer* ptr = try_as<MemoryPointer>(obj)) {
			*valid = true;
			return ptr->pointer;
		} else if(obj->nil_p()) {
			*valid = true;
			return 0;
		}

		Exception* exc =
			Exception::make_type_error(state, Fixnum::type, obj, "invalid type for FFI");
		state->thread_state()->raise_exception(exc);

		*valid = false;
		return 0;
	}

	char* rbx_ffi_to_string(STATE, Object* obj, bool* valid) {
		if(String* str = try_as<String>(obj)) {
			*valid = true;
			return const_cast<char*>(str->c_str());
		}

		Exception* exc =
			Exception::make_type_error(state, Fixnum::type, obj, "invalid type for FFI");
		state->thread_state()->raise_exception(exc);

		*valid = false;
		return 0;
	}

	Object* rbx_ffi_from_int32(STATE, int32_t ll) {
		return Integer::from(state, ll);
	}

	Object* rbx_ffi_from_int64(STATE, int64_t ll) {
		return Integer::from(state, ll);
	}

	Object* rbx_ffi_from_float(STATE, float val) {
		return Float::create(state, val);
	}

	Object* rbx_ffi_from_double(STATE, double val) {
		return Float::create(state, val);
	}

	Object* rbx_ffi_from_ptr(STATE, void* ptr) {
		if(!ptr) return Qnil;
		return MemoryPointer::create(state, ptr);
	}

	Object* rbx_ffi_from_string(STATE, char* ptr) {
		if(!ptr) return Qnil;
		String* str = String::create(state, ptr);
		str->taint(state);
		return str;
	}

	Object* rbx_ffi_from_string_with_pointer(STATE, char* ptr) {
		Object* s;
		Object* p;

		if(ptr) {
			s = String::create(state, ptr);
			s->taint(state);
			p = MemoryPointer::create(state, ptr);
		} else {
			s = p = Qnil;
		}

		Array* ary = Array::create(state, 2);
		ary->set(state, 0, s);
		ary->set(state, 1, p);

		return ary;
	}

	Float* rbx_float_allocate(STATE) {
		return Float::create(state, 0.0);
	}

	Class* rbx_class_of(STATE, Object* obj) {
		return obj->class_object(state);
	}

	Object* rbx_make_proc(STATE, CallFrame* call_frame) {
		Object* obj = call_frame->scope->block();
		if(RTEST(obj)) {
			Object* prc = Proc::from_env(state, obj);
			if(prc == Primitives::failure()) {
				Exception::internal_error(state, call_frame, "invalid block type");
				return 0;
			}

			return prc;
		} else {
			return Qnil;
		}
	}

}

#endif
