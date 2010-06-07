require 'helpers'

string = ""
book = "So Long, and Thanks for All the Fish"

ITERATIONS = 100000

time_with_tracing do
  ITERATIONS.times {
    string << book 
  }
end

time_without_tracing do
  ITERATIONS.times {
    string << book 
  }
end
