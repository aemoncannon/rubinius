
ITERATIONS = ARGV[0].to_i


def enable_tracing
  Ruby.primitive :vm_enable_tracing  
end


def disable_tracing
  Ruby.primitive :vm_disable_tracing
end


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
  if ARGV[1];enable_tracing;end
  t = Time.now.to_f
  yield
  elapsed = Time.now.to_f - t
  puts "#{elapsed} seconds."
  if ARGV[1];disable_tracing;end
  elapsed
end
