#include "allegro.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <mylib.c>
#include <timer.c>

#define scrwid 320
#define scrhei 200
#define pi 3.141593
#define fs 8

#define uchar unsigned char

void perturb (int f);
void mycircle(int cx,int cy,int r,int c);

// Maps used for background writing
uchar **img,**img2,**imgtmp;
float amount[scrwid][scrhei][2][2][2];
int pix[scrwid][scrhei][2],piy[scrwid][scrhei][3];
int fon[fs];
float var[fs],op[fs],damp[fs],force[fs];
float toff=0;

float saw(float x) {
  while (x<0 || x>=1) {
    if (x<0)
      x=x+1;
    if (x>=1)
      x=x-1;
  }
  return x;
}

int huefor(float t,float o) {
  float ctmp;
  ctmp=myabs(t-o);
  if (ctmp>0.5)
    ctmp=(float)1-ctmp;
//  return 255*((float)0.5+0.5*cos(2*pi*ctmp));
  ctmp=ctmp*6;
  if (ctmp<1)
    return 255;
  if (ctmp>2)
    return 0;
  return 255*((float)2-ctmp)/4; // I don't understand the /4 !
}
  

int main(void) {
  
  int f,i,j,k,c,x,y,ix,iy,displayloop;
  int usingmap,makingmap,mmx,mmy,tmpmap,maploop;
    float rx,ry,nrx,nry,px,py,thru,ctmp;
  RGB rgb;
  FILE *fp;
  
  srand((int)time(NULL));
  
  usingmap=0; makingmap=1; mmx=0; mmy=0;
  

   img=(uchar **)calloc(scrhei,sizeof(uchar *));
    img2=(uchar **)calloc(scrhei,sizeof(uchar *));
    for (y=0;y<scrhei;y++) {
        img[y]=(uchar *)calloc(scrwid,sizeof(uchar));
        img2[y]=(uchar *)calloc(scrwid,sizeof(uchar));
        for (x=0;x<scrwid;x++) {
            img[y][x]=255*y/scrhei;
            img2[y][x]=myrnd()*255;
            if (x<scrwid-1 && y<scrhei-1) {
              pix[x][y][usingmap]=x;
              piy[x][y][usingmap]=y;
              for (i=0;i<=1;i++)
                for (j=0;j<=1;j++)
                  amount[x][y][i][j][usingmap]=(float)1/4;
            }
        }
    }
  
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

  starttimer();

  while(!key[KEY_ESC]) {
    
    // Generate some more of the map
    for (maploop=1;maploop<scrwid*scrhei/20;maploop++) {
      rx=(float)mmx/scrwid*2-1;
      ry=(float)(mmy-scrhei/2)/scrwid*2;
      if (fon[0]) {
        rx = mysgn(rx)/var[7]*mypow(myabs(rx),1/var[0]);
        ry = mysgn(ry)/var[7]*mypow(myabs(ry),1/var[0]);
      }
      if (fon[1]) {
        rx = rx / var[1]; ry = ry / var[1];
      }
      if (fon[2]) {
        nrx = rx * cos(var[2]) + ry * sin(var[2]);
        nry = -rx * sin(var[2]) + ry * cos(var[2]);
        rx = nrx; ry=nry;
      }
      if (fon[3]) {
        ry = ry - mysgn(ry) * sin(var[6]*pi*myabs(ry)) * var[3];
      }
      if (fon[4]) {
        ry = ((myabs(ry) - 1) / var[4] + 1) * mysgn(ry);
      }
      if (fon[5]) {
        rx = rx - mysgn(rx) * sin(var[6]*pi*myabs(rx)) * var[5];
      }
      px=(rx+1)/2*scrwid;
      py=scrhei/2+(ry)/2*scrwid;
      ix=(int)px;
      iy=(int)py;
      if (ix<0 || ix>=scrwid-1 || iy<0 || iy>=scrhei-1) {
        ix=px; iy=py;
      }
      amount[mmx][mmy][0][0][makingmap]=((float)ix+1-(float)px)*((float)(iy+1)-(float)py);
      amount[mmx][mmy][1][0][makingmap]=((float)px-(float)ix)*((float)(iy+1)-(float)py);
      amount[mmx][mmy][0][1][makingmap]=((float)ix+1-(float)px)*((float)py-(float)iy);
      amount[mmx][mmy][1][1][makingmap]=((float)px-(float)ix)*((float)py-(float)iy);
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
        hline(screen, scrwid/2, f*2, scrwid/2+(var[f] - op[f]) * scrwid * 4, 0);
      }
    }
        toff=toff-(float)1/128;
    for (c=0;c<=255;c++) {
      thru=saw((float)c/255-toff);
      rgb.r=huefor(thru,(float)0);
      rgb.g=huefor(thru,(float)1/3);
      rgb.b=huefor(thru,(float)2/3);
      set_color(c,&rgb);
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
