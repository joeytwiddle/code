@echo off
rem
rem Program Dir
rem By Paul Clark (C)Jan 1993
rem

echo.
echo Programs:
echo.

if "%1"=="" goto nopre
dir %1\*.exe /b
dir %1\*.com /b
dir %1\*.bat /b
goto okay

:nopre
dir *.exe /b
dir *.com /b
dir *.bat /b

:okay
echo.