#!/usr/local/bin/runhugs +l

> module Main where
> import Hlib
> import System
> main = do
>   args <- getArgs
>   go args
>
> go args = do
>           file <- readFile (head args)
>           putStrLn (treelist file)
> 
> treelist file = treebreak (lines (file)) 0
>
> treebreak lines = Branch "" ( break [] lines (head (head lines)) )
>
> break sofar h:t c
> | head h == c    = break sofar++[tail h] t c
> | otherwise      = [ Branch [c] (treebreak sofar) ] ++ break [] t (head (head t))

