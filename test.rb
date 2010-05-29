require 'helpers'

ITERATIONS = 1000000

class Rabbit

  def bar(a,b)
    a + b
  end

  def foo(a,b)
    bar(a,b)
  end

end

def run()
  i = 0
  while i < ITERATIONS
    rabbit = Rabbit.new
    i = i + rabbit.foo(1,0)
  end
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


