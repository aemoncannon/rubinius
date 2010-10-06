require 'helpers'


def foo(i)
  k = 0
  if i < ITERATIONS/2
    k = 2
  else
    k = -2
  end
  k
end

def times(i)
  k = 0
  q = 0
  while q < 1
    k += foo(i)
    q += 1
  end
  k
end

def calc(i)
  k = 666
  k += times(i)
  k
end

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    j += calc(i)
    i += 1
  end
  j
end

expect(run()) do
  run()
end


