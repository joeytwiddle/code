RANDOMIZE TIMER

RESTORE
no% = -1
DO
READ file$
no% = no% + 1
LOOP UNTIL file$ = "*END*"

choice% = INT(no% * RND)
RESTORE
FOR file% = 1 TO choice%
READ file$
NEXT file%

OPEN "BACKGRND.BAT" FOR OUTPUT AS #1
PRINT #1, "COPY " + file$ + ".PCX C:\WINDOWS\BACKGRND.PCX",
CLOSE

END

DATA WOBBLE
DATA ROAR!
DATA *END*

