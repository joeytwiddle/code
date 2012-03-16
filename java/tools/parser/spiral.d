
import Math.*

# A fudge that quickly gives us everything we need to start drawing pictures :)
import Canvas.QuickStart

## Implies:
# DCanvas canvas ...

## Could also imply:
# ctx = canvas.getContext("2d")

centreX = canvas.width / 2
centreY = canvas.height / 2




// OK other languages, screw you guys.
// I want properties to be magical getters sometimes!



# With the exception of the new type/class Spiral, everything here is basically
# part of the main thread.

# CONSIDER: Support for multiple constructors...?

class Spiral

  widthOfOneCycle = 40

  phase = 0

  # I would not be against make rotationsPerSecond private just for fun, but I
  # don't have a syntax for it!  Thought: we could put the keyword private
  # before it :)  That does not mean we can't have a var e.g. private = true.

  rotationsPerSecond = 1.0
  speed = 2*PI/rotationsPerSecond



  plot() =

    maxrad = sqrt( (canvas.width / 2)^2 + (canvas.height / 2)^2 )
    # ^ is an operator introduced by Math on the number types

    ctx.moveTo(centreX,centreY)

    for rad = 0 to maxrad step 1

      ang = phase + 2*PI * rad / widthOfOneCycle

      x = centreX + rad*sin(ang)
      y = centreY + rad*cos(ang)

      ctx.lineTo(x,y)



  move(deltaTime) =

    phase += speed * deltaTime
    # Wait a minute, that's a mutable!  What's the plan, Stan?
    # We will find a way to deal with it when we get there!
    # Perhaps the parser/interpreter will see this and mark phase as a mutable.


# Should we allow multiple functions of the same name with different signatures, like Java?

# It would be nice to allow the same named operators to work on different types.
# e.g. + for numbers and + for vectors and + for strings.
# aka operator overriding, which C++ also provides!




s = new Spiral()


doPlot() =
  s.plot()
  s.move(0.1)

startTimer(doPlot, 0.1)



