require 'helpers'

ITERATIONS = 100000

class Rabbit

  def bar(a,b)
    a + b
  end

  def value(a,b)
    k = 0
    100.times do
      k += 1
    end
    k
  end

end

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    rabbit = Rabbit.new
    j += rabbit.value(0,1)
    i += 1
  end
  puts "Result: #{j}"
end


########## Run traced #############

t = Time.now.to_f

enable_tracing
run()
disable_tracing

trace_time = Time.now.to_f - t
puts "Trace time: #{trace_time} seconds."



########## Run non-traced #############

t = Time.now.to_f

run()

non_trace_time = Time.now.to_f - t

puts "Non-trace time: #{non_trace_time} seconds."

puts ""

puts "Tracing  #{non_trace_time / trace_time} times faster."


