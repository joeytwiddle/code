@echo off
echo.
echo Newback
dir d:\wallpapers\*.jpg /b /ogn /s > c:\newback.tmp
c:\tools\newbackb.exe
call c:\tools\newbackc.bat
rem del c:\newback.tmp
rem del c:\tools\newbackc.bat
