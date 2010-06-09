require 'helpers'

ITERATIONS = 100000

def foo(i)
  k = 0
  if i < ITERATIONS/2
    k = 2
  else
    k = -3
  end
  k
end

def calc(i)
  k = 0
  1.times do
    k += foo(i)
  end
  k
end

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    j += calc(i)
    i += 1
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


