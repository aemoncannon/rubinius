require 'helpers'

ITERATIONS = ARGV[0].to_i

def run()
  j = 0
  i = 0

  while i < ITERATIONS
    i += 1
    k = 0
    while k < 1000
      j += 1
      k += 1
    end
  end

end

time do
  run()
end


