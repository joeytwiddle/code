@echo off
c:
cd \
del *.tar
del *.zip
cd \joey\code\c
call tar cf \fromfil.tar joeylib\*.c joeylib\*.h tools\*.c phd\improc\*.c phd\stats\*.c phd\stats\*.bat phd\regions\*.c phd\regions\*.bat phd\quads\*.c phd\quads\*.bat
cd \
pkzip fromfil.zip fromfil.tar
