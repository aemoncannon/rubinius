ITERATIONS = ARGV[0].to_i

def enable_tracing
  Ruby.primitive :vm_enable_tracing  
end

def disable_tracing
  Ruby.primitive :vm_disable_tracing
end

def expect(val)
  if ARGV[1];enable_tracing;end
  t = Time.now.to_f
  result = yield
  if ARGV[1];disable_tracing;end
  elapsed = Time.now.to_f - t
  puts "#{elapsed} seconds."

  if result == val
    puts "."
    exit(0)
  else
    $stderr << "FAILURE: Expected #{val}, found #{result}.\n"
    exit(1)
  end

end
