@echo off
rem
rem Directory Dir
rem By Paul Clark (C)Jan 1993
rem

echo.
echo Directories:
echo.

if "%1"=="" goto nopre
dir %1\*. /b
goto okay

:nopre
dir *. /b

:okay
echo.