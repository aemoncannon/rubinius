require 'helpers'
require 'json'

@src = File.read("json1.txt")

def run
  ITERATIONS.times do
    result = JSON.parse(@src).to_json
    #  txt = result.inspect.to_s
    #  puts txt
  end
end

with_harness do
  run()
end
