DECLARE SUB newripple (r%)

CONST scrwid% = 320
CONST scrhei% = 200
CONST res% = 1
CONST rs% = 5
CONST grx% = scrwid% / res%
CONST gry% = scrhei% / res%
CONST pi = 3.141593
CONST lx% = 1
CONST ly% = 1
CONST ex% = .8
CONST ey% = .6
CONST k = 10
CONST j = 3

COMMON SHARED t%

DIM SHARED ox%(rs%), oy%(rs%), w%(rs%), st%(rs%), v%(rs%), n%(rs%)

SCREEN 13

RANDOMIZE TIMER

FOR c% = 0 TO 255
PALETTE c%, c% * 63 / 255
NEXT c%
COLOR 255

DO

t% = 0
FOR r% = 1 TO rs%
    CALL newripple(r%)
NEXT r%

t% = 20
    FOR x% = 1 TO scrwid% STEP res%
    FOR y% = 1 TO scrhei% STEP res%
        dzdx = 0: dzdy = 0
        FOR r% = 1 TO rs%
            xy = SQR((x% - ox%(r%)) ^ 2 + (y% - oy%(r%)) ^ 2)
            IF xy > 2 AND xy > v%(r%) * (t% - st%(r%)) - w%(r%) AND xy < v%(r%) * (t% - st%(r%)) THEN
                a = pi / w%(r%) * SIN(2 * pi * n%(r%) / w%(r%) * (xy - v%(r%) * (t% - st%(r%))))
                b = 1 / xy
                norm = 1500 / (xy * xy) * EXP(j * (xy - v%(r%) * (t% - st%(r%))) / w%(r%))
                dzdx = dzdx + a * b * (x% - ox%(r%)) * w%(r%) * norm
                dzdy = dzdy + a * b * (y% - oy%(r%)) * w%(r%) * norm
            END IF
        NEXT r%
        dot = dzdx * lx% + dzdy * ly%
        dot = dzdx * ex% + dzdy * ey%
        c% = 128 + dot * 128 / 4
        IF c% < 0 THEN c% = 0
        IF c% > 255 THEN c% = 255
        LINE (x%, y%)-STEP(res% - 1, res% - 1), c%, BF
    NEXT y%
    NEXT x%

LOOP

SUB newripple (r%)
    st%(r%) = t%
    ox%(r%) = RND * scrwid%
    oy%(r%) = RND * scrhei%
    w%(r%) = 50 + RND * 150
    v%(r%) = RND * 20 + 1
    n%(r%) = 2 + RND * 7
END SUB

