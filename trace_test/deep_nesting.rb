require 'helpers'


def bar()
end

def foo(i)
  bar()
end

def one
  1
end

def times(i)
  q = 0
  while q < one
    q += one
    foo(i)
  end
  2
end

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    j += times(i)
    i += one
  end
  j
end

with_harness do
  run()
end
