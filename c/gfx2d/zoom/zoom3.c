#include "allegro.h"
#include "grx20.h"
//#include "drawing.h"
#include <math.h>
//#include "rand.h"
#include <time.h>
#include <stdlib.h>

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

ripple ripples[maxripples];
float amount[scrwid][scrhei][2][2];
int pix[scrwid][scrhei],piy[scrwid][scrhei];
//GrColor colors[255];
int numripples,mytime;

float myrnd();
float mysquare(float f);
void mycircle(int cx,int cy,int r,GrColor c);
void newripple(int i);

void main(void) {
    
    int xy,i,j,c,x,y,front,back,n,minc,maxc;
    int ix,iy,k;
    float dzdx,dzdy,a,b,dot,norm,tmp;
    float rx,ry,sx,sy,px,py;
    long p,q;
    RGB rgb;

    for (x=0;x<scrwid;x++) {
    for (y=0;y<scrhei;y++) {
        rx=(float)x/scrwid*2-1;
        ry=(float)y/scrhei*2-1;
        sx=rx*.9;
        sy=ry*.9;
        px=(sx+1)/2*scrwid;
        py=(sy+1)/2*scrhei;
        ix=(int)px;
        iy=(int)py;
        amount[x][y][0][0]=((float)ix+1-(float)px)*((float)(iy+1)-(float)py);
        amount[x][y][1][0]=((float)px-(float)ix)*((float)(iy+1)-(float)py);
        amount[x][y][0][1]=((float)ix+1-(float)px)*((float)py-(float)iy);
        amount[x][y][1][1]=((float)px-(float)ix)*((float)py-(float)iy);
        pix[x][y]=ix;
        piy[x][y]=iy;
//        printf("%f",amount[x][y][0][0]+amount[x][y][1][0]+amount[x][y][0][1]+amount[x][y][1][1]);
        if (mysquare(amount[x][y][0][0]+amount[x][y][1][0]+amount[x][y][0][1]+amount[x][y][1][1]-1)>0.00001) {
            printf("%d %d %f %f ",ix,iy,px,py);
            printf("%f+%f(%f*%f)+%f+%f=%f? \n",amount[x][y][0][0],amount[x][y][1][0],(float)px-(float)ix,(float)(iy+1)-(float)py,amount[x][y][0][1],amount[x][y][1][1],amount[x][y][0][0]+amount[x][y][1][0]+amount[x][y][0][1]+amount[x][y][1][1]);
        }
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

    while(!key[KEY_ESC]) {
        for (x=0; x<scrwid; x++) {
        for (y=0; y<scrhei; y++) {
        c=0;
        rx=(float)x/scrwid*2-1;
        ry=(float)y/scrhei*2-1;
        sx=rx*.9;
        sy=ry*.9;
        px=(sx+1)/2*scrwid;
        py=(sy+1)/2*scrhei;
        ix=(int)px;
        iy=(int)py;
        k=0;
        for (i=-1;i<=1;i++) {
        for (j=-1;j<=1;j++) {
            c=c+getpixel(screen, ix+i, iy+j);
            k++;
        }
        }
        c=c/k;
        putpixel(buffer, x, y, c);
        }
        }
for (i=1;i<=10;i++)
        circlefill(buffer,myrnd()*scrwid,myrnd()*scrhei,8,myrnd()*255);
        putpixel(buffer,scrwid/2,scrhei/2,255);
      blit (buffer, screen, 0, 0, 0, 0, scrwid, scrhei);
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
