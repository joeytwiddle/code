#!/usr/bin/runhugs
-- rats ghc doesn't like that!  (use the compile.sh)

-- Usage:
--   treelist <file> | vim -
--     then start folding.

-- What does it do?
--   Given a text stream, it looks for a tree-like pattern in the data.
--   Subsequent lines which start with the same String are grouped together
--     under a branch.
--   I'm talking about matches at the begining of adjacent lines forming
--     groups.
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
--   - Add options allowing user to specify min group size, min diff, and
--     output (when to strip, whether or not to include a line for each branch,
--     how to group).
--   - Possibly leave out a single if it == the last branch opening (user has
--     already seen it)
--     Nah that shouldn't happen (it shouldn't branch on just 1), or maybe it
--     should be an option.
--   - Could output visualisation of tree branches (oh no not indent passing
--     again!)
--   - Offer user option to only break in specified situations (eg. at a '/' or
--     '.')
--       OK think I got that now with onlyBranchAfter but should turn off
--         minlength and minsize (reduce to 1)
--       No there are still problems, if the algorithm finds a wider match and
--         skips the column with the special char!

-- Design problem:
--   If we don't want it to branch on small differences (eg <3 chars)
--     then we have to keep the path knowledge in the child.
--   Actually, I think it might be OK, because it doesn't branch
--     ["branch","bravo"] until they are searched up to col=2.
--   No I think the problem is there.  To see what's rejected, employ commented
--     code on line: "| same = {- ... -}"
--   As you can see the code I finished up with is rather inefficient.  I think
--     a slightly different search design should solve this, but I initially
--     coded with the algorithm in my head, then realised I needed to change
--     the algorithm and then change the code.

-- Things I also like to do to vim when I'm viewing a tree:
--   :set foldtext=getline(v:foldstart).'\ \ \ ['.(v:foldend-v:foldstart).'\ lines]'
-- A couple of maps in my ~/.vim/... which open and close folds on + an -:
--   :map - zc
--   :map + zo

module Main where
-- import Hlib
import System

---------------------------------- Options ----------------------------------

showFoldLines    = yes -- When on shows common heads more clearly, when off keeps more like original file
showTags         = yes
showCurls        = yes -- needed for folding
showBranchSizes  = yes
indentwithspaces = no  -- Very bad without showFoldLines (overwrites all occurances of the folded string portion!)
minlength = 1 ::Int
minsize   = 3 ::Int
onlyBranchAfter = ['\000'..'\255'] -- all (no restriction)
-- onlyBranchAfter = "/. _-" -- eg. for directories, file extensions etc.

-- onlyindentafter = "" -- same as branchafter surely?
-- alwaysBranchFirst = True -- dunno what this was supposed to do

tagopen  = tag '+'
tagbasic = tag '.'
tagleaf  = tag '.'
tagclose = tag '-'
curlopen = curl " -{"
curlclose = curl " }-"

--------------------------------- Data types ---------------------------------

data Tree = Single String |
            Branch Bool String [Tree] |
            Leaf [String]
  -- deriving (Show)
-- I think a Leaf is just a set of (inline) Singles
-- I can't remember what the Branch's boolean means
-- I now believe we get a Leaf if it is 1) single hence not checked, or 2) too
--   few to bother.

instance Show Tree where
  show (t) = showTree "" "" t
  -- show (Leaf ss) = concat (map addn ss)
    -- where addn s = s++"\n"

yes = True
no = False

tag x = if showTags then x:" " else ""
tagreplace t xs = if showTags then t ++ drop 2 xs else xs
curl x = if showCurls then x else ""

size (Single s) = 1
size (Branch same chars bs) = sum (map size bs)
size (Leaf leaves) = length leaves

------------------------------------ Main ------------------------------------

main = do
       args <- getArgs
       go args

go args = do
          file <- readFile (head args)
          putStrLn (show (treelist file))

test = go ["test.txt"]
-- test = go ["/stuff/data/cdrom_a2.find"]

treelist file = treebreak 0 (lines (file))

-- Break a String of lines into a Tree where lines starting with the same
-- characters share a branch.
treebreak :: Int -> [String] -> (Tree)
treebreak col lines
  | toofewtobother             = (Leaf (concat breaks))
  | length breaks == 0         = Leaf []
  | (length breaks) == 1       = treebreak (col+1) lines
  | otherwise                  = Branch True specialchar nextlevel
  where breaks = findbreaks col [] lines
        toofewtobother = length lines < minsize
        -- col > 1
        -- (length firstOfFirst)
        -- col>2000
        -- (myhead firstOfFirst) == '_'
        specialchar = take col firstOfFirst
        firstOfFirst = (head.head) breaks
        nextlevel = map (subtree col) breaks
        -- badchar = debug qqq if qqq == "" then False else elem (qqq!!((length qqq) - 1)) onlyBranchAfter
        -- qqq = take col firstOfFirst

-- subtree col [] = Leaf []
subtree col [x] = Single (drop col x)
subtree col ss =
  if head ss == [] then
    Leaf []
  else
    Branch False [(head ss)!!col] ([(treebreak 1 (map (drop (col)) ss))])

-- At a given column, break a given list of Strings into lists of strings
-- which have that character the same:
findbreaks :: Int -> [String] -> [String] -> [[String]]
findbreaks col sofar [] = [sofar]
findbreaks col sofar [x] = [sofar++[x]]
findbreaks col sofar (x:y:rest)
  | col >= min (length x) (length y) = break
  | (x!!col) == (y!!col)             = continue
  | otherwise                        = break
  where break = (sofar++[x]):(findbreaks col [] (y:rest))
        continue = findbreaks col (sofar++[x]) (y:rest)

-- Output the Tree in desired format:
-- Note some conditions on Branch mean sometimes tree Branch's fold is not
--   exposed.
showTree :: String -> String -> Tree -> String
showTree sofar indent (Single s) = tagbasic++(indent++""++s++""++"\n")
showTree sofar indent (Leaf leaves) = concat (map (sort) leaves)
  -- where sort xs = "= " ++ indent ++ ">"++xs++"<" ++ "\n"
  where sort xs = tagleaf ++ indent ++ xs ++ "\n"
showTree sofar indent (Branch same chars bs)
  | same && oktobranch  = wrapcurls
  | same                = theBlockUnfolded
  | otherwise           = theBlockNoBranch
  where theBlock         = concat (map (showTree nextsofar realindent) bs)
        theBlockUnfolded = concat (map (showTree nextsofar nextindent) bs)
        theBlockNoBranch = concat (map (showTree sofar indent) bs)
        oktobranch = notTooSmall && canBranch
        notTooSmall = (length chars)>=minlength
        canBranch = elem (chars!!((length chars)-1)) onlyBranchAfter
        realindent = if indentwithspaces then spacedindent else nextindent
        spacedindent = replicate (length indent + length chars) ' '
        nextindent = indent ++ chars
        nextsofar  = sofar ++ chars
        wrapcurls = if showFoldLines then wrapSeparate else wrapInline theBlock
        wrapSeparate = tagopen++sofar++chars++curlopen ++ showsize ++ "\n"
                       ++ theBlock ++
                       tagclose++realindent++curlclose ++ "\n"
        showsize = if showBranchSizes
                   then (" x " ++ show (size (Branch same chars bs)) )
                   else ""

-- If we aren't displaying a separate foldline, we hack the stringblock to add
-- the tags and curls on the lines at either end of the block:
wrapInline block = tagreplace tagopen (ls!!0) ++ curlopen ++
                   splurge mid ++ "\n" ++
                   tagreplace tagclose (ls!!last) ++ curlclose ++ "\n"
  where ls = lines block
        last = (length ls) - 1
        mid = tail (take last ls) -- drops one off each end
        splurge xs = foldl sp "" xs -- puts newlines inbetween
        sp x y = x ++ "\n" ++ y

