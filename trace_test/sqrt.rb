require 'helpers'

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    j += Math.sqrt(4.0)
  end
  j
end

expect(run()) do
  run()
end



