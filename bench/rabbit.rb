require 'helpers'

ITERATIONS = 100000

class Rabbit

  def bar(a,b)
    a + b
  end

  def value(a,b)
    k = 0
    1000.times do
      k += (a + b)
    end
    k
  end

end

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    rabbit = Rabbit.new
    j += rabbit.value(4,12)
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


