@echo off
rem
rem Program Dir
rem By Paul Clark (C)Jan 1993
rem

echo.
echo Programs:
echo.

if "%1"=="" goto nopre
dir %1\*.com /b
dir %1\*.bat /b
dir %1\*.exe /b
goto okay

:nopre
dir *.com /b
dir *.bat /b
dir *.exe /b

:okay
echo.
