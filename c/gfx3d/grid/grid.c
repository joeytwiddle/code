#include <joeylib.c>

#define scrwid 320
#define scrhei 200

void main() {
  allegrosetup(scrwid,scrhei);
  psychedelicpalette();
  mypalette(255,0,0,0);
  V3d pos=V3d(0,0,0);
  V3d vel=V3d(0,0,0);
  V3d acc=V3d(0,0,0);
  JBmp b=JBmp(scrwid,scrhei);
  PPsetup(scrwid,scrhei,4);
  do {
    b.clear(255);
    V3d off=V3d(mymod(pos.x,2),mymod(pos.y,2),mymod(pos.z,2));
    for (int k=23;k>=3;k-=2)
      for (int i=-6;i<=6;i+=2)
        for (int j=-6;j<=6;j+=2) {
          int x,y;
          V3d cen=V3d(i,j,k)-off;
          int c=256.0*mymod(5*mysquare(i/2)-11*j/2-7*k/2,6)/6.0;
          PPgetscrpos(cen,&x,&y);
          b.filledcircle(x,y,0.1*PPgetunit(cen),c);
        }
    b.writetoscreen();
    acc=hang(acc,V3d::origin,0.9,0.01);
    vel=hang(vel,V3d::origin,0.99,0)+acc;
    pos=pos+vel;
  } while (!key[KEY_ESC]);
}
