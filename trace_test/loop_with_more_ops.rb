require 'helpers'

def foo()
  1
end

def run()
  i = 0
  while i < ITERATIONS
    foo()
    foo()
    foo()
    i += foo()
  end
  i
end

with_harness do
  run()
end


