#include <joeylib.c>

class S {
public:
  float off,fre,amp;
  S() { }
  S(float a,float b,float c) {
    off=a;
    fre=b;
    amp=c;
  }
};

void main() {
  int scrwid=320; int scrhei=200;
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  List<S> xs=List<S>();
  List<S> ys=List<S>();
  randomise();
  for (int i=0;i<12;i++) {
    S s=S(myrnd()*scrwid,(1.0+myrnd()*4.0)*pi/(float)scrwid,myrnd()*1.0);
    if (i%2)
      xs+s;
    else
      ys+s;
  }
  JBmp j=JBmp(scrwid,scrhei);
  do {
    for (int x=0;x<scrwid;x++)
      for (int y=0;y<scrhei;y++) {       
        float sum=0;
        for (int i=1;i<=xs.len;i++) {
          S s=xs.num(i);
          sum+=s.amp*sin(s.off+s.fre*x);
        }
        for (int i=1;i<=ys.len;i++) {
          S s=ys.num(i);
          sum+=s.amp*sin(s.off+s.fre*y);
        }
        sum=3.0*sum/(float)(xs.len+ys.len);
        j.bmp[y][x]=ucharchop(128+128*sum);
      }
    j.writetoscreen();
    for (int i=1;i<=xs.len;i++) {
      xs.p2num(i)->off+=magrnd(pi/128);
      xs.p2num(i)->fre+=magrnd(pi/(float)10/(float)scrwid);
      xs.p2num(i)->amp+=magrnd(0.005);
    }
    for (int i=1;i<=ys.len;i++) {
      ys.p2num(i)->off+=magrnd(pi/128);
      ys.p2num(i)->fre+=magrnd(pi/(float)10/(float)scrwid);
      ys.p2num(i)->amp+=magrnd(0.005);
    }
  } while (!key[KEY_ESC]);
}
