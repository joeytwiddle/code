@echo off
c:
call cd \
call mkdir tmptar
call cd tmptar
call cp \joey\code\c\joeylib\* .
call tar cf jl.tar *.*
call pkzip ..\jl jl.tar
cd ..
deldir tmptar