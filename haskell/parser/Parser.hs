module Parser where

import Grammar
import Hlib
import Trace

teststringa = "<test>hello?</test>"
teststringa2 = "<test a=b>hello?</test>"
teststringb = "<t a=b><i>it</i><j>jt</j></t>"
teststringc = "<greeting type=happy><sender>Joey</sender><message>Hello!</message></greeting>"

parsestr :: IO String
parsestr = readFile "~j/code/java/tools/parser/grm.grm"
-- parsestr = return (teststringa2)

remainderof (Yes a b c d) = d
remainderof (No) = "Failure"

thebigmatch ps = match ps (Atom "Main")

resulta ps = ((show (thebigmatch ps))++"\nLeft over: "++"\nLeaving: "++(show (remainderof (thebigmatch ps))))

resultb ps = (showrep rules "hugs" (thebigmatch ps) )++"\nLeaving: "++(show (remainderof (thebigmatch ps)))

go = do
     ps <- parsestr
     putStr (resultb ps)

-- go = filter okay [("<g>Hello</g>","")]
--   where okay (x,y) = canfitarule x (getruleforatom (Atom "XML") rules)

neaten :: String -> String
neaten s = sreplace (sstrip 20 s) "\n" "\\n"

getruleforatom :: Type -> [Rule] -> Rule
getruleforatom (Atom a) ((t,ts,rs):rst) = if (t == (Atom a)) then
    (t,ts,rs)
  else
    getruleforatom (Atom a) rst

getrulesetforatom :: Type -> [Rule] -> RuleSet
getrulesetforatom (Atom a) rs = rus
  where (t,rus,rps) = (getruleforatom (Atom a) rs)

getreplacementsforatom :: Type -> [Rule] -> [Replacement]
getreplacementsforatom (Atom a) rs = rps
  where (t,rus,rps) = (getruleforatom (Atom a) rs)

getreplacementfrom :: [Replacement] -> String -> Replacement
getreplacementfrom [] s = ("",[])
getreplacementfrom ((rn,rs):rest) r = if (rn==r) then
    (rn,rs)
  else
    getreplacementfrom rest r

getreplacement :: Type -> String -> [Rule] -> Replacement
getreplacement (Atom a) r rules = getreplacementfrom (getreplacementsforatom (Atom a) rules) r
getreplacement t r rules = ("",[])

splitstr :: String -> [(String,String)]
splitstr [] = [("","")]
splitstr (s:ss) = [([s],ss)] ++ (map addhead (splitstr ss))
  where addhead (a,b) = (s:a,b)
--        s = head sss
--        ss = tail ss


match :: String -> Type -> Match

-- obsolete, all Vars should have exclusion
match s (Var v) = Yes (Var v) s [] ""

match s (VarExcl v e) = if (earliest == -1) then
    Yes (VarExcl v e) s [] ""
  else
    Yes (VarExcl v e) left [] right
  where earliest = foldl takemin (-1) (map (indexof s) e)
        takemin (-1) x = x
        takemin x (-1) = x
        takemin x y = min x y
        left = take earliest s
        right = drop earliest s

--  if (gotane) then
--                          (Yes (VarExcl v e) left [] right)
--                        else
--                          (Yes (VarExcl v e) s [] "")
--  where gotane = foldl (||) False (map test indexes)
--        test i = if (i == -1) then False else True
--        indexes = map (indexof s) e
--        whichof [] = -2
--        whichof (a:r) = if (a== -1) then (whichof r) else a
--        which = whichof indexes
--        left = take which s
--        right = drop which s

match s (Str x) = if (sstarts s x) then
    Yes (Str x) (take (length x) s) [] (drop (length x) s)
  else
    No

match s (Atom a) = if (thematch == []) then
    No --    trace (a++" = \""++neaten s++"\" failed.\n") No
  else
    Yes (Atom a) s thematch left
  where thematch = (matchwithruleset (getrulesetforatom (Atom a) rules) s)
        Yes p q r left = lastof thematch

lastof [a] = a
lastof (a:b) = lastof b

-- match s (Atom a) = map me trymatch
--   where me ms = Yes (Atom a) s ms ""
--         trymatch = matchwithruleset (getrulesetforatom (Atom a) rules) s

indexof "" s = -1
indexof (a:b) c = if (a==c) then
    0
  else
    if (i == -1) then -1 else i+1
  where i = indexof b c

isin :: Eq a => [a] -> a -> Bool
isin [] i = False
isin (x:xs) i = if (x==i) then True else isin xs i


matchwithruleset :: RuleSet -> String -> [Match]
-- matchwithruleset [] "" = [Yes (Var "final") "" [] ""]
matchwithruleset [] s = []
matchwithruleset (r:rs) s = if (thematch == []) then
    matchwithruleset rs s
  else
    thematch
  where thematch = matchwithrule r s

separate :: [a] -> [[a]]
separate [] = []
separate (x:xs) = [[x]]++separate xs


matchwithrule :: [Type] -> String -> [Match]

matchwithrule [] "" = [] -- [[Yes (Var "end") "" [] ""]]
matchwithrule [] s = []
matchwithrule xs "" = []
matchwithrule (h:t) s = if (headmatches) then
    headmatch:restofmatch
  else
    []
  where headmatch = match s h
        headmatches = (headmatch /= No) && ((t == []) || (restofmatch /= []))
        rest (No) = s
        rest (Yes a b c d) = d
        restofmatch = matchwithrule t (rest headmatch)

-- This is dodgy: no alternatives are returned if head (theone) fails
-- for earlier functions
-- contradiction between search (tree like) and results (trying to keep linear)
-- OK, earlier functions won't mind because they give this fn exactly what they want it to deal with


instance Show Type where
  show (Atom a) = a
  show (Var v) = "<"++v++">"
  show (VarExcl v e) = "<"++v++"~>"
--  show (VarExcl v e) = "<"++v++"> (no \""++e++"\")"
  show (Str s) = "\""++(sreplace (sstrip 20 s) "\n" "\\n")++"\""

instance Show Match where
  show No = "Failure"
  show m = myshowm m 0


myshowms :: [Match] -> Int -> String
myshowms [] i = "[]"
myshowms (m:ms) i = "["++(myshowm m i)++comma++drop 1 (myshowms ms i)
  where comma = if (not (null ms)) then
                  comma2 (head ms)
                else
                  ""
        comma2 (Yes (Var "end") s ms r) = ""
        comma2 x = ", "

myshowm :: Match -> Int -> String
myshowm (Yes (Str str) s ms r) i = "'"++(sreplace (sstrip 20 str) "\n" "\\n")++"'"
myshowm (Yes t s ms r) i = (show t)++thematch t++them
  where gotsome = (not (null ms))
        trailnl = if gotsome then "\n" else ""
        thematch (Atom a) = " \""++(sreplace (sstrip 20 s) "\n" "\\n")++"\""
        thematch tt = " = \""++(sreplace (sstrip 20 s) "\n" "\\n")++"\""
        them = if gotsome then
            " with ("++(show (length ms))++"):\n"++ -- " leaving \""++r++"\" with:\n"++
            (replicate (2*(i+1)) ' ')++(myshowms ms (i+1))++"\n"++ -- ++" leaving \""++(sreplace (sstrip 20 r) "\n" "\\n")++"\"\n"++
            (replicate (2*i) ' ')
          else ""

findmatch :: [Match] -> Type -> Match
findmatch [] t = No
findmatch (m:restms) t = if (m == No) then
    No
  else
    if (equal tt t) then
      Yes t str ms rest
    else
      findmatch restms t
  where Yes tt str ms rest = m
        equal (VarExcl a b) (VarExcl c d) = (a==c)
        equal (Var a) (VarExcl c d) = (a==c)
        equal (VarExcl a b) (Var c) = (a==c)
        equal (Atom a) (Atom b) = (a==b)
        equal ta tb = (ta==tb)

showreplacement :: [Rule] -> String -> Type -> [Match] -> String
-- showreplacement reps rep (Str s) ms = sreplaceinorder [("!qt!","\"" ),("\\n","temp"),("\\n","\n"),("temp","\\n")] s -- if ((findmatch ms (Str s)) /= No) then s else ""
showreplacement reps rep (Str s) ms = sreplaceinorder [("!qt!","\"" ),("\\n","temp"),("\\n","\n"),("temp","\\n")] s
showreplacement reps rep (Var v) ms = if (banana==No) then
    "Failed to find Var "++v++"\n"
  else
    str
  where (Yes t str matches l) = banana
        banana = findmatch ms (Var v)
showreplacement reps rep (VarExcl v e) ms = if (banana==No) then
    "Failed to find VarExcl "++v++" ("++e++")\n"
  else
    str
  where (Yes t str matches l) = banana
        banana = findmatch ms (Var v)
showreplacement reps rep (Atom a) ms = if (banana==No) then
    "Failed to find Atom "++a++" in "++show ms++"\n"
  else
--    if ((sinstr a "Rep")>=0) then
--      "\nRendering Atom "++a++":\n  "++(sstrip 300 (show ms))++"\n  "++sumstring++"\n"
--    else
      sumstring
  where (Yes t str matches l) = banana
        banana = findmatch ms (Atom a)
        sumstring =(foldl (++) "" (map (showrep reps rep) matches))

showreplacements :: [Rule] -> String -> [Match] -> [Type] -> String
showreplacements reps rep ms [] = ""
showreplacements reps rep ms (r:rs) = banana ++ (showreplacement reps rep r ms ++ showreplacements reps rep ms rs)
  where banana = trace "" "" -- ("Doing "++show r++"\n") ""

-- the completed replacement = showrep(rules,name_of_replacement_type,match)
showrep :: [Rule] -> String -> Match -> String
showrep rs r No = "showrep: Failed match!\n"
showrep rs r (Yes a s ms l) = banana ++ if (replacementexists) then
--    showreplacement rs r a ms
    showreplacements rs r ms ( snd thereplacement )
  else
    foldl (++) "" (map (showrep rs r) ms)
  where thereplacement = getreplacement a r rs
        replacementexists = thereplacement /= ( "" , [] )
        banana = trace "" "" -- trace ("\nShowing "++show a++" against "++(sstrip 200 (show ms))++"\n") "" -- "hello\n" ""
