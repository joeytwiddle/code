@echo off
call cp %1 %1.blurbback
call cp \myblurb + %1 %1.blurbnew
call mv %1.blurbnew %1
rem mkdir blurbback
rem call cp *.c blurbback
rem mkdir blurbed
rem call cp *.c blurbed
rem for %%f in (*.c) do call cp \myblurb + %%f blurbed\%%f
rem call cp blurbed\*.c .
rem deltree /Y blurbed