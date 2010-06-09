require 'helpers'

ITERATIONS = 100000

def run()
  j = 0
  i = 0

  while i < ITERATIONS
    i += 1
    j += Math.sqrt(4.0)
  end

  puts "Result: #{j}"
end


trace_time = time_with_tracing do
  run()
end

non_trace_time = time_without_tracing do
  run()
end

puts "Tracing is #{non_trace_time / trace_time} times faster."


