@echo off
rem
rem Swap 1.0
rem By Paul Clark (c)Jul 1992
rem
if "%1"=="/?" goto needhelp
if "%1"=="" goto needhelp
if "%2"=="" goto needhelp
if not exist %1 goto notexist
if not exist %2 goto notexist
rename %1 0.0
rename %2 %1
rename 0.0 %2
goto end

:notexist
echo File not found
goto end

:needhelp
echo.
echo Swaps two files around, using rename.
echo.
echo SWAP file1 file2
echo.
echo file1 and file2 are the names of the files to be swapped
echo.
goto end

:end