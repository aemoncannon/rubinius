require 'helpers'
require 'json'

@src = File.read("json2.txt")

def run
  result = JSON.parse(@src).to_json
  txt = result.inspect.to_s
  puts txt
end

with_harness do
  run()
end
