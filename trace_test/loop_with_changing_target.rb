require 'helpers'

class Cat
  def legs
    4
  end
end

class Chimp
  def legs
    2
  end
end


def foo(target)
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    j += target.legs
  end
  j
end



def run()
  j = 0
  j += foo(Cat.new)
  j += foo(Chimp.new)
  puts j
  j
end



with_harness do
  run()
end
