call rm rec*.bmp
call rm data.txt
call rm data.bmp
call rm lines.bmp
call rm quads.bmp
call rm overlay*.bmp
rem For img??, ?? use scale 2.0
rem For image?? use scale 2.5
rem Actually, don't worry because the program now deals with this.
call g -i gfs\%1.gf -ois 2.0 -ic 0.1 -ml 20 -oi ..\images\%1.bmp nod -ol -rqw 600 -fl 0.8 -tur
call mkdir results
call mkdir results\%1
call mv rec*.bmp results\%1
call mv data.* results\%1
call mv line*.bmp results\%1
call mv quads*.bmp results\%1
call mv overlay*.bmp results\%1
