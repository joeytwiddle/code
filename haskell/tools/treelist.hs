-- module Main where
-- import Hlib
import System
main = do
  args <- getArgs
  go args

data Tree = Layer String Tree | MinBranch [Tree] | Branch String [Tree] | Leaf [String]
  -- deriving (Show)

instance Show Tree where
  show (t) = showTree "" t
  -- show (Leaf ss) = concat (map addn ss)
    -- where addn s = s++"\n"

showTree indent (Layer l t) = showTree (indent++""++l++"") t
showTree indent (MinBranch bs) = concat (map (showTree indent) bs)++"\n"
showTree indent (Branch c bs) = indent++"{"++c++"\n"++ concat (map (showTree (indent++c)) bs) ++ indent++"}\n"
showTree indent (Leaf ls) = concat (map (sort) ls)
  where sort xs = indent ++ ">"++xs++"<" ++ "\n"

-- test = go ["/stuff/data/cdrom_a2.find"]
test = go ["test.txt"]

go args = do
          file <- readFile (head args)
          putStrLn (show (treelist file))

treelist file = treebreak 0 (lines (file))

treebreak :: Int -> [String] -> (Tree)
treebreak col lines
  | indextoolarge = (Leaf (concat breaks))
  | length breaks == 0  = Leaf []
  | (length breaks) == 1 = Layer (take (col+1) (head (head breaks))) (treebreak (0) (stripc (col+1) lines))
  | otherwise = MinBranch ( map subtree breaks )
  where breaks = findbreaks col [] lines
        indextoolarge = length lines <= 1 -- col > 1 -- (length (head (head breaks))) -- col>2000 -- (myhead (head (head breaks))) == '_'
        -- strip n xs = map (stripc n) xs
        stripc n ys = map (drop n) ys
{-++clip (show (map length breaks)))-}

subtree [] = Leaf []
subtree [x] = Leaf [x]
subtree ss = if head ss == [] then Leaf [] else Layer [head (head ss)] (MinBranch [(treebreak 0 (map tail ss))])

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
