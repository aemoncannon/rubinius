require 'helpers'


def foo(m = 1)
  k = 0
  while k < 1000
    k += m
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


with_harness do
  run()
end


