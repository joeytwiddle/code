@echo Trying to move if new...
jfc ..\%1 %1 yesno oneway
if ERRORLEVEL 1 goto kill
mv %1 ..\%1
goto end
:kill
:end
