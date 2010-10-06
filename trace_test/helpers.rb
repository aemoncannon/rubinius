ITERATIONS = ARGV[0].to_i

def enable_tracing
  Ruby.primitive :vm_enable_tracing  
end

def disable_tracing
  Ruby.primitive :vm_disable_tracing
end

def expect(val)
  if ARGV[1];enable_tracing;end
  result = yield
  if ARGV[1];disable_tracing;end

  if result == val
    puts "SUCCESS"
    exit(0)
  else
    $stderr << "FAILURE: Expected #{val}, found #{result}.\n"
    exit(1)
  end

end
