IF INKEY$ = " " THEN
    choose% = 1
    PRINT
    PRINT "Choose a vis from the list:"
    PRINT
    DO
    LOOP UNTIL INKEY$ <> " "
ELSE
    choose% = 0
END IF
OPEN "c:\newvis.tmp" FOR INPUT AS #1
DIM f$(100)
fs% = 0
DO
    LINE INPUT #1, f$(fs%)
    IF choose% THEN
        s$ = STR$(fs% + 1) + ": " + f$(fs%)
        PRINT s$; " / ";
    END IF
    IF f$(fs%) <> "current.vws" THEN fs% = fs% + 1
LOOP UNTIL EOF(1)
CLOSE #1
IF choose% THEN PRINT : PRINT
RANDOMIZE TIMER
DO
    IF choose% THEN
       INPUT "Which vis? ", f%
    ELSE
        f% = RND * (fs% + 1)
    END IF
LOOP UNTIL f% >= 1 AND f% <= fs%
f% = f% - 1

OPEN "c:\Progra~1\Winamp\Plugins\plugin.ini" FOR INPUT AS #1
OPEN "c:\Progra~1\Winamp\Plugins\plugin2.ini" FOR OUTPUT AS #2
OPEN "c:\Progra~1\Winamp\Plugins\" + f$(f%) FOR INPUT AS #3
DO
    LINE INPUT #1, tmp$
    PRINT #2, tmp$
LOOP UNTIL tmp$ = "[wVis Plug-in 4]"
LINE INPUT #3, tmp$
DO
    LINE INPUT #3, tmp$
    PRINT #2, tmp$
LOOP UNTIL EOF(3)
DO
    LINE INPUT #1, tmp$
    'PRINT ">"; tmp$; "<"
LOOP UNTIL tmp$ = "" OR EOF(1)
DO UNTIL EOF(1)
    LINE INPUT #1, tmp$
    PRINT #2, tmp$
LOOP
CLOSE

OPEN "c:\tools\newvis3.bat" FOR OUTPUT AS #1
PRINT #1, "@copy " + CHR$(34) + "c:\Progra~1\Winamp\Plugins\plugin2.ini " + CHR$(34) + " c:\Progra~1\Winamp\Plugins\plugin.ini"
PRINT "Vis " + f$(f%) + " chosen."
CLOSE #1

