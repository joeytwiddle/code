--Paul Clark (PC5065)

--HASKELL:  First Assignment

--1  Find the area of paint needed to paint a room.

paint :: Integer -> Integer -> Integer -> Integer

paint l w h =
	if (l<0) || (w<0) || (h<0) then error "paint called with -ve parameters" else
	2*h*l + 2*h*w + w*l

paintTest :: Bool
paintTest = paint 1 2 3 == 20



--2  Find the suffix of given length of a given word.

suffix :: String -> Integer -> String

suffix w l =
	if length w < l then error "suffix called with word shorter than suffix required" else
	drop (length w - l) w

suffixTest :: Bool
suffixTest = suffix "carrot" 3 == "rot"



--3  Test whether one integer is divisible by another.

divides :: Integer -> Integer -> Bool

divides a b = a `mod` b == 0

dividesTest :: Bool
dividesTest =
	divides 10 5 &&
	not (divides 8 3)



--I was unable to complete questions 4 and 5 so I haven't included them
