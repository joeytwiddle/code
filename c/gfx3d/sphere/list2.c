//#include <maths.c>
#include <list.c>
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
//  List<V3d> l(5);
  for (int i=1;i<=numps;i++) {
    V3d r=V3d::add(V3d(scrwid/2,scrhei/2,0),V3d::mult(scrhei/2,V3d::random()));
    l.add(r);
    m.add(r.mod());
  }
  for (int i=1;i<=numps;i++) {
    V3d t=(V3d)(l.num(i));
    putpixel(screen,t.x,t.y,255);
    putpixel(screen,0,scrhei/2+t.z*scrhei/2,255);
//    printf(">%f< ",m.num(i));
//    t.print();
  }
  // l.foralldo(&dispfn);
  // l.foralldo(&V3d::print);
//  l.foralldo(&V3d::print,"\n");
 // m.foralldo(&printfloat,",");
 while (!key[KEY_ESC]) { }
}

