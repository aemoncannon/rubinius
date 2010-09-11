require 'helpers'

def run()
  i = 0
  while i < ITERATIONS
    i += 1
  end
  i
end

expect(ITERATIONS) do
  run()
end


