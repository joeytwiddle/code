@echo off
call for %%f in (*.*) do call remid %%f
mkdir new
call for %%f in (*.*) do call moveifnew %%f
for %%f in (*.*) do del %%f
cd new
call for %%f in (*.*) do call moveifbetter %%f
cd ..
echo Files in this directory can be copied straight over
echo Files in new/ have differences in both files
