import Trace

data Type = Atom String | Var String | Str String
  deriving (Eq)

data Match = No | Yes Type String [Match] String
  deriving (Eq)

type RuleSet = [[Type]]

type Rule = ( Type , RuleSet )

rule1 = ( Atom "XML" ,
  [
    [ Str "<" , Var "type" , Atom "Params" , Str ">" , Atom "Inside" , Str "</" , Var "type" , Str ">" ],
    [ Str "<" , Var "type" , Str ">" , Atom "Inside" , Str "</" , Var "type" , Str ">" ],
    [ Str "<" , Var "type" , Str "/>" ]
  ] )

rule2 = ( Atom "Params" ,
  [
    [ Atom "Param", Atom "Params" ],
    [ Atom "Param" ]
  ] )

rule4 = ( Atom "Param" ,
  [
    [ Str " " , Var "param" , Str "=" , Var "value" ]
  ] )

rule3 = ( Atom "Inside" ,
  [
    [ Var "text" ] ,
    [ Atom "XML" , Atom "Inside" ],
    [ ]
  ] )

rules = [rule1,rule2,rule3,rule4]

go = head (matches "<g a=b>Hi!</g>" (Atom "XML"))
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
splitstr "" = [("","")]
splitstr (s:ss) = [([s],ss)] ++ (map addhead (splitstr ss))
  where addhead (a,b) = (s:a,b)
--        s = head sss
--        ss = tail ss

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

matches s (Atom a) = if ((length trymatch)>0) then
    [head trymatch]
  else
    []
  where trymatch = matcheswithruleset (getrulesetforatom (Atom a) rules) s

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

matcheswithruleset :: RuleSet -> String -> [Match]
-- matcheswithruleset [] "" = [Yes (Var "final") "" [] ""]
matcheswithruleset [] s = []
matcheswithruleset (r:rs) s = (matcheswithrule r s) ++ (matcheswithruleset rs s)

matcheswithrule :: [Type] -> String -> [Match]
matcheswithrule [] "" = [Yes (Var "end") "" [] ""]
matcheswithrule [] s = []

matcheswithrule (t:ts) s = filter okay (map tryposs poss)
  where poss = splitstr s
        tryposs (x,y) = if (not (null (matches x t))) && (not (null (matcheswithrule ts y))) then
                          addto (head (matches x t)) [head (matcheswithrule ts y)]
                        else
                          No
        okay No = False
        okay x = True
        addto (Yes tt ss ms rs) mms = Yes tt ss (ms++mms) rs
--                          Yes t x ([head (matches x t)] ++ [head (matcheswithrule ts y)]) ""

--matcheswithrule (t:ts) s = trace ("Trying "++show t++" on \""++s++"\"\n")
--  filter succeeded (map test matchesfort)
--  where matchesfort = trace ("Matches for "++show t++" on \""++s++"\" are "++show (matches s t)++"\n")
--          matches s t
--        test (No) = No
--        test (Yes t a ms b) = if restmatch b then
--            Yes t a (ms++rest b) ""
--          else
--            No
--        rest b = matcheswithrule ts b
--        restmatch b = (length (rest b))>0
--        succeeded (No) = False
--        succeeded x = True

instance Show Type where
  show (Atom a) = a
  show (Var v) = "<"++v++">"
  show (Str s) = "\""++s++"\""

instance Show Match where
  show No = "Failure"
  show (Yes t s ms r) = (show t)++" matches \""++s++"\" with "++(show ms)++" and \""++r++"\" left"

void s = ""