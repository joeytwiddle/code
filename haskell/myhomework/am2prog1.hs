-- Extended Abstract Machine, a la Nielson and Nielson et le Paul Clark (PC5065)
-- Parts 1 and 2

type Var = String
type Number = Integer

-- Abstract Syntax 

data Inst = 	PUSH Number | ADD | MULT | SUB |
                TRUE | FALSE | EQL | LE | AND | NEG |
                FETCH Var | STORE Var | COPY |
                NOOP | BRANCH Code Code | LOOP Code Code  deriving Show

type Code =     [Inst]

data Aexpr = 	N Number | 
		V Var |
		Add Aexpr Aexpr |
		Mult Aexpr Aexpr |
		Sub Aexpr Aexpr

data Bexpr =	TT | 
		FF | 
		Eq Aexpr Aexpr |
		Le Aexpr Aexpr |
		Neg Bexpr |
		And Bexpr Bexpr

-- or and implies removed

data Stm =	Ass Var Aexpr | If Bexpr Stm Stm |
		While Bexpr Stm |
		Skip | Comp Stm Stm |
		For Var Aexpr Aexpr Stm

-- Semantic Functions

type Z = Integer
type T = Bool

type State = Var -> Z

-- Union type for Stack:  (Z Union T)*

data ZT = I Integer | B Bool  deriving Show

type Stack = [ZT]

type Config = (Code, Stack, State)


-- Transitions for OS of AM

t :: Config -> Config

t ([], e, s)                            = ([], e, s) 
t (((PUSH n) : c), e, s)                = t (c, ((I n) : e), s)
t ((ADD : c), ((I m) : (I n) : e), s)   = t (c, ((I (m+n)) : e), s)
t ((MULT : c), ((I m) : (I n) : e), s)  = t (c, ((I (m*n)) : e), s)
t ((SUB : c), ((I m) : (I n) : e), s)   = t (c, ((I (m-n)) : e), s)
t ((TRUE : c), e, s)                    = t (c, ((B True) : e), s)
t ((FALSE : c), e, s)                   = t (c, ((B False) : e), s)
t ((EQL : c), ((I m) : (I n) : e), s)   = if m == n
                                          then t (c, ((B True) : e), s)
                                          else t (c, ((B False) : e), s)
t ((LE : c), ((I m) : (I n) : e), s)    = if m <= n
                                          then t (c, ((B True) : e), s)
                                          else t (c, ((B False) : e), s)
t ((NEG : c), ((B True) : e), s)        = t (c, ((B False) : e), s)
t ((NEG : c), ((B False) : e), s)       = t (c, ((B True) : e), s)
t ((AND : c), ((B b1) : (B b2) : e), s) = if b1 == True && b2 == True
                                          then t (c, ((B True) : e), s)
                                          else t (c, ((B False) : e), s)
t (((FETCH x) : c), e, s)               = t (c, ((I (s x)) : e), s)
t (((STORE x) : c), ((I n) : e), s)     = t (c, e, (update s x n))
t ((NOOP : c), e, s)                    = t (c, e, s)

t (((BRANCH c1 c2) : c), ((B b) : e), s) =
    if  b == True  then t ((c1 ++ c), e, s) else t ((c2 ++ c), e, s)

t (((LOOP c1 c2) : c), e, s) =
    t ((c1 ++ [(BRANCH (c2 ++ [(LOOP c1 c2)]) [NOOP])] ++ c), e, s) 

t ((COPY : c), ((I n) : e), s) = (c, ((I n) : (I n) : e), s)

t ((COPY : c), ((B b) : e), s) = (c, ((B b) : (B b) : e), s)

-- for stuck derivation sequences :
t (c, e, s) = (c, e, s)  


-- Initial state function (all variables zeroed)

initState :: State

initState x = 0

-- State update function: s[x -> v]

update :: State -> Var -> Z -> State

update s x v y = if x == y then v else (s y)

-- shows the final configuration

showConfig :: Config -> IO()

showConfig (c, e, s) = putStr ("("++(show c)++", "++(show e)++", {"++st++ "})")
    where
         st = (concat (map f ["x", "y", "z"]))
         f x = x++" -> "++show (s x)++" "

-- top level function: takes initial code and state and returns final config

am_code :: Code -> State -> IO()

am_code c s = showConfig (t (c, [], s))

-- Mapping Aexpr -> Code

ca :: Aexpr -> Code

ca (N n)	= [PUSH n]
ca (V x)	= [FETCH x]
ca (Add a1 a2)	= ca a2 ++ ca a1 ++ [ADD]
ca (Mult a1 a2)	= ca a2 ++ ca a1 ++ [MULT]
ca (Sub a1 a2)	= ca a2 ++ ca a1 ++ [SUB]

-- Mapping Bexpr -> Code

cb :: Bexpr -> Code

cb (TT)		= [TRUE]
cb (FF)		= [FALSE]
cb (Eq a1 a2)	= ca a2 ++ ca a1 ++ [EQL]
cb (Le a1 a2)	= ca a2 ++ ca a1 ++ [LE]
cb (Neg b)	= cb b ++ [NEG]
cb (And b1 b2)	= cb b2 ++ cb b1 ++ [AND]

-- Mapping Stm -> Code

cs :: Stm -> Code

cs (Ass x a)		= ca a ++ [STORE x]
cs (Skip)		= [NOOP]
cs (Comp s1 s2)		= cs s1 ++ cs s2
cs (If b s1 s2)		= cb b ++ [BRANCH (cs s1) (cs s2)]
cs (While b s)		= [LOOP (cb b) (cs s)]
cs (For x a1 a2 s)	= cs (Ass x a1) ++ cs (If (Le (V x) a2) (Comp (s) (Comp (Ass x (Add (V x) (N 1))) (For x (V x) a2 s))) Skip)

-- tests

t1 :: IO()
t1 = am_code [FETCH "x", FETCH "y", ADD, STORE "z"] (update (update initState "x" 7) "y" 3)

t2 :: IO()
t2 = am_code (cs (Ass "z" (Add (V "x") (V "y")))) (update (update initState "x" 7) "y" 3)

f1 :: IO()
f1 = am_code (cs (If (Eq (V "x") (N 0)) (Ass "x" (N 5)) Skip)) (update initState "x" 0)
-- works as expected

f2 :: IO()
f2 = am_code (cs (While (Le (V "x") (N 5)) (Comp (Ass "x" (Add (V "x") (V "y"))) (Ass "y" (Sub (V "y") (N 1)))))) (update (update initState "x" 3) "y" 2)
-- works as expected

t3 :: IO()
t3 = am_code [FETCH "x", COPY] (update initState "x" 7)

t4 :: IO()
t4 = am_code (cs (For "x" (N 0) (N 5) (Ass "y" (Add (V "y") (N 2))))) (update initState "y" 0)

f3 :: IO()
f3 = am_code (cs (For "x" (N 0) (V "y") (Ass "z" (Add (Mult (V "y") (V "z")) (V "x"))))) (update (update initState "y" 3) "z" 4)
-- works as expected
