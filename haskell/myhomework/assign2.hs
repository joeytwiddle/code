-- Paul N. Clark (PC5065)


-- Question 1
-- Add together the elements of a list with spaces inbetween to generate
-- a phrase.

phrase :: [String] -> String
phrase ws = init (foldr (phrase2) "" ws)

phrase2 :: String -> String -> String
phrase2 w1 w2 = w1 ++ " " ++ w2

phrasetest :: Bool
phrasetest = phrase ["Haskell","is","brill"] == "Haskell is brill"


-- Question 2
-- Produce a sentence from a phrase (ie. make first letter capital and
-- add "." at end).

sentence :: [String] -> String
sentence ws = sentence2 (phrase ws)

sentence2 :: String -> String
sentence2 p = (toUpper (head p)) : (tail p) ++ "."

sentencetest :: Bool
sentencetest = sentence ["bristol","university"] == "Bristol university."


-- Question 3
-- Find the list of positions of items in a list.

listpos :: [item] -> [Integer]
listpos l = [0..(length l)-1]

listpostest :: Bool
listpostest = listpos ["a","b","c"] == [0,1,2]


-- Question 4
-- Convert a list into a list of pairs, where each item is labelled with
-- its position in the list.

convert :: [item] -> [(Integer,item)]
convert l = zip (listpos l) l

converttest :: Bool
converttest = convert ["a","b","z"] == [(0,"a"),(1,"b"),(2,"z")]


-- Question 5
-- Find the labels of a list of labelled items.

label :: [(label,item)] -> [label]
label l = map fst l

labeltest :: Bool
labeltest = label [(0,"a"),(1,"b"),(2,"c")] == [0,1,2]


-- Question 6
-- Finds the positions in a list at which a given element appears.

find :: String -> [String] -> [Integer]
find i l = label (filter (isMatch i) (convert l))

isMatch :: String -> (Integer,String) -> Bool
isMatch i (a,b) = b==i

findtest :: Bool
findtest = find "x" ["x","a","y","x"] == [0,3]

-- Note: I had problems trying to make this function generic.


-- Question 7
-- Find the length of a list.

myLength :: [item] -> Integer
myLength l = sum (map myLength2 l)

myLength2 :: item -> Integer
myLength2 i = 1

myLengthtest :: Bool
myLengthtest = myLength ["j","k","l"] == 3

-- Question 8
-- Compose three function as (.) does with two.

compose3 :: (a->b) -> (c->a) -> (d->c) -> d -> b
compose3 a b c = a . b . c

init3 :: String -> String
init3 = compose3 init init init

compose3test :: Bool
compose3test = init3 "abcdefghijklm" == "abcdefghij"


-- Question 9
-- Find a factorial using folding rather than recursion.

fac :: Integer -> Integer
fac n = foldl (*) 1 [1..n]

factest :: Bool
factest = fac 5 == 120


-- Question 10
-- Given a word and a sentence, find the positions where the word appears
-- in the sentence.

search :: String -> String -> [Integer]
search w s = find w (search2 s [0..((length s)-(length w))] (length w))

-- search2 takes the sentence, the list of labels and the length of the search
-- word, and produces a list of labels and their corresponding slices of the
-- sentence.

search2 :: String -> [Integer] -> Integer -> [String]
search2 s l len = map (search3 s len) l

search3 :: String -> Integer -> Integer -> String
search3 s len l = take len (drop l s)

searchtest :: Bool
searchtest = search "car" "cargo carrot car" == [0,6,13]
