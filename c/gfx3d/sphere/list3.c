//#include <maths.c>
#include <list.c>
#include <JBmp.c>
#include <allegro.h>

#define scrwid 320
#define scrhei 200
#define numps 200000

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
    V3d r=V3d::mult(scrhei/4,V3d::random());
    l.add(r);
    m.add(r.mod());
  }
  JBmp j=JBmp(scrwid,scrhei);
  for (int i=1;i<=numps;i++) {
    V3d t=(V3d)(l.num(i));
    j.incpixel(scrwid*2/3+t.x,scrhei/2+t.y);
    j.incpixel(scrwid*1/3+t.z,scrhei/2+t.y);
//    printf(">%f< ",m.num(i));
//    t.print();
  }
  j.writetoscreen();
  // l.foralldo(&dispfn);
  // l.foralldo(&V3d::print);
//  l.foralldo(&V3d::print,"\n");
 // m.foralldo(&printfloat,",");
 while (!key[KEY_ESC]) { }
}

