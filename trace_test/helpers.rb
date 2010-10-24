ITERATIONS = ARGV[0].to_i
MODE = ARGV[1]

def enable_tracing
  Ruby.primitive :vm_enable_tracing  
end

def disable_tracing
  Ruby.primitive :vm_disable_tracing
end

def with_harness()

  result = nil
  if MODE == "trace"
    enable_tracing
    result = yield
    disable_tracing
  elsif MODE == "interp"
    result = yield
  end

  File.open(".run_result", "w"){ |f|
    f << result
  }

end
