@echo off
rem
rem Picture Dir
rem By Paul Clark (C)Jan 1993
rem

echo.
echo Pictures:
echo.

if "%1"=="" goto nopre
dir %1\*.bmp /b
dir %1\*.pcx /b
dir %1\*.gif /b
dir %1\*.jpg /b
goto okay

:nopre
dir *.bmp /b
dir *.pcx /b
dir *.gif /b
dir *.jpg /b

:okay
echo.