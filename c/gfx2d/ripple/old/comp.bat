@echo off
rem gcc current.c c:\joey\djgpp\contrib\grx23\lib\dj2\libgrx20.a -lm -g
gcc current.c c:\joey\code\djgpp\contrib\grx23\lib\dj2\libgrx20.a -lm -g -o go.exe %1 %2 %3 %4 %5 %6 %7 %8 %9
if not errorlevel 0 a>tmptype tmp
