@echo off
echo Choosing new wallpaper...
dir c:\joey\graphics\pictures\wallpapers\*.bmp /b /ogn > c:\newback.tmp
c:\tools\newback2.exe
call c:\tools\newback3.bat
del c:\newback.tmp
del c:\tools\newback3.bat
echo done.