#include <stdlib.h>
#include <allegro.h>
#include <math.h>
#include <mylib.c>

#define scrwid 640
#define scrhei 480

#define numparts 80

#define planedist 6
#define eyewid 0.1
#define znear 0.1
#define zcreatenear planedist
#define zfar 20
#define partrad 0.04
#define brownian 0.00004
#define warpvel 0.00005

#define glasses 1

// x=-1 is left of screen, x=1 is right
// y and z fit this space
// (0,0,0) is centre of screen

typedef struct {
  V3d pos,vel;
} Part;

Part part[numparts];

int sgn(int x) {
  if (x>0)
    return 1;
  if (x<0)
    return -1;
  return 0;
}

void newpart(int i) {
  float x,y;
  //part[i].pos=(V3d)newV3d(floatrnd(-1,1),floatrnd(-1,1),floatrnd(znear,zfar));
  x=floatrnd(-1,1); y=floatrnd(-1,1);
  part[i].pos.z=floatrnd(zcreatenear,zfar);
  // scrx/pd=x/z
  part[i].pos.x=part[i].pos.z*x/planedist;
  part[i].pos.y=part[i].pos.z*y/planedist;
  part[i].vel=newV3d(0,0,0);
}

void findplanehit(V3d eye,V3d far,float plane,float *x,float *y) {
  // (y-eyey)/(plane-eyez)=(fary-eyey)/(farz-eyez)
  *x=eye.x+(plane-eye.z)*(far.x-eye.x)/(far.z-eye.z);
  *y=eye.y+(plane-eye.z)*(far.y-eye.y)/(far.z-eye.z);
}

int plotpart2(Part p,float eyex,int c) {
  float x,y;
  int scrx,scry,rad;
  findplanehit(newV3d(eyex,0,0),p.pos,planedist,&x,&y);
  scrx=scrwid/2+scrwid/2*x;
  scry=scrhei/2-scrwid/2*y;
  findplanehit(newV3d(eyex,0,0),V3dadd(p.pos,newV3d(partrad,0,0)),planedist,&x,&y);
  rad=scrwid/2+scrwid/2*x-scrx;
  if (abs(scrx-scrwid/2)>scrwid/2 || abs(scry-scrhei/2)>scrhei/2)
    return 0; // Off screen
  else {
    circle(screen,scrx,scry,rad,c);
    return 1; // On screen
  }
}

int plotpart(int i,int c) {
  if (glasses) {
    plotpart2(part[i],-eyewid,sgn(c)*1);
    plotpart2(part[i],eyewid,sgn(c)*2);
  } else
    plotpart2(part[i],0,c);
}

void movepart(int i) {
  if (plotpart(i,0)==0 || part[i].pos.z<znear)
    newpart(i);
  // Brownian motion
  part[i].vel.x=part[i].vel.x+floatrnd(-brownian,brownian);
  part[i].vel.y=part[i].vel.y+floatrnd(-brownian,brownian);
  part[i].vel.z=part[i].vel.z+floatrnd(-brownian,brownian);
  // Warping towards user
  part[i].vel.z=part[i].vel.z-warpvel;
  part[i].pos=V3dadd(part[i].pos,part[i].vel);
  /* Bouncing off the walls
  if (myabs(part[i].pos.x)>1)
    part[i].vel.x=-part[i].vel.x;
  if (myabs(part[i].pos.y)>1)
    part[i].vel.y=-part[i].vel.y;
  if (myabs(part[i].pos.z)>1)
    part[i].vel.z=-part[i].vel.z;*/
  plotpart(i,255);
}

int main(String *argv,int argc) {
  int i;
  allegrosetup(scrwid,scrhei);
  mypalette(0,0,0,0);
  mypalette(1,0.4,0,0);
  mypalette(2,0,0.7,0);
  for (i=0;i<numparts;i++) {
    newpart(i);
  }
  while (!key[KEY_ESC]) {
    for (i=0;i<numparts;i++) {
      movepart(i);
    }
  }
  allegro_exit();
}
