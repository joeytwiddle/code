// Interpolated
// 21d centre
// gaussian distributed addition amplitude and radius
// using map to remove artefacts

#include <joeylib.c>

#define debug false

int scrwid=320; int scrhei=200;
int rscrwid=640; int rscrhei=480;
  int jump=20;
  int sjump=8;
  float scale=0.2;
  float offrnd=1.0;
  float offvdamp=0.99;
  float offdamp=0.95;
  float ampcen=0;
  Map2d<int> intmap;

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
        hang(&(v->amp),0,0.99,0.5);
        x->add(*v);
        hang(&(x->cx),scrwid/2,offdamp,0);
        hang(&(x->cy),scrhei/2,offdamp,0);
        hang(&(x->amp),0,0.95,0);
        hang(&(x->rad),radfromamp(x->amp),0.9,0);
        if (x->rad<30)
          x->rad=30;
}

  List<S> xs=List<S>();
  List<S> xvs=List<S>();

V3d hillfor(int x,int y) {
  if (intmap.pos[x][y]==0) {
      float sum=0;
      for (int i=1;i<=xs.len;i++) {
        S s=xs.num(i);
        sum+=plusminus(i)*s.amp*sin(sqrt(mysquare(x-s.cx)+mysquare(y-s.cy))/s.rad*2);
      }
      sum=scale*sum/(float)(xs.len);
      int newcol=128+128*sum;
      intmap.pos[x][y]=newcol;
  }
        V3d v=V3d((2.0*(float)(x-scrwid/2)/scrhei)*3,1-2*(float)intmap.pos[x][y]/255/1.5,(-1+2.0*(float)y/scrhei)*3);
        V3d vr=v.rotate(V3d(0,1,0),-pi/16);
        V3d vrr=vr.rotate(V3d(1,0,0),-pi/4);
        vrr=vrr+V3d(0,0,4);
  return vrr;
}

void main() {
  allegrosetup(rscrwid,rscrhei);
  makepalette(&greypalette);
//  makepalette(&myRGB::hue);
  PPsetup(rscrwid,rscrhei,2);
  randomise();
  for (int i=0;i<20;i++) {
    float amp=0;
    S s=S(myrnd()*scrwid,myrnd()*scrhei,radfromamp(amp),0);
    S v=S(0,0,0,0);
    xs+s;
    xvs+v;
  }
  for (int rndloop=0;rndloop<1000;rndloop++) {
  for (int i=0;i<20;i++) {
        S *v=xvs.p2num(i);
        S *x=xs.p2num(i);
        domovement(v,x);
      }
      }
  JBmp j=JBmp(rscrwid,rscrhei);
  intmap=Map2d<int>(scrwid,scrhei);
  int start=jump;
  do {
    j.clear();
    intmap.clear(0);
    start-=7;
    if (start<=0)
      start=start+jump;
    for (int x=0;x<scrwid;x+=jump) {
      int xa,ya,xb,yb,xc,yc;
      PPgetscrpos(hillfor(x,0),&xa,&ya);
      for (int y=0;y<scrhei;y+=sjump) {       
        PPgetscrpos(hillfor(x,y),&xb,&yb);
        j.line(xa,ya,xb,yb,255);
        xa=xb; ya=yb;
      }
    }
    for (int y=start;y<scrhei;y+=jump) {
      int xa,ya,xb,yb,xc,yc;
      PPgetscrpos(hillfor(0,y),&xa,&ya);
      for (int x=0;x<scrwid;x+=sjump) {       
        PPgetscrpos(hillfor(x,y),&xb,&yb);
        j.line(xa,ya,xb,yb,255);
        xa=xb; ya=yb;
      }
    }
      for (int i=1;i<=xs.len;i++) {
//        j.line(myrnd()*scrwid,myrnd()*scrhei,myrnd()*scrwid,myrnd()*scrhei,myrnd()*255);
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
