#include <joeylib.c>

#define scrwid 320
#define scrhei 200

int gridsize=6;
int gjump=2;

JBmp left=JBmp(scrwid,scrhei);
JBmp right=JBmp(scrwid,scrhei);
JBmp b=JBmp(scrwid,scrhei);

void plotsphere(V3d cen,int c) {
  int x,y;
  float p=PPgetunit(cen);
  if (p) {
  float rad=0.1*p;
  PPgetscrpos(cen,PPlefteye,&x,&y);
    left.filledcircle(x,y,rad,c);
  PPgetscrpos(cen,PPrighteye,&x,&y);
    right.filledcircle(x,y,rad,8*c);
//  b.filledcircle(x,y,,c);
//  plotpart2(part[i],-eyewid,sgn(c)*b))
//  plotpart2(part[i],eyewid,sgn(c)*(128+b)))
  }
}

void main() {
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  mypalette(255,0,0,0);
  mypalette(0,0,0,0);
  mypalette(255,1,1,1);
  mypalette(128,0,0,0);
  for (int r=0;r<8;r++) {
  for (int g=0;g<8;g++) {
    int palc=r+g*8;
    mypalette(palc,(r==0?0:0.3+0.4*r/7.0),(g==0?0:0.1+0.2*g/7.0),(g==0?0:0.3+0.5*g/7.0));
    }
      }
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
  int frame=0;
  starttimer();
  do {
    frame++;
    float pd=2.0+1.8*sin(2*pi*frame/1000.0);
    PPsetup(scrwid,scrhei,pd);
    left.clear(0);
    right.clear(0);
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
          int x,y;
          V3d cen=V3d(i,j,k)-off;
          cen=V3d::qorientate(cen,ori);
          int c=7-chop(7*(cen.z+pd+1)/(gridsize+2),0,7);
          plotsphere(cen,c);
        }
    for (int i=0;i<scrwid;i++) {
    for (int j=0;j<scrhei;j++) {
      b.bmp[j][i]=(left.bmp[j][i] | right.bmp[j][i]);
    }
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
