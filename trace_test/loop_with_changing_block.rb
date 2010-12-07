require 'helpers'

def foo
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    j += yield i
  end
  j
end


def run()
  j = 0
  j += foo do |i|
    i + 1
  end
  j += foo do |i|
    i + 2
  end
  j += foo do |i|
    i + 3
  end
  j
end


with_harness do
  run()
end
