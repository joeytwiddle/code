@echo off
gxx current.c -o g.exe -lalleg -w %1 %2 %3 %4 %5 -fconserve-space -s -g
if ERRORLEVEL 1 goto exit
@echo on
r
:exit
