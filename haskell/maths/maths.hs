data Expr = V Char | A Char | E (Con,[Expr])
type Con = String

c1 = E ("equals",[E ("plus",[A 'A',A 'B']),A 'C'])
r1 = E ("equals",[A 'B',E ("minus",[A 'C',A 'A'])])
--rule1 = E ("implies",[c1,r1])
rule1 = (c1,r1)
rule2 = (c2,r2)
rule3 = (c3,r3)
rule4 = (c4,r4)
c2 = E ("equals",[E ("square",[A 'A']),A 'B'])
r2 = E ("equals",[A 'A',E ("sqrt",[A 'B'])])
--rule2 = E ("implies",[c2,r2])
c3 = E ("equals",[E ("plus",[A 'A',A 'B']),A 'C'])
r3 = E ("equals",[A 'A',E ("minus",[A 'C',A 'B'])])
--rule3 = E ("implies",[c3,r3])
c4 = E ("equals",[A 'A',A 'B'])
r4 = E ("equals",[A 'B',A 'A'])
--rule4 = E ("implies",[c4,r4])

rules=[rule1,rule2,rule3,rule4]

sitwant = E ("equals",[V 'x',A 'F'])
currentsit = E ("equals",[E ("plus",[E ("square",[V 'x']),
		E ("square",[V 'y'])]),
		E ("square",[V 'z'])])

go = disprs rules >>
	p "Current expression: " >> putStr(tostringe currentsit) >> nl >>
	p "Want expression of form: " >> putStr(tostringe sitwant) >> nl >>
	p "Result: " >>
	putStr(forall solns tostringe (++) [])
	where solns = find sitwant currentsit rules 4

-- find takes situationwanted, currentsituation, setofrules
-- and by attempting rules, it converts currentsituation into
-- a situation which fits situationwanted
-- will search to a depth n

find sw cs rules 0 = []
find sw cs rules n = if cancon sw cs then
			[cs]
		else
			forall (possrules rules cs) f (++) []
		where f r = find sw (apply r cs) rules (n-1)

-- A kind of for next loop
-- list is list of things to loop through
-- f is function to perform on each thing
-- c is the function which combines the results of f
-- e is the thing to combine to when the end of the list is reached
forall :: [a] -> (a->b) -> (b->b->b) -> b -> b
forall [] f c e = e
forall (x:xs) f c e = c (f x) (forall xs f c e)

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
apply (c,r) s = recon r (fit c s)

-- display expression
tostringe :: Expr -> String
tostringe (V c) = [c]
tostringe (A c) = [c]
tostringe (E (e,[a])) = e++"("++(tostringe a)++")"
tostringe (E (e,[a1,a2])) = (tostringe a1)++" "++(tostringc e)++" "++(tostringe a2)
--tostringe (E (e,as)) = e++"("++(tostringa as)++")"

tostringc "equals" = "="
tostringc "plus" = "+"
tostringc "minus" = "-"

tostringa [] = []
tostringa [a] = tostringe a
tostringa (a:as) = (tostringe a)++","++(tostringa as)

dispe a = return ()

-- display a list of expressions
dispes [] = return ()
dispes (e:es) = dispe e >> nl >> dispes es

-- display known rules
disprs rules = p "Rules:" >> nl >>
	disprs2 rules

disprs2 [] = return ()
disprs2 (r:rs) = dispr r >> disprs2 rs

dispr (e1,e2) = putStr(tostringe e1) >> putStr(" -> ") >>
		putStr(tostringe e2) >> nl
--dispr (a,b) = dispe a >> putStr(" -> ") >> dispe b >> nl
--dispr (a,b) = dispe a >> putStr(" -> ") >> dispe b >> nl

-- display fit
dispf :: [(Char,Expr)] -> IO()
dispf [] = return ()
dispf ((c,e):rest) = putStr("(") >> putChar(c) >> putStr(",")
			>> dispe e >> putStr("),") >> dispf rest

-- find a fit, a set of appropriate mappings from the variables
-- and any's in wanted to subtrees of the second expression
fit :: Expr -> Expr -> [(Char,Expr)]
fit (A c1) e = [(c1,e)]
fit (V c1) (V c2) = if (c1/=c2) then [] else [(c1,V c2)]
fit (E (s1,as1)) (E (s2,as2)) = if (s1/=s2 || length as1/=length as2) then
				[]
			else
	forall [0..(length as1)-1] f (++) []
	where f n = fit (as1!!n) (as2!!n)
fit a b = []

-- Reconstruct an expression, using a fit mapping
recon :: Expr -> [(Char,Expr)] -> Expr
recon (E (s,as)) f = E (s,forall as r (++) [])
		where r e = [recon e f]
recon (V c) f = accomp c f
recon (A c) f = accomp c f

accomp :: Char -> [(Char,Expr)] -> Expr
accomp a [] = V a
accomp a ((b,c):rest) = if (b==a) then
				c
			else
				(accomp (a) (rest))
