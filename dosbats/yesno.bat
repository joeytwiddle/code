@echo off
rem
rem Yes / No reply finder
rem By Paul Clark (C)Dec 1992
rem
rem Other files : Must have KEYIN.COM to run
rem Input  : The thing to be questioned must be placed in in
rem Output : The reply ( Y or N ) will be placed in out
rem

echo.
echo [33mDo you want %in% (Yes/No)?[37m

:loop

call \tools\keyin.com

if errorlevel 89 if not errorlevel 90 goto yes
if errorlevel 78 if not errorlevel 79 goto no

goto loop

:yes
echo [32mYes[37m
set out="Y"
goto end

:no
echo [31mNo[37m
set out="N"
goto end

:end
echo.