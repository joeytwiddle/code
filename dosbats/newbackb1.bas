' newbackb1 : allowed you to choose the background.  Ran out of string space!

PRINT
PRINT "Press <space> now to choose background..."
choose% = 0
t& = TIMER
DO
k$ = INKEY$
IF k$ = " " THEN choose% = 1
LOOP UNTIL TIMER > t& + 1 OR choose% OR k$ <> ""
IF choose% THEN
    PRINT
    PRINT "Choose a wallpaper from the list:"
    PRINT
END IF
OPEN "c:\newback.tmp" FOR INPUT AS #1
DIM f$(1000)
fs% = 0
DO
    LINE INPUT #1, s$
    IF NOT LEFT$(s$, 19) = "D:\wallpapers\TILES" THEN
      f$(fs%) = s$
      IF choose% THEN
          s$ = STR$(fs% + 1) + ": " + s$
          PRINT s$; " / ";
      END IF
      fs% = fs% + 1
    END IF
LOOP UNTIL EOF(1)
CLOSE #1
IF choose% THEN PRINT : PRINT
RANDOMIZE TIMER
IF choose% THEN PRINT
DO
    IF choose% THEN
       INPUT "Which background? ", f%
    ELSE
        f% = RND * (fs% + 1)
    END IF
LOOP UNTIL f% >= 1 AND f% <= fs%
f% = f% - 1
OPEN "c:\tools\newbackc.bat" FOR OUTPUT AS #1
PRINT #1, "@copy " + CHR$(34) + f$(f%) + CHR$(34) + " c:\windows\current.jpg /y"
PRINT
PRINT "Wallpaper " + f$(f%) + " chosen."
CLOSE #1

