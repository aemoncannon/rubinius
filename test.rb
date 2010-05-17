require 'helpers'

enable_tracing

i = 0
while i < 100000000
  i += 1
end

disable_tracing


