//line(screen,myrnd()*scrwid,myrnd()*scrhei,myrnd()*scrwid,myrnd()*scrhei,myrnd()*255);

#include <stdlib.h>
#include <joeylib.c>

#define scrwid 640
#define scrhei 480

#define numparts 80
#define pgwell 0.01
#define ptwist 0.02

#define planedist 6
#define eyewid 0.1
#define znear 0.1
#define zcreatenear 4
#define zfar 20

#define dampening .99
#define partrad 0.04
#define brownian 0.0003
#define pbrownian 1
#define warpvel 0.0001
#define gwellstr 0.0001
#define twiststr 0.01

#define glasses 1

// x=-1 is left of screen, x=1 is right
// y and z fit this space
// (0,0,0) is centre of screen

typedef struct {
  V3d pos,vel;
  int gwell; // Gravity well
  float gwellpow;
  int twist; // Twister
  V3d twistaxis;
  float twistpow;
} Part;

Part part[numparts];

int gwells=0;
int gwell[numparts];
int twists=0;
int twist[numparts];

int sgn(int x) {
  if (x>0)
    return 1;
  if (x<0)
    return -1;
  return 0;
}

void newpart(int i) {
  float x,y,p;
  int j;
  //part[i].pos=V3d(floatrnd(-1,1),floatrnd(-1,1),floatrnd(znear,zfar));
  x=floatrnd(-1,1); y=floatrnd(-1,1);
  part[i].pos.z=floatrnd(zcreatenear,zfar);
  // scrx/pd=x/z
  part[i].pos.x=part[i].pos.z*x/planedist;
  part[i].pos.y=part[i].pos.z*y/planedist;
  part[i].vel=V3d(0,0,0);
  // Ensure not in the gwell list
  for (j=0;j<gwells;j++) {
    if (gwell[j]==i) {
      gwell[j]=gwell[gwells];
      gwells--;
    }
  }
  // Ensure not in the twist list
  for (j=0;j<twists;j++) {
    if (twist[j]==i) {
      twist[j]=twist[twists];
      twists--;
    }
  }
  p=myrnd();
  if (p<pgwell) {
    part[i].gwell=1;
    part[i].gwellpow=floatrnd(1,3);
    gwell[gwells]=i; gwells++;
  } else
    part[i].gwell=0;
  if (p>pgwell && p<pgwell+ptwist) {
    part[i].twist=1;
    part[i].twistaxis=V3d::random();
    part[i].twistpow=floatrnd(-.1,.1);
    twist[twists]=i; twists++;
//    printf("twist %f ",part[i].twistpow); V3d::print(part[i].twistaxis); printf("\n");
  } else
    part[i].twist=0;
}

void findplanehit(V3d eye,V3d far,float plane,float *x,float *y) {
  // (y-eyey)/(plane-eyez)=(fary-eyey)/(farz-eyez)
  *x=eye.x+(plane-eye.z)*(far.x-eye.x)/(far.z-eye.z);
  *y=eye.y+(plane-eye.z)*(far.y-eye.y)/(far.z-eye.z);
}

int plotpart2(Part p,float eyex,int c) {
  float x,y;
  int scrx,scry,rad;
  findplanehit(V3d(eyex,0,0),p.pos,planedist,&x,&y);
  scrx=scrwid/2+scrwid/2*x;
  scry=scrhei/2-scrwid/2*y;
  findplanehit(V3d(eyex,0,0),p.pos+V3d(partrad,0,0),planedist,&x,&y);
  rad=scrwid/2+scrwid/2*x-scrx;
  if (abs(scrx-scrwid/2)>scrwid/2 || abs(scry-scrhei/2)>scrhei/2) {
    return 0; // Off screen
  } else {
    circle(screen,scrx,scry,rad,c);
    if (p.gwell) {
      line(screen,scrx-rad*p.gwellpow,scry-rad*p.gwellpow,scrx+rad*p.gwellpow,scry+rad*p.gwellpow,c);
      line(screen,scrx-rad*p.gwellpow,scry+rad*p.gwellpow,scrx+rad*p.gwellpow,scry-rad*p.gwellpow,c);
    }
    if (p.twist) {
      findplanehit(V3d(eyex,0,0),p.pos+p.twistaxis,planedist,&x,&y);
      line(screen,scrx,scry,scrwid/2+scrwid/2*x,scrhei/2-scrwid/2*y,c);
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
  if (part[i].gwell) {
    // Warping towards user
    part[i].vel.z=part[i].vel.z-warpvel;
  } else if (part[i].twist) {
  } else {
    // Pulled by gravity wells
    for (j=0;j<gwells;j++) {
      // Gravity well j affects particle i
      if (gwell[j]!=i)
        part[i].vel=part[i].vel+(part[gwell[j]].gwellpow/V3d::dist(part[gwell[j]].pos,part[i].pos)+1)*gwellstr*V3d::norm(part[gwell[j]].pos-part[i].pos);
    }
    // Twisted by twisters
    for (j=0;j<twists;j++) {
      //part[i].vel=V3dadd(part[i].vel,V3dmult(twiststr*part[twist[j]].twistpow/mysquare(V3ddist(part[i].pos,part[twist[j]].pos)),V3dnormcross(part[twist[j]].twistaxis,V3dsub(part[i].pos,part[twist[j]].pos))));
      if (twist[j]!=i)
        part[i].pos=part[twist[j]].pos+V3d::rotate(part[i].pos-part[twist[j]].pos,part[twist[j]].twistaxis,part[twist[j]].twistpow*twiststr/mysquare(V3d::dist(part[i].pos,part[twist[j]].pos)));
    }
  }
  // Damping
  part[i].vel=dampening*part[i].vel;
  // Movement
  part[i].pos=part[i].pos+part[i].vel;
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
