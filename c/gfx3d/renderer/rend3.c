#include <joeylib.c>

void main() {
  int scrwid=320;
  int scrhei=200;
  PPsetup(scrwid,scrhei,4.0);
  Octree<Renderable*> o=Octree<Renderable*>(V3d::o,10.0,&getpos);
  for (int i=0;i<=10000;i++) {
    if (myrnd()<0.5)
      o.add(new Sphere(5.0*V3d::random(),myrnd()*0.1));
    else {
      V3d tmp=5.0*V3d::random();
      o.add(new Triangle3d(tmp+V3d::random(0.4),tmp+V3d::random(0.4),tmp+V3d::random(0.4)));
    }
  }
  Ori ori=Ori();
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  do {
    Viewpoint v=Viewpoint(V3d::o,ori);
    render(&o,&v);
    ori.roll(0.004);
    ori.yaw(0.008);
  } while (!key[KEY_SPACE]);
}
