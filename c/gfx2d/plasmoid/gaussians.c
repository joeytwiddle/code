// Interpolated
// 21d centre
// gaussian distributed addition amplitude and radius

#include <joeylib.c>

class S {
public:
  float cx,cy,rad,amp;
  S() { }
  S(float a,float b,float c,float d) {
    cx=a;
    cy=b;
    rad=c;
    amp=d;
  }
  void add(S o) {
    cx=cx+o.cx;
    cy=cy+o.cy;
    rad=rad+o.rad;
    amp=amp+o.amp;
  }
};

int scrwid=320; int scrhei=200;

float radfromamp(float amp) {
  return amp/64.0*scrwid/3.0;
}

void main() {
  int jump=21;
  float scale=0.3;
  float offrnd=1.0;
  float offvdamp=0.9;
  float offdamp=0.99;
  float radrnd=pi/10/scrwid;
  float radvdamp=0.99;
  float raddamp=0.9999;
  float amprnd=0.005;
  float ampvdamp=0.99;
  float ampdamp=0.99;
  float ampcen=40;
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  makepalette(&myRGB::hue);
  List<S> xs=List<S>();
  List<S> xvs=List<S>();
  randomise();
  for (int i=0;i<25;i++) {
    float amp=ampcen;
    S s=S(myrnd()*scrwid,myrnd()*scrhei,radfromamp(amp),amp);
    S v=S(0,0,0,0);
    xs+s;
    xvs+v;
  }
  JBmp j=JBmp(scrwid,scrhei);
  do {
    for (int x=0;x<scrwid;x+=jump)
      for (int y=0;y<scrhei;y+=jump) {       
        float sum=0;
        for (int i=1;i<=xs.len;i++) {
          S s=xs.num(i);
          sum+=plusminus(i)*s.amp*gaussian(sqrt(mysquare(x-s.cx)+mysquare(y-s.cy))/s.rad);
        }
        sum=scale*sum/(float)(xs.len);
        int newcol=ucharchop(128+128*sum);
        j.bmp[y][x]=newcol;
        if (x>0 && y>0)
          for (int i=0;i<=jump;i++)
            for (int k=0;k<=jump;k++)
              j.bmp[y-jump+k][x-jump+i]=(1.0-i/(float)jump)*(1.0-k/(float)jump)*j.bmp[y-jump][x-jump]+(1.0-i/(float)jump)*(k/(float)jump)*j.bmp[y][x-jump]+(i/(float)jump)*(1.0-k/(float)jump)*j.bmp[y-jump][x]+(i/(float)jump)*(k/(float)jump)*newcol;
      }
    for (int i=1;i<=xs.len;i++) {
      S *v=xvs.p2num(i);
      S *x=xs.p2num(i);
      v->cx=hang(v->cx,0,offvdamp,offrnd);
      v->cy=hang(v->cy,0,offvdamp,offrnd);
      v->rad=hang(v->rad,0,radvdamp,radrnd);
      v->amp=hang(v->amp,0,0.9,0.4);
      x->add(xvs.num(i));
      x->cx=hang(x->cx,scrwid/2,offdamp,0);
      x->cy=hang(x->cy,scrhei/2,offdamp,0);
      x->amp=hang(x->amp,ampcen,0.995,0);
      x->rad=hang(x->rad,radfromamp(x->amp),raddamp,0);
      j.hline(scrwid/2,i*6,scrwid/2+x->amp,128);
      j.hline(scrwid/2,i*6+2,scrwid/2+x->rad,128);
      j.setpixel(x->cx,x->cy,x->amp);
    }
    j.writetoscreen();
  } while (!key[KEY_ESC]);
}
