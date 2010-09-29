require 'helpers'


def foo()
  k = 0
  k.times do
    k += 1
  end
  k
end

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    j += foo()
  end
  j
end


expect(run()) do
  run()
end


