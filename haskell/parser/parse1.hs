import Trace

data Type = Atom String | Var String | Str String
  deriving (Eq)

data Match = Fail | Succeed Type String [Match]
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

getruleforatom (Atom a) rs = if ((fst (head rs)) == (Atom a)) then
    head rs
  else
    getruleforatom (Atom a) (tail rs)

match :: String -> [Rule] -> Type -> Match
match s rs (Atom a) = trace ("match: \""++s++"\" to "++(show (Atom a))++"\n") (getmatch s rs (getruleforatom (Atom a) rs))

getmatch :: String -> [Rule] -> Rule -> Match
getmatch s rs (t,[]) = trace ("getmatch: X \""++s++"\" to "++show t++"\n") Fail
getmatch s rs (t,ir:irs) = if (thematch == Fail) then
    getmatch s rs (t,irs)
  else
    thematch
  where thematch = domatch s rs ir

void a = ""

domatch :: String -> [Rule] -> [Type] -> Match
domatch s rs [] = Fail
domatch s rs (a:cs) = trace ("domatch: "++(show a)++" couldbe "++(show (take 5 possmatches) ++ "\n")) ( matching possmatches rs cs )
  where possmatches = trace ("Possibilities for "++show a++" on "++s++" are "++show matchesrhere++"\n") take 50 matchesrhere
        matchesrhere = (matches a s rs)

matching :: [(Type,String,String)] -> [Rule] -> [Type] -> Match
matching [] rs cs = Fail
-- dodgy
matching ((t,sm,""):ms) rs [] = Succeed (Str "end") "" []
matching ((t,sm,sr):ms) rs cs = if (result /= Fail) then
    Succeed t sm [result]
  else
    matching ms rs cs
  where result = trace ("matching: \""++sr++"\" against "++show cs++"\n") domatch sr rs cs

matches :: Type -> String -> [Rule] -> [(Type,String,String)]
matches (Str x) s rs = if ((take (length x) s) == x) then
    [(Str x,take (length x) s,drop (length x) s)]
  else
    []
-- dodgy, used to be []
matches (Var v) "" rs = [] -- (Var v,"","")]
matches (Var v) s rs = (Var v,[head s],tail s) : rest
  where rest = map addhead (matches (Var v) (tail s) rs)
        addhead (x,y,z) = (x,(head s):y,z)
matches (Atom a) "" rs = []
matches (Atom a) s rs = rest ++ if (success) then
    [(Atom a,strm thematch,strr thematch)]
  else
    []
  where success = (thematch /= Fail)
        thematch = match s rs (Atom a)
        strm (Succeed (Atom a) sm ms) = sm
        strr (Succeed (Atom a) sm ms) = take (length sm) s
        rest = trace ("") realrest
        realrest = map (supplant (drop ((length s)-1) s)) (matches (Atom a) (take ((length s)-1) s) rs)
        supplant c (t,a,b) = (t,a,b++c)

go = showmatch (match "<greeting>Hello</greeting>" rules (Atom "XML"))

showmatch Fail = "Fail"
showmatch (Succeed t s ms) = "Succeed " ++ s

instance Show Type where
  show (Atom a) = a
  show (Var v) = "<"++v++">"
  show (Str s) = "\""++s++"\""