@echo off
rem
rem Document Dir
rem By Paul Clark (C)Jan 1993
rem

echo.
echo Documents:
echo.

if "%1"=="" goto nopre
dir %1\*.doc /b
dir %1\*.wri /b
dir %1\*.txt /b
goto okay

:nopre
dir *.doc /b
dir *.wri /b
dir *.txt /b

:okay
echo.