-- module Main where
-- import Hlib
import System
main = do
  args <- getArgs
  go args

data Tree = Branch String [Tree] | Leaf [String]
  -- deriving (Show)

instance Show Tree where
  show (Branch c bs) = showTree "" (Branch c bs)
  -- show (Leaf ss) = concat (map addn ss)
    -- where addn s = s++"\n"

showTree indent (Branch c bs) = indent++""++c++"\n"++ concat (map (showTree (indent++replicate (length c) ' ')) bs) ++ indent++"}\n"
showTree indent (Leaf ls) = concat (map (sort) ls)
  where sort xs = indent ++ xs ++ "\n"

-- test = go ["/stuff/data/cdrom_a2.find"]
test = go ["test.txt"]

go args = do
          file <- readFile (head args)
          putStrLn (show (treelist file))

treelist file = treebreak 0 (lines (file))

treebreak :: Int -> [String] -> (Tree)
treebreak col lines
  | indextoolarge = Leaf (concat breaks)
  | (length breaks) == 1 = assert (breaks == [lines]) (Branch (take (col+1) (head (head breaks))) [(treebreak (0) (stripc (col+1) lines))])
  | otherwise = Branch (take (col+1) (head (head breaks))) ( map (treebreak (0)) (strip (col+1) breaks))
  where breaks = findbreaks col [] lines
        indextoolarge = length lines == 1 -- col > 1 -- (length (head (head breaks))) -- col>2000 -- (myhead (head (head breaks))) == '_'
        strip n xs = map (stripc n) xs
        stripc n ys = map (drop n) ys
{-++clip (show (map length breaks)))-}

clip xs
  | length xs < 50 = xs
  | otherwise      = take 50 xs

myheads [] = '_'
myheads (h:t) = h

assert x y = if x then y else error ( show x ++ "is false" )

findbreaks :: Int -> [String] -> [String] -> [[String]]
findbreaks col sofar [] = [sofar]
findbreaks col sofar [x] = [sofar++[x]]
findbreaks col sofar (x:y:rest)
  | col >= min (length x) (length y) = break
  | (x!!col) == (y!!col)             = continue
  | otherwise                        = break
  where break = (sofar++[x]):(findbreaks col [] (y:rest))
        continue = findbreaks col (sofar++[x]) (y:rest)
