-- A base is an ordered list of symbols
type Base = [Char]

-- Pre-defined bases
binary = ['0','1']
decimal = ['0'..'9']
hex = ['0'..'9'] ++ ['a'..'f']
octal = ['0'..'7']
ascii = ['\000'..'\127'] -- that is decimal, not octal!
wretchA = ['0'..'9'] ++ ['.']
wretchB = ['0'..'9'] ++ ['a'..'z'] ++ ['A'..'Z'] ++ ['.','-']

-- Convert from integer to string in given base
strbase :: Base -> Integer -> String
strbase base 0 = []
strbase base i = (strbase base divi) ++ [base!!(toInt modi)]
  where order = length base
        (divi,modi) = divMod i (toInteger order)

-- Convert from string in given base to integer
numbase :: Base -> String -> Integer
numbase base "" = 0
numbase base (d:ds) = weight*(order^column) + numbase base ds
  where order  = toInteger(length base)
        column = toInteger(length ds)
        weight = toInteger(seek d base)

-- Convert string s from base a to base b
baseconv :: String -> Base -> Base -> String
baseconv s basea baseb = strbase baseb (numbase basea s)

-- Finds index number of item x in list y
seek :: Eq a => a -> [a] -> Int
seek x (y:ys)
  | x==y = 0
  | otherwise = 1 + seek x ys

-- Shows as string the calculation involved (if you then evaluate this on command line it comes out fine =)
numbaseshow :: Base -> String -> String
numbaseshow base "" = "0"
numbaseshow base (d:ds) = show weight ++ "*(" ++ show order ++ "^" ++ show column ++ ")+" ++ (numbaseshow base ds)
  where order = length base
        column = length ds
        weight = seek d base

-- Show conversions in wretchA base for numbers x-n
countandcheck x n
  | (n<x)     = []
  | b==n      = countandcheck x (n-1) ++ [(b,a)]
  | otherwise = error ( (show b) ++ " /= " ++ (show n) )
      where a = strbase wretchB n
            b = numbase wretchB a
