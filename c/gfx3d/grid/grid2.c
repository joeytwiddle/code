#include <joeylib.c>

#define scrwid 320
#define scrhei 200

int gridsize=10;
int gjump=2;

void main() {
  allegrosetup(scrwid,scrhei);
  psychedelicpalette();
  mypalette(255,0,0,0);
  V3d pos=V3d(0,0,0);
  V3d vel=V3d(0,0,0);
  V3d acc=V3d(0,0,0);
  Ori ori;
  float roll=0;
  float yaw=0;
  JBmp b=JBmp(scrwid,scrhei);
  PPsetup(scrwid,scrhei,4);
  do {
    b.clear(255);
    V3d off=V3d(mymod(pos.x,gjump),mymod(pos.y,gjump),mymod(pos.z,gjump));
    int ioff=-mydiv(pos.x,gjump);
    int joff=-mydiv(pos.y,gjump);
    int koff=-mydiv(pos.z,gjump);
    for (int k=gridsize;k>=-gridsize;k-=gjump)
      for (int i=-gridsize;i<=gridsize;i+=gjump)
        for (int j=-gridsize;j<=gridsize;j+=gjump) {
          int x,y;
          V3d cen=V3d(i,j,k)-off;
          cen=V3d::orientate(cen,ori);
          int c=256.0*mymod(5*mysquare((i-ioff)/gjump)-11*(j-joff)/gjump-7*(k-koff)/gjump,6)/6.0;
          PPgetscrpos(cen,&x,&y);
          b.filledcircle(x,y,0.1*PPgetunit(cen),c);
        }
    b.writetoscreen();
    acc=hang(acc,V3d::origin,0.93,0.02);
    vel=hang(vel,V3d::origin,0.93,0)+acc;
    pos=pos+vel;
    roll=hang(roll,0,0.92,0.01);
    yaw=hang(yaw,0,0.92,0.01);
    ori.roll(roll);
    ori.yaw(yaw);
  } while (!key[KEY_ESC]);
}
