OPEN "c:\newback.tmp" FOR INPUT AS #1
fs% = 0
DO
    LINE INPUT #1, s$
    fs% = fs% + 1
LOOP UNTIL EOF(1)
CLOSE #1

RANDOMIZE TIMER
f% = RND * fs%

OPEN "c:\newback.tmp" FOR INPUT AS #1
fs% = 0
q$ = ""
DO
    LINE INPUT #1, s$
    fs% = fs% + 1
    IF fs% > f% AND NOT LEFT$(s$, 19) = "D:\wallpapers\TILES" THEN
      q$ = s$
    END IF
LOOP UNTIL EOF(1) OR NOT q$ = ""
CLOSE #1

OPEN "c:\tools\newbackc.bat" FOR OUTPUT AS #1
PRINT #1, "@copy " + CHR$(34) + q$ + CHR$(34) + " c:\windows\current.jpg /y"
PRINT
PRINT "Wallpaper " + q$ + " chosen."
CLOSE #1

