#include <joeylib.c>

void main() {
  int scrwid=320;
  int scrhei=200;
  PPsetup(scrwid,scrhei,1.5);
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
    JBmp j=JBmp(scrwid,scrhei,0);
    Frustrum f=Frustrum(V3d::o,ori,0.1,20);
//    OrderedList<Renderable*> *ol=o.getorderedlist(f);
    List<Renderable*> *ol=o.getclippedlist(f);
    Viewpoint v=Viewpoint(V3d::o,ori);
    for (int i=1;i<=ol->len;i++) {
      Renderable *r=ol->num(ol->len+1-i);
      r->render(&j,&v);
//      printg(Sformat("%f,%f,%f",r->cen.x,r->cen.y,r->cen.z),1);
    }
    j.writetoscreen();
//    printf(Sformat("%i %i",o.totalobjs,ol->len),0);
    ori.roll(0.004);
    ori.yaw(0.008);
  } while (!key[KEY_SPACE]);
}
