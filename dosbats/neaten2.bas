DECLARE FUNCTION myspctrim$ (s$)
DECLARE FUNCTION plateau! (x!)
DECLARE FUNCTION rightfrom$ (s$, n%)
DECLARE FUNCTION countocc% (s$, p$)

CONST tabs% = 2

debug% = 0

IF INSTR(COMMAND$, " ") > 0 THEN
    in$ = LEFT$(COMMAND$, INSTR(COMMAND$, " ") - 1)
    rest$ = rightfrom$(COMMAND$, INSTR(COMMAND$, " ") + 1)
ELSE
    in$ = COMMAND$
    rest$ = ""
END IF
bak$ = LEFT$(in$, INSTR(in$, ".") - 1) + ".nbk"
out$ = in$

IF rest$ = "/D" OR rest$ = "/d" THEN debug% = 1

REM Make a backup

OPEN in$ FOR INPUT AS #1
OPEN bak$ FOR OUTPUT AS #2
DO WHILE NOT (EOF(1))
    LINE INPUT #1, l$
    PRINT #2, l$
LOOP
CLOSE #1
CLOSE #2

REM Create neat copy from backup over original

OPEN bak$ FOR INPUT AS #1
OPEN out$ FOR OUTPUT AS #2

i% = 0
tmpind% = 0
last% = 0
LINE INPUT #1, l$
l$ = myspctrim$(l$)

DO WHILE (NOT (EOF(1))) OR (last% < 2)
    IF RIGHT$(l$, 1) = "}" AND LEN(l$) > 1 AND INSTR(l$, "//") = 0 AND INSTR(l$, "#define") = 0 THEN
      l$ = LEFT$(l$, LEN(l$) - 1)
      nextl$ = "}"
    ELSE
      IF NOT (EOF(1)) THEN LINE INPUT #1, nextl$
      IF EOF(1) THEN last% = last% + 1
    END IF
    nextl$ = myspctrim$(nextl$)
    IF LEFT$(nextl$, 1) = "{" THEN
      l$ = l$ + " {"
      nextl$ = myspctrim$(rightfrom$(nextl$, 2))
    END IF
    IF LEFT$(l$, 2) = "/*" THEN
        l$ = "/* " + myspctrim$(RIGHT$(l$, LEN(l$) - 2))
    END IF
    IF LEFT$(l$, 2) = "//" THEN
        l$ = "// " + myspctrim$(RIGHT$(l$, LEN(l$) - 2))
    END IF
    i% = i% - tabs% * countocc(l$, "}")
    IF LEFT$(l$, 7) = "public:" THEN tmpind% = tmpind% - tabs%
    IF LEFT$(l$, 2) = "*/" THEN tmpind% = tmpind% - 3
    IF debug% THEN PRINT i%, tmpind%, l$
    IF LEN(l$) > 0 THEN t$ = STR$(ASC(LEFT$(l$, 1))) ELSE t$ = ""
    PRINT #2, STRING$(plateau(i% + tmpind%), " ") + l$
   
    tmpind% = 0
    i% = i% + tabs% * countocc(l$, "{")
    i% = i% + 3 * countocc(l$, "/*")
    i% = i% - 3 * countocc(l$, "*/")
    IF (LEFT$(l$, 2) = "if" AND countocc(l$, "{") = 0) THEN tmpind% = tmpind% + tabs%
    l$ = nextl$
LOOP

CLOSE #1
CLOSE #2

FUNCTION countocc% (s$, p$)
    t$ = s$
    c% = 0
    DO WHILE INSTR(t$, p$) > 0
        c% = c% + 1
        t$ = MID$(t$, INSTR(s$, p$) + 1, LEN(t$))
    LOOP
    countocc% = c%
END FUNCTION

FUNCTION myspctrim$ (s$)
  done% = 0
  DO WHILE LEN(s$) > 0 AND done% = 0
    a% = ASC(LEFT$(s$, 1))
    IF a% = 9 OR a% = 32 THEN
      s$ = RIGHT$(s$, LEN(s$) - 1)
    ELSE
      done% = 1
    END IF
  LOOP
  myspctrim$ = s$
END FUNCTION

FUNCTION plateau (x)
    IF x < 0 THEN plateau = 0 ELSE plateau = x
END FUNCTION

FUNCTION rightfrom$ (s$, n%)
rightfrom$ = RIGHT$(s$, LEN(s$) - n% + 1)
END FUNCTION

