require 'helpers'


def foo()
  k = 0
  while k < 1000
    k += 1
  end
  k
end

def run()
  j = 0
  i = 0
  while i < $iterations
    i += 1
    j += foo()
  end
end


time do
  run()
end


