@echo off
echo.
echo Shutting down computer...
echo.
echo Clearing write-behind cache...
smartdrv /c
echo Done
echo.
echo Parking drive heads
diskmon /park
echo.
echo Shut down aborted
echo.