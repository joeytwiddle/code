// Interpolated
// 21d centre
// gaussian distributed addition amplitude and radius
// using map to remove artefacts

#include <joeylib.c>

#define debug false

int scrwid=320; int scrhei=200;
int jump=8;
float scale=0.45;
float offrnd=0.01;
float offvdamp=0.99;
float offdamp=0.95;
float ampcen=0;

class S {
public:
  V3d pos;
  float rad,amp;
  S() { 
  }
  S(V3d a,float c,float d) {
    pos=a;
    rad=c;
    amp=d;
  }
  void add(S o) {
    pos=pos+o.pos;
    rad=myabs(rad+o.rad);
    amp=amp+o.amp;
  }
};

List<S> xs=List<S>();
List<S> xvs=List<S>();

float radfromamp(float amp) {
  return 0.1+0.01*amp/1024.0;
}

void domovement(S *v,S *x) {
  v->pos=v->pos*offvdamp+V3d::randomvolume(offrnd);
  hang(&(v->rad),0,0.99,0.05);
  hang(&(v->amp),0,0.99,0.5);
  x->add(*v);
  x->pos=x->pos*offdamp;
  hang(&(x->amp),0,0.95,0);
  hang(&(x->rad),radfromamp(x->amp),0.9,0);
  if (x->rad<0.05)
    x->rad=0.05;
}

float getpotentialfor(V3d here) {
      float sum=0;
      for (int i=1;i<=xs.len;i++) {
        S s=xs.num(i);
        sum+=plusminus(i)*s.amp*gaussian(V3d::dist(here,s.pos)/s.rad);
      }
      sum=scale*sum/(float)(xs.len);
  return sum;
}

void main() {
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  makepalette(&myRGB::hue);
  randomise();
  for (int i=0;i<20;i++) {
    float amp=0;
    S s=S(V3d::randomvolume(1),radfromamp(amp),0);
    S v=S(V3d(0,0,0),0,0);
    xs+s;
    xvs+v;
  }
  for (int rndloop=0;rndloop<100;rndloop++) {
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
      V3d here=V3d((x-scrwid/2)*2.0/scrhei,-1.0+2.0*y/scrhei,0);
      float sum=getpotentialfor(here);
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
      /* if (debug) {
           // int c=j.point(x->cx,x->cy)+x->amp;
           int c=0;
           j.vliner(x->cx,x->cy,plusminus(i)*x->amp,c);
           j.hliner(x->cx,x->cy,x->rad,c);
           j.setpixel(x->cx+radfromamp(x->amp),x->cy,c);
         }*/
    }
    j.writetoscreen();
  } while (!key[KEY_ESC]);
}
