smalls :: [a] -> [Int] -> Int -> [a]
smalls [] [] l = []
smalls (o:os) (i:is) l = if (i<=l) then
		(o:rest)
	else
		rest
	where rest = smalls os is l

tests = smalls ['a','b','c','d','e','f'] [1,2,3,4,5,6] 3

limit :: Num n => [n] -> (n->n->Bool) -> n
limit (f:(s:r)) fn = if (fn f s) then s else limit (s:r) fn

testl = limit [1,9,2,8,3,7,4,6,5,6,5,5,5] f where f a b = abs (b-a) < 4

--abs x = if x<0 then -x else x

data Tree a = Node a [Tree a]

--size (Node x []) = 1
--size (Node x (t:ts)) = (size t) + (size (Node x ts))
size (Node x ts) = 1 + sum (map size ts)

contents :: Tree a -> [a]
contents (Node x ts) = x:concat (map contents ts)

atree = (Node 1 [(Node 2 []),(Node 3 [(Node 4 [])])])

obey :: (String -> String) -> IO ()
obey cp = getLine >>= \l ->
	  let reply = cp l in
	    if reply=="" || l=="quit" then
	      return ()
	    else
	      putStr reply >> putStr "\n" >> obey cp

-- either of below works
data Position = Pos Int Int deriving Eq

type Person = (String,Int)

data Direction = U | D | L | R

--data Position = Pos Int Int
--instance Eq Position where
  --p==q = ((xcoord p) == (xcoord q)) && ((ycoord p) == (ycoord q))
         
xcoord (Pos x y) = x
ycoord (Pos x y) = y
