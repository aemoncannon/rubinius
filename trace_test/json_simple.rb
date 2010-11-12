require 'helpers'
require 'json'

@src = File.read("json2.txt")

def run
  result = JSON.parse(@src).to_json
end

with_harness do
  run()
end
