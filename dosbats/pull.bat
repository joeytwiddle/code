@echo off
rem
rem Pull path
rem By Paul Clark (C)Dec 1992
rem
rem The reason for redirecting the output to TEMP.PNC is simply to prevent the
rem files being copied from being displayed.
rem

copy %tools%\cd.pnc + %temp%\path.pnc %temp%\cd.bat > %temp%\temp.pnc
del %temp%\temp.pnc
%temp%\cd.bat
del %temp%\cd.bat