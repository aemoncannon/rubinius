require 'helpers'

def run()
  i = 0
  while i < ITERATIONS
    i += 1
  end
  i
end

with_harness do
  run()
end


