@echo Trying to move if new...
jfc %1 ..\%1 yesno oneway
if ERRORLEVEL 1 goto kill
goto end
:kill
mv %1 new\%1
:end
