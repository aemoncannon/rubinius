require 'helpers'
require 'json'

@src = File.read("json2.txt")

def run
  JSON.parse(@src).to_json
end

expect(run()) do
  run()
end
