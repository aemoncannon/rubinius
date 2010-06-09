require 'helpers'

def foo(i)
  if i < $iterations * 0.5
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
  while i < $iterations
    i += 1
    j += calc(i)
  end
end


time do
  run()
end



