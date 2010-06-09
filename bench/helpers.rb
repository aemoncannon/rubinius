#def enable_tracing
#  Ruby.primitive :vm_enable_tracing  
#end
#
#
#def disable_tracing
#  Ruby.primitive :vm_disable_tracing
#end
#
#
#def set_no_trace(obj)
#  Ruby.primitive :vm_set_no_trace, obj
#end


#def time_with_tracing
#  t = Time.now.to_f
#  enable_tracing
#  yield
#  disable_tracing
#  trace_time = Time.now.to_f - t
#  puts "Trace time: #{trace_time} seconds."
#  trace_time
#end

def time
  t = Time.now.to_f
  yield
  elapsed = Time.now.to_f - t
  puts "Time: #{trace_time} seconds."
  elapsed
end
