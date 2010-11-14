require 'helpers'

def run()
  j = ""
  i = 0
  while i < ITERATIONS
    j += i.to_s
  end
  j
end

with_harness do
  run()
end
