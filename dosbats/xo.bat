@echo off
gxx current.c -o g.exe -lalleg -w %1 %2 %3 %4 %5 -O3
if ERRORLEVEL 1 goto exit
r
:exit