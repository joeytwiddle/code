@echo off
gxx current.c -o g.exe -lalleg %1 %2 %3 %4 %5
if ERRORLEVEL 1 goto exit
r
:exit