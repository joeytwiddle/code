/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


// Ripples
// using integer maths

#include <joeylib.c>
#include "grx20.h"

#define accuracy 100000

#define coldifftol 4
#define scrwid 320
#define scrhei 200
#define res 4
#define maxripples 20
#define pi 3.141593
#define big 65536
#define lx 1
#define ly 1
#define ex 0.8
#define ey 0.6
#define j 3
#define indent 0
#define jump 32

JBmp *pic=new JBmp(scrwid,scrhei);
Map2d<int> *colmap=new Map2d<int>(scrwid,scrhei);

typedef struct {
    int st,x,y,w,v,n,front,back;
} ripple;

BITMAP *buffer;

ripple ripples[maxripples];
//GrColor colors[255];
int numripples,mytime;

void mycircle(int cx,int cy,int r,GrColor c);
void newripple(int i);

int getcol(int x,int y) {
  int quib=colmap->pos[x][y];
  if (quib>0)
    return quib;
  else {
    int xy,i,c,front,back,n,minc,maxc;
    int dzdx,dzdy,abn,dot,norm,tmp;
    long p,q;
    RGB rgb;
            dzdx=0;
            dzdy=0;
            for (i=0;i<numripples;i++) {
                ripples[i].front=ripples[i].v*(mytime-ripples[i].st);
                ripples[i].back=ripples[i].v*(mytime-ripples[i].st)-ripples[i].w;
                if (ripples[i].back>scrwid)
                        newripple(i);
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
                    abn=accuracy*pi/ripples[i].w*sin(2*pi*ripples[i].n/ripples[i].w*(xy-ripples[i].v*(mytime-ripples[i].st)))*10.0/mysquare(xy+1)*2.0*(xy-ripples[i].back)/(ripples[i].front-ripples[i].back);
                    //printf("a%f",a);
                    //printf("b%f",b);
                    //norm=1500/(mysquare(xy))*(exp(3*(xy-(ripples[i].v)*((mytime-ripples[i].st)))/ripples[i].w));
                    //printf("n%f",norm);
                    dzdx=dzdx+abn*(x-ripples[i].x)*ripples[i].w;
                    dzdy=dzdy+abn*(y-ripples[i].y)*ripples[i].w;
                }
            }
            //dot=dzdx*lx+dzdy*ly;
            //dot=dot*(dzdx*0.8+dzdy*0.6);
            //dot=dzdx*0.8+dzdy*0.6;
            dot=(float)dzdx * (float).8 + (float)dzdy * (float).6;
            c=128+(float)dot*16/accuracy;
/*            if (dzdx==0 && dzdy==0)
                c=255;*/
            if (c<minc)
                minc=c;
            if (c>maxc)
                maxc=c;
            if (c<0)
                c=0;
            if (c>255)
                c=255;
    colmap->pos[x][y]=c;
  return c;
  }
}

void fillsquare(int x,int y,int w,int h) {
        if (key[KEY_SPACE])
          exit(0);
  int nwcol=getcol(x,y);
  int necol=getcol(x+w,y);
  int swcol=getcol(x,y+h);
  int secol=getcol(x+w,y+h);
  int cx=x+w/2;
  int cy=y+h/2;
  int ave=(nwcol+necol+swcol+secol)/4;
  int m=max(max(diff(nwcol,ave),diff(necol,ave)),max(diff(swcol,ave),diff(secol,ave)));
  if (m<coldifftol || max(w,h)<=res) {
    pic->interpolate(indent+x,y,indent+x+w,y+h,nwcol,necol,swcol,secol);
    pic->writetoscreen();
  } else {
    fillsquare(x,y,w/2,h/2);
    fillsquare(cx,y,w/2,h/2);
    fillsquare(x,cy,w/2,h/2);  
    fillsquare(cx,cy,w/2,h/2);
  }
}

int main(void) {
    
    int xy,i,c,x,y,front,back,n,minc,maxc;
    int dzdx,dzdy,abn,dot,norm,tmp;
    long p,q;
    RGB rgb;

//    srand(456789);
        srand((int)time(NULL));
        //printf("%d\n",(int)time(NULL));


    mytime=10;
    maxc=0; minc=255;


    // Set up ripples
    numripples=5;
    for (i=0; i<numripples; i++) {
        newripple(i);
    }


  allegro_init ();
  install_keyboard ();
  install_timer ();
  set_gfx_mode (GFX_AUTODETECT, scrwid, scrhei, 0, 0);
  set_pallete (desktop_palette);
  buffer = create_bitmap (scrwid, scrhei);
      clear (buffer);

    // Set up grayscale colours
    for (c=0;c<=255;c++) {
        i=0;
        rgb.r=c*63/255;
        rgb.g=0;
        rgb.b=0;
        set_color(c,&rgb);
//        colors[c]=GrAllocColor(c,i,i);
    }
    

//      circlefill (buffer, x, y, 3, 255);
      textout_centre (buffer, font, "Press SPACE!", 60, 220, 4);

      blit (buffer, screen, 0, 0, 0, 0, scrwid, scrhei);

    while(!key[KEY_ESC]) {
        colmap->clear(0);
        for (x=0; x+jump<scrwid; x=x+jump) {
        for (y=0; y+jump<scrhei; y=y+jump) {
          fillsquare(x,y,jump,jump);
          pic->writetoscreen();
//            GrFilledBox(x,y,x+res-1,y+res-1,colors[c]);
        }
        }
    /*for (i=0;i<numripples;i++) {
        mycircle(ripples[i].x,ripples[i].y,ripples[i].front,colors[0]);
        mycircle(ripples[i].x,ripples[i].y,ripples[i].back,colors[0]);
    }*/
      blit (buffer, screen, 0, 0, 0, 0, scrwid, scrhei);
    mytime=mytime+2;
    }
destroy_bitmap(buffer);
exit(0);
    getch();

//    GrSetMode(GR_default_text);
    printf("max col %d\n",maxc);
    printf("min col %d\n",minc);
  
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
