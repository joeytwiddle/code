import Trace

data Type = Atom String | Var String | Str String
  deriving (Eq)

data Match = No | Yes Type String [Match] String
  deriving (Eq)

type RuleSet = [[Type]]

type Rule = ( Type , RuleSet )

rule1 = ( Atom "XML" ,
  [
    [ Str "<" , Var "type" , Str " ", Atom "Params" , Str ">" , Atom "Inside" , Str "</" , Var "type" , Str ">" ],
    [ Str "<" , Var "type" , Str ">" , Atom "Inside" , Str "</" , Var "type" , Str ">" ],
    [ Str "<" , Var "type" , Str "/>" ]
  ] )

rule2 = ( Atom "Params" ,
  [
    [ Atom "Param" ],
    [ Atom "Param", Str " ", Atom "Params" ]
  ] )

rule4 = ( Atom "Param" ,
  [
    [ Var "param" , Str "=" , Var "value" ]
  ] )

rule3 = ( Atom "Inside" ,
  [
    [ Atom "XMLs" ],
    [ Var "text" ]
    -- why doesn't it still resolve when Var "text"?
--    [ Atom "XMLInside" ]
--    [ Var "text" ]
  ] )

rule5 = ( Atom "XMLs" ,
  [
    [ Atom "XML" ]
--    [ Atom "XML" , Atom "XMLs" ]
  ] )

rules = [rule1,rule2,rule3,rule4,rule5]

parsestr :: IO String
-- parsestr = readFile "grm.grm"
parsestr = return ("<g a=b><x>Hello</x></g>")

go = do
     ps <- parsestr
     putStr (show (head (matches ps (Atom "XML"))))
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


-- List of possible matches, no space left at end
matches :: String -> Type -> [Match]

matches s (Var v) = [Yes (Var v) s [] ""]
-- map tosuccess (splitstr s)
-- where tosuccess (a,b) = Yes (Var v) a [] b

--matches "" (Var v) = [Yes (Var v) "" [] ""]
--matches s (Var v) = [Yes (Var v) "" [] s] ++ map addhead (matches (tail s) (Var v))
--  where addhead (Yes (Var v) str ms left) = Yes (Var v) (head s:str) ms left

matches s (Str x) = if (take (length x) s)==x then
    [Yes (Str x) x [] (drop (length x) s)]
  else
    []

matches s (Atom a) = map me trymatch
  where me ms = Yes (Atom a) s ms ""
        trymatch = matcheswithruleset (getrulesetforatom (Atom a) rules) s

--matches s (Atom a) = map addme (matcheswithruleset (getrulesetforatom (Atom a) rules) s)
--  where addme m = Yes (Atom a) s [m] ""

--matches s (Atom a) = filter okay (map try (splitstr s))
--  where try (x,y) = if ((length (ms x))>0) then
--                      (head (ms x))
--                    else
--                      No
--        ms z = matcheswithruleset ruleset z
--        okay m = m/=No
--        ruleset = getrulesetforatom (Atom a) rules



matcheswithruleset :: RuleSet -> String -> [[Match]]
-- matcheswithruleset [] "" = [Yes (Var "final") "" [] ""]
matcheswithruleset [] s = []
matcheswithruleset (r:rs) s = (matcheswithrule r s) ++
                              (matcheswithruleset rs s)
--  where mapify mss = map suc mss
--        suc ms = Yes


matcheswithrule :: [Type] -> String -> [[Match]]

matcheswithrule [] "" = [[Yes (Var "end") "" [] ""]]
matcheswithrule [] s = []
matcheswithrule xs "" = []

-- This is dodgy: no alternatives are returned if head (theone) fails
-- for earlier functions
-- contradiction between search (tree like) and results (trying to keep linear)
-- OK, earlier functions won't mind because they give this fn exactly what they want it to deal with

matcheswithrule ts s = -- trace ("\""++s++"\" == "++show ts++" >> "++show (headifcan results)++"\n")
  headifcan results
  where tryposs (a,b) = possmatches ts a b
        poss = splitstr s
        results = (foldr (++) [] (map tryposs poss))
        headifcan r = if (null r) then [] else [head r]


possmatches (t:ts) sa sb = if ((not (null amatches)) && (not (null bmatches))) then
    [[head amatches]++head bmatches]
--    addtoeach amatches bmatches
  else
    []
  where amatches = matches sa t
        bmatches = matcheswithrule ts sb

addtoeach :: [Match] -> [[Match]] -> [[Match]]
addtoeach as [] = []
addtoeach as (b:bs) = (map addb as) ++ addtoeach as bs
  where addb a = [a]++b

--[a++head pms]++addtoeach a (tail pms)
--([x]++y):addtoeach xs y





instance Show Type where
  show (Atom a) = a
  show (Var v) = "<"++v++">"
  show (Str s) = "\""++s++"\""

instance Show Match where
  show No = "Failure"
  show m = myshowm m 0
--  show (Yes t s ms r) = (show t)++" matches \""++s++"\" with\n  "++(show ms) -- ++" and \""++r++"\" left"


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
myshowm (Yes (Var "end") s ms r) i = "#"
myshowm (Yes t s ms r) i = (show t)++thematch t++them
  where gotsome = (not (null ms))
        trailnl = if gotsome then "\n" else ""
        thematch (Atom a) = " \""++s++"\""
        thematch tt = " = \""++s++"\""
        them = if gotsome then
            " with:\n"++
            (replicate (2*(i+1)) ' ')++(myshowms ms (i+1))++"\n"++
            (replicate (2*i) ' ')
          else ""

void s = ""