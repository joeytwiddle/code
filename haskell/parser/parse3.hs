import Trace

data Type = Atom String | Var String | Str String
  deriving (Eq)

data Match = No | Yes Type String [Match] String
  deriving (Eq)

type Rule = ( Type , [ [ Type ] ] )

rule1 = ( Atom "XML" ,
  [
    [ Str "<" , Var "type" , Atom "Param" , Str ">" , Atom "Inside" , Str "</" , Var "type" , Str ">" ],
    [ Str "<" , Var "type" , Str ">" , Atom "Inside" , Str "</" , Var "type" , Str ">" ],
    [ Str "<" , Var "type" , Str "/>" ]
  ] )

rule2 = ( Atom "Param" ,
  [
    [ Str " " , Var "param" , Str "=" , Str "value" , Atom "Param" ],
    [ ]
  ] )

rule3 = ( Atom "Inside" ,
  [
    [ Var "text" ] ,
    [ Atom "XML" , Atom "Inside" ],
    [ ]
  ] )

rules = [rule1,rule2,rule3]

go = matches "<g>Hello</g>" (Atom "XML")
-- go = filter okay [("<g>Hello</g>","")]
--   where okay (x,y) = canfitarule x (getruleforatom (Atom "XML") rules)

getruleforatom (Atom a) rs = if ((fst (head rs)) == (Atom a)) then
    snd (head rs)
  else
    getruleforatom (Atom a) (tail rs)

splitstr :: String -> [(String,String)]
splitstr "" = [("","")]
splitstr s:ss = [(s,ss)] ++ map addhead (splitstr ss)
  where addhead (a,b) = (s:a,b)

matches :: String -> Type -> [Match]

matches s (Var v) = map tosuccess (splitstr s)
  where tosuccess (a,b) = Yes (Var v) a [] b

--matches "" (Var v) = [Yes (Var v) "" [] ""]
--matches s (Var v) = [Yes (Var v) "" [] s] ++ map addhead (matches (tail s) (Var v))
--  where addhead (Yes (Var v) str ms left) = Yes (Var v) (head s:str) ms left

matches s (Str x) = if (take (length x) s)==x then
    [Yes (Str x) x [] (drop (length x) s)]
  else
    []

matches "" (Atom a) = map addme (matcheswithruleset (getruleforatom (Atom a)) ""
matches s (Atom a) =

matcheswithruleset :: Rule -> String -> [Match]
-- matcheswithruleset [] "" = [Yes (Var "final") "" [] ""]
matcheswithruleset [] s = []
matcheswithruleset (r:rs) s = (matcheswithrule r s) ++ (matcheswithruleset rs s)

matcheswithrule [] "" = [Yes (Var "end") "" [] ""]
matcheswithrule [] s = []
matcheswithrule (t:ts) s = filter succeeded (map test matchesfort)
  where trymatch = if (thematch
        matchesfort = matches t s
        test (No) = No
        test (Yes t a ms b) = if restmatch then
          Yes t a ms++rest left
        rest = matcheswithrule ts b
        restmatch = (length rest)>0
        succeeded (No) = False
        succeeded x = True

matches s (Atom a) = filter okay possmatches
  where possmatches = matches s (Var "dummy")
        okay (Yes (Var "dummy") x [] y) = canfitarule x (getruleforatom (Atom a) rules)

canfitarule s [] = False
canfitarule s (r:rs) = if (canfitrule s r) then
    trace ( ("Can fit "++s++" to "++show r++"\n")) True
  else
    trace ( ("Can't fit "++s++" to "++show r++"\n")) canfitarule s rs

canfitrule "" [] = True
canfitrule s [] = trace "Fail\n" False
canfitrule s (t:ts) = trace ("Trying to fit "++s++" to "++show t++" with "++show (matches s t)++"\n")
  canfitrulewith (ts) (matches s t)

canfitrulewith [] [] = True
--canfitrulewith [] ms = True
canfitrulewith ts [] = False
canfitrulewith ts (m:ms) = canfitrule (snd m) ts || canfitrulewith ts ms

instance Show Type where
  show (Atom a) = a
  show (Var v) = "<"++v++">"
  show (Str s) = "\""++s++"\""

void s = ""