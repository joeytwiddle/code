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
  int jump=5;
  float offrnd=pi/128;
  float offdamp=0.999;
  float frernd=pi/10/scrwid;
  float fredamp=0.99;
  float amprnd=0.005;
  float ampdamp=0.99;
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
  float yweird=0.5;
  float xweird=0.5;
  do {
    for (int x=0;x<scrwid;x+=jump)
      for (int y=0;y<scrhei;y+=jump) {       
        float sum=0;
        for (int i=1;i<=xs.len;i++) {
          S s=xs.num(i);
          int fakex=scrwid/2+(x-scrwid/2)*(1.0-xweird*y/scrhei);
          sum+=s.amp*sin(s.off+s.fre*fakex);
        }
        for (int i=1;i<=ys.len;i++) {
          S s=ys.num(i);
          int fakey=y*(1.0-yweird+yweird*(1.0-y/scrhei));
          sum+=s.amp*sin(s.off+s.fre*y);
        }
        sum=3.0*sum/(float)(xs.len+ys.len);
        j.bmp[y][x]=ucharchop(128+128*sum);
        if (x>0 && y>0)
          for (int i=0;i<=jump;i++)
            for (int k=0;k<=jump;k++)
              j.bmp[y-jump+k][x-jump+i]=(1.0-i/(float)jump)*(1.0-k/(float)jump)*j.bmp[y-jump][x-jump]+(1.0-i/(float)jump)*(k/(float)jump)*j.bmp[y][x-jump]+(i/(float)jump)*(1.0-k/(float)jump)*j.bmp[y-jump][x]+(i/(float)jump)*(k/(float)jump)*j.bmp[y][x];
      }
    j.writetoscreen();
    for (int i=1;i<=xs.len;i++) {
      xvs.p2num(i)->off=xvs.p2num(i)->off*offdamp+magrnd(offrnd);
      xvs.p2num(i)->fre=xvs.p2num(i)->fre*fredamp+magrnd(frernd);
      xvs.p2num(i)->amp=xvs.p2num(i)->amp*ampdamp+magrnd(amprnd);
      xs.p2num(i)->add(xvs.num(i));
      xs.p2num(i)->fre=xs.p2num(i)->fre*fredamp;
    }
    for (int i=1;i<=ys.len;i++) {
      yvs.p2num(i)->off=yvs.p2num(i)->off*offdamp+magrnd(offrnd);
      yvs.p2num(i)->fre=yvs.p2num(i)->fre*fredamp+magrnd(frernd);
      yvs.p2num(i)->amp=yvs.p2num(i)->amp*ampdamp+magrnd(amprnd);
      ys.p2num(i)->add(yvs.num(i));
      ys.p2num(i)->fre=ys.p2num(i)->fre*fredamp;
    }
    xweird=(xweird+magrnd(0.1)-.5)*.99+.5;
    yweird=(yweird+magrnd(0.1)-.5)*.99+.5;
  } while (!key[KEY_ESC]);
}
