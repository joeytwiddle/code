@echo off
rem
rem Delete Directory
rem By Paul Clark (C)Jul 1993
rem
rem Displays a directory name and deletes all the files in it without prompt
rem If the second parameter is /RD, the directory will also be removed

echo Deleting %1's files
attrib %1\*.* -s -h -r
for %%f in (%1\*.*) do call delete %%f
rem if "%2"=="/RD" goto rmdir
rem if "%2"=="/rd" goto rmdir
rem goto end

:rmdir
echo Removing %1
rmdir %1

:end
