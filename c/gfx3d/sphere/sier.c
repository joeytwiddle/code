#include <joeylib.c>

#define scrwid 320
#define scrhei 200
#define numps 10000

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
  V3d r=V3d(0,0,0);
  V3d ca=V3d::rotate(V3d(0,1,0),V3d(0,0,1),112*2*pi/360);
  V3d cb=V3d::rotate(V3d::rotate(V3d(0,1,0),V3d(0,0,1),112*2*pi/360),V3d(0,1,0),2*pi/3);
  V3d cc=V3d::rotate(V3d::rotate(V3d(0,1,0),V3d(0,0,1),112*2*pi/360),V3d(0,1,0),-2*pi/3);
  V3d cd=V3d(0,1,0);
  for (int i=1;i<=numps;i++) {
    float tmp=myrnd();
    if (tmp<.25)
      r=(r+ca)/2;
    else if (tmp<.5)
      r=(r+cb)/2;
    else if (tmp<.75)
      r=(r+cc)/2;
    else
      r=(r+cd)/2;
//    V3d r=sqrt(sqrt(myrnd()))*(V3d::random());
//    r.y=quantiseto(0.3,r.y);
    l+r;
    m.add(r.mod());
  }

  JBmp j=JBmp(scrwid,scrhei);
  int time=0;
  starttimer();
  while ((key[KEY_ESC]==0)) {
    time++;
    V3d rotaxis=(V3d::rotate(V3d(0,1,0),V3d(1,0,0),time*pi/500));
    V3d::rotatelist(l,rotaxis,pi/40);
    j.clear();
    for (int i=1;i<=l.length();i++) {
//      V3d t=(V3d)(l.num(i));
//      t.changeto(V3d::rotate(t,rotaxis,pi/40));
//      l.replace(i,t);
      V3d t=(V3d)(l.num(i));
      j.setpixel(scrwid/2+scrhei/2*t.x,scrhei/2*(1+t.y),127*(1-t.z));
//      j.setpixel(0,scrhei/2*(1+t.z),127*(1-t.x));
    }
    j.writetoscreen();
    framedone();
  }
  allegro_exit();
  displayframespersecond();
}

