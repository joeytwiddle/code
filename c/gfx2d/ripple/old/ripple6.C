#include "c:\joey\code\djgpp\contrib\grx23\include\grx20.h"
#include "drawing.h"
#include <math.h>
#include "rand.h"
#include <time.h>

#define scrwid 320
#define scrhei 200
#define res 4
#define maxripples 20
#define pi 3.141593
#define lx 1;
#define ly 1;
#define ex 0.8;
#define ey 0.6;
#define j 3;
#define scale 1/8;

typedef struct {
    int st,x,y,w,v,n,front,back;
} ripple;

ripple ripples[maxripples];
GrColor colors[255];
int numripples,mytime;

float myrnd();
float mysquare(float f);
void mycircle(int cx,int cy,int r,GrColor c);

int main(void) {
    
    int xy,i,c,x,y,front,back,n,minc,maxc;
    float dzdx,dzdy,a,b,dot,norm,tmp;
    long p,q;

    SRND(456789);
    
    GrSetMode(GR_width_height_graphics,320,200);
    
    //GrSetMode(GR_default_graphics);
    //GrSetRGBcolorMode();
    
    // Set up grayscale colours
    for (c=0;c<=255;c++) {
        i=0;
        colors[c]=GrAllocColor(c,i,i);
    }
    
    // Set up ripples
    numripples=5;
    for (i=0; i<numripples; i++) {
        ripples[i].st=mytime;
        ripples[i].x=myrnd()*scrwid;
        ripples[i].y=myrnd()*scrhei;
        ripples[i].w=50+myrnd()*150;
        ripples[i].v=1+myrnd()*4;
        ripples[i].n=2+myrnd()*7;
        printf("%f\n",myrnd());
    }

    mytime=10;
    maxc=0; minc=255;

    while(!kbhit()) {
        for (x=0; x<scrwid; x=x+res) {
        for (y=0; y<scrhei; y=y+res) {
            dzdx=0;
            dzdy=0;
            for (i=0;i<numripples;i++) {
                ripples[i].front=ripples[i].v*(mytime-ripples[i].st);
                ripples[i].back=ripples[i].v*(mytime-ripples[i].st)-ripples[i].w;
            }
            for (i=0;i<numripples;i++) {
                xy=(sqrt((x-ripples[i].x)*(x-ripples[i].x)+(y-ripples[i].y)*(y-ripples[i].y)));
                //printf("xy%i",xy);
                //printf("x%i",x);
                //printf("y%i",y);
                //printf("rx%i",ripples[i].x);
                //printf("ry%i",ripples[i].y);
                //printf("f%i\n",front);
                //printf("b%i\n",back);
                if ((xy>2)&&(xy<ripples[i].front)&&(xy>ripples[i].back)) {
                    //printf("*********");
                    a=pi/ripples[i].w*sin(2*pi*ripples[i].n/ripples[i].w*(xy-ripples[i].v*(mytime-ripples[i].st)));
                    //printf("a%f",a);
                    b=(float)1/xy;
                    //printf("b%f",b);
                    norm=1500/(mysquare(xy))*(exp(3*(xy-(ripples[i].v)*((mytime-ripples[i].st)))/ripples[i].w));
                    //printf("n%f",norm);
                    dzdx=dzdx+a*b*(x-ripples[i].x)*ripples[i].w*norm;
                    dzdy=dzdy+a*b*(y-ripples[i].y)*ripples[i].w*norm;
                }
            }
            //dot=dzdx*lx+dzdy*ly;
            //dot=dot*(dzdx*0.8+dzdy*0.6);
            //dot=dzdx*0.8+dzdy*0.6;
            dot=dzdx * 1 * 0.8 + dzdy * 1 * 0.6;
            c=128+dot*128*scale;
            if (c<minc)
                minc=c;
            if (c>maxc)
                maxc=c;
            if (c<0)
                c=0;
            if (c>255)
                c=255;
            /*GrPlot(x,y,colors[c]);
            GrPlot(x+1,y,colors[c]);
            GrPlot(x,y+1,colors[c]);
            GrPlot(x+1,y+1,colors[c]);*/
            GrFilledBox(x,y,x+res-1,y+res-1,colors[c]);
        }
        }
    /*for (i=0;i<numripples;i++) {
        mycircle(ripples[i].x,ripples[i].y,ripples[i].front,colors[0]);
        mycircle(ripples[i].x,ripples[i].y,ripples[i].back,colors[0]);
    }*/
    mytime=mytime+2;
    }
    getch();

    GrSetMode(GR_default_text);
    printf("max col %d\n",maxc);
    printf("min col %d\n",minc);
  
}

float myrnd() {
    return (float)RND()/RND_MAX/20;
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
            GrLine(ox,oy,x,y,c);
        ox=x;
        oy=y;
    }
    }
}

/*
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
*/

/**
 ** COLOROPS.C ---- test WRITE, XOR, OR, and AND draw modes
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

//#include "test.h"
//#include "rand.h"

//TESTFUNC(colorops)
//{
    
    
/*    GrFBoxColors bcolors,ocolors,icolors;
    GrColor bg,c;
    int x = GrSizeX();
    int y = GrSizeY();
    int ww = (x * 2) / 3;
    int wh = (y * 2) / 3;
    int ii,jj;
    int wdt = ww / 150;
    int bw = x / 16;
    int bh = y / 16;
    int bx,by;

    // This won't work very well under X11 in pseudocolor
    //** mode (256 colors or less) if not using a private
    //** color map. The missing colors break RGB mode      
    GrSetRGBcolorMode();

    bcolors.fbx_intcolor = GrAllocColor(160,100,30);
    bcolors.fbx_topcolor = GrAllocColor(240,150,45);
    bcolors.fbx_leftcolor = GrAllocColor(240,150,45);
    bcolors.fbx_rightcolor = GrAllocColor(80,50,15);
    bcolors.fbx_bottomcolor = GrAllocColor(80,50,15);

    ocolors.fbx_intcolor = GrAllocColor(0,120,100);
    ocolors.fbx_topcolor = GrAllocColor(0,180,150);
    ocolors.fbx_leftcolor = GrAllocColor(0,180,150);
    ocolors.fbx_rightcolor = GrAllocColor(0,90,60);
    ocolors.fbx_bottomcolor = GrAllocColor(0,90,60);

    icolors.fbx_intcolor = GrAllocColor(30,30,30);
    icolors.fbx_bottomcolor = GrAllocColor(0,180,150);
    icolors.fbx_rightcolor = GrAllocColor(0,180,150);
    icolors.fbx_leftcolor = GrAllocColor(0,90,60);
    icolors.fbx_topcolor = GrAllocColor(0,90,60);

    c  = GrAllocColor(250,250,0);
    bg = GrNOCOLOR;

    for(ii = 0,by = -(bh / 3); ii < 17; ii++) {
        for(jj = 0,bx = (-bw / 2); jj < 17; jj++) {
        GrFramedBox(bx+2*wdt,by+2*wdt,bx+bw-2*wdt-1,by+bh-2*wdt-1,2*wdt,&bcolors);
        bx += bw;
        }
        by += bh;
    }

    GrFramedBox(ww/4-5*wdt-1,wh/4-5*wdt-1,ww/4+5*wdt+ww+1,wh/4+5*wdt+wh+1,wdt,&ocolors);
    GrFramedBox(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);

    GrSetClipBox(ww/4,wh/4,ww/4+ww,wh/4+wh);

    drawing(ww/4,wh/4,ww,wh,c,bg);
    while(!kbhit()) {
        drawing(ww/4+(RND()%100),
        wh/4+(RND()%100),
        ww,
        wh,
        ((RND() / 16) & (GrNumColors() - 1)),
        bg
        );
    }
    getch();
    GrFramedBox(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);
    drawing(ww/4,wh/4,ww,wh,c,bg);
    while(!kbhit()) {
        drawing(ww/4+(RND()%100),
        wh/4+(RND()%100),
        ww,
        wh,
        ((RND() / 16) & (GrNumColors() - 1)) | GrXOR,
        bg
        );
    }
    getch();
    GrFramedBox(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);
    drawing(ww/4,wh/4,ww,wh,c,bg);
    while(!kbhit()) {
        drawing(ww/4+(RND()%100),
        wh/4+(RND()%100),
        ww,
        wh,
        ((RND() / 16) & (GrNumColors() - 1)) | GrOR,
        bg
        );
    }
    getch();
    GrFramedBox(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);
    drawing(ww/4,wh/4,ww,wh,c,bg);
    while(!kbhit()) {
        drawing(ww/4+(RND()%100),
        wh/4+(RND()%100),
        ww,
        wh,
        ((RND() / 16) & (GrNumColors() - 1)) | GrAND,
        bg
        );
    }
    getch();*/

