@echo off
dir *.fli /b > tmp
aaplay tmp
del tmp