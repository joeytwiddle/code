@echo off
echo Choosing new WinAmp vis
del c:\Progra~1\Winamp\Plugins\plugin2.ini
dir c:\Progra~1\Winamp\Plugins\*.vws /b > c:\newvis.tmp
c:\tools\newvis2.exe
call c:\tools\newvis3.bat
del c:\newvis.tmp
del c:\tools\newvis3.bat
rem del c:\Progra~1\Winamp\Plugins\plugin2.ini
echo done.