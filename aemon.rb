
cl = Rubinius::CodeLoader.new("hell0")
cm = cl.compile_file("test.rb", "test.rbc")
puts cm.decode

