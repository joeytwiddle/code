@echo off
rem
rem Colour prompt with date and time window in upper right corner (high res)
rem By Paul Clark (C)Nov 1992
rem
rem If you like this prompt, please send me $1000 and you will receive the
rem latest copy of exactly the same thing.
rem

PROMPT $e[s$e[0;47m$e[1;111H$e[31m Date: $d $e[2;111H$e[34m  Time: $t   $e[0;37;40m$e[u$e[1;36m$p$g$e[1;37m

IF not "%1"=="/V" goto skip
echo.
echo Colour prompt with date and time window installed.
:skip