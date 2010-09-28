require 'helpers'

def foo(i)
  if i < ITERATIONS * 0.5
    2
  else
    1
  end
end

def calc(i)
  puts "calc(#{i})"
  k = 0
  while k < 100
    k += foo(i)
  end
  k
end

def run()
  j = 0
  i = 0
  while i < ITERATIONS
    puts "j: #{j}"
    i += 1
    j += calc(i)
  end
  puts "finising."
  j
end

expect(run()) do
  run()
end



