require 'helpers'

BAILOUT = 16
MAX_ITERATIONS = 1000

class Mandelbrot

  def initialize
    puts "Rendering"
    y = -39
    while(y <= 39)
      puts
      x = -39
      while(x <= 39)
        i = iterate(x/40.0,y/40.0)
#        if (i == 0)
#          print "*"
#        else
#          print " "
#        end
        x += 1
      end
      y += 1
    end
  end

  def iterate(x,y)
    cr = y - 0.5
    ci = x
    zi = 0.0
    zr = 0.0
    i = 0
    
    ret = 0
    cont = true
    while(cont)
      i += 1
      temp = zr * zi
      zr2 = zr * zr
      zi2 = zi * zi
      zr = zr2 - zi2 + cr
      zi = temp + temp + ci

      if (zi2 + zr2 > BAILOUT)
        ret = i
        cont = false
      end

      if (i > MAX_ITERATIONS)
        ret = 0
        cont = false
      end

    end
    ret
  end

end

time do
  Mandelbrot.new
end
