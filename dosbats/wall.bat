@echo off
rem
rem Windows wallpaper setter 2.0
rem By Paul Clark (C)Jan 1993
rem

if not "%1"=="" goto set

echo.
echo The wallpaper (background picture) in Windows can be set from any of the
echo following pictures:
echo.
DIR C:\WINDOWS\BCKGRNDS /B
echo.
echo To select a wallpaper, type  WALL  followed by the first (few)
echo letter(s) of the picture.
echo For example, to select the rose picture, type:
echo     WALL R    or    WALL RO    or    WALL ROS    etc.
echo.
goto end

:set
COPY /B C:\WINDOWS\BCKGRNDS\%1*.BMP C:\WINDOWS\BACKGRND.BMP
goto end

:end