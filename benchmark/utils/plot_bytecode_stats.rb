BYTECODE_COUNTS = [9135892, 4576900, 1503768, 508864, 6872, 7119094, 16, 44607, 12575055, 17684078, 4529127, 0, 4519384, 4561223, 0, 36840781, 0, 55, 8723, 27156421, 57972749, 8028266, 2231, 9908, 0, 0, 1077, 42, 0, 1074, 1070, 0, 1, 0, 0, 2002888, 94, 1401, 8119, 24549, 0, 0, 0, 10135, 211, 10557109, 0, 42820, 5813, 22257101, 13381, 754, 370, 0, 1329, 0, 13266, 6032193, 283, 0, 6034374, 0, 0, 1872, 2493, 4692, 620, 2799, 9036665, 0, 329, 1968, 896, 0, 5722, 3461, 0, 151, 22858, 9044335, 215, 11047271, 7779, 4519708, 11054831, 2931, 3666, 0, 6050767, 58, 0, 2373, 39, 1074, 11258, 415, 12130, 335, 1425, 4716]

BYTECODE_TIMES = [3.68, 2.02, 0.86, 0.29, 0, 3.36, 0, 0.02, 5.71, 7.77, 2.22, 0, 2.05, 2.09, 0, 16.41, 0, 0, 0, 12.53, 26.94, 3.5, 0.01, 0.01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.22, 0, 0, 0, 0.01, 0, 0, 0, 0.01, 0, 4.8, 0, 0.02, 273.04, 938.22, 1356.69, 0.04, 0, 0, 0, 0, 0.01, 2.72, 0, 0, 1012.11, 0, 0, 0, 0, 0.01, 0, 0, 4.32, 0, 0, 0, 0, 0, 0.01, 0, 0, 0, 0, 4.21, 0, 4.93, 0.01, 2.18, 5.27, 0, 0.01, 0, 2.96, 0, 0, 0, 0, 0, 0.01, 0, 0.01, 0, 0, 0]

BYTECODE_NAMES = ["op_noop", "op_push_nil", "op_push_true", "op_push_false", "op_push_int", "op_push_self", "op_set_literal", "op_push_literal", "op_goto", "op_goto_if_false", "op_goto_if_true", "op_ret", "op_swap_stack", "op_dup_top", "op_dup_many", "op_pop", "op_pop_many", "op_rotate", "op_move_down", "op_set_local", "op_push_local", "op_push_local_depth", "op_set_local_depth", "op_passed_arg", "op_push_current_exception", "op_clear_exception", "op_push_exception_state", "op_restore_exception_state", "op_raise_exc", "op_setup_unwind", "op_pop_unwind", "op_raise_return", "op_ensure_return", "op_raise_break", "op_reraise", "op_make_array", "op_cast_array", "op_shift_array", "op_set_ivar", "op_push_ivar", "op_push_const", "op_set_const", "op_set_const_at", "op_find_const", "op_push_cpath_top", "op_push_const_fast", "op_set_call_flags", "op_allow_private", "op_send_method", "op_send_stack", "op_send_stack_with_block", "op_send_stack_with_splat", "op_send_super_stack_with_block", "op_send_super_stack_with_splat", "op_push_block", "op_passed_blockarg", "op_create_block", "op_cast_for_single_block_arg", "op_cast_for_multi_block_arg", "op_cast_for_splat_block_arg", "op_yield_stack", "op_yield_splat", "op_string_append", "op_string_build", "op_string_dup", "op_push_scope", "op_add_scope", "op_push_variables", "op_check_interrupts", "op_yield_debugger", "op_is_nil", "op_check_serial", "op_check_serial_private", "op_push_my_field", "op_store_my_field", "op_kind_of", "op_instance_of", "op_meta_push_neg_1", "op_meta_push_0", "op_meta_push_1", "op_meta_push_2", "op_meta_send_op_plus", "op_meta_send_op_minus", "op_meta_send_op_equal", "op_meta_send_op_lt", "op_meta_send_op_gt", "op_meta_send_op_tequal", "op_meta_send_call", "op_push_my_offset", "op_zsuper", "op_push_block_arg", "op_push_undef", "op_push_stack_local", "op_set_stack_local", "op_push_has_block", "op_push_proc", "op_check_frozen", "op_cast_multi_value", "op_invoke_primitive", "op_push_rubinius"]


count_points = []
BYTECODE_COUNTS.each_with_index{|c,i|
  count_points << {:op => i, :op_name => BYTECODE_NAMES[i], :val => c }
}


time_points = []
BYTECODE_TIMES.each_with_index{|t,i|
  op_name = BYTECODE_NAMES[i]

  # Omit these for timing - since they launch new interpreters in rubinius.
  unless ["op_send_stack", 
          "op_send_stack_with_block", 
          "op_send_method", 
          "op_yield_stack"].include?(op_name)

    time_points << {:op => i, :op_name => BYTECODE_NAMES[i], :val => t }

  end
}


def graph_points(points)
  norm = points.max{|a,b| a[:val] <=> b[:val] }
  sorted_points = points.sort_by{|p| p[:val] }
  sorted_points.each{|p|
    width = ((p[:val].to_f / norm[:val].to_f) * 50.0).to_i 
    bar = "".ljust(width, "#")
    value = p[:val].to_s.ljust(15, " ")
    puts p[:op_name].rjust(30, " ") + " " + value + " " + bar
  }
end


graph_points(count_points)
graph_points(time_points)
