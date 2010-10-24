require 'helpers'

def foo(i)
  if i < ITERATIONS * 0.5
    2
  else
    1
  end
end

def calc(i)
  k = 0
  while k < 100
    k += foo(i)
  end
  k
end

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    j += calc(i)
  end
  j
end

with_harness do
  run()
end



