#!/usr/bin/runhugs
-- rats ghc doesn't like that!  (use the compile.sh)

-- Usage:
--   treelist <file> | vim -
--     then start folding.

-- What does it do?
--   Given a text stream, it looks for a tree-like pattern in the data.
--   Subsequent lines which start with the same String are grouped together under a branch.
--   I'm talking about matches at the begining of adjacent lines forming groups.
--   I'm talking about re-treeing indented text, or path listings, etc...

-- Output:
--   Currently vim is the only supported method of navigating the tree...
--   Currently output adds one of "+-." at the start of each line
--   (for user, signifies branch start, branch end, or single line),
--   adds curly braces around branches (needed for my vim folding rule),
--   and strips the common start-String (replaces it with spaces).

-- Bugs:
--   If the data contains '{'s or '}'s then vim's folding will get confused.

-- Todo:
--   - Add options allowing user to specify min group size, min diff, and output (when to
--     strip, whether or not to include a line for each branch, how to group).
--   - Possibly leave out a single if it == the last branch opening (user has already seen it)
--     Nah that shouldn't happen (it shouldn't branch on just 1), or maybe it should be an option.
--   - Could output visualisation of tree branches (oh no not indent passing again!)
--   - Offer user option to only break in specified situations (eg. at a '/' or '.')

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

-- Things I also like to do to vim when I'm viewing a tree:
--   :set foldtext=getline(v:foldstart).'\ \ \ ['.(v:foldend-v:foldstart).'\ lines]'
-- A couple of maps in my ~/.vim/... which open and close folds on + an -:
--   :map - zc
--   :map + zo

module Main where
-- import Hlib
import System
main = do
  args <- getArgs
  go args

-- Options
minlength :: Int
minlength = 1
minsize :: Int
minsize = 3
indentwithspaces = False
showBranchSizes = False
-- onlyindentafter = ""
-- onlyBranchAfter = "/"
-- alwaysBranchFirst = True

data Tree = Single String |
            Branch Bool String [Tree] |
            Leaf [String]
  -- deriving (Show)

-- I think a Leaf is just a set of (inline) Singles
-- I can't remember what the Branch's boolean means

instance Show Tree where
  show (t) = showTree "" "" t
  -- show (Leaf ss) = concat (map addn ss)
    -- where addn s = s++"\n"

-- test = go ["/stuff/data/cdrom_a2.find"]
test = go ["test.txt"]

go args = do
          file <- readFile (head args)
          putStrLn (show (treelist file))

treelist file = treebreak 0 (lines (file))

treebreak :: Int -> [String] -> (Tree)
treebreak col lines
  | toofewtobother             = (Leaf (concat breaks))
  | length breaks == 0         = Leaf []
  | (length breaks) == 1       = treebreak (col+1) lines
  | otherwise                  = Branch True ((take (col) (head (head breaks)))) (( map (subtree col) breaks ))
  where breaks = findbreaks col [] lines
        toofewtobother = length lines < minsize -- col > 1 -- (length (head (head breaks))) -- col>2000 -- (myhead (head (head breaks))) == '_'
        -- strip n xs = map (stripc n) xs
        stripc n ys = map (drop n) ys
        -- badchar = debug qqq if qqq == "" then False else elem (qqq!!((length qqq) - 1)) onlyBranchAfter
        -- qqq = take col (head (head breaks))

-- subtree col [] = Leaf []
subtree col [x] = Single (drop col x)
subtree col ss =
  if head ss == [] then
    Leaf []
  else
    Branch False [(head ss)!!col] ([(treebreak 1 (map (drop (col)) ss))])

-- At a given column, break a given list of Strings into lists of strings
-- which have that character the same.
findbreaks :: Int -> [String] -> [String] -> [[String]]
findbreaks col sofar [] = [sofar]
findbreaks col sofar [x] = [sofar++[x]]
findbreaks col sofar (x:y:rest)
  | col >= min (length x) (length y) = break
  | (x!!col) == (y!!col)             = continue
  | otherwise                        = break
  where break = (sofar++[x]):(findbreaks col [] (y:rest))
        continue = findbreaks col (sofar++[x]) (y:rest)

showTree sofar indent (Single s) = ". "++(indent++""++s++""++"\n")
showTree sofar indent (Branch same chars bs)
  | same && (length chars)>=minlength    = "+ "++sofar++chars++" {" ++ showsize ++ "\n"
                                       ++ concat (map (showTree (sofar++chars) ((realindent))) bs)
                                       ++ "- "++realindent++" }\n"
  | same                             = {-"| "++indent++"("++chars++")\n" ++-} concat (map (showTree (sofar++chars) (indent++chars)) bs)
  | otherwise                        = concat (map (showTree sofar (indent)) bs)
  where realindent = if indentwithspaces then replicate (length indent + length chars) ' ' else indent ++ chars
  -- | otherwise = "< "++indent++"{"++chars++"\n"++ concat (map (showTree sofar (indent++(replicate (length chars) ' '))) bs) ++ "> "++indent++"}\n"
        showsize = if showBranchSizes then (" x " ++ show (size (Branch same chars bs)) ) else ""
showTree sofar indent (Leaf leaves) = concat (map (sort) leaves)
  -- where sort xs = "= " ++ indent ++ ">"++xs++"<" ++ "\n"
  where sort xs = "* " ++ indent ++ xs ++ "\n"

size (Single s) = 1
size (Branch same chars bs) = sum (map size bs)
size (Leaf leaves) = length leaves
