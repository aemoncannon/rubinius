require 'helpers'

ITERATIONS = 100000

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    if(i < (ITERATIONS/2))
      j += 1
    else
      j -= 1
    end
  end
  puts "Result: #{j}"
end


expect(run()) do
  run()
end

