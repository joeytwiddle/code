#include "allegro.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "../mylib/mylib.c"

#define scrwid 320
#define scrhei 200
#define pi 3.141593
#define fs 6

// Maps used for background writing
unsigned char map[scrhei][scrwid];
unsigned char map2[scrhei][scrwid];
float amount[scrwid][scrhei][2][2];
int pix[scrwid][scrhei],piy[scrwid][scrhei];
int fon[fs];
float var[fs],op[fs],damp[fs],force[fs];

void main(void) {
    
    int i,j,k,c,x,y;
    RGB rgb;

    srand((int)time(NULL));

    for (x=0;x<scrwid;x++) {
    for (y=0;y<scrhei;y++) {
        rx=(float)x/scrwid*2-1;
        ry=(float)(y-scrhei/2)/scrwid*2;

        px=(qx+1)/2*scrwid;
        py=scrhei/2+(qy)/2*scrwid;
        ix=(int)px;
        iy=(int)py;
        amount[x][y][0][0]=((float)ix+1-(float)px)*((float)(iy+1)-(float)py);
        amount[x][y][1][0]=((float)px-(float)ix)*((float)(iy+1)-(float)py);
        amount[x][y][0][1]=((float)ix+1-(float)px)*((float)py-(float)iy);
        amount[x][y][1][1]=((float)px-(float)ix)*((float)py-(float)iy);
        pix[x][y]=ix;
        piy[x][y]=iy;
    }
    }

    allegro_init ();
    install_keyboard ();
    install_timer ();
    set_gfx_mode (GFX_AUTODETECT, scrwid, scrhei, 0, 0);
    set_pallete (desktop_palette);
    _farsetsel(screen->seg);
    for (c=0;c<=255;c++) {
        if (c<128) {
            rgb.r=c*63/127;
            rgb.g=0;
            rgb.b=0;
        } else {
            rgb.r=127;
            rgb.g=(c-128)*63/127;
            rgb.b=rgb.g;
        }
        set_color(c,&rgb);
    }
    
    while(!key[KEY_ESC]) {
        for (y=0; y<scrhei; y++) {
            movedata(screen->seg, bmp_read_line(screen,y), _my_ds(), map[y], scrwid);
        }
        for (x=0; x<scrwid; x++) {
        for (y=0; y<scrhei; y++) {
            c=0;
            for (i=0;i<=1;i++) {
            for (j=0;j<=1;j++) {
                c=c+amount[x][y][i][j]*map[piy[x][y]+j][pix[x][y]+i];
            }
            }
            c--;
            map2[y][x]=c;
        }
        }
        for (y=0; y<scrhei; y++) {
            movedata(_my_ds(), map2[y], screen->seg, bmp_write_line(screen,y), scrwid);
        }
        for (i=1;i<=10;i++) {
            circlefill(screen,myrnd()*scrwid,myrnd()*scrhei,8,myrnd()*255);
        }
    }

}

/*
DECLARE SUB perturb (var!, op!, damp!, force!)
DECLARE SUB newvel ()
DECLARE SUB newrot ()
DECLARE SUB move (x!, y!)
DECLARE FUNCTION scry% (y!)
DECLARE SUB newp (x!, y!)
DECLARE FUNCTION scrx% (x!)

COMMON SHARED goingout%, rotation%, velout, rot, dripping%, squidging%

DIM SHARED fon%(fs%), var(fs%), op(fs%), damp(fs%), force(fs%)

REM 1 Velocity
op(1) = 1.02: damp(1) = .999: force(1) = .002
REM 2 Rotation
op(2) = 0: damp(2) = .999: force(2) = .002
REM 3 Drip
op(3) = 1: damp(3) = .999: force(3) = .005
REM 4 Dribble
op(4) = 1: damp(4) = .999: force(4) = .005
REM 5 Slide
op(5) = 0: damp(5) = .999: force(5) = .002
REM 6 Accelerate
op(6) = 1: damp(6) = .999: force(6) = .005

FOR f% = 1 TO fs%
    var(f%) = op(f%)
NEXT f%

RANDOMIZE TIMER

t% = 0
FOR p% = 1 TO ps%
    CALL newp(x(p%, t%), y(p%, t%))
NEXT p%

DO
   
    'LOCATE 1, 1
    'PRINT t%, var(1), var(2)

    FOR disploop% = 1 TO 1
   
    nt% = (t% + 1) MOD (ts% + 1)
    FOR p% = 1 TO ps%
        PSET (scrx%(x(p%, nt%)), scry%(y(p%, nt%))), 0
        x(p%, nt%) = x(p%, t%)
        y(p%, nt%) = y(p%, t%)
        CALL move(x(p%, nt%), y(p%, nt%))
        IF RND < .1 OR x(p%, nt%) < -1 OR x(p%, nt%) > 1 OR y(p%, nt%) < -1 OR y(p%, nt%) > 1 OR (ABS(x(p%, nt%)) < small AND ABS(y(p%, nt%)) < small) THEN CALL newp(x(p%, nt%), y(p%, nt%))
        PSET (scrx%(x(p%, nt%)), scry%(y(p%, nt%))), 1 + (p% MOD 15)
    NEXT p%
   
    FOR f% = 1 TO fs%
        LINE (scrwid% / 2, f% * 2)-(scrwid% / 2 + (var(f%) - op(f%)) * scrwid% * 4, f% * 2), 0
        CALL perturb(var(f%), op(f%), damp(f%), force(f%))
        IF fon%(f%) THEN
            LINE (scrwid% / 2, f% * 2)-(scrwid% / 2 + (var(f%) - op(f%)) * scrwid% * 4, f% * 2), 7 + f%
        END IF
        'LOCATE f%, 1
        'PRINT LEFT$(STR$(var(f%)), 6);
        'IF fon%(f%) THEN
         '   PRINT f%
        'ELSE
        '    PRINT "    "
        'END IF
    NEXT f%

    NEXT disploop%

    f% = INT(RND * (fs% + 1))
    IF RND < .1 THEN fon%(f%) = (RND < .3)
   
    t% = nt%

LOOP

SUB move (x, y)
    IF fon%(1) THEN
        x = x * var(1): y = y * var(1)
    END IF
    IF fon%(6) THEN
        x = var(1) * SGN(x) * ABS(x) ^ var(6)
        y = var(1) * SGN(y) * ABS(y) ^ var(6)
    END IF
    IF fon%(2) THEN
        nx = x * COS(var(2)) + y * SIN(var(2))
        ny = -x * SIN(var(2)) + y * COS(var(2))
        x = nx
        y = ny
    END IF
    IF fon%(3) THEN
        'y = y - .01 * (y - 1)
        'y = 1 + .99 * (y - 1)
        'y = (y + 1) * .7 - 1
        y = y * var(3)
    END IF
    IF fon%(4) THEN
        y = ((ABS(y) - 1) * var(4) + 1) * SGN(y)
    END IF
    IF fon%(5) THEN
        x = x + var(5) * SGN(x)
    END IF
END SUB

SUB newp (x, y)
    x = 2 * (RND - .5)
    y = 2 * (RND - .5)
END SUB

SUB perturb (var, op, damp, force)
    var = (var - op) * damp + force * (RND - .5) + op
END SUB

FUNCTION scrx% (x)
    scrx% = scrwid% * (x + 1) / 2
END FUNCTION

FUNCTION scry% (y)
    scry% = scrhei% * (y + 1) / 2
END FUNCTION


*/
