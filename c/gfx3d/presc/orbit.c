// Prescience

//#define NOGRAPHICS 1

#include <joeylib.c>

int scrwid=640;
int scrhei=480;

JBmp j=JBmp(scrwid,scrhei);

V3d pos;
Ori ori;

#define time thetime
float time;
float timecen=0;
float timespan=15.0;

int col;

void plotplanet(V3d cen,float r) {
  int x,y;
  float p=PPgetunitnoadd(cen);
//  cen.print();
//  printf("%f ",V3d::normdot(cen,V3d::k));
  if (p) {
    float rad=r*p;
    PPgetscrposnoadd(cen,&x,&y);
    j.opencircle(x,y,rad,col);
//    printf("%i %i %f\n",x,y,rad);
  }
}

V3d modify(V3d x) {
  return (x-pos).disorientate(ori);
}

void plotscene() {
  j.clear();
  for (time=timecen-timespan;time<=timecen+timespan;time+=1.0) {
    if (timespan==0)
      col=255;
    else
      col=255-255*myabs(time-timecen)/timespan;
    V3d planeta=V3d(2,0,0)+V3d::rotate(V3d(2.1,0,0),V3d(0,2,1),2*pi*time/200.0);
    plotplanet(modify(planeta),0.3);
  }
  j.writetoscreen();
}

void main() {
  pos=V3d(-2,2,-8);
  ori.forcez(V3d(2,0,10));
  allegrosetup(scrwid,scrhei);
  PPsetup(scrwid,scrhei,2);
  makepalette(&greypalette);
  while (!key[KEY_SPACE]) {
    plotscene();
    timecen+=1.1;
  }
}
