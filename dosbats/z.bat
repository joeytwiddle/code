x %1 %2 %3 %4 %5 %6 %7 %8 %9 -O3
'@echo off
'gxx current.c -o g.exe -w -lalleg %1 %2 %3 %4 %5 -O3 -fconserve-space -s
'if ERRORLEVEL 1 goto exit
'r
':exit
