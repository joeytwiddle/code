#include <joeylib.c>

#define scrwid 320
#define scrhei 200
#define numps 500

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
    l.add(r);
    m.add(r.mod());
  }

  JBmp j=JBmp(scrwid,scrhei);
  int time=0;
  while (!key[KEY_ESC]) {
    time++;
    V3d rotaxis=V3d::rotate(V3d(0,1,0),V3d(1,0,0),time*pi/500);
    V3d rotaxis2=V3d::rotate(V3d(0,1,0),V3d(1,0,0),time*pi/500);
    j.clear();
    for (int i=1;i<=l.length();i++) {
//      V3d t=(V3d)(l.num(i));
//      t.changeto(V3d::rotate(t,rotaxis,pi/40));
//      l.replace(i,t);
      V3d v=(V3d)(l.num(i));
      V3d t=(V3d::rotate((V3d::rotate(v,V3d(1,0,0),pi/700*time)),V3d(0,1,0),pi/40*time));
      j.setpixel(scrwid/2+scrhei/2*t.x,scrhei/2*(1+t.y),127*(1-t.z));
    }
    j.writetoscreen();
  }
}

