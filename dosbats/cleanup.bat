@echo off
echo Cleaning up tools...
for %%f in (c:\tools\*.bat) do call remend %%f
echo Done.