require 'helpers'


def run()
  str = ""
  (0..ITERATIONS).each_with_index do |ea, i|
    str = "asjfdkj" + "lksdjf"
  end
  j = 0
  (0..ITERATIONS).each_with_index do |ea, i|
    j += i
  end
  j
end


with_harness do
  run()
end
