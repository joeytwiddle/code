#include "allegro.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "../mylib/mylib.c"
#include <timer.c>

#define scrwid 320
#define scrhei 200
#define pi 3.141593
#define fs 8
#define crowded 64
#define lonely 64

#define uchar unsigned char

void perturb (int f);
void mycircle(int cx,int cy,int r,int c);
int countneighbours(int x,int y);
int countneighbourschecking(int x,int y);

// Maps used for background writing
uchar **img,**img2,**imgtmp;
float amount[scrwid][scrhei][2][2][2];
int pix[scrwid][scrhei][2],piy[scrwid][scrhei][3];
int fon[fs];
float var[fs],op[fs],damp[fs],force[fs];

int lifefrom(int x,int y) {
  int c=countneighbourschecking(x,y)/8;
  if (c==0)
    return myrnd()*255;
  if (c>255-crowded || c<lonely)
    return 0;
  return c;
}  

int countneighbours(int x,int y) {
  int i,j,c=0;
  for (i=-1;i<=1;i++) {
    for (j=-1;j<=1;j++) {
      if (i!=0 && j!=0)
        c=c+img[y+j][x+i];
    }
  }
  return c;
}

int countneighbourschecking(int x,int y) {
  int i,j,dx,dy,c=0;
  for (i=-1;i<=1;i++) {
    dx=x+i;
    if (dx<0)
      dx=dx+scrwid;
    if (dx>=scrwid)
      dx=dx-scrwid;
    for (j=-1;j<=1;j++) {
      dy=y+j;
      if (dy<0)
        dy=dy+scrhei;
      if (dy>=scrhei)
        dy=dy-scrhei;
      if (i!=0 || j!=0)
        c=c+img[dy][dx];
    }
  }
  return c;
}

int main(void) {
  
  int f,i,j,k,c,x,y,ix,iy,displayloop;
  int usingmap,makingmap,mmx,mmy,tmpmap,maploop;
  float rx,ry,nrx,nry,px,py;
  RGB rgb;
  FILE *fp;
  
  img=(uchar **)calloc(scrhei,sizeof(uchar *));
  for (y=0;y<scrhei;y++) {
    img[y]=(uchar *)calloc(scrwid,sizeof(uchar));
    for (x=0;x<scrwid;x++) {
      img[y][x]=myrnd()*255;
    }
  }
  img2=(uchar **)calloc(scrhei,sizeof(uchar *));
  for (y=0;y<scrhei;y++) {
    img2[y]=(uchar *)calloc(scrwid,sizeof(uchar));
    for (x=0;x<scrwid;x++) {
      img2[y][x]=myrnd()*255;
    }
  }
  
  srand((int)time(NULL));
  
  usingmap=0; makingmap=1; mmx=0; mmy=0;
  
  /* Originals from QB
     op[0] = 1; damp[0] = .999; force[0] = .005;
     op[1] = 1.02; damp[1] = .999; force[1] = .002;
     op[2] = 0; damp[2] = .999; force[2] = .002;
     op[3] = 1; damp[3] = .999; force[3] = .005;
     op[4] = 1; damp[4] = .999; force[4] = .005;
     op[5] = 0; damp[5] = .999; force[5] = .002;
     */
  
  // 0 Accelerate
  op[0] = 1; damp[0] = .999; force[0] = .005;
  // 1 Velocity
  op[1] = 1.02; damp[1] = .999; force[1] = .01;
  // 2 Rotation
  op[2] = 0; damp[2] = .995; force[2] = .03;
  // 3 y splurge
  op[3] = 0; damp[3] = .999; force[3] = .01;
  // 4 Dribble
  op[4] = 1; damp[4] = 0; force[4] = .01;
  // 5 x splurge
  op[5] = 0; damp[5] = .999; force[5] = .01;
  op[6]=2;damp[6]=.9999;force[6]=.01;
  op[7]=1;damp[7]=.999;force[7]=.01;
  
  
  for (f=0;f<fs;f++) {
    var[f] = op[f];
    fon[f]=1;
  }
  
  allegrosetup(scrwid,scrhei);
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

  starttimer();

  while(!key[KEY_ESC]) {
    
    // Animate
    for (x=0; x<scrwid; x++) {
      for (y=0; y<scrhei; y++) {
        img2[y][x]=lifefrom(x,y);
      }
    }
    for (y=0; y<scrhei; y++) {
      movedata(_my_ds(), img2[y], screen->seg, bmp_write_line(screen,y), scrwid);
    }
    imgtmp=img;
    img=img2;
    img2=imgtmp;
    for (i=1;i<=5;i++) {
      mycircle(myrnd()*scrwid,myrnd()*scrhei,2+myrnd()*8,myrnd()*255);
    }
    framedone();
  }

  allegro_exit();
  displayframespersecond();

}

void perturb (int f) {
  var[f] = (var[f] - op[f]) * damp[f] + force[f] * (myrnd() - .5) + op[f];
}

void mycircle(int cx,int cy,int r,int c) {
  int x,h,y,px,py;
  for (x=-r;x<=r;x++) {
    h=sqrt(mysquare(r)-mysquare(x));
    for (y=-h;y<=h;y++) {
      px=cx+x; py=cy+y;
      if (px>=0 && px<scrwid && py>=0 && py<scrhei)
        img[py][px]=myrnd()*256;
    }
  }
}
