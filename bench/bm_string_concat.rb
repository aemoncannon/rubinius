require 'helpers'

string = ""
book = "So Long, and Thanks for All the Fish"

time do
  $iterations.times {
    string << book 
  }
end

