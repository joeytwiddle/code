#include <stdlib.h>
#include <allegro.h>
#include <math.h>
#include <mylib.c>

#define scrwid 640
#define scrhei 480

#define numparts 200

#define planedist 2
#define partrad 0.04
#define brownian 0.001

// x=-1 is left of screen, x=1 is right
// y and z fit this space
// (0,0,0) is centre of screen

typedef struct {
  V3d pos,vel;
} Part;

Part part[numparts];

void newpart(int i) {
  part[i].pos=(V3d)newV3d(floatrnd(-1,1),floatrnd(-1,1),floatrnd(-1,1));
  part[i].vel=newV3d(0,0,0);
}

void findplanehit(V3d eye,V3d far,float plane,float *x,float *y) {
  // (y-eyey)/(plane-eyez)=(fary-eyey)/(farz-eyez)
  *x=eye.x+(plane-eye.z)*(far.x-eye.x)/(plane+far.z-eye.z);
  *y=eye.y+(plane-eye.z)*(far.y-eye.y)/(plane+far.z-eye.z);
}

void plotpart(int i,int c) {
  float x,y;
  int scrx,scry,rad;
  findplanehit(newV3d(0,0,0),part[i].pos,planedist,&x,&y);
  scrx=scrwid/2+scrwid/2*x;
  scry=scrhei/2-scrwid/2*y;
  findplanehit(newV3d(0,0,0),V3dadd(part[i].pos,newV3d(partrad,0,0)),planedist,&x,&y);
  rad=scrwid/2+scrwid/2*x-scrx;
  circle(screen,scrx,scry,rad,c);
}

void movepart(int i) {
  plotpart(i,0);
  part[i].vel.x=part[i].vel.x+floatrnd(-brownian,brownian);
  part[i].vel.y=part[i].vel.y+floatrnd(-brownian,brownian);
  part[i].vel.z=part[i].vel.z+floatrnd(-brownian,brownian);
  part[i].pos=V3dadd(part[i].pos,part[i].vel);
  if (myabs(part[i].pos.x)>1)
    part[i].vel.x=-part[i].vel.x;
  if (myabs(part[i].pos.y)>1)
    part[i].vel.y=-part[i].vel.y;
  if (myabs(part[i].pos.z)>1)
    part[i].vel.z=-part[i].vel.z;
  plotpart(i,255);
}

int main(String *argv,int argc) {
  int i;
  allegrosetup(scrwid,scrhei);
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
