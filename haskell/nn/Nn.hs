module Nn where

input = "  XX  "
     ++ "XX   X"
     ++ "X    X"
     ++ "X   X "
     ++ "X XX  "
     ++ "X   X "
     ++ "X    X"
     ++ " X   X"
     ++ "X X X "

images :: [[Char]]
images = [ "  XX  "
        ++ " X  X "
        ++ "X    X"
        ++ "X    X"
        ++ "XXXXXX"
        ++ "X    X"
        ++ "X    X"
        ++ "X    X"
        ++ "X    X",
           "XXXXX "
        ++ "X    X"
        ++ "X    X"
        ++ "X    X"
        ++ "XXXXX "
        ++ "X    X"
        ++ "X    X"
        ++ "X    X"
        ++ "XXXXX " ]

nonodes = length (head images)

weight :: Int -> Int -> Int
weight i j = w2 images i j

getweights :: [[Int]]
getweights = forall [0..nonodes-1] (v) (++) []
             where v i = [forall [0..nonodes-1] (w i) (++) []]
                   w i j = [weight i j]

w2 [] i j = 0
w2 (im:rest) i j = if (i==j) then
                     0
                   else
                     (psi im i)*(psi im j) + w2 rest i j

psi im i = if (im!!i=='X') then
             1
           else
             -1

go :: IO()
go = display input >> it getweights input

it ws im = display next >>
           it ws next
           where next = gimmenode ws im 0

display [] = putChar '\n'
display list = display2 (take 6 list) >> display (drop 6 list)

display2 [] = putChar '\n'
display2 (x:xs) = putChar x >> display2 xs

gimmenode ws im i = if (i==nonodes) then
                   []
                 else
                   (sign (phi ws im i):gimmenode ws im (i+1))

sign n = if (n>0) then
           'X'
         else
           ' '

phi ws im i = forall (remove [0..nonodes-1] i) v (+) 0
              where v j = ((ws!!i)!!j)*(psi im j)

remove [] x = []
remove (x:xs) y = if (x==y) then
                    xs
                  else
                    (x:remove xs y)

-- A kind of for next loop
-- list is list of things to loop through
-- f is function to perform on each thing
-- c is the function which combines the results of f
-- e is the thing to combine to when the end of the list is reached
forall :: [a] -> (a->b) -> (b->b->b) -> b -> b
forall [] f c e = e
forall (x:xs) f c e = c (f x) (forall xs f c e)
