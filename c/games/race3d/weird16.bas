DECLARE FUNCTION rou$ (num!)
DECLARE SUB plot (ox!, oy!, oz!, x!, y!, z!)
CONST pi = 3.141592654#
COMMON SHARED writing%
RANDOMIZE TIMER
SCREEN 13
writing% = 0
DO
IF writing% = 0 THEN
frames = 10
am = INT(RND * 5)
bm = INT(RND * 5)
cm = INT(RND * 5)
dm = INT(RND * 5)
em = INT(RND * 5)
fm = INT(RND * 5)
REM o is the centre point for the value
ado = RND * 2 * pi
bdo = RND * 2 * pi
cdo = RND * 2 * pi
ddo = RND * 2 * pi
edo = RND * 2 * pi
fdo = RND * 2 * pi
aro = RND * 2
bro = RND * 2
cro = RND * 2
dro = RND * 2
ero = RND * 2
fro = RND * 2
REM p is the angle at which oscialltion begins
adp = RND * 2 * pi
bdp = RND * 2 * pi
cdp = RND * 2 * pi
ddp = RND * 2 * pi
edp = RND * 2 * pi
fdp = RND * 2 * pi
arp = RND * 2 * pi
brp = RND * 2 * pi
crp = RND * 2 * pi
drp = RND * 2 * pi
erp = RND * 2 * pi
frp = RND * 2 * pi
REM q is the amplitude of oscillation
adq = RND * pi * 1 / 4
bdq = RND * pi * 1 / 4
cdq = RND * pi * 1 / 4
ddq = RND * pi * 1 / 4
edq = RND * pi * 1 / 4
fdq = RND * pi * 1 / 4
arq = RND * .5
brq = RND * .5
crq = RND * .5
drq = RND * .5
erq = RND * .5
frq = RND * .5
END IF

OPEN "swirl.inc" FOR OUTPUT AS #1
PRINT #1, "groovy!"
FOR frame = 1 TO frames
CLS
PRINT am, bm, cm, dm, em, fm
thru = frame / frames
ad = ado + adq * SIN(thru * 2 * pi + adp)
bd = bdo + bdq * SIN(thru * 2 * pi + bdp)
cd = cdo + cdq * SIN(thru * 2 * pi + cdp)
dd = ddo + ddq * SIN(thru * 2 * pi + ddp)
ed = edo + edq * SIN(thru * 2 * pi + edp)
fd = fdo + fdq * SIN(thru * 2 * pi + fdp)
ar = aro + arq * SIN(thru * 2 * pi + arp)
br = bro + brq * SIN(thru * 2 * pi + brp)
cr = cro + crq * SIN(thru * 2 * pi + crp)
dr = dro + drq * SIN(thru * 2 * pi + drp)
er = ero + erq * SIN(thru * 2 * pi + erp)
fr = fro + frq * SIN(thru * 2 * pi + frp)

PRINT #1, "groovy!"

PRINT #1, "if (frame ==" + STR$(frame) + ") {"

first% = 1

FOR k = 0 TO 1 STEP .005
x = 0: y = 0: z = 0
a = am * 2 * pi * k + ad
b = bm * 2 * pi * k + bd
c = cm * 2 * pi * k + cd
d = dm * 2 * pi * k + dd
e = em * 2 * pi * k + ed
f = fm * 2 * pi * k + fd
x = x + ar * SIN(a)
x = x + br * SIN(b)
y = y + cr * SIN(c)
y = y + dr * SIN(d)
z = z + er * SIN(e)
z = z + fr * SIN(f)
IF first% = 1 THEN
        first% = 0
ELSE
        CALL plot(ox, oy, oz, x, y, z)
END IF
ox = x: oy = y: oz = z
NEXT k

PRINT #1, "}"

NEXT frame
CLOSE
DO
k$ = INKEY$
LOOP UNTIL k$ <> ""
IF k$ = "a" THEN writing% = 1 ELSE writing% = 0
LOOP
END

SUB change (u, v, ang, rad)
x = x + rad * SIN(ang)
y = y + rad * COS(ang)
END SUB

SUB plot (ox, oy, oz, x, y, z)
COLOR INT(64 + 64 * z / 8)
LINE (100 + ox * 20, 100 + oy * 20)-(100 + x * 20, 100 + y * 20)
LINE (220 + oz * 20, 100 + oy * 20)-(220 + z * 20, 100 + y * 20)
LOCATE 1, 1
cyl$ = "    cylinder <" + rou(ox) + "," + rou(oy) + "," + rou(oz) + ">,<" + rou(x) + "," + rou(y) + "," + rou(z) + ">,0.07"
'PRINT cyl$
IF writing% THEN
PRINT #1, "object {"
PRINT #1, cyl$
PRINT #1, "    rotate swr_rot"
PRINT #1, "    }"
END IF
END SUB

FUNCTION rou$ (num)
rou$ = STR$(INT(num * 1000) / 1000)
END FUNCTION

