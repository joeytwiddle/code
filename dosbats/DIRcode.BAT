@echo off
rem
rem Code Dir
rem By Paul Clark (C)Feb 1999
rem

echo.
echo Code:
echo.

if "%1"=="" goto nopre
dir %1\*.jav /b
dir %1\*.bas /b
dir %1\*.c /b
goto okay

:nopre
dir *.jav /b
dir *.bas /b
dir *.c /b

:okay
echo.