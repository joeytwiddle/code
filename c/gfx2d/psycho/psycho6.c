// Psycho Delia
// with integer maths
// unsuccessful at converting all to integer maths

#include "allegro.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "../mylib/mylib.c"

#define scrwid 320
#define scrhei 200
#define pi 3.141593
#define fs 8

#define accuracy 10000

#define uchar unsigned char

void perturb (int f);
void mycircle(int cx,int cy,int r,int c);
int sgn(int x) {
  if (x>0)
    return 1;
  if (x<0)
    return -1;
  return 0;
}

// Maps used for background writing
uchar **img,**img2,**imgtmp;
int amount[scrwid][scrhei][2][2][2];
int pix[scrwid][scrhei][2],piy[scrwid][scrhei][3];
int fon[fs];
int var[fs],op[fs],damp[fs],force[fs];

int main(void) {
  
  int f,i,j,k,c,x,y,ix,iy,displayloop;
  int usingmap,makingmap,mmx,mmy,tmpmap,maploop;
  int rx,ry,nrx,nry;
  float px,py;
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
  op[0] = accuracy*1; damp[0] = accuracy*.999; force[0] = accuracy*.005;
  // 1 Velocity
  op[1] = accuracy*1.02; damp[1] = accuracy*.999; force[1] = accuracy*.01;
  // 2 Rotation
  op[2] = accuracy*0; damp[2] = accuracy*.995; force[2] = accuracy*.05;
  // 3 y splurge
  op[3] = accuracy*0; damp[3] = accuracy*.999; force[3] = accuracy*.01;
  // 4 Dribble
  op[4] = accuracy*1; damp[4] = accuracy*0; force[4] = accuracy*.01;
  // 5 x splurge
  op[5] = accuracy*0; damp[5] = accuracy*.999; force[5] = accuracy*.01;
  op[6]=accuracy*2;damp[6]=accuracy*.9999;force[6]=accuracy*.01;
  op[7]=accuracy*1;damp[7]=accuracy*.999;force[7]=accuracy*.01;
  
  
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
  
  while(!key[KEY_ESC]) {
    
    // Generate some more of the map
    for (maploop=1;maploop<scrwid*scrhei/20;maploop++) {
      rx=accuracy*(float)((float)mmx/(float)scrwid*2-1);
      ry=accuracy*(mmy-scrhei/2)/scrwid*2;
      if (fon[0]) {
        rx = accuracy*mypow(abs(rx),(float)accuracy/var[0])*sgn(rx)/var[7];
        ry = accuracy*mypow(abs(ry),(float)accuracy/var[0])*sgn(ry)/var[7];
      }
      if (fon[1]) {
        rx = accuracy*rx / var[1]; ry = accuracy*ry / var[1];
      }
      if (fon[2]) {
        nrx = (rx * cos((double)((double)var[2]/(double)accuracy)) + ry * sin((double)((double)var[2]/(double)accuracy)));
        nry = (-rx * sin((double)((double)var[2]/(double)accuracy)) + ry * cos((double)((double)var[2]/(double)accuracy)));
        rx = nrx; ry=nry;
      }
      if (fon[3]) {
        ry = ry - sgn(ry) * sin((double)var[6]*pi*abs(ry)/accuracy) * var[3]/accuracy;
      }
      if (fon[4]) {
        ry = (accuracy*(abs(ry) - 1) / var[4] + 1) * sgn(ry);
      }
      if (fon[5]) {
        rx = rx - sgn(rx) * sin((float)pi*abs(rx)*var[6]/accuracy) * var[5]/accuracy;
      }
      px=(float)((float)((float)rx/(float)accuracy)+1)/2*scrwid;
      py=scrhei/2+scrwid*(float)ry/(float)accuracy/2;
      ix=(int)px;
      iy=(int)py;
      if (ix<0 || ix>=scrwid-1 || iy<0 || iy>=scrhei-1) {
        ix=px; iy=py;
      }
      amount[mmx][mmy][0][0][makingmap]=accuracy*((float)ix+1-(float)px)*((float)(iy+1)-(float)py);
      amount[mmx][mmy][1][0][makingmap]=accuracy*((float)px-(float)ix)*((float)(iy+1)-(float)py);
      amount[mmx][mmy][0][1][makingmap]=accuracy*((float)ix+1-(float)px)*((float)py-(float)iy);
      amount[mmx][mmy][1][1][makingmap]=accuracy*((float)px-(float)ix)*((float)py-(float)iy);
      pix[mmx][mmy][makingmap]=ix;
      piy[mmx][mmy][makingmap]=iy;
      if (ix<0 || ix>=scrwid-1 || iy<0 || iy>=scrhei-1) {
        pix[mmx][mmy][makingmap]=scrwid/2;
        piy[mmx][mmy][makingmap]=scrhei/2;
        for (i=0;i<=1;i++) {
          for (j=0;j<=1;j++) {
            amount[mmx][mmy][i][j][makingmap]=0;
          }
        }
      }
      mmx++;
      if (mmx>=scrwid) {
        mmx=0;
        mmy++;
        if (mmy>=scrhei) {
          mmy=0;
          tmpmap=usingmap;
          usingmap=makingmap;
          makingmap=tmpmap;
          for (f=0;f<fs;f++) {
            perturb(f);
          }
        }
      }
    }
    
    // Animate
    for (x=0; x<scrwid; x++) {
      for (y=0; y<scrhei; y++) {
        c=0;
        for (i=0;i<=1;i++) {
          for (j=0;j<=1;j++) {
            c=c+amount[x][y][i][j][usingmap]*img[piy[x][y][usingmap]+j][pix[x][y][usingmap]+i];
          }
        }
        c=c/accuracy;
        c--;
        img2[y][x]=c;
      }
    }
    /* for (y=0;y<scrhei;y++) {
         for (x=0;x<scrwid;x++) {
           _farpokeb(screen->seg, (unsigned long)screen->line[y]+x, img2[y][x]);
         }
       }*/
    for (y=0; y<scrhei; y++) {
      movedata(_my_ds(), img2[y], screen->seg, bmp_write_line(screen,y), scrwid);
    }
    for (f=0;f<fs;f++) {
      if (fon[f]) {
        hline(screen, scrwid/2, f*2, scrwid/2+scrwid*4*(var[f] - op[f])/accuracy, 0);
      }
    }
    imgtmp=img;
    img=img2;
    img2=imgtmp;
    for (i=1;i<=5;i++) {
      mycircle(myrnd()*scrwid,myrnd()*scrhei,2+myrnd()*8,myrnd()*255);
    }
  }
  
}

void perturb (int f) {
  var[f] = (var[f] - op[f]) * (float)(damp[f]/accuracy) + force[f] * (myrnd() - .5) + op[f];
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
