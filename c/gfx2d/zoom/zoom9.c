#include "allegro.h"
#include "grx20.h"
//#include "drawing.h"
#include <math.h>
//#include "rand.h"
#include <time.h>
#include <stdlib.h>

#define ang pi/64
#define scrwid 320
#define scrhei 200
#define res 4
#define maxripples 20
#define pi 3.141593
#define big 65536
#define lx 1;
#define ly 1;
#define ex 0.8;
#define ey 0.6;
#define scale 1/8;

typedef struct {
    int st,x,y,w,v,n,front,back;
} ripple;

BITMAP *buffer;
unsigned char tmp[scrhei][scrwid];
unsigned char tmp2[scrhei][scrwid];

ripple ripples[maxripples];
float amount[scrwid][scrhei][2][2];
int pix[scrwid][scrhei],piy[scrwid][scrhei];
int dx[scrwid][scrhei],dy[scrwid][scrhei];
//GrColor colors[255];
int numripples,mytime;

float myrnd();
float mysquare(float f);
void mycircle(int cx,int cy,int r,GrColor c);
void newripple(int i);

void main(void) {
    
    int xy,i,j,c,x,y,front,back,n,minc,maxc;
    int ix,iy,k,address,jx,jy,kx,ky;
    float dzdx,dzdy,a,b,dot,norm;
    float rx,ry,sx,sy,px,py;
    long p,q;
    RGB rgb;

    for (x=0;x<scrwid;x++) {
    for (y=0;y<scrhei;y++) {
        kx=x-scrwid/2;
        ky=y-scrhei/2;
        jx=kx*cos(ang)+ky*sin(ang);
        jy=-kx*sin(ang)+ky*cos(ang);
        ix=scrwid/2+0.9*jx;
        iy=scrhei/2+0.9*jy;
        dx[x][y]=ix;
        dy[x][y]=iy;
    }
    }

//    srand(456789);
        srand((int)time(NULL));
        //printf("%d\n",(int)time(NULL));


  allegro_init ();
  install_keyboard ();
  install_timer ();
  set_gfx_mode (GFX_AUTODETECT, scrwid, scrhei, 0, 0);
  set_pallete (desktop_palette);
  buffer = create_bitmap (scrwid, scrhei);
      clear (buffer);

//      textout_centre (buffer, font, "Press SPACE!", 60, 220, 4);


    // Set up grayscale colours
    for (c=0;c<=255;c++) {
        i=0;
        rgb.r=c*63/255;
        rgb.g=0;
        rgb.b=0;
        set_color(c,&rgb);
//        colors[c]=GrAllocColor(c,i,i);
    }
    
        for (x=0; x<scrwid; x++) {
        for (y=0; y<scrhei; y++) {
            putpixel(buffer,x,y,128);
        }
        }

      blit (buffer, screen, 0, 0, 0, 0, scrwid, scrhei);

   _farsetsel(screen->seg);

    while(!key[KEY_ESC]) {
        for (y=0; y<scrhei; y++) {
     movedata(screen->seg, bmp_read_line(screen,y), _my_ds(), tmp[y], scrwid);
     }
        for (x=0; x<scrwid; x++) {
        for (y=0; y<scrhei; y++) {
        c=tmp[dy[x][y]][dx[x][y]];
        c--;
        tmp2[y][x]=c;
        }
        }
        for (y=0; y<scrhei; y++) {
     movedata(_my_ds(), tmp2[y], screen->seg, bmp_write_line(screen,y), scrwid);
     }
for (i=1;i<=10;i++)
        circlefill(screen,myrnd()*scrwid,myrnd()*scrhei,8,myrnd()*255);
//        putpixel(buffer,scrwid/2,scrhei/2,255);

//      blit (buffer, screen, 0, 0, 0, 0, scrwid, scrhei);
    }

destroy_bitmap(buffer);
exit(0);
    getch();

//    GrSetMode(GR_default_text);
    printf("max col %d\n",maxc);
    printf("min col %d\n",minc);
  
}

float myrnd() {
//      return (float)((random() % big)/big);
        return (float)rand()/RAND_MAX;
}

float mysquare(float f) {
    return f*f;
}

void mycircle(int cx,int cy,int r,GrColor c) {
    float a;
    int x,y,ox,oy;
    if (r>0) {
    for (a=0;a<=2*pi;a=a+pi/64) {
        x=cx+r*sin(a);
        y=cy+r*cos(a);
        if (a>0)
//            GrLine(ox,oy,x,y,c);
        ox=x;
        oy=y;
    }
    }
}

void newripple(int i) {
        ripples[i].st=mytime;
        ripples[i].x=myrnd()*scrwid;
        ripples[i].y=myrnd()*scrhei;
        ripples[i].w=50+myrnd()*150;
        ripples[i].v=1+myrnd()*4;
        ripples[i].n=2+myrnd()*7;
//        printf("%f\n",myrnd());
}
