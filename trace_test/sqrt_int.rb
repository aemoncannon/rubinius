require 'helpers'

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    j += Math.sqrt(4)
  end
  j
end

with_harness do
  run()
end



