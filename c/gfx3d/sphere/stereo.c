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
  int x,y;

  allegrosetup(scrwid,scrhei);
  makestereopalette();
  PPsetup(scrwid,scrhei,2,0);

//  List<V3d> l(5);
  for (int i=1;i<=numps;i++) {
    V3d r=sqrt(myrnd())*(V3d::random());
    l+r;
    m+r.mod();
  }

  JBmp j=JBmp(scrwid,scrhei);
  int time=0;
  while (!key[KEY_ESC]) {
    time++;
    V3d rotaxis=V3d::rotate(V3d(0,1,0),V3d(1,0,0),time*pi/700);
    V3d::rotatelist(l,rotaxis,pi/40);
    j.clear();
    for (int i=1;i<=l.length();i++) {
//      V3d t=(V3d)(l.num(i));
//      t.changeto(V3d::rotate(t,rotaxis,pi/40));
//      l.replace(i,t);
      V3d t=(V3d)(l.num(i));
      PPgetscrpos(t,PPlefteye,&x,&y);
      j.setpixel(x,y,1+63*(1-t.z));
      PPgetscrpos(t,PPrighteye,&x,&y);
      j.setpixel(x,y,129+63*(1-t.z));
//      j.setpixel(0,scrhei/2*(1+t.z),127*(1-t.x));
    }
    j.writetoscreen();
  }
}

