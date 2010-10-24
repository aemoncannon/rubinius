require 'helpers'


def foo(i)
  if i < ITERATIONS/2
    true
  else
    false
  end
end

def times(i)
  q = 0
  while q < 1
    q += 1
    foo(i)
  end
  2
end

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    j += times(i)
    i += 1
  end
  j
end

with_harness do
  run()
end


