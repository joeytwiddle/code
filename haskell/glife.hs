import Random (random,mkStdGen)

go :: IO()
go = animate startgrid

animate g = displaygrid g >> putStr "Next generation:\n" >> animate (nextgen g)

point (x,y) g = booltoint (occupied (x,y) g)

occupied :: (Int,Int) -> [[Bool]] -> Bool
occupied (x,y) g = if (x<1) then
                  occupied (x+gw,y) g
		else
		  if (y<1) then
		    occupied (x,y+gh) g
		  else
		    if (x>gw) then
		      occupied (x-gw,y) g
		    else
		      if (y>gh) then
		        occupied (x,y-gh) g
		      else
		        element x (element y g)
		where gh = height g
		      gw = width g

booltoint :: Bool -> Int
booltoint False = 0
booltoint True = 1

inttobool :: Int -> Bool
inttobool 0 = False
inttobool 1 = True

width :: [[Bool]] -> Int
width g = length (head g)

height :: [[Bool]] -> Int
height g = length g

element :: Int -> [a] -> a
element 1 (x:xs) = x
element n (x:xs) = element (n-1) xs

startgrid = take 16 (startgridrows listofbools)

startgridrows l = (a : startgridrows b)
		where (a,b)=splitAt 16 l

murandom (a,b) s = ( f : t )
  where
    f = toInt (a + (mod (fst r) (b-a+1)))
    t = murandom (a,b) (snd r)
    r = random s
myrandom :: (Integer,Integer) -> Integer -> [Int]
myrandom (a,b) s = murandom (a,b) (mkStdGen (toInt s))

listofbools = map inttobool (myrandom (0,1) 9247)

neighbourcount :: (Int,Int) -> [[Bool]] -> Int
neighbourcount (x,y) g = point (x-1,y) g
                       + point (x+1,y) g
                       + point (x,y-1) g
                       + point (x,y+1) g
                       + point (x+1,y+1) g
                       + point (x-1,y+1) g
                       + point (x-1,y-1) g
                       + point (x+1,y-1) g

nextgen :: [[Bool]] -> [[Bool]]
nextgen g = nextgen2 1 g

nextgen2 :: Int -> [[Bool]] -> [[Bool]]
nextgen2 y g = if (y>height g) then
                 []
               else
                 [nextgen3 (1,y) g] ++ (nextgen2 (y+1) g)

nextgen3 :: (Int,Int) -> [[Bool]] -> [Bool]
nextgen3 (x,y) g = if (x>width g) then
                     []
                   else
                     [newpoint (x,y) g] ++ (nextgen3 (x+1,y) g)

newpoint :: (Int,Int) -> [[Bool]] -> Bool
newpoint (x,y) g = if occupied (x,y) g then
                     if (nc<=1)||(nc>=4) then
                       False
                     else
                       True
                   else
                     if (nc>=3) then
                       True
                     else
                       False
                   where nc = neighbourcount (x,y) g

displaygrid :: [[Bool]] -> IO()
displaygrid [] = return()
displaygrid (r:rs) = displayrow r >> putStr "\n" >> displaygrid rs

displayrow :: [Bool] -> IO()
displayrow [] = return()
displayrow (x:xs) = displaypoint x >> displayrow xs

displaypoint :: Bool -> IO()
displaypoint False = putStr " ."
displaypoint True = putStr " O"
