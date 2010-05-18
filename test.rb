require 'helpers'

ITERATIONS = 50000





t = Time.now.to_f

enable_tracing

i = 0
while i < ITERATIONS
  i += 1
end

disable_tracing

trace_time = Time.now.to_f - t
puts "Trace time: #{trace_time} seconds."




t = Time.now.to_f

i = 0
while i < ITERATIONS
  i += 1
end

non_trace_time = Time.now.to_f - t
puts "Non-trace time: #{non_trace_time} seconds."

puts ""

puts "Tracing  #{non_trace_time / trace_time} times faster."



