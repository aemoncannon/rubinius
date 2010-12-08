require 'helpers'

def run()
  hash = {}
  j = 0
  i = 0
  while i < ITERATIONS
    i += 1
    begin
      j += 1 / 0
    rescue
    ensure
      j += 2
    end
  end
  j
end

with_harness do
  run()
end
