// Interpolated
// 21d centre
// gaussian distributed addition amplitude and radius
// using map to remove artefacts

#include <joeylib.c>

#define debug false

int scrwid=320; int scrhei=200;
int jump=16;
int res=1;
float scale=100.0;
float offrnd=0.1;
float offvdamp=0.9;
float offdamp=0.9;
float ampcen=0;
/*float offrnd=0.05;
float offvdamp=0.95;
float offdamp=0.95;
float ampcen=0;*/

Map2d<float> *rend=new Map2d<float>(scrhei,scrhei); // depth map
JBmp *b=new JBmp(scrwid,scrhei);

#define smallest 0.0015
int indent=(scrwid-scrhei)/2;
int steps=10;
float back=1;
float forget=back;
int lessthan=-5;
int coldifftol=24;

float crazy=123.456;

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

void domovement(S *v,S *x) {
  v->pos=v->pos*offvdamp+V3d::randomvolume(offrnd);
  hang(&(v->amp),0,0.9,0.3);
  hang(&(v->rad),0,0.9,0.01);
  x->add(*v);
  x->pos=x->pos*offdamp;
  hang(&(x->amp),0,0.95,0);
  hang(&(x->rad),0.7,0.9,0);
  if (x->rad<0.1)
    x->rad=0.1;
/*  v->pos=v->pos*offvdamp+V3d::randomvolume(offrnd);
  hang(&(v->amp),0,0.9,0.02);
  hang(&(v->rad),0,0.9,0.01);
  x->add(*v);
  x->pos=x->pos*offdamp;
  hang(&(x->amp),6,0.9,0);
  hang(&(x->rad),0.8,0.9,0);*/
}

float getpotentialfor(V3d here) {
      float sum=0;
      for (int i=1;i<=xs.len;i++) {
        S s=xs.num(i);
        sum+=-s.amp*gaussian(V3d::dist(here,s.pos)/s.rad);
      }
      sum=scale*sum/(float)(xs.len);
//      printf("%f\n",sum);
  return sum;
}

V3d searchline(V3d front,V3d back,bool wanted) {
  V3d t,mid;
  t=(front-back);
  if (t.mod()<smallest)
    return back;
  mid=((front+back)/2.0);
  // if (mid.c>forget)
  // return Quaternion(0,0,forget,0);
  if ((getpotentialfor(mid)<lessthan)==wanted)
    return searchline(front,mid,wanted);
  else
    return searchline(mid,back,wanted);
}

float getdepth(int i,int j) {
//  printf("gd\n");
  if (!rend->inmap(i,j))
    return back;
  if (rend->pos[i][j]==crazy) {
        // Shoot ray into image
        V3d near=V3d((i-scrhei/2)*2.0/scrhei,(j-scrhei/2)*2.0/scrhei,-2);
        V3d far=V3d(near.x*2.0,near.y*2.0,2.0);
        rend->pos[i][j]=back;
        V3d next,last=near;
        bool lastin=(getpotentialfor(near)<lessthan);
        
        for (int k=1;k<steps;k++) {
          float dk=(float)k/(float)steps;
          next=near*(1.0-dk)+far*dk;
          if ((getpotentialfor(next)<lessthan)!=lastin) {
            rend->pos[i][j]=searchline(last,next,!lastin).z;
            break;
          }
          last=next;
        }
  }
  return rend->pos[i][j];
}

int getcol(int rx,int ry) {
    int col;
    float d=getdepth(rx,ry);
    if (d==back)
      col=0;
    else {
      float diff=2.0*d-getdepth(rx-1,ry)-getdepth(rx,ry-1);
      col=chop(128.0-diff*4800.0,0,255);
    }
  return col;
}

void fillsquare(int x,int y,int w,int h) {
//  printf("fs\n");
//  b->setrectangle(indent+x,y,w,h,myrnd()*255);
//  b->writetoscreen();
        if (key[KEY_SPACE])
          exit(0);
  int nwcol=getcol(x,y);
  int necol=getcol(x+w,y);
  int swcol=getcol(x,y+h);
  int secol=getcol(x+w,y+h);
  int cx=x+w/2;
  int cy=y+h/2;
  int ave=(nwcol+necol+swcol+secol)/4;
  int m=max(max(diff(nwcol,ave),diff(necol,ave)),max(diff(swcol,ave),diff(secol,ave)));
  if (m<coldifftol || max(w,h)<=res) {
    b->interpolate(indent+x,y,indent+x+w,y+h,nwcol,necol,swcol,secol);
//    b->writetoscreen();
  } else {
    fillsquare(x,y,w/2,h/2);
    fillsquare(cx,y,w/2,h/2);
    fillsquare(x,cy,w/2,h/2);  
    fillsquare(cx,cy,w/2,h/2);
  }
}

void main() {

  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  _farsetsel(screen->seg);
  PALETTE pal;
  get_palette(pal);

  randomise(0.456);
  for (int i=0;i<30;i++) {
    S s=S(V3d::randomvolume(1),0,0);
    S v=S(V3d(0,0,0),0,0);
    xs+s;
    xvs+v;
  }
  for (int rndloop=0;rndloop<100;rndloop++) {
    for (int i=0;i<xs.len;i++) {
      S *v=xvs.p2num(i);
      S *x=xs.p2num(i);
      domovement(v,x);
    }
  }
  do {

    rend->clear(crazy);

    for (int i=0;i<scrhei;i+=jump) {
      for (int j=0;j<scrhei;j+=jump) {
        fillsquare(i,j,jump,jump);
      }
      b->writetoscreen();
    }

    for (int lkg=0;lkg<1;lkg++)
    for (int i=1;i<=xs.len;i++) {
      S *v=xvs.p2num(i);
      S *x=xs.p2num(i);
      domovement(v,x);
    }
    b->writefile(getnextfilename("bmp"));
  } while (!key[KEY_ESC]);
}
