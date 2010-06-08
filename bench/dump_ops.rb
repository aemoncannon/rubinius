cl = Rubinius::CodeLoader.new("hell0")
cm = cl.compile_file(ARGV[0], ARGV[0] + "c")
puts cm.decode

