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
c3 = Equals (Plus (Any 'A') (Any 'B')) (Any 'C')
r3 = Equals (Any 'A') (Minus (Any 'C') (Any 'B'))
rule3 = (c3,r3)
c4 = Equals (Any 'A') (Any 'B')
r4 = Equals (Any 'B') (Any 'A')
rule4 = (c4,r4)

rules=[rule1,rule2,rule3,rule4]

sitwant :: Expr
sitwant = Equals (Var 'x') (Any 'F')
currentsit :: Expr
currentsit = Equals (Plus (Square (Var 'x')) (Square (Var 'y'))) (Square
		(Var 'z'))

go = disprs rules >>
	p "Current expression: " >> dispe currentsit >> nl >>
	p "Want expression of form: " >> dispe sitwant >> nl >>
	p "Result: " >>
	find sitwant currentsit rules 4

-- find takes situationwanted, currentsituation, setofrules
-- and by attempting rules, it converts currentsituation into
-- a situation which fits situationwanted
-- will search to a depth n

find :: Expr -> Expr -> [Rule] -> Int -> IO()
find sw cs rules 0 = return ()
find sw cs rules n = if cancon sw cs then
			p "Solution: " >> dispe cs >> nl
		else
			forall (possrules rules cs) f (>>) (return ())
		where f r = dispe cs >> p " " >> dispr r >> p " " >> dispe (apply r cs) >> nl >> find sw (apply r cs) rules (n-1)

-- A kind of for next loop
-- list is list of things to loop through
-- f is function to perform on each thing
-- c is the function which combines the results of f
-- e is the thing to combine to when the end of the list is reached
forall :: [a] -> (a->b) -> (b->b->b) -> b -> b
forall [] f c e = e
forall (x:xs) f c e = c (f x) (forall xs f c e)

--find sw (apply (bestrule rules cs) cs) rules

-- can expression fit into expression wanted?
cancon want exp = length (fit want exp)==length (fit want want)

-- find first rule which we can use
possrules [] c = []
possrules (r:rs) c = if (cancon (first r) c) then
					(r:possrules rs c)
				else
					possrules rs c

first (a,b) = a
p s = putStr(s)
nl = putStr("\n")

-- apply a rule to an expression
apply :: Rule -> Expr -> Expr
apply (c,r) s = recon r (fit c s)

-- display expression
dispe :: Expr -> IO()
dispe (Var a) = putChar(a)
dispe (Any a) = putChar(a)
dispe (Plus a b) = dispe a >> putChar('+') >> dispe b
dispe (Minus a b) = dispe a >> putChar('-') >> dispe b
dispe (Equals a b) = dispe a >> putChar('=') >> dispe b
dispe (Square a) = dispe a >> putStr("^2")
dispe (Square a) = p "(" >> dispe a >> putStr("^2") >> p ")"
dispe (Sqrt a) = putStr("sqrt(") >> dispe a >> putStr(")")

-- display a list of expressions
dispes [] = return ()
dispes (e:es) = dispe e >> nl >> dispes es

-- display known rules
disprs rules = p "Rules:" >> nl >>
	disprs2 rules

disprs2 [] = return ()
disprs2 (r:rs) = dispr r >> nl >> disprs2 rs

dispr :: Rule -> IO()
dispr (a,b) = dispe a >> putStr(" -> ") >> dispe b
--dispr (a,b) = dispe a >> putStr(" -> ") >> dispe b >> nl

-- display fit
dispf :: [(Char,Expr)] -> IO()
dispf [] = return ()
dispf ((c,e):rest) = putStr("(") >> putChar(c) >> putStr(",")
			>> dispe e >> putStr("),") >> dispf rest

-- find a fit, a set of appropriate mappings from the variables
-- and any's in wanted to subtrees of the second expression
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

-- Reconstruct an expression, using a fit mapping
recon :: Expr -> [(Char,Expr)] -> Expr
recon (Equals a b) f = Equals (recon a f) (recon b f)
recon (Plus a b) f = Plus (recon a f) (recon b f)
recon (Minus a b) f = Minus (recon a f) (recon b f)
recon (Square a) f = Square (recon a f)
recon (Sqrt a) f = Sqrt (recon a f)
recon (Var a) f = accomp a f
recon (Any a) f = accomp a f

accomp :: Char -> [(Char,Expr)] -> Expr
accomp a [] = Var a
accomp a ((b,c):rest) = if (b==a) then
				c
			else
				(accomp (a) (rest))
