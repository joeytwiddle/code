@echo off

if "%phones%"=="true" echo Using headphones - using phones config
if "%phones%"=="true" copy c:\paul\sounds\ft\phones.cfg c:\paul\sounds\ft\ft2.cfg

if not "%phones%"=="true" echo Using speakers - using normal config
if not "%phones%"=="true" copy c:\paul\sounds\ft\norm.cfg c:\paul\sounds\ft\ft2.cfg

c:\paul\sounds\ft\ft2 %1 %2 %3 %4 %5 %6 %7 %8 %9

if "%phones%"=="true" echo Saving phones config and restoring volume
if "%phones%"=="true" copy c:\paul\sounds\ft\ft2.cfg c:\paul\sounds\ft\phones.cfg
if "%phones%"=="true" call phones

if not "%phones%"=="true" echo Saving normal config
if not "%phones%"=="true" copy c:\paul\sounds\ft\ft2.cfg c:\paul\sounds\ft\norm.cfg