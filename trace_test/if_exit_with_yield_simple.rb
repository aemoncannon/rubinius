require 'helpers'

def calc(i)
  1.times do
    3
  end
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



with_harness do
  run()
end



