possible [] = [[]]
possible (h:t) = map addh (possible t) ++ possible t
  where addh xs = h:xs