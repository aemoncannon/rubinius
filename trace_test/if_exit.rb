require 'helpers'

def foo()
  23
end

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    if(i < (ITERATIONS/2))
      a = 24
      b = foo()
      c = (b - a) * 1
      d = c / 1
      j += d
    else
      a = 24
      b = foo()
      c = (b - a) * 1
      d = c / 1
      j -= d
    end
  end
  puts "Result: #{j}"
end


with_harness do
  run()
end

