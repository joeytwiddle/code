-- Thief (Declarative Programming assignment 3)
-- Paul Clark (pc5065)
-- Type main <seed> to play

-- Jun/00 - made compatible with Hugs98

-- Beware contradictions: because the user is playing the game, I have called
-- him the goody (g), despite him being a thief!
-- Booglies (initially baddies) are bs.  Treasure (initally prize) is p.  Score is s.



-- Get required external functions
--import Random (random,mkStdGen)
import System.Random

--import AnsiTerminal (cls,at)
import System.Console.ANSI (clearScreen, setCursorPosition)

--import IOExtensions (getCh)
import System.IO (getChar)

import Data.Char (toUpper)

--clearScreen = putStr cls
--putStringAt pos str = putStr (at pos str)

putStringAt (x,y) str = do setCursorPosition y x
                           putStr str

getCh = getChar

-- Set up some data types
data Dir = L | R | U | D | N
type Pos = (Int,Int)
type World = (Pos,[Pos],Pos,[Pos],[Dir],Int)

-- World (game state) is: goodie's position, list of booglies' positions, position of treasure,
-- random position list, random direction list, score.



-- MAIN FUNCTIONS

-- Main initialising function
-- Had to rename it from main to go, because GHC expects main to take no arguments
go :: Integer -> IO()
go seed =
  instructions >>
  startscreen sw >>
  play sw
  where sw = startworld seed
        (g,bs,p,rd,rp,s) = sw

-- Main playing function.  World is already displayed, take next step in gameplay
play :: World -> IO()
play w = if (caught w) then
           end w
         else
         if (collect w) then
           newprize w
         else
         if (enough w) then
           end2 w
         else
           getCh >>= \key ->
           if key=='q' || key=='Q' then
             quit w
           else
             movements w key



-- CONSTANTS AND INTIAL GAME STATE

-- Width and height of playing grid
width :: Int
width = 20
height :: Int
height = 12

-- Initial state of game
startworld :: Integer -> World
startworld seed = ((1,1),[(div width 2,div height 2)],(width,height),randpos seed,randdir seed,0)

-- Random lists from which values are pulled during game
murandom (a,b) s = ( f : t )
  where
    f = fromIntegral (a + (mod (fst r) (b-a+1)))
    t = murandom (a,b) (snd r)
    r = random s
myrandom :: (Integer,Integer) -> Integer -> [Int]
myrandom (a,b) s = murandom (a,b) (mkStdGen (fromIntegral s))
randpos :: Integer -> [Pos]
randpos seed = zip (map fromIntegral (myrandom (toInteger(1),toInteger(width)) seed)) (map fromIntegral (myrandom (toInteger(1),toInteger(height)) seed))
randdir :: Integer -> [Dir]
randdir seed = map inttodir (myrandom (0,4) seed)

-- Maps random number 0-4 into a direction
inttodir :: Int -> Dir
inttodir i = [L,R,U,D,N]!!(i)
-- inttodir :: Integer -> Dir
-- inttodir i = [L,R,U,D,N]!!fromIntegral(i)



-- CHECKS ON GAME STATE

-- Has player been caught by a booglie?
caught :: World -> Bool
caught (g,bs,p,rp,rd,s) = isin g bs

-- Has player landed on prize (treasure)?
collect :: World -> Bool
collect (g,bs,p,rp,rd,s) = g==p

-- Has player collected enough treasure to save grandma?
enough :: World -> Bool
enough (g,bs,p,rp,rd,s) = s>=20



-- CHANGES IN GAME STATE

-- Next step in game when prize has been collected
newprize :: World -> IO()
newprize (g,bs,p,rp,rd,s) =
  if (newp==g) then
    newprize (g,bs,p,tail rp,rd,s)
  else
    plot newp "**" >>
    showsc news >>
    play neww
    where newb = head bs
          newp = head rp
          newrp = tail rp
          news = s+1
          neww = (g,bs++[newb],newp,newrp,rd,news)

-- Next step in game when player moves
movements :: World -> Char -> IO()
movements (g,bs,p,rp,rd,s) key =
  clear g >>
  clearbs bs >>
  restp p bs >>
  plot newg "=)" >>
  showbs newbs (drop (length bs) rd) >>
  putStringAt (6,1) [] >>
  play (newg,newbs,p,rp,newrd,s)
  where newg = moveg g key
        (newbs,newrd) = movebs bs rd



-- MOVEMENT OF CHARACTERS

-- Moves player according to keypress
moveg :: Pos -> Char -> Pos
moveg g key = addifcan g (dirtopos (keytodir key))

-- Moves the booglies
movebs :: [Pos] -> [Dir] -> ([Pos],[Dir])
movebs [] rd = ([],rd)
movebs (b:bs) rd = ([addifcan b (dirtopos (head rd))]++newbs,newrd)
                   where (newbs,newrd) = movebs bs (tail rd)

-- Changes a position by a vector only if it will not
-- move the position outside the boundary
addifcan :: Pos -> Pos -> Pos
addifcan p d = if (outofbounds newpos) then
                 p
               else
                 newpos
               where newpos = addpos p d

-- Checks if a position is out of the boundary
outofbounds :: Pos -> Bool
outofbounds (x,y) = x<1 || x>width || y<1 || y>height

-- Adds two position vectors
addpos :: Pos -> Pos -> Pos
addpos (a,b) (c,d) = (a+c,b+d)

-- Converts keys into directions
keytodir :: Char -> Dir
keytodir 'j' = L
keytodir 'l' = R
keytodir 'i' = U
keytodir 'k' = D
keytodir 'J' = L
keytodir 'L' = R
keytodir 'I' = U
keytodir 'K' = D
keytodir a = N

-- Converts directions into position vectors
dirtopos :: Dir -> Pos
dirtopos L = (-1,0)
dirtopos R = (1,0)
dirtopos U = (0,-1)
dirtopos D = (0,1)
dirtopos N = (0,0)



-- DISPLAY

-- Set up screen ready for a game
startscreen :: World -> IO()
startscreen (g,bs,p,rp,rd,s) =
  clearScreen >>
  putStringAt (1,1) "THIEF" >>
  plot (0,0) horbar >>
  plot (0,height+1) horbar >>
  forall [1..height] verbar (>>) (return()) >>
  plot g "=)" >>
  plot p "**" >>
  showbs bs rd >>
  showsc s
  where horbar = map returnx [1..(width+2)*2]
        returnx i = 'X'
        verbar y = plot (0,y) "XX" >> plot (width+1,y) "XX"

-- Restore prize if it has been hidden by a booglie
restp :: Pos -> [Pos] -> IO()
restp p bs = if (isin p bs) then
               plot p "**"
             else
               return ()

-- Shows booglies
showbs :: [Pos] -> [Dir] -> IO()
showbs [] _ = (return())
showbs (b:bs) (r:rd) = (showb b) >> (showbs bs rd)
            where showb b = plot b (boogliefor r)

-- showbs bs (r:rd) = forall bs showb (>>) (return())
--             where showb b = plot b (boogliefor r)
            
boogliefor N = "><"
boogliefor U = "|8"
boogliefor D = "8|"
boogliefor L = ">8"
boogliefor R = "8<"

-- Clears booglies positions
clearbs :: [Pos] -> IO()
clearbs bs = forall bs clear (>>) (return())

-- Plots a string at playing area position
plot :: Pos -> String -> IO()
plot pos str = putStringAt (scrpos pos) str

-- Clears a playing area position
clear :: Pos -> IO()
clear pos = putStringAt (scrpos pos) "  "

-- Displays the score
showsc :: Int -> IO()
showsc s =
  putStringAt (1,3) ("Score : "++show s) >>
  putStringAt (6,1) []



-- BEGINNING AND END IO

-- Show instructions if desired
instructions :: IO()
instructions =
  clearScreen >>
  putStringAt (1,1) "THIEF\n\n" >>
  putStr "Would you like instructions (Y/N)?\n\n" >>
  getkeyyn >>= \key ->
  if key=='Y' then
    putStr "In thief you play a poverty-stricken criminal who desperately needs\n" >>
    putStr "some money to pay for a life-saving operation for his grandmother.\n" >>
    putStr "Having searched the land high and low for valuable treasure, you\n" >>
    putStr "have come across a cave full of blind booglies.  Inside the cave\n" >>
    putStr "are lots of exciting rewards with which to pay for your grandmother's\n" >>
    putStr "op.  You must collect the treasure whilst avoiding being eaten by\n" >>
    putStr "the ravenous booglies.  But beware!  Every time you pick up a piece\n" >>
    putStr "of treasure, the booglies sense this, and in their rage they multiply\n" >>
    putStr "in the hope of capturing you.  You will need 20 pieces of treasure in\n" >>
    putStr "order to have enough to save your grannie.\n\n" >>
    putStr "You: =)   Treasure: **   Evil scary ravenous booglies: ><\n\n" >>
    putStr "Use keys IKJL for up, down, left, right, and Q to quit.\n\n" >>
    putStr "Press a key to start playing.  Good luck!\n" >>
    getChar >>= \waste ->
    return()
  else
    return()

-- Display for end by being caught by a booglie
end :: World -> IO()
end (g,bs,p,rp,rd,s) =
  putStringAt (1,height+8) "You got caught!\n" >>
  putStr ((pre s)++" scored "++(show s)++"!")

-- Comment preceding final score
pre :: Int -> String
pre s = if (s>=10) then
           "But never mind, because you"
         else
           "I'm afraid you only"

-- Display for end by succeeding
end2 :: World -> IO()
end2 w =
  putStringAt (1,height+8) "Congratulations!  You have collected enough treasure to save your grandmother!\nThe End."

-- Check user really wants to quit
quit :: World -> IO()
quit w =
  putStringAt (7,1) question >>
  getkeyyn >>= \key ->
  if key=='N' then
    putStringAt (7,1) blank >>
    play w
  else
    putStringAt (1,height+8) "Bye.  Please play again soon!"
  where question = "Do you really want to quit Thief (Y/N)?"
        blank = "                                       "



-- MISCELLANEOUS

-- The position on the screen of a playing area position
scrpos :: Pos -> Pos
scrpos (x,y) = (5+2*x,5+y)

-- Checks to see if an item is in a list
isin :: Eq a => a -> [a] -> Bool
isin x [] = False
isin x (y:ys) = x==y || isin x ys

-- A kind of for next loop
-- list is list of things to loop through
-- f is function to perform on each thing
-- c is the function which combines the results of f
-- e is the thing to combine to when the end of the list is reached
forall :: [a] -> (a->b) -> (b->b->b) -> b -> b
forall [] f c e = e
forall (x:xs) f c e = c (f x) (forall xs f c e)

-- I used this loop to do some of the other functions more easily.
-- For example, showbs and clearbs used to be:
-- showbs [] = return()
-- showbs (b:bs) = plot b "><" >> showbs bs
-- clearbs [] = return()
-- clearbs (b:bs) = clear b >> clearbs bs

-- Gets y/n input from user
getkeyyn :: IO Char
getkeyyn =
  getCh >>= \key ->
  if key=='y' || key=='n' || key=='Y' || key=='N' then
    return (toUpper key)
  else
    getkeyyn
