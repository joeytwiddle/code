@echo Trying to remove identical files...
jfc %1 ..\%1 yesno
if ERRORLEVEL 1 goto end
del %1
:end
