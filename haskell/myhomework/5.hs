-- Assignment 1
-- Paul Clark (pc5065)



-- Question 1

sentence :: [[Char]] -> [Char]
sentence a = [toUpper (head s)] ++ tail s ++ "."
	     where
	       s = tail (foldl f "" a)
	       f a b = a++" "++b

sentence_test :: Bool
sentence_test = sentence ["the","cat","sat","on","the","mat"] == "The cat sat on the mat."



-- Question 2

find :: Eq a => a -> [a] -> [Int]
find x xs = find2 x xs 0

find2 :: Eq a => a -> [a] -> Int -> [Int]
find2 x xs n = if (null xs) then
                 []
               else
                 if head xs == x then
                   (n:rest)
                 else
                   rest
               where rest = find2 x (tail xs) (n+1)

find_test :: Bool
find_test = find "q" ["a","b","q","t","q"] == [2,4]



-- Question 3

-- (f.g)(x) = f(g(x))
-- (compose3 f g h)(x) = f(g(h(x)))

compose3 :: (c->d) -> (b->c) -> (a->b) -> (a->d)
compose3 f g h = f.g.h

compose3_test :: Bool
compose3_test = compose3 f g h 7 == 26
                where
                  f x = x/2
                  g x = x+3
                  h x = x^2



-- Question 4

search :: [Char] -> [Char] -> [Int]
search xs ys = search2 xs ys 0

search2 :: [Char] -> [Char] -> Int -> [Int]
search2 xs ys n = if (null ys) then
		    []
		  else
		    if starts xs ys then
		      (n:rest)
		    else
		      rest
		  where rest = search2 xs (tail ys) (n+1)

starts :: [Char] -> [Char] -> Bool
starts xs ys = (take (length xs) ys) == xs

search_test :: Bool
search_test = search "the" "look the other way" == [5,10]



-- Question 5

solution :: Int
solution = make (head (answers 9))

answers :: Int -> [[Int]]
answers n = if (n==0) then
	      [[]]
	    else
	      extend (answers (n-1))

-- Given a list of partial solutions length n, extend will return a list
-- of partial solutions length n+1, ensuring no duplicate numbers are
-- used

extend :: [[Int]] -> [[Int]]
extend xs = if (null xs) then
	      []
	    else
	      addgood w (kill [1,2,3,4,5,6,7,8,9] w) ++ (extend (tail xs))
	    where w = head xs

-- kill removes each of xs from ys

kill :: [Int] -> [Int] -> [Int]
kill xs ys = if (null ys) then
	       xs
	     else
	       kill (remove xs (head ys)) (tail ys)

-- remove removes element x from list l

remove :: [Int] -> Int -> [Int]
remove l x = if (null l) then  
               []
             else
               if (x == (head l)) then
                 tail l
               else
                 (head l : remove (tail l) x)

-- addgood will add each of ys to xs, returning those which are partial solutions

addgood :: [Int] -> [Int] -> [[Int]]
addgood xs ys = if (null ys) then
	      []
   	    else
	      if works newattempt then
		[newattempt] ++ rest
	      else
		rest
	    where
	      newattempt = xs ++ [head ys]
	      rest = (addgood xs (tail ys))

-- works test whether xs is a partial solution (only tests for n, not 1..n)

works :: [Int] -> Bool
works xs = (mod (make xs) (length xs))==0

-- make and make2 convert list [a,...,z] into integer a...z

make :: [Int] -> Int
make xs = make2 (reverse xs)

make2 :: [Int] -> Int
make2 xs = if (null xs) then
             0
           else
             head xs + 10*(make2 (tail xs))

solution_test :: Bool
solution_test = (solution == 381654729) && (length(answers 9)==1)
