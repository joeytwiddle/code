-- These are model answers to the first assignment (see end of Chapter 2 of
-- the course notes).  This file forms a Hugs program.
--
--                                                      Ian Holyer 17-Jan-96

----------------
-- Question 1 --
----------------

-- Find the area of paint needed to cover the walls and ceiling of a
-- rectangular room, given its length, width and height as positive integers.
-- Any units can be used; the answer will be in square units.

paint :: Integer -> Integer -> Integer -> Integer

paint l w h = 2*l*h + 2*w*h + l*w

paintTest :: Bool
paintTest = paint 6 5 4 == 118

----------------
-- Question 2 --
----------------

-- Find the suffix of a given length at the end of a word.

suffix :: Integer -> String -> String

suffix n s = reverse (take n (reverse s))

suffixTest :: Bool
suffixTest =
   suffix 4 "goodness" == "ness"

----------------
-- Question 3 --
----------------

-- Test whether one integer is divisible by another.

divisible :: Integer -> Integer -> Bool

divisible m n = m `mod` n == 0

divisibleTest :: Bool
divisibleTest =
   divisible 15 5 == True &&
   divisible (-16) 5 == False

----------------
-- Question 4 --
----------------

-- Convert a 20th century date from a rigid format with two digits for each
-- part, eg "01-12-95", to a more readable format, eg "1-Dec-1995".

date :: String -> String

date s =
   day (take 2 s) ++ "-" ++ month (take 2 (drop 3 s)) ++ "-" ++ year (drop 6 s)

dateTest :: Bool
dateTest = date "01-12-95" == "1-Dec-1995"


-- Convert a day string by getting rid of a possible leading zero.

day :: String -> String

day s = if head s /= '0' then s else tail s

dayTest :: Bool
dayTest = day "01" == "1" && day "11" == "11"


-- Convert a month string from numerical to textual form.

month :: String -> String

month s = take 3 (drop (4 * (read s - 1))
   "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec")

monthTest :: Bool
monthTest = month "1" == "Jan" && month "12" == "Dec"


-- Convert a year string from abbreviated to full format

year :: String -> String

year s = "19" ++ s

yearTest :: Bool
yearTest = year "95" == "1995"

----------------
-- Question 5 --
----------------

-- Convert a positive integer such as 42 into an ordinal string such as "42nd".

ordinal :: Integer -> String

ordinal n = addSuffix (show n)

ordinalTest :: Bool
ordinalTest =
   ordinal 1 == "1st"   && ordinal 2 == "2nd"   && ordinal 3 == "3rd"    &&
   ordinal 11 == "11th" && ordinal 12 == "12th" && ordinal 13 == "13th"  &&
   ordinal 21 == "21st" && ordinal 22 == "22nd" && ordinal 23 == "23rd"  &&
   ordinal 4 == "4th"   && ordinal 44 == "44th" && ordinal 444 == "444th"


-- Add a suffix "st" or "nd" or "rd" to a numerical string, as appropriate.
-- (Note: this definition uses suffix from question 2, and relies on suffix
-- returning an answer even if the argument string is too short. Testing is
-- implicit in the testing for ordinal above.)

addSuffix :: String -> String

addSuffix s =
   if suffix 1 s == "1" && suffix 2 s /= "11" then s ++ "st" else
   if suffix 1 s == "2" && suffix 2 s /= "12" then s ++ "nd" else
   if suffix 1 s == "3" && suffix 2 s /= "13" then s ++ "rd" else
   s ++ "th"
