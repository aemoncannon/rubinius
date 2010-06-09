require 'helpers'

def run()
  j = 0
  i = 0
  while i < $iterations
    i += 1
    j += Math.sqrt(4.0)
  end
end

time do
  run()
end



