@echo off
dir *.fli /b > tmp2
aaplay tmp2
del tmp2
