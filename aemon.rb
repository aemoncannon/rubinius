
cl = Rubinius::CodeLoader.new("hell0")
cm = cl.compile_file("test1.rb", "test1.rbc")
puts cm.decode

