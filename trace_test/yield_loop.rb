require 'helpers'

def run()
  j = 0
  ITERATIONS.times do |i|
    j += i
  end
end

with_harness do
  run()
end


