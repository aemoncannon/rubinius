require 'helpers'

def run()
  hash = {}
  j = 0
  i = 0
  while i < ITERATIONS
    hash[i] = i
    i += 1
    j += hash[i - 1]
  end
  j
end

expect(run()) do
  run()
end
