@echo off
if "%1"=="" goto noarg
@edit %1.c
goto end
:noarg
@edit current.c
:end
