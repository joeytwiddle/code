//- Lots of small particles

//line(screen,myrnd()*scrwid,myrnd()*scrhei,myrnd()*scrwid,myrnd()*scrhei,myrnd()*255);

#include <stdlib.h>
#include <allegro.h>
#include <math.h>
#include <mylib.c>

#define scrwid 640
#define scrhei 480

#define numparts 300
#define pgwell 0.02

#define planedist 6
#define eyewid 0.1
#define znear 0.1
#define zcreatenear 4
#define zfar 20

#define dampening .99
#define partrad 0.01
#define brownian 0.0003
#define pbrownian 1
#define warpvel 0.0001
#define gwellstr 0.0001

#define glasses 0

// x=-1 is left of screen, x=1 is right
// y and z fit this space
// (0,0,0) is centre of screen

typedef struct {
  V3d pos,vel;
  float gwell;
} Part;

Part part[numparts];

int gwells=0;
int gwell[numparts];

int sgn(int x) {
  if (x>0)
    return 1;
  if (x<0)
    return -1;
  return 0;
}

void newpart(int i) {
  float x,y;
  int j;
  //part[i].pos=(V3d)newV3d(floatrnd(-1,1),floatrnd(-1,1),floatrnd(znear,zfar));
  x=floatrnd(-1,1); y=floatrnd(-1,1);
  part[i].pos.z=floatrnd(zcreatenear,zfar);
  // scrx/pd=x/z
  part[i].pos.x=part[i].pos.z*x/planedist;
  part[i].pos.y=part[i].pos.z*y/planedist;
  part[i].vel=newV3d(0,0,0);
  if (myrnd()<pgwell) {
    part[i].gwell=floatrnd(1,3);
    gwell[gwells]=i; gwells++;
  } else {
    // Ensure not in the gwell list
    for (j=0;j<gwells;j++) {
      if (gwell[j]==i) {
        gwell[j]=gwell[gwells];
        gwells--;
      }
    }
    part[i].gwell=0;
  }
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
  if (abs(scrx-scrwid/2)>scrwid/2 || abs(scry-scrhei/2)>scrhei/2) {
    return 0; // Off screen
  } else {
    circle(screen,scrx,scry,rad,c);
    if (p.gwell>0.5) {
      line(screen,scrx-rad,scry-rad,scrx+rad,scry+rad,c);
      line(screen,scrx-rad,scry+rad,scrx+rad,scry-rad,c);
    }
    return 1; // On screen
  }
}

int plotpart(int i,int c) {
  int b,o=0;
  if (glasses) {
    b=127*((part[i].pos.z-zfar)/(znear-zfar));
    if (plotpart2(part[i],-eyewid,sgn(c)*b))
      o=1;
    if (plotpart2(part[i],eyewid,sgn(c)*(128+b)))
      o=1;
    return o;
  } else
    return plotpart2(part[i],0,c);
}

void movepart(int i) {
  int j;
  //xor_mode(0);
  if (plotpart(i,0)==0 || part[i].pos.z<znear || part[i].pos.z>zfar)
    newpart(i);
  // Brownian motion
  if (myrnd()<pbrownian) {
  part[i].vel.x=part[i].vel.x+floatrnd(-brownian,brownian);
  part[i].vel.y=part[i].vel.y+floatrnd(-brownian,brownian);
  part[i].vel.z=part[i].vel.z+floatrnd(-brownian,brownian);
  }
  if (part[i].gwell<0.5) {
    // Pulled by gravity wells
    for (j=0;j<gwells;j++) {
      // Gravity well j affects particle i
      if (gwell[j]!=i)
        part[i].vel=V3dadd(part[i].vel,V3dmult(part[gwell[j]].gwell/(V3ddist(part[gwell[j]].pos,part[i].pos)+1)*gwellstr,V3dnorm(V3dsub(part[gwell[j]].pos,part[i].pos))));
    }
  } else {
    // Warping towards user
    part[i].vel.z=part[i].vel.z-warpvel;
  }
  // Damping
  part[i].vel=V3dmult(dampening,part[i].vel);
  // Movement
  part[i].pos=V3dadd(part[i].pos,part[i].vel);
  /* Bouncing off the walls
  if (myabs(part[i].pos.x)>1)
    part[i].vel.x=-part[i].vel.x;
  if (myabs(part[i].pos.y)>1)
    part[i].vel.y=-part[i].vel.y;
  if (myabs(part[i].pos.z)>1)
    part[i].vel.z=-part[i].vel.z;*/
  //xor_mode(1);
  plotpart(i,255);
}

int main(String *argv,int argc) {
  int i;
  allegrosetup(scrwid,scrhei);
  mypalette(0,0,0,0);
  mypalette(255,1,1,1);
  if (glasses) {
  mypalette(128,0,0,0);
  for (i=1;i<128;i++) {
    mypalette(i,0.3+0.4*i/128,0,0);
    mypalette(128+i,0,0.3+0.5*i/128,0);
  }
  }
  for (i=0;i<numparts;i++) {
    part[i].gwell=0;
    newpart(i);
  }
  while (!key[KEY_ESC]) {
    for (i=0;i<numparts;i++) {
      movepart(i);
    }
  }
  allegro_exit();
}
