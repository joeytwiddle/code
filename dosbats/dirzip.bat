@echo off
rem
rem Zip Dir
rem By Paul Clark (C)Jan 1993
rem

echo.
echo Zips:
echo.

if "%1"=="" goto nopre
dir %1\*.zip /b
dir %1\*.tar /b
dir %1\*.gz /b
goto okay

:nopre
dir *.zip /b
dir *.tar /b
dir *.gz /b

:okay
echo.
