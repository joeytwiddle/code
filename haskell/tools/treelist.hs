-- module Main where
-- import Hlib
import System
main = do
  args <- getArgs
  go args

data Tree = Branch [Tree] | Leaf [String]
  -- deriving (Show)

instance Show Tree where
  show (Branch bs) = "{\n" ++ concat (map indent (map show bs)) ++ "}\n"
    where indent s = "  "++s
  show (Leaf ss) = concat (map addn ss)
    where addn s = s++"\n"

test = go ["/stuff/data/cdrom_a2.find"]

go args = do
          file <- readFile (head args)
          putStrLn (show (treelist file))

treelist file = treebreak 0 (lines (file))

treebreak :: Int -> [String] -> (Tree)
treebreak col lines
  | indextoolarge = Leaf (concat breaks)
  | (length breaks) == 1 = assert (breaks == [lines]) (treebreak (col+1) lines)
  | otherwise = Branch ( map (treebreak (col+1)) breaks)
  where breaks = findbreaks col [] lines
        indextoolarge = col > (3)

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
