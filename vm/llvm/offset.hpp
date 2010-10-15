#ifndef RBX_LLVM_OFFSET_HPP
#define RBX_LLVM_OFFSET_HPP

namespace offset {
  const static int trace_branch_tbl = 0;
  const static int trace_executor = 1;

  const static int trace_info_exit_ip = 0;
  const static int trace_info_exit_stk = 1;
  const static int trace_info_exit_cf = 2;
  const static int trace_info_entry_cf = 3;
  const static int trace_info_expected_exit_ip = 4;
  const static int trace_info_nestable = 5;
  const static int trace_info_recording = 6;
  const static int trace_info_nested = 7;
  const static int trace_info_next_ip = 8;
  const static int trace_info_exit_trace_node = 9;

  const static int cf_previous = 0;
  const static int cf_static_scope = 1;
  const static int cf_msg = 2;
  const static int cf_cm = 3;
  const static int cf_flags = 4;
  const static int cf_ip = 5;
  const static int cf_sp = 6;
  const static int cf_jit_data = 7;
  const static int cf_top_scope = 8;
  const static int cf_scope = 9;
  const static int cf_arguments = 10;
  const static int cf_stk = 11;

  const static int args_recv = 0;
  const static int args_block = 1;
  const static int args_total = 2;
  const static int args_ary = 3;
  const static int args_container = 4;

  const static int vars_on_heap = 0;
  const static int vars_parent = 1;
  const static int vars_self = 2;
  const static int vars_block = 3;
  const static int vars_module = 4;
  const static int vars_last_match = 5;
  const static int vars_tuple = 6;

  const static int varscope_block = 1;
  const static int varscope_method = 2;
  const static int varscope_module = 3;
  const static int varscope_parent = 4;
  const static int varscope_heap_locals = 5;
  const static int varscope_self = 6;
  const static int varscope_num_locals = 7;
  const static int varscope_isolated = 8;
  const static int varscope_locals = 9;

  const static int tuple_full_size = 1;
  const static int tuple_field = 2;

  const static int cm_backend_method = 12;
  const static int cm_jit_data = 13;
  const static int cm_literals = 14;
  const static int cm_static_scope = 11;

  const static int msg_name = 0;
  const static int msg_module = 1;
  const static int msg_method = 2;
  const static int msg_method_missing = 3;

  const static int blockinv_flags = 0;
  const static int blockinv_self = 1;
  const static int blockinv_static_scope = 2;
  const static int blockinv_module = 3;

  const static int blockenv_scope = 1;
  const static int blockenv_top_scope = 2;
  const static int blockenv_local_count = 3;
  const static int blockenv_method = 4;
  const static int blockenv_module = 5;
  const static int blockenv_vmm = 6;
  const static int blockenv_execute = 7;

  const static int runtime_data_method = 0;
  const static int runtime_data_name = 1;
  const static int runtime_data_module = 2;
};

#endif
