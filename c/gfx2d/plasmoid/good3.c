// Interpolated
// 21d centre
// gaussian distributed addition amplitude and radius
// using map to remove artefacts

#include <joeylib.c>

//#define debug true
#define debug false

int scrwid=320; int scrhei=200;
  int jump=8;
  float scale=2.0;
  float offrnd=1.0;
  float offvdamp=0.99;
  float offdamp=0.95;
  float ampcen=0;

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
    rad=myabs(rad+o.rad);
    amp=amp+o.amp;
  }
};

float radfromamp(float amp) {
  return 15+amp/1024.0*scrwid/2.0;
}

void domovement(S *v,S *x) {
        hang(&(v->cx),0,offvdamp,offrnd);
        hang(&(v->cy),0,offvdamp,offrnd);
        hang(&(v->rad),0,0.99,2);
        hang(&(v->amp),0,0.99,0.2);
        x->add(*v);
        hang(&(x->cx),scrwid/2,offdamp,0);
        hang(&(x->cy),scrhei/2,offdamp,0);
        hang(&(x->amp),0,0.9,0);
        hang(&(x->rad),radfromamp(x->amp),0.95,0);
        if (x->rad<30)
          x->rad=30;
}

void main() {
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  makepalette(&myRGB::hue);
  List<S> xs=List<S>();
  List<S> xvs=List<S>();
  randomise();
  for (int i=0;i<20;i++) {
    float amp=0;
    S s=S(myrnd()*scrwid,myrnd()*scrhei,radfromamp(amp),0);
    S v=S(0,0,0,0);
    xs+s;
    xvs+v;
  }
  for (int rndloop=0;rndloop<10000;rndloop++) {
  for (int i=0;i<20;i++) {
    
        S *v=xvs.p2num(i);
        S *x=xs.p2num(i);
        domovement(v,x);
      }
      }
  JBmp j=JBmp(scrwid,scrhei);
  Map2d<int> intmap=Map2d<int>(scrwid,scrhei);
  do {
    for (int x=0;x<scrwid;x+=jump)
    for (int y=0;y<scrhei;y+=jump) {       
      float sum=0;
      for (int i=1;i<=xs.len;i++) {
        S s=xs.num(i);
        sum+=plusminus(i)*s.amp*gaussian(sqrt(mysquare(x-s.cx)+mysquare(y-s.cy))/s.rad);
      }
      sum=scale*sum/(float)(xs.len);
      int newcol=128+128*sum;
      intmap.pos[x][y]=newcol;
      if (x>0 && y>0) {
        int nwcol=intmap.pos[x-jump][y-jump];
        int wcol=intmap.pos[x-jump][y];
        int ncol=intmap.pos[x][y-jump];
        for (int i=0;i<=jump;i++)
          for (int k=0;k<=jump;k++) {
            float e=(float)i/(float)jump;
            float w=1.0-e;
            float s=(float)k/(float)jump;
            float n=1.0-s;
            j.bmp[y-jump+k][x-jump+i]=ucharmod(s*e*newcol+n*w*nwcol+s*w*wcol+n*e*ncol);
            // j.bmp[y-jump+k][x-jump+i]=(1.0-i/(float)jump)*(1.0-k/(float)jump)*nwcol+(1.0-i/(float)jump)*(k/(float)jump)*wcol+(i/(float)jump)*(1.0-k/(float)jump)*ncol+(i/(float)jump)*(k/(float)jump)*newcol;
          }
      }
    }
      for (int i=1;i<=xs.len;i++) {
        S *v=xvs.p2num(i);
        S *x=xs.p2num(i);
        domovement(v,x);
        int c=j.point(x->cx,x->cy)+x->amp;
        if (debug) {
        j.vliner(x->cx,x->cy,plusminus(i)*x->amp,c);
        j.hliner(x->cx,x->cy,x->rad,c);
        j.setpixel(x->cx+radfromamp(x->amp),x->cy,c);
        }
      }
      j.writetoscreen();
    } while (!key[KEY_ESC]);
  }
