@echo off
if "%1"=="" goto noargs
gpp -fconserve-space -s -O3 %1.c -o %1.exe %JPATH%\code\c\joeylib\.comp\joeylib.o -lalleg -I %JPATH%\code\c\joeylib\.comp
REM This stuff just don't fit on the DOS line!
rem -w   -fconserve-space -s -ggdb %2 %3 %4 %5
if ERRORLEVEL 1 goto exit
@echo on
@rem r
call .\%1.exe
@goto exit
:noargs
@call oldx
:exit
