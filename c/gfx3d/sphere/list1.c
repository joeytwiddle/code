#include <list.c>

void dispfn(V3d p) {
  p.print();
}

void printfloat(float f) {
  printf("%f",f);
}

int main() {
  List<V3d> l;
  List<float> m;
//  List<V3d> l(5);
  for (int i=1;i<=20;i++) {
    V3d r=V3d::random();
    l.add(r);
    m.add(r.mod());
  }
/*  for (int i=1;i<=20;i++) {
    V3d t=(V3d)(l.num(i));
    printf(">%f< ",m.num(i));
    t.print();
  }*/
  // l.foralldo(&dispfn);
  // l.foralldo(&V3d::print);
  l.foralldo(&V3d::print,"\n");
  m.foralldo(&printfloat,",");
}

