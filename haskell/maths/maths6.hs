data Expr = Var Char | Any Char | Plus Expr Expr | Minus Expr Expr
		| Equals Expr Expr
		| Square Expr
		| Sqrt Expr

type Rule = (Expr,Expr)

c1 = Equals (Plus (Any 'A') (Any 'B')) (Any 'C')
r1 = Equals (Any 'B') (Minus (Any 'C') (Any 'A'))
rule1 = (c1,r1)
c2 = Equals (Square (Any 'A')) (Any 'B')
r2 = Equals (Any 'A') (Sqrt (Any 'B'))
rule2 = (c2,r2)

sitwant :: Expr
sitwant = Equals (Var 'y') (Any 'F')

-- find takes situationwanted, currentsituation, setofrules
find :: Expr -> Expr -> [Rule] -> Expr
find w c rules = if cancon w c then
			c
		else
			find w (apply (bestrule rules c) c) rules

cancon want exp = length (fit want exp)==length (fit want want)

bestrule (r:rs) c = if (cancon (first r) c) then
					r
				else
					bestrule rs c

first (a,b) = a

p s = putStr(s)

cdash=apply rule1 current
cddash=apply rule2 cdash

sit1 = Equals (Plus (Square (Var 'x')) (Square (Var 'y'))) (Square
		(Var 'z'))

current = sit1
wanted = sitwant

go = dispe (find sitwant sit1 rules)

rules=[rule1,rule2]

go2 = dispf (fit sitwant sit3)

go1 = dispe sit1 >> nl >> dispe sit2 >> nl >> dispe sit3
sit2 = apply rule1 sit1
sit3 = apply rule2 sit2
nl = putStr("\n")

rule3 = (sit1,sit3)

apply :: Rule -> Expr -> Expr
apply (c,r) s = recon r (fit c s)

-- Display Expr
dispe :: Expr -> IO()
dispe (Var a) = putChar(a)
dispe (Any a) = putChar(a)
dispe (Plus a b) = dispe a >> putChar('+') >> dispe b
dispe (Minus a b) = dispe a >> putChar('-') >> dispe b
dispe (Equals a b) = dispe a >> putChar('=') >> dispe b >> putChar(' ')
dispe (Square a) = dispe a >> putStr("^2")
dispe (Sqrt a) = putStr("sqrt(") >> dispe a >> putStr(")")

-- Display Rules


-- Display Rule
dispr :: Rule -> IO()
dispr (a,b) = dispe a >> putStr(" -> ") >> dispe b >> nl

-- Display fit
dispf :: [(Char,Expr)] -> IO()
dispf [] = return ()
dispf ((c,e):rest) = putStr("(") >> putChar(c) >> putStr(",")
			>> dispe e >> putStr("),") >> dispf rest

fit :: Expr -> Expr -> [(Char,Expr)]
fit (Equals a b) (Equals c d) = (fit a c)++(fit b d)
fit (Plus a b) (Plus c d) = (fit a c)++(fit b d)
fit (Minus a b) (Minus c d) = (fit a c)++(fit b d)
fit (Square a) (Square b) = fit a b
fit (Sqrt a) (Sqrt b) = fit a b
fit (Var a) (Var b) = if (a==b) then
			[(a,Var b)]
		else
			[]
fit (Any a) b = [(a,b)]
fit a b = []

recon :: Expr -> [(Char,Expr)] -> Expr
recon (Equals a b) f = Equals (recon a f) (recon b f)
recon (Plus a b) f = Plus (recon a f) (recon b f)
recon (Minus a b) f = Minus (recon a f) (recon b f)
recon (Square a) f = Square (recon a f)
recon (Sqrt a) f = Sqrt (recon a f)
recon (Var a) f = accomp a f
recon (Any a) f = accomp a f

accomp :: Char -> [(Char,Expr)] -> Expr
-- ??
--accomp a [] = Var a
-- ??
accomp a ((b,c):rest) = if (b==a) then
				c
			else
				(accomp (a) (rest))
