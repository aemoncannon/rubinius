require 'helpers'

ITERATIONS = 10000

def run()
  j = 0
  i = 0

  while i < ITERATIONS
    i += 1
    k = 0
    while k < 1000
      j += 1
      k += 1
    end

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


