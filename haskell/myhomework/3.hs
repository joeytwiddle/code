-- Assignment 1
-- Paul Clark (pc5065)



-- Question 1

sentence :: [[Char]] -> [Char]
sentence a = [toUpper (head (sentence2 a))] ++ tail (sentence2 a) ++ "."
	where
		sentence2 a = tail (foldl sentence3 "" a)
		sentence3 a b = a++" "++b

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
starts xs ys = chop ys (length xs) == xs

chop :: [Char] -> Int -> [Char]
chop xs n = if (n == 0) then
	      []
	    else
	      (head xs:chop (tail xs) (n-1))

search_test :: Bool
search_test = search "the" "look the other way" == [5,10]



-- Question 5

answers :: Int -> [[Int]]
answers n = if (n==0) then
	      [[]]
	    else
	      filterifworks (possibles (answers (n-1)))

possibles :: [[Int]] -> [[Int]]
possibles xs = if (null xs) then
		 []
	       else
	         add w (kill [1,2,3,4,5,6,7,8,9] w) ++ (possibles (tail xs))
	       where w = head xs

kill :: [Int] -> [Int] -> [Int]
kill xs ys = if (null ys) then
	       xs
	     else
	       kill (remove xs (head ys)) (tail ys)

add :: [Int] -> [Int] -> [[Int]]
add xs ys = if (null ys) then
	      []
   	    else
	      [xs ++ [head ys]] ++ (add xs (tail ys))

filterifworks :: [[Int]] -> [[Int]]
filterifworks xs = filter (works) xs

works :: [Int] -> Bool
works xs = (mod (sum xs) (length xs))==0

remove :: [Int] -> Int -> [Int]
remove l x = if (null l) then
               []
             else 
               if (x == (head l)) then
                 tail l
               else
                 (head l : remove (tail l) x)
