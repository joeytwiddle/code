@echo off
if "%1"=="" goto noargs
gxx %1.c -o %1.exe -lalleg -w %2 %3 %4 %5 -s -g
goto exit
:noargs
gxx current.c -o %1.exe -lalleg -w %2 %3 %4 %5 -fconserve-space -s
:exit
