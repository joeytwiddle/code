module Hlib where

-- Joey's Haskell Library Module


-- String manipulation

sstrip 0 s = "..."
sstrip n (h:t) = h:sstrip (n-1) t
sstrip n "" = ""

sstartsboth :: String -> String -> (Bool,String)
sstartsboth x "" = (True,x)
sstartsboth "" s = (False,"")
sstartsboth (h:t) (sh:st) = if (h==sh && works) then
                              (True,rest)
                            else
                              (False,"")
  where (works,rest) = sstartsboth t st

sstarts x s = success
  where (success,str) = sstartsboth x s

sreplaceinorder :: [(String,String)] -> String -> String
sreplaceinorder [] s = s
sreplaceinorder ((s,r):rest) x = sreplaceinorder rest (sreplace x s r)

sreplace :: String -> String -> String -> String
sreplace "" s r = ""
sreplace x s r = if (outcome) then
                   r ++ sreplace rest s r
                 else
                   (h:sreplace t s r)
  where h = head x
        t = tail x
        (outcome,rest) = sstartsboth x s

sinstr [] s = -1
sinstr x s = if (sstarts x s) then
    0
  else
    if (next== -1) then
      next
    else
      next+1
  where next = sinstr (tail x) s


-- Files

--processfile :: String -> (String -> a) -> a
--processfile fname fn = fn (readFile fname)

