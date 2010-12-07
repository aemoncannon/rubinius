require 'helpers'

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    a = 5 * 20
    b = a / 100
    i += b
    k = 0
    while k < 1000
      j += 1
      k += 1
    end
  end
  j
end

with_harness do
  run()
end


