#include <joeylib.c>

#define scrwid 320
#define scrhei 200

int gridsize=10;
int gjump=2;

void main() {
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  mypalette(255,0,0,0);
  V3d pos=V3d(0,0,0);
  V3d vel=V3d(0,0,0);
  V3d acc=V3d(0,0,0);
  Ori ori;
  float roll=0;
  float yaw=0;
  JBmp b=JBmp(scrwid,scrhei);
  int frame=0;
  starttimer();
  do {
    frame++;
    PPsetup(scrwid,scrhei,2.0+1.8*sin(2*pi*frame/1000.0));
    b.clear(255);
//    b.fadeby(16);
    V3d off=V3d(mymod(pos.x,gjump),mymod(pos.y,gjump),mymod(pos.z,gjump));
    int ioff=-mydiv(pos.x,gjump);
    int joff=-mydiv(pos.y,gjump);
    int koff=-mydiv(pos.z,gjump);
    ori.quickorisetup();
    int ks=mysgn(ori.qz.z);
    int js=-mysgn(ori.qz.y);
    int is=-mysgn(ori.qz.x);
    for (int k=ks*gridsize;k!=-ks*gridsize;k=k-ks*gjump)
      for (int i=-is*gridsize;i!=is*gridsize;i=i+is*gjump)
        for (int j=-js*gridsize;j!=js*gridsize;j=j+js*gjump) {
          int x,y;
          V3d cen=V3d(i,j,k)-off;
          if (((j+joff+js*gridsize)/gjump)%2==0) {
            cen.x+=gjump/2;
            cen.z+=gjump/2;
          }
          cen=V3d::qorientate(cen,ori);
          if (cen.z>-4) {
            int c=255-ucharchop(255*(cen.z+2)/(gridsize+2));
            PPgetscrpos(cen,&x,&y);
            b.filledcircle(x,y,0.1*PPgetunit(cen),c);
          }
        }
    b.writetoscreen();
    acc=hang(acc,V3d::origin,0.9,0.01);
    vel=hang(vel,ori.qz/10,0.99,0)+acc;
    pos=pos+vel;
    roll=hang(roll,0,0.92,0.01);
    yaw=hang(yaw,0,0.92,0.01);
    ori.roll(roll);
    ori.yaw(yaw);
    framedone();
  } while (!key[KEY_ESC]);
  savetimer();
  allegro_exit();
  displayframespersecond();
}
