#include "allegro.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "../mylib/mylib.c"

#define scrwid 320
#define scrhei 200
#define pi 3.141593
#define fs 6

void perturb (int f);

// Maps used for background writing
unsigned char map[scrhei][scrwid];
unsigned char map2[scrhei][scrwid];
float amount[scrwid][scrhei][2][2];
int pix[scrwid][scrhei],piy[scrwid][scrhei];
int fon[fs];
float var[fs],op[fs],damp[fs],force[fs];

int main(void) {
    
    int f,i,j,k,c,x,y,ix,iy,displayloop;
    float rx,ry,nrx,nry,px,py;
    RGB rgb;

    srand((int)time(NULL));

/* Originals
op[0] = 1; damp[0] = .999; force[0] = .005;
op[1] = 1.02; damp[1] = .999; force[1] = .002;
op[2] = 0; damp[2] = .999; force[2] = .002;
op[3] = 1; damp[3] = .999; force[3] = .005;
op[4] = 1; damp[4] = .999; force[4] = .005;
op[5] = 0; damp[5] = .999; force[5] = .002;
*/

// 0 Accelerate
op[0] = 1; damp[0] = .999; force[0] = .003;
// 1 Velocity
op[1] = 1.02; damp[1] = .999; force[1] = .008;
// 2 Rotation
op[2] = 0; damp[2] = .999; force[2] = .01;
// 3 Drip
op[3] = 1; damp[3] = .999; force[3] = .02;
// 4 Dribble
op[4] = 1; damp[4] = .999; force[4] = .02;
// 5 Slide
op[5] = 0; damp[5] = .999; force[5] = .01;

    for (f=0;f<fs;f++) {
        var[f] = op[f];
        fon[f]=1;
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

        for (f=0;f<fs;f++) {
            perturb(f);
        }
        // Generate new map
    for (x=0;x<scrwid;x++) {
    for (y=0;y<scrhei;y++) {
        rx=(float)x/scrwid*2-1;
        ry=(float)(y-scrhei/2)/scrwid*2;
        if (fon[1]) {
//inv            rx = rx * var[1]; ry = ry * var[1];
            rx = rx / var[1]; ry = ry / var[1];
        }
        if (fon[0]) {
//inv            rx = var[1]*mysgn(rx)*mypow(myabs(rx),var[6]);
//            ry = var[1]*mysgn(ry)*mypow(myabs(ry),var[6]);
            rx = mysgn(rx)/var[1]*mypow(myabs(rx),1/var[6]);
            ry = mysgn(ry)/var[1]*mypow(myabs(ry),1/var[6]);
        }
        if (fon[2]) {
            nrx = rx * cos(var[2]) + ry * sin(var[2]);
            nry = -rx * sin(var[2]) + ry * cos(var[2]);
            rx = nrx; ry=nry;
        }
        if (fon[3]) {
//        'y = y - .01 * (y - 1)
//        'y = 1 + .99 * (y - 1)
//        'y = (y + 1) * .7 - 1
//inv            ry = ry * var[3];
            ry = ry / var[3];
        }
        if (fon[4]) {
//            ry = ((myabs(ry) - 1) * var[4] + 1) * mysgn(ry);
            ry = ((myabs(ry) - 1) / var[4] + 1) * mysgn(ry);
        }
        if (fon[5]) {
            rx = rx + var[5] * mysgn(rx);
        }
        px=(rx+1)/2*scrwid;
        py=scrhei/2+(ry)/2*scrwid;
        ix=(int)px;
        iy=(int)py;
        amount[x][y][0][0]=((float)ix+1-(float)px)*((float)(iy+1)-(float)py);
        amount[x][y][1][0]=((float)px-(float)ix)*((float)(iy+1)-(float)py);
        amount[x][y][0][1]=((float)ix+1-(float)px)*((float)py-(float)iy);
        amount[x][y][1][1]=((float)px-(float)ix)*((float)py-(float)iy);
        pix[x][y]=ix;
        piy[x][y]=iy;
        if (ix<0 || ix>=scrwid || iy<0 || iy>=scrhei) {
            for (i=0;i<=1;i++) {
            for (j=0;j<=1;j++) {
                amount[x][y][i][j]=0;
            }
            }
        }
    }
    }
        for (displayloop=0;displayloop<6;displayloop++) {
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
        for (f=0;f<fs;f++) {
            if (fon[f]) {
                hline(screen, scrwid/2, f*2, scrwid/2+(var[f] - op[f]) * scrwid * 4, 0);
            }
        }
        }
        for (i=1;i<=10;i++) {
            circlefill(screen,myrnd()*scrwid,myrnd()*scrhei,8,myrnd()*255);
        }
        f = myrnd() * (fs + 1);
        if (myrnd()<.8) {
            fon[f] = (myrnd() < .5);
        }

    }

}

void perturb (int f) {
    var[f] = (var[f] - op[f]) * damp[f] + force[f] * (myrnd() - .5) + op[f];
}

/* MAY NEED THIS LATER!
    p=(unsigned char **)calloc((*height),sizeof(unsigned char *));
	if(!p) {
	    fprintf(stderr,"readppm: calloc error 1\n");
	    exit(0);
	}
	for(i=0;i<(*height);i++) {
	    p[i]=(unsigned char *)calloc((*width),sizeof(unsigned char));
	    if(!p[i]) {
		fprintf(stderr,"readppm: calloc error 2\n");
		exit(0);
	    }
	}
*/
