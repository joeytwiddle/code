#include <joeylib.c>

#define scrwid 320
#define scrhei 200
#define numps 5000

void dispfn(V3d p) {
  p.print();
}

void printfloat(float f) {
  printf("%f",f);
}

int main() {
  List<V3d> l;
  List<float> m;
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
//  List<V3d> l(5);
  for (int i=1;i<=numps;i++) {
    V3d r=sqrt(myrnd())*(V3d::random());
    r=quantiseto(0.2,r.mod())*r.norm();
    l.add(r);
    m.add(r.mod());
  }

  JBmp j=JBmp(scrwid,scrhei);
  int time=1;
  int type=1;
      starttimer();
  while (!key[KEY_ESC]) {
    if ((time%50)==0) {
      type=1-type;
      displayframespersecond();
      starttimer();
    }
    time++;
    V3d rotaxis=V3d::rotate(V3d(0,1,0),V3d(1,0,0),time*pi/500);
    if (!type)
      V3d::rotatelist(l,rotaxis,pi/40);
    j.clear();
    for (int i=1;i<=l.length();i++) {
//      V3d t=(V3d)(l.num(i));
//      t.changeto(V3d::rotate(t,rotaxis,pi/40));
//      l.replace(i,t);
      V3d t=(V3d)(l.num(i));
/*      V3d t;
      if (mymod(time/100)<.5)
        t=(V3d::rotate(v,rotaxis,pi/40*time));
      else
        t=(V3d::oldrotate(v,rotaxis,pi/40*time));*/
if (type) {
      t=(V3d::rotate(t,rotaxis,(float)pi/40));
      l.replace(i,t);
      }
      j.setpixel(scrwid/2+scrhei/2*t.x,scrhei/2*(1+t.y),127*(1-t.z));
      j.setpixel(0,scrhei/2*(1+t.z),127*(1-t.x));
    }
    j.writetoscreen();
    framedone();
  }
}

