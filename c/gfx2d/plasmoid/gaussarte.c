// Interpolated
// 21d centre
// gaussian distributed addition amplitude and radius
// interesting artefacts from uchar modding and interpolation

#include <joeylib.c>

class S {
public:
  float cx,cy,rad,amp;
  S() { 
  }
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
  float offrnd=0.4;
  float offvdamp=0.95;
  float offdamp=0.995;
  float radrnd=0.5;
  float radvdamp=0.9;
  float raddamp=0.9999;
  float amprnd=0.005;
  float ampvdamp=0.99;
  float ampdamp=0.99;
  float ampcen=0;
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  makepalette(&myRGB::hue);
  List<S> xs=List<S>();
  List<S> xvs=List<S>();
  randomise();
  for (int i=0;i<10;i++) {
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
      if (x>0 && y>0) {
        int nwcol=ucharmirror(j.bmp[y-jump][x-jump]-newcol);
        int wcol=ucharmirror(j.bmp[y][x-jump]-newcol);
        int ncol=ucharmirror(j.bmp[y-jump][x]-newcol);
        for (int i=0;i<=jump;i++)
          for (int k=0;k<=jump;k++) {
            float e=(float)i/(float)jump;
            float w=1.0-e;
            float s=(float)k/(float)jump;
            float n=1.0-s;
            j.bmp[y-jump+k][x-jump+i]=ucharmod(newcol+n*w*nwcol+s*w*wcol+n*e*ncol);
            // j.bmp[y-jump+k][x-jump+i]=(1.0-i/(float)jump)*(1.0-k/(float)jump)*nwcol+(1.0-i/(float)jump)*(k/(float)jump)*wcol+(i/(float)jump)*(1.0-k/(float)jump)*ncol+(i/(float)jump)*(k/(float)jump)*newcol;
          }
      }
    }
      for (int i=1;i<=xs.len;i++) {
        S *v=xvs.p2num(i);
        S *x=xs.p2num(i);
        hang(&v->cx,0,offvdamp,offrnd);
        hang(&v->cy,0,offvdamp,offrnd);
        hang(&v->rad,0,radvdamp,radrnd);
        hang(&v->amp,0,0.99,0.2);
        x->add(xvs.num(i));
        hang(&x->cx,scrwid/2,offdamp,0);
        hang(&x->cy,scrhei/2,offdamp,0);
        hang(&x->amp,0,0.995,0);
        hang(&x->rad,radfromamp(ampcen),raddamp,0);
        int c=j.point(x->cx,x->cy)+x->amp;
        j.vliner(x->cx,x->cy,plusminus(i)*x->amp,c);
        j.hliner(x->cx,x->cy,x->rad,c);
      }
      j.writetoscreen();
    } while (!key[KEY_ESC]);
  }
