@echo off
rem
rem Escape sequence generator for use with ANSI.SYS
rem By Paul Clark (C)Dec 1992
rem

:start
if "%1"=="" goto end

echo [%1
shift
goto start

:end