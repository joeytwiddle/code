-- Usage:
-- treelist <file> | vi -
-- then start folding
-- and :set foldtext=getline(v:foldstart).'\ \ \ ['.(v:foldend-v:foldstart).'\ lines]'

-- module Main where
-- import Hlib
import System
main = do
  args <- getArgs
  go args

data Tree = Layer String Tree | MinBranch [Tree] | Single String | Branch Bool String [Tree] | Leaf [String]
  -- deriving (Show)

instance Show Tree where
  show (t) = showTree "" "" t
  -- show (Leaf ss) = concat (map addn ss)
    -- where addn s = s++"\n"

showTree sofar indent (Layer l t) = "| "++indent++l++"\n" ++ showTree (sofar++l) (indent++(replicate (length l) ' ')) t
showTree sofar indent (Single s) = ". "++(sofar++""++s++""++"\n")
showTree sofar indent (MinBranch bs) = "( "++indent++"\n" ++ concat (map (showTree sofar (indent)) bs)++ ")\n"
showTree sofar indent (Branch same c bs)
  | same      = "{ "++indent++c++"\n"++ concat (map (showTree (sofar++c) (indent++(replicate (length c) ' '))) bs) ++ "}\n"
  | otherwise = concat (map (showTree sofar (indent)) bs)
  -- | otherwise = "< "++indent++"{"++c++"\n"++ concat (map (showTree sofar (indent++(replicate (length c) ' '))) bs) ++ "> "++indent++"}\n"
showTree sofar indent (Leaf ls) = concat (map (sort) ls)
  where sort xs = "= " ++ indent ++ ">"++xs++"<" ++ "\n"

-- test = go ["/stuff/data/cdrom_a2.find"]
test = go ["test.txt"]

go args = do
          file <- readFile (head args)
          putStrLn (show (treelist file))

treelist file = treebreak 0 (lines (file))

treebreak :: Int -> [String] -> (Tree)
treebreak col lines
  | indextoolarge        = (Leaf (concat breaks))
  | length breaks == 0   = Leaf []
  | (length breaks) == 1 = treebreak (col+1) lines
  | otherwise = Branch True ((take (col) (head (head breaks)))) (( map (subtree col) breaks ))
  where breaks = findbreaks col [] lines
        indextoolarge = length lines <= 1 -- col > 1 -- (length (head (head breaks))) -- col>2000 -- (myhead (head (head breaks))) == '_'
        -- strip n xs = map (stripc n) xs
        stripc n ys = map (drop n) ys
{-++clip (show (map length breaks)))-}

-- subtree col [] = Leaf []
subtree col [x] = Single (drop col x)
subtree col ss =
  if head ss == [] then
    Leaf []
  else
    Branch False [(head ss)!!col] ([(treebreak 1 (map (drop (col)) ss))])

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
