#include <joeylib.c>

#define scrwid 320
#define scrhei 200
#define numps 200
#define trails 30

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
  int **oldx,**oldy;

  oldx=new (int*)[numps];
  oldy=new (int*)[numps];
  for (int i=0;i<numps;i++) {
    oldx[i]=new int[trails];
    oldy[i]=new int[trails];
  }

    

  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  PPsetup(scrwid,scrhei,2,0);

//  List<V3d> l(5);
  for (int i=1;i<=numps;i++) {
    V3d r=sqrt(myrnd())*(V3d::random());
    l+r;
    m+r.mod();
  }

  JBmp j=JBmp(scrwid,scrhei);
  int time=0;
  int trail=0;
  starttimer();
  while (!key[KEY_ESC]) {
    time++;
    V3d rotaxis=V3d::rotate(V3d::rotatea(V3d(0,1,0),V3d(0,0,1),time*pi/333),V3d(0,1,0),time*pi/247);
//    rotaxis.print();
    V3d::rotatelista(l,rotaxis,pi/200);
//    j.clear();
    for (int i=1;i<=l.length();i++) {
//      V3d t=(V3d)(l.num(i));
//      t.changeto(V3d::rotate(t,rotaxis,pi/20));
//      l.replace(i,t);
      V3d *t=(l.p2num(i));
      PPgetscrpos(*t,&x,&y);
      j.setpixelnodarker(x,y,127*(1-(2*t->z+t->y-t->x)/4));
      j.setpixel(oldx[i-1][trail],oldy[i-1][trail],0);
      oldx[i-1][trail]=x;
      oldy[i-1][trail]=y;
    }
    trail=(trail+1)%trails;
    j.writetoscreen();
    framedone();
  }
  allegro_exit();
  displayframespersecond();
}

