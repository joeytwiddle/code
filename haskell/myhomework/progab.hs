-- Semantic Functions For Arithmetic and Boolean Expressions
-- and for some statements (referred to here as commands).
-- Adapted from Nielson and Nielson, Appendix B
-- and appended to by Paul Clark (pc5065).

type Var = String
type Number = Integer

-- Abstract Syntax 

-- a ::= n | v | a1+a2 | a1*a2 | a1-a2
-- b ::= TRUE | FALSE | a1=a2 | a1 <= a2 | ~b | b1 /\ b2 | b1 \/ b2 | b1 -> b2
-- c ::= x|->n | if b then c1 else c2 | while b do c | skip | c1;c2

data Aexpr = 	N Number | 
		V Var |
		Add Aexpr Aexpr |
		Mult Aexpr Aexpr |
		Sub Aexpr Aexpr


data Bexpr =	TRUE | 
		FALSE | 
		Eq Aexpr Aexpr |
		Le Aexpr Aexpr |
		Neg Bexpr |
		And Bexpr Bexpr |
		Or Bexpr Bexpr |
		Imp Bexpr Bexpr

data Command =	Ass Var Aexpr | If Bexpr Command Command |
		While Bexpr Command |
		Skip | Comp Command Command

-- Semantic Functions

type Z = Integer
type T = Bool

type State = Var -> Z

-- Semantic function for Aexpr

a :: Aexpr -> State -> Z

a (N n) s 	= n
a (V n) s 	= s n
a (Add x y) s 	= (a x s) + (a y s)
a (Mult x y) s 	= (a x s) * (a y s)
a (Sub x y) s 	= (a x s) - (a y s)

-- Semantic function for Bexpr

b :: Bexpr -> State -> Bool

b (TRUE) s	= True
b (FALSE) s	= False
b (Eq a1 a2) s	= if (a a1 s) == (a a2 s) then True else False
b (Le a1 a2) s	= if (a a1 s) <= (a a2 s) then True else False
b (Neg b1) s	= if (b b1 s) == True then False else True
b (And b1 b2) s	= if (b b1 s) && (b b2 s) then True else False
b (Or b1 b2) s	= if (b b1 s) || (b b2 s) then True else False
b (Imp b1 b2) s	= if (b b1 s) && (b (Neg b2) s) then False else True

-- Semantic function for Command

t :: Command -> State -> State

t (Ass v a1) s		= update s v (a a1 s)
t (If b1 c1 c2) s	= if (b b1 s) then (t c1 s) else (t c2 s)
t (While b1 c1) s	= if (b b1 s) then (t (While b1 c1) (t c1 s)) else (t Skip s)
t Skip s		= s
t (Comp c1 c2) s	= (t c2 (t c1 s))

-- Initial state function (all variables zeroed)

initState :: State

initState x = 0

-- State update function: s[x -> v]

update :: State -> Var -> Z -> State

update s x v y = if x==y then v else (s y)

-- Test 1 (part 1 test 1)

f0 :: Bool

f0 = b (Eq (Add (V "x") (V "y")) (Sub (V "z") (N 1))) (update (update (update initState "z" 7) "y" 3) "x" 2)

-- Test 2 (part 1 test 2)

f1 :: Bool

f1 = b (And (TRUE) (Le (V "x") (N 1))) (update initState "x" 2)

-- Test 3 (part 1 test 3)

f2 :: Bool

f2 = b (Or (TRUE) (Le (V "x") (N 1))) (update initState "x" 2)

-- Test 4 (checks to see if ass works, displays final value of x)

f3 :: Z

f3 = a (V "x") f3a

f3a :: State

f3a = t (Ass ("x") (N 1)) (update initState "x" 2)

-- Test 5 (part 2 test 1 - displays final values of x and y)

f4 :: (Z,Z)

f4 = (a (V "x") f4a, a (V "y") f4a)

f4a :: State

f4a = t (Comp (Ass "x" (N 5)) (Ass "y" (V "x"))) t0

t0 :: State

t0 = (update (update initState "y" 1) "x" 3)

-- Test 6 (part 2 test 2 - displays final value of x)

f5 :: Z

f5 = (a (V "x") f5a)

f5a :: State

f5a = t (While (Le (V "x") (N 8)) (Ass "x" (Add (V "x") (V "y")))) t0

-- (thanks for your help!)