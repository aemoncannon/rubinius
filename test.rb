require 'helpers'

ITERATIONS = 5000


def foo()
  1
end


t = Time.now.to_f

enable_tracing

i = 0
while i < ITERATIONS
  i = i + foo()
end

disable_tracing

trace_time = Time.now.to_f - t
puts "Trace time: #{trace_time} seconds."



t = Time.now.to_f

i = 0
while i < ITERATIONS
  i = i + foo()
end

non_trace_time = Time.now.to_f - t
puts "Non-trace time: #{non_trace_time} seconds."

puts ""

puts "Tracing  #{non_trace_time / trace_time} times faster."


