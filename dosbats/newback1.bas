IF INKEY$ = " " THEN
    choose% = 1
    PRINT
    PRINT "Choose a wallpaper from the list:"
    PRINT
    DO
    LOOP UNTIL INKEY$ <> " "
ELSE
    choose% = 0
END IF
OPEN "c:\newback.tmp" FOR INPUT AS #1
DIM f$(100)
fs% = 0
DO
    LINE INPUT #1, f$(fs%)
    IF choose% THEN
        s$ = STR$(fs% + 1) + ": " + f$(fs%)
        PRINT s$; " / ";
    END IF
    fs% = fs% + 1
LOOP UNTIL EOF(1)
CLOSE #1
IF choose% THEN PRINT : PRINT
RANDOMIZE TIMER
DO
    IF choose% THEN
       INPUT "Which background? ", f%
    ELSE
        f% = RND * (fs% + 1)
    END IF
LOOP UNTIL f% >= 1 AND f% <= fs%
f% = f% - 1
OPEN "c:\tools\newback3.bat" FOR OUTPUT AS #1
PRINT #1, "@copy " + CHR$(34) + "d:\wallpapers\" + f$(f%) + CHR$(34) + " c:\windows\current.bmp"
PRINT "Wallpaper " + f$(f%) + " chosen."
CLOSE #1

