@echo off

rem
rem Make List
rem By Paul Clark (C)Aug 1993
rem

copy con %temp%\temp.bat > %temp%\temp.tmp
del %temp%\temp.tmp
echo Executing commands
call %temp%\temp.bat
del %temp%\temp.bat
echo Done