class Bar
  def bar
    true
  end
end

def foo(obj)
  1000000.times do
    obj.bar
  end
end

obj = Bar.new

foo(obj)
foo(obj)
