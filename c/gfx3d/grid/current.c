#include <joeylib.c>

#define scrwid 320
#define scrhei 200

int gridsize=10;
int gjump=2;

void main() {
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
//  mypalette(255,0,0,0);
//  randomise();
  V3d pos=V3d(0,0,0);
  V3d vel=V3d(0,0,0);
  V3d acc=V3d(0,0,0);
  Ori ori;
  float droll=0;
  float dyaw=0;
  float roll=0;
  float yaw=0;
  float pitch=0;
  JBmp b=JBmp(scrwid,scrhei);
  int frame=0;
  starttimer();
  do {
    frame++;
    float pd=2.0+1.8*sin(2*pi*frame/1000.0);
    PPsetup(scrwid,scrhei,pd,0);
    b.clear(0);
//    b.fadeby(16);
    V3d off=V3d(mymod(pos.x,gjump),mymod(pos.y,gjump),mymod(pos.z,gjump));
    int ioff=-mydiv(pos.x,gjump);
    int joff=-mydiv(pos.y,gjump);
    int koff=-mydiv(pos.z,gjump);
    ori.quickorisetup();
    // Need to find largest axis, and do loops with that one outside
    int ks=mysgn(ori.qz.z);
    int js=-mysgn(ori.qz.y);
    int is=-mysgn(ori.qz.x);
    for (int k=ks*gridsize;k!=-ks*gridsize;k=k-ks*gjump)
      for (int i=-is*gridsize;i!=is*gridsize;i=i+is*gjump)
        for (int j=-js*gridsize;j!=js*gridsize;j=j+js*gjump) {
          V3d cen=V3d(i,j,k)-off;
          cen=V3d::qorientate(cen,ori);
          int c=255-ucharchop(255*(cen.z+pd+1)/(gridsize+2));
          PPlinenoadd(&b,V3d::o,cen,cen-0.1*vel.norm(),0.04,c);
        }
    b.writetoscreen();
    acc=hang(acc,V3d::origin,0.95,0.01);
    vel=hang(vel,ori.qz/10,0.94,0)+acc;
    pos=pos+vel;
    droll=hang(droll,0,0.9,0.01);
    dyaw=hang(dyaw,0,0.95,0.01);
    roll=hang(roll,0,0.95,0)+droll;
    yaw=hang(yaw,0,0.92,0)+dyaw;
    pitch=hang(pitch,0,0.999,0.01);
    pos=pos+ori.qz*4;
    ori.roll(roll/5.0);
    ori.yaw(yaw/5.0);
    pos=pos-ori.z()*4;
    framedone();
  } while (!key[KEY_SPACE] && !key[KEY_ESC]);
  savetimer();
  allegro_exit();
  displayframespersecond();
}
