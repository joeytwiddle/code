showDots = putStr res
  where res = map showDot years
        showDot x = if (x=="2002") then '*' else if (isPallendrome x) then '.' else ' '

years = map show [0..2400]

findPallendromes :: [String] -> [String]
findPallendromes ps = filter isPallendrome ps

isPallendrome :: String -> Bool
isPallendrome [] = True
isPallendrome [c] = True
isPallendrome (c:cs) = (c == last cs) && isPallendrome (init cs)
