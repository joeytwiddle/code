import IOExtensions (getCh)

type World = (Int,[Int],[Int])
-- current player, list of beans on board, list of beans held by players



go = play nw

iscomputer (2,bs,pbs) = True
iscomputer (n,bs,pbs) = False

-- How many moves into the future should the computer look
searchdepth :: Int
searchdepth = 2

-- Pessimistic: computer assumes opponents will play perfectly (~searchdepth)
pessimistic = True
-- Otherwise, computer will assume opponents will make all moves with equal likelihood

play :: World -> IO()
play w =
  putStrLn (displayboardforplayer w) >>
  if (endofgame w) then
    putStrLn ("Game over.  You are left holding "++show(pbs)++".")
  else if (cantmove) then
    putStrLn ("Sorry, you can't move!") >>
    play (1 + mod (player+1) (length pbs),bs,pbs)
  else if (iscomputer w) then
    putStrLn "Computer is thinking..." >>
    putStrLn (foldl (++) "" (map disp owned)) >>
    putStrLn ("Chosen move "++show (best)++".") >>
    play (domove w best 0)
  else
    getCh >>= \key ->
    putStrLn ("You chose to move pos "++show(moveforkey w key) ++ " (" ++ show (bs!!((moveforkey w key)-1)) ++ " beans)") >>
    if (key=='q' || key=='Q') then
      putStrLn "Quit" >> return ()
    else if (islegal w (moveforkey w key)) then
      play (domoveforkey w key)
    else
      (putStrLn "Not a legal move" >> play w)
  where (player,bs,pbs) = w
        cantmove = not ( foldl (||) False (map (islegal w) (whatdoiown bs pbs player)) )
        disp pos = (show pos) ++ " scores " ++ (show (scoreformove w player searchdepth pos)) ++ ","
        best = bestmove w searchdepth
        owned = whatdoiown bs pbs player



-- dobestmove (player,bs,pbs) = domove (player,bs,pbs) (bestmove (player,bs,pbs)) 0

bestmove (player,bs,pbs) depth = pickbest (whatdoiown bs pbs player) (scoreformove (player,bs,pbs) player depth)
--  where bestpos = (whatdoiown bs pbs player)!!0

pickbestsofar [] scorefn (be,bs) = be
pickbestsofar (h:t) scorefn (be,bs) =
  if (ns>bs) then
    pickbestsofar t scorefn (h,ns)
  else
    pickbestsofar t scorefn (be,bs)
  where ns = scorefn h

pickbest :: Ord b => [a] -> (a->b) -> a
pickbest (h:t) scorefn = pickbestsofar t scorefn (h,scorefn h)

scoreformove :: World -> Int -> Int -> Int -> Int
scoreformove (player,bs,pbs) realplayer depth pos =
  if (islegal (player,bs,pbs) pos) then
    scoreforstate (domove (player,bs,pbs) pos 0) realplayer depth
  else
    (-1)

scoreforstate :: World -> Int -> Int -> Int
scoreforstate w realplayer depth =
  if (endofgame w || depth==0) then
    (pbs!!(realplayer-1)) - (spacesperplayer bs pbs)
  else
    if (player==realplayer) then
      scoreformove w realplayer (depth-1) (bestmove w (depth-1))
    else
      if (pessimistic) then
        mymin results
      else
        if ((length results)==0) then
          0
        else
          div (sum results) (length results)
  where (player,bs,pbs) = w
        legalmoves = filter (islegal w) (whatdoiown bs pbs player)
        results = map (scoreformove w realplayer (depth-1)) legalmoves

mymin [a] = a
mymin (a:b) = if (a < m) then a else m
  where m = mymin b

domoveforkey w key = domove w pos 0
  where pos = moveforkey w key

moveforkey (player,bs,pbs) key = (whatdoiown bs pbs player)!!((read [key]) - 1)

newboard 0 = []
newboard size = (toInt 4:newboard (size-1))

newpbs 0 = []
newpbs ps = (toInt 0:newpbs (ps-1))

newworld players size = (toInt 1,newboard size,newpbs players)

nw = newworld 2 16

chop ls 0 = []
chop (h:t) n = h:(chop t (n-1))

sublist (h:t) start end =
  if (start==0) then
    chop (h:t) (end+1)
  else
    sublist t (start-1) (end-1)

line [] w = ""
line (h:t) w =
  if (isinlist h owned) then
    show (keyfor w h) ++ "-" ++ line t w
  else
    "--" ++ line t w
  where (player,bs,pbs) = w
        owned = whatdoiown bs pbs player

indexof x (h:t) = if (x==h) then 0 else 1 + indexof x t

keyfor (player,bs,pbs) pos = 1 +
  indexof pos (whatdoiown bs pbs player)

displayboardforplayer (player,bs,pbs) =
     "+" ++ line (reverse [(half+1)..full]) (player,bs,pbs) ++ "+\n"
  ++ displayboard bs ++ "\n"
  ++ "+" ++ line [1..half] (player,bs,pbs)
  where half = div full 2
        full = length bs
        owned = whatdoiown bs pbs player

displayboard :: [Int] -> String
displayboard bs =
     show (reverse (sublist bs half (full-1))) ++ "\n"
  ++ show (sublist bs 0 (half-1))
  where half = div full 2
        full = length bs

islegal :: World -> Int -> Bool
islegal (player,bs,pbs) pos =
     ( (playerowning bs pbs pos) == player )
  && ( i>0 )
  where i = bs!!(pos-1)

endofgame (player,bs,pbs) =  ((sum bs) <= 4)

domove :: World -> Int -> Int -> World
domove (player,bs,pbs) pos held =
  if (held==1 && i==0) then             -- end of turn
    (nextplayer,addtoboard bs pos 1,pbs)
  else if (held==1 && i==3) then        -- end of turn, steal 4 beans by landing on 3
    (nextplayer,removefromboard bs pos 3,addtoplayer pbs player 4)
  else if (held==1 && i>0 && i/=4) then -- drop last and pick up all
    domove (player,removefromboard bs pos i,pbs) nextpos (held+i)
  else if (held==0) then                -- start of players move, pick up all
    domove (player,removefromboard bs pos i,pbs) nextpos (held+i)
  else if (i==3) then                   -- completing a four for owner to take
    domove (player,removefromboard bs pos 3,addtoplayer pbs owner 4) nextpos (held-1)
  else                                  -- just drop a bean and move on
    domove (player,addtoboard bs pos 1,pbs) nextpos (held-1)
  where i = bs!!(pos-1)
        nextplayer = 1 + (mod player (length pbs))
        nextpos = 1 + (mod pos (length bs))
        owner = playerowning bs pbs pos
--        (bs,pbs) = (uncheckedbs,uncheckedpbs)
--        (bs,pbs) = check uncheckedbs uncheckedpbs 1

addtolist :: [Int] -> Int -> Int -> [Int]
addtolist (h:t) 1 x = (h+x:t)
addtolist (h:t) pos x = (h:addtoboard t (pos-1) x)

addtoboard bs pos n = addtolist bs (pos) n
removefromboard bs pos n = addtolist bs (pos) (-n)
addtoplayer pbs player n = addtolist pbs (player) n

-- check bs pbs pos =
--   if atend then
--     (bs,pbs)
--   else if (i == 4) then
--     check newbs newpbs (pos+1)
--   else
--     check bs pbs (pos+1)
--   where i = bs!!(pos-1)
--         atend = (pos == ((length bs) + 1))
--         newbs = removefromboard bs pos 4
--         newpbs = addtoplayer pbs (playerowning bs pbs pos) 4

-- pickup4s (player,bs,pbs) = (player,newbs,newpbs)
--   where (newbs,newpbs) = check bs pbs 1

whatdoiown :: [Int] -> [Int] -> Int -> [Int]
whatdoiown bs pbs player = [startpos..endpos]
  where
    beansperplayer = spacesperplayer bs pbs
    startpos = 1 + (player - 1) * beansperplayer
    endpos = startpos + beansperplayer - 1

spacesperplayer bs pbs = div (length bs) (length pbs)

isinlist e [] = False
isinlist e (h:t) = if (e==h) then True else isinlist e t

findsatisfier quiz (h:t) = if (quiz h) then h else findsatisfier quiz t

playerowning bs pbs pos = findsatisfier owns [1..(length pbs)]
  where owns player = isinlist pos (whatdoiown bs pbs player)