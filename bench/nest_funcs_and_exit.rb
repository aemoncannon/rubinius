require 'helpers'

ITERATIONS = 10000

def foo(i)
  if i < ITERATIONS/2
    2
  else
    1
  end
end

def calc(i)
  k = 0
  while k < 100
    k += foo(i)
  end
  k
end


def run()
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    j += calc(i)
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


