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
  void add(S o) {
    off=off+o.off;
    fre=fre+o.fre;
    amp=amp+o.amp;
  }
};

void main() {
  int scrwid=320; int scrhei=200;
  int jump=10;
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  List<S> xs=List<S>();
  List<S> ys=List<S>();
  List<S> xvs=List<S>();
  List<S> yvs=List<S>();
  randomise();
  for (int i=0;i<12;i++) {
    S s=S(myrnd()*scrwid,(1.0+myrnd()*4.0)*pi/(float)scrwid,myrnd()*1.0);
    S v=S(0,0,0);
    if (i%2) {
      xs+s;
      xvs+v;
    } else {
      ys+s;
      yvs+v;
    }
  }
  JBmp j=JBmp(scrwid,scrhei);
  do {
    for (int x=0;x<scrwid;x+=jump)
      for (int y=0;y<scrhei;y+=jump) {       
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
        int se=ucharchop(128+128*sum);
        j.bmp[y][x]=se;
        if (x>0 && y>0) {
          int nw=j.bmp[y-jump][x-jump];
          int ne=j.bmp[y-jump][x];
          int sw=j.bmp[y][x-jump];
          for (int i=0;i<=jump;i++)
            for (int k=0;k<=jump;k++) {
              float a=(float)i/(float)jump;
              float na=1.0-a;
              float b=(float)k/(float)jump;
              float nb=1.0-b;
              j.bmp[y-jump+k][x-jump+i]=na*nb*nw+a*nb*ne+na*b*sw+a*b*se;
            }
        }
      }
    j.writetoscreen();
    for (int i=1;i<=xs.len;i++) {
      xvs.p2num(i)->off+=magrnd(pi/128);
      xvs.p2num(i)->fre+=magrnd(pi/(float)10/(float)scrwid);
      xvs.p2num(i)->amp+=magrnd(0.005);
      xs.p2num(i)->add(xvs.num(i));
    }
    for (int i=1;i<=ys.len;i++) {
      yvs.p2num(i)->off+=magrnd(pi/128);
      yvs.p2num(i)->fre+=magrnd(pi/(float)10/(float)scrwid);
      yvs.p2num(i)->amp+=magrnd(0.005);
      ys.p2num(i)->add(yvs.num(i));
    }
  } while (!key[KEY_ESC]);
}
