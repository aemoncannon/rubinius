require 'helpers'


def foo()
  k = 0
  while k < 1000
    k += 1
    if k > 500
      k += k / 0
    end
  end
  k
end


def run()
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    begin
      j += foo()
    rescue Exception => e
      j += 1
    end
  end
  j
end


with_harness do
  run()
end
