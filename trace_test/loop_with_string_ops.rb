require 'helpers'

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    a = "apples and oranges".gsub("e", "q")
    b = a.include?("es")
    i += 1
    if b
      j += 1
    end
  end
  j
end

with_harness do
  run()
end
