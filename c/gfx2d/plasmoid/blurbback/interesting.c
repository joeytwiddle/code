// Interpolated
// 21d centre
// gaussian distributed addition amplitude and radius
// using map to remove artefacts

#include <joeylib.c>

#define debug false

int scrwid=320; int scrhei=200;
int jump=4;
float scale=100.0;
float offrnd=0.05;
float offvdamp=0.99;
float offdamp=0.8;
float ampcen=0;

Map2d<float> *rend=new Map2d<float>(scrhei,scrhei); // depth map
JBmp *b=new JBmp(scrwid,scrhei);

#define smallest 0.005
int indent=(scrwid-scrhei)/2;
int steps=10;
float back=1;
float forget=back;
int lessthan=-5;

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
  return 0.1+0.2*amp/10000.0;
}

void domovement(S *v,S *x) {
  v->pos=v->pos*offvdamp+V3d::randomvolume(offrnd);
  hang(&(v->rad),0,0.9,0.01);
  hang(&(v->amp),0,0.9,0.5);
  x->add(*v);
  x->pos=x->pos*offdamp;
  hang(&(x->amp),30,0.5,0);
  hang(&(x->rad),0.3,0.9,0);
  if (x->rad<0.01)
    x->rad=0.01;
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

// Light effect for pixel rx,ry
void render(int rx,int ry) {
  if (rx>=jump && ry>=jump && rx<rend->width && ry<rend->height) {
    int col;
//    if (rend->pos[rx][ry]==back && rend->pos[rx+jump][ry]==back && rend->pos[rx][ry+jump]==back)
    if (rend->pos[rx][ry]==back)
      col=0;
    else {
      float diff=2.0*rend->pos[rx][ry]-rend->pos[rx-jump][ry]-rend->pos[rx][ry-jump];
      col=chop(128.0-diff*2400.0,0,255);
    }
//    putpixel(screen,indent+rx,ry,col);
          for (int x=0;x<jump;x++) {
            for (int y=0;y<jump;y++) {
              b->setpixel(indent+rx+x,ry+y,col);
            }
          }
  }
}

void main() {

  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  _farsetsel(screen->seg);
  PALETTE pal;
  get_palette(pal);

  randomise(0.456);
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
  do {

    for (int i=scrhei/3;i<scrhei;i+=jump) {
      for (int j=0;j<scrhei;j+=jump) {

        // Shoot ray into image
//        V3d displace=Quaternion(cenx,ceny,0,0);
        V3d near=V3d((i-scrwid/2)*2.0/scrhei,(j-scrhei/2)*2.0/scrhei,-2);
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

        render(i,j);

        if (key[KEY_SPACE])
          exit(0);

      }
      b->writetoscreen();
    }

    for (int i=1;i<=xs.len;i++) {
      S *v=xvs.p2num(i);
      S *x=xs.p2num(i);
      domovement(v,x);
    }
    b->writefile(getnextfilename("bmp"));
  } while (!key[KEY_ESC]);
}
