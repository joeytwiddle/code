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
  allegrosetup(scrwid,scrhei);
  makepalette(&(greypalette));
  JBmp j=JBmp(scrwid,scrhei);
  while (!key[KEY_ESC]) {
    for (int loop=1;loop<20;loop++) {
    V3d t=V3d::mult(scrhei/2.2,V3d::randomvolume());
    if (t.z>-.1 && t.z<.1) {
      j.incpixel(scrwid/2+t.x,scrhei/2+t.y,255);
    }
    }
    j.writetoscreen();
  }
}

