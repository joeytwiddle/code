-- Paul Clark (pc5065)
-- Assignment 3


-- Question 1
-- total : Takes two lists (secret and guess) and returns the number of
-- occurrences of each guess item in the secret item.

total :: Eq a => [a] -> [a] -> Integer
total s g = length s - length (s \\ g)

totaltest :: Bool
totaltest = total "12345" "02173" == 3


-- Question 2
-- gold, silver : Take secret and guess lists and return gold and silver
-- scores.

gold :: Eq a => [a] -> [a] -> Integer
gold s g = sum (map gold2 (zip s g))

gold2 :: Eq a => (a,a) -> Integer
gold2 (a,b) =
	if a==b then 1
	else 0

goldtest :: Bool
goldtest = gold "12345" "02173" == 1

silver :: Eq a => [a] -> [a] -> Integer
silver s g = total s g - gold s g

silvertest :: Bool
silvertest = silver "12345" "02173" == 2


-- Question 3
-- reply : Constructs appropriate reply to player's guess, and alters no. of
-- guesses appropraitely.

reply :: Integer -> String -> String -> (Integer,String)
reply guesses s g =
	if s==g || g=="?" then (0,"End of the game.") else
	if length s /= length g then (guesses,"Secret and guess must be same length.")
	else (guesses+1,"Golds: "++ show (gold s g) ++"  Silvers: "++ show (silver s g ) ++".")

replytest :: Bool
replytest =
	reply 5 "12345" "0217" == (5,"Secret and guess must be same length.")
	&& reply 6 "12345" "12345" == (0,"End of the game.")
	&& reply 7 "12345" "?" == (0,"End of the game.")
	&& reply 9 "12345" "02173" == (10,"Golds: 1  Silvers: 2.")


-- Question 4
-- Given functions

game :: Integer -> Dialogue
game n = interact go where
	go input = play 1 instructions secret (lines input)
	secret = pad 4 (show (randomise n))
	randomise n = (n*31415821+1) `mod` 10000

instructions :: String
instructions = "Try to guess my 4 digit secret.  Type ? if you give up."

play :: Integer -> String -> String -> [String] -> String
play n message secret guesses =
	message ++ "\n" ++ prompt n ++ continue n secret guesses

prompt :: Integer -> String
prompt n = if n /= 0 then "Guess " ++ show n ++ ": " else ""

continue :: Integer -> String -> [String] -> String
continue 0 secret gs = ""
continue n secret [] = ""
continue n secret (g:gs) =
	play newn newmessage secret gs
	where (newn,newmessage) = reply n secret g

-- pad : Add zeros to front of string to make it correct length.

pad :: Integer -> String -> String
pad l s =
	if l==length s then s
	else pad l ("0" ++ s)

padtest :: Bool
padtest =
	pad 5 "" == "00000"
	&& pad 4 "12" == "0012"
