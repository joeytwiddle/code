@echo off
gxx current.c -o g.exe -w -lalleg %1 %2 %3 %4 %5 -O3 -fconserve-space
