import Grammar
import Hlib
import Trace

rule1 = ( Atom "XML" ,
  [
    [ Str "<" , VarExcl "type" "<> ", Str " ", Atom "Params" , Str ">" , Atom "Inside" , Str "</" , VarExcl "type" " <>" , Str ">" ],
    [ Str "<" , VarExcl "type" "<> ", Str ">" , Atom "Inside" , Str "</" , VarExcl "type" "<> ", Str ">" ],
    [ Str "<" , VarExcl "type" "<> ", Str "/>" ]
  ] )

rule2 = ( Atom "Params" ,
  [
    [ Atom "Param", Str " ", Atom "Params" ],
    [ Atom "Param" ]
  ] )

rule4 = ( Atom "Param" ,
  [
    [ VarExcl "param" "<> =", Str "=" , VarExcl "value" "<> " ]
  ] )

rule3 = ( Atom "Inside" ,
  [
    [ Atom "XMLs" ],
    [ VarExcl "text" "<>" ]
  ] )

rule5 = ( Atom "XMLs" ,
  [
    [ Atom "XML" , Atom "XMLs" ] ,
    [ Atom "XML" ]
  ] )

-- rules = [rule1,rule2,rule3,rule4,rule5]

teststringa = "<test>hello?</test>"
teststringa2 = "<test a=b>hello?</test>"
teststringb = "<t a=b><i>it</i><j>jt</j></t>"
teststringc = "<greeting type=happy><sender>Joey</sender><message>Hello!</message></greeting>"

parsestr :: IO String
-- parsestr = readFile "grm.grm"
parsestr = return (teststringa2)

go = do
     ps <- parsestr
     putStr (show (match ps (Atom "Main")))
-- go = filter okay [("<g>Hello</g>","")]
--   where okay (x,y) = canfitarule x (getruleforatom (Atom "XML") rules)

getruleforatom :: Type -> [Rule] -> Rule
getruleforatom (Atom a) (r:rs) = if ((fst r) == (Atom a)) then
    r
  else
    getruleforatom (Atom a) rs

getrulesetforatom :: Type -> [Rule] -> RuleSet
getrulesetforatom (Atom a) rs = snd (getruleforatom (Atom a) rs)

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
    No
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
  show (Str s) = "\""++s++"\""

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
myshowm (Yes (Str str) s ms r) i = "'"++str++"'"
myshowm (Yes t s ms r) i = (show t)++thematch t++them
  where gotsome = (not (null ms))
        trailnl = if gotsome then "\n" else ""
        thematch (Atom a) = " \""++s++"\""
        thematch tt = " = \""++s++"\""
        them = if gotsome then
            " leaving \""++r++"\" with:\n"++
            (replicate (2*(i+1)) ' ')++(myshowms ms (i+1))++"\n"++
            (replicate (2*i) ' ')
          else ""

void s = ""
