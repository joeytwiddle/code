#!/usr/bin/runhugs
-- rats ghc doesn't like that!

-- Usage:
--   treelist <file> | vim -
--     then start folding
--     and :set foldtext=getline(v:foldstart).'\ \ \ ['.(v:foldend-v:foldstart).'\ lines]'

-- What does it do?
--   Given a text stream, it looks for a tree-like pattern in the data.
--   Subsequent lines which start with the same String are grouped together under a branch.

-- Output:
--   Currently output adds a one of "+.-" to the start of each line (irrelevant),
--   adds curly braces around branches (needed for my vim folding rule),
--   and strips the common start-String (replaces it with spaces).

-- Todo:
--   - Add options allowing user to specify min group size, min diff, and output (when to
--     strip, whether or not to include a line for each branch, how to group).
--   - Possibly leave out a single if it == the last branch opening (user has already seen it)
--     Nah that shouldn't happen (it shouldn't branch on just 1), or maybe it should be an option.
--   - Could output visualisation of tree branches (oh no not indent passing again!)

-- Design problem:
--   If we don't want it to branch on small differences (eg <3 chars)
--     then we have to keep the path knowledge in the child.
--   Actually, I think it might be OK, because it doesn't branch ["branch","bravo"]
--     until they are searched up to col=2.
--   No I think the problem is there.  To see what's rejected, employ commented
--     code on line: "| same = {- ... -}"
--   As you can see the code I finished up with is rather inefficient.  I think a slightly
--     different search design should solve this, but I initially coded with the algorithm
--     in my head, then realised I needed to change the algorithm and then change the code.

module Main where
-- import Hlib
import System
main = do
  args <- getArgs
  go args

data Tree = Single String | Branch Bool String [Tree] | Leaf [String]
  -- deriving (Show)

instance Show Tree where
  show (t) = showTree "" "" t
  -- show (Leaf ss) = concat (map addn ss)
    -- where addn s = s++"\n"

showTree sofar indent (Single s) = ". "++(indent++""++s++""++"\n")
showTree sofar indent (Branch same c bs)
  | same && (length c)>=3    = "+ "++sofar++c++" {\n"++ concat (map (showTree (sofar++c) ((realindent))) bs) ++ "-"++realindent++"}\n"
  | same      = {-"| "++indent++"("++c++")\n" ++-} concat (map (showTree (sofar++c) (indent++c)) bs)
  | otherwise = concat (map (showTree sofar (indent)) bs)
  where realindent = replicate (length indent + length c) ' '
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
  | otherwise            = Branch True ((take (col) (head (head breaks)))) (( map (subtree col) breaks ))
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
