require 'helpers'

ITERATIONS = 1000000


def calc(i)
  k = 0
  if i < ITERATIONS/2
    k = 2
  else
    k = -3
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


expect(run()) do
  run()
end

