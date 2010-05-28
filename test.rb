require 'helpers'

ITERATIONS = 20000

def bar()
  1
end

def foo()
  bar()
end

t = Time.now.to_f

enable_tracing

i = 0
while i < ITERATIONS
  i = i + foo()
end

puts "Finished! #{i}"

disable_tracing

trace_time = Time.now.to_f - t
puts "Trace time: #{trace_time} seconds."



t = Time.now.to_f

i = 0
while i < ITERATIONS
  i = i + foo()
end

puts "Finished! #{i}"

non_trace_time = Time.now.to_f - t
puts "Non-trace time: #{non_trace_time} seconds."

puts ""

puts "Tracing  #{non_trace_time / trace_time} times faster."


