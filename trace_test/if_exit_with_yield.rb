require 'helpers'


def foo(i)
  k = 0
  if i < ITERATIONS/2
    k = 2
  else
    k = -3
  end
  k
end

def calc(i)
  k = 666
  1.times do
    val = foo(i)
    k += foo(i)
  end
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


