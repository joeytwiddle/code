// Should use myRGB instead of V3d since it is uchar therefore faster

/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

int size;
float curve;
float scale;
bool wireframe;
float gaus;
Map2d<float> alt,drv,vel;
Map2d<Corner> vs;

void dogeol(int n) {
  for (int i=0;i<size;i++)
  for (int j=0;j<size;j++)
  for (int k=1;k<=n;k++) {
    hang(&drv.pos[i][j],0.5,0.998,vel.pos[i][j]);
    hang(&vel.pos[i][j],0,0.99,0.01);
  }
}

float strength(int s) {
  return pow((float)s/(float)size,curve);
  // return mysquare((float)s/(float)size);
  // return (float)s/(float)size;
}

void geol(int x,int y,float a,float b,int s) {
  alt.pos[x][y]=oldpull((a+b)/2.0,drv.pos[x][y],strength(s));
  // alt.pos[x][y]=oldpull((a+b)/2.0,0.5*mysquaresgn(drv.pos[x][y]),strength(s));
  // alt.pos[x][y]=1.0;
}

void makemountain(int l,int t,int r,int b) {
  int cx=(l+r)/2;
  int cy=(t+b)/2;
  // int s=max(max(diff(cx,r),diff(cy,b)),max(diff(cx,l),diff(cy,t)));
  int s=max(diff(cx,l),diff(cy,t));
  float nw=alt.pos[l][t];
  float ne=alt.pos[r][t];
  float se=alt.pos[r][b];
  float sw=alt.pos[l][b];
  float c=(nw+ne+sw+se)/4.0;
  alt.pos[cx][cy]=c;
  geol(l,cy,nw,sw,s);
  geol(r,cy,ne,se,s);
  geol(cx,t,nw,ne,s);
  geol(cx,b,sw,se,s);
  // alt.display();
  if (s>0) {
    makemountain(l,t,cx,cy);
    makemountain(cx,t,r,cy);
    makemountain(l,cy,cx,b);
    makemountain(cx,cy,r,b);
  }
}

class GeolText : public Texture {
  float snow,green,waterlevel;
public:
  GeolText(float sn,float g,float w) {
    snow=sn;
    green=g;
    waterlevel=w;
  }
  myRGB colat(V3d x) {
    return ( x.y>snow ? myRGB::white :
             x.y>green ? pull(0.5*myRGB::green,(x.y-green)/(snow-green),myRGB::white) :
             x.y>waterlevel+0.01 ? 0.5*myRGB::green : 0.6*myRGB::blue );
  }
};

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  size=a.intafter("-gs","grid size",20);
  curve=a.floatafter("-us","unspikeyness",1.0);
  float scale=a.floatafter("-hs","height scale",1.0);
  float waterlevel=a.floatafter("-wl","water level",-0.2);
  float rotspeed=a.floatafter("-rs","rotation speed",pi/80.0);
  float elevang=a.floatafter("-ea","elevation angle",pi/7.0);
  wireframe=a.argexists("-w","wireframe");
  bool funky=a.argexists("-f","funky");
  bool groovy=a.argexists("-g","groovy");
  bool dots=a.argexists("-d","dots");
//  gaus=a.floatafter("-gh","gaussian hillock",10.0);
  a.done();
  
  int scrwid=(wireframe ? 640 : 320);
  int scrhei=(wireframe ? 480 : 200);
  if (!wireframe)
    allegrosetup(scrwid,scrhei,24);
  else {
    allegrosetup(scrwid,scrhei,8);
//  mypalette(0,myRGB(0,0,0));
//  mypalette(255,myRGB(0,255,0));
//  makepalette(&greypalette);
    makepalette(&greenpalette);
//  PALETTE pal;
//  generate_332_palette(pal);
//  set_palette(pal);
  }
  PPsetup(scrwid,scrhei,4.0,0);
  
  alt=Map2d<float>(size,size,0.0);
  randomise();
  drv=Map2d<float>(size,size,&myrnd);
  vel=Map2d<float>(size,size,0.0);
  vs=Map2d<Corner>(size,size);
  
  int time=0;
  
  dogeol(300);
  GeolText gt=GeolText(0.5,0,waterlevel);

  RGBmp rgbmp=RGBmp(scrwid,scrhei);
  
  while (!keypressed()) {
    
    time++;
    
    // Generate new mountain
    alt.pos[0][0]=drv.pos[0][0];
    alt.pos[size-1][0]=drv.pos[size-1][0];
    alt.pos[0][size-1]=drv.pos[0][size-1];
    alt.pos[size-1][size-1]=drv.pos[size-1][size-1];
    makemountain(0,0,size-1,size-1);
    
    V3d from=V3d::rotate(V3d(0,1,-3),V3d(0,1,0),rotspeed*(float)time);
    Viewpoint v=Viewpoint::fromto(from,V3d(0,0,0),V3d(0,1,0));
    PPvp=v;
    // Viewpoint v=Viewpoint(mb*(ma*V3d(0,0,-4)),mb*(ma*ori));

    // Render mountain
    PPclear();
    Matrix ma;
    Matrix mb;//=Matrix::rotation(V3d(1,0,0),elevang);
    Octree<Renderable*> o=Octree<Renderable*>(V3d::o,2.0,&getpos);
    for (register int i=0;i<size;i++)
    for (register int j=0;j<size;j++) {
      V3d x=V3d((float)i/(float)size*2.0-1.0,alt.pos[i][j],(float)j/(float)size*2.0-1.0);
//      float k=gaussian(x.xz().mod()/gaus);
//      x.y=x.y*k;
//      x.y=k;
//      x.y=x.y*ramp(pythag((float)i/(float)size,(float)j/(float)size)/pythag(1,1)*2.0);
      x=x*V3d(1,2,1)-V3d(0,1,0);
      x=x*V3d(1,scale,1);
      bool wat;
      if (x.y<waterlevel) {
        x.y=waterlevel;
        wat=true;
      } else
      wat=false;
//      x=ma*x;
//      x=mb*x;
      if (wireframe)
        x=v.bring(x);
      vs.pos[i][j]=Corner(x);
//      vs.pos[i][j].rgb=col;
      if (wireframe) {
        if (i>0)
          if (!groovy || !wat)
          PPplotline(x,vs.pos[i-1][j].pos);
        if (j>0)
          if (!funky || wat)
            if (dots)
              PPplotsphere(x,0.007);
            else
              PPplotline(x,vs.pos[i][j-1].pos);
      } else
      if (i>0 && j>0) {
        o.add(new Gouraud(&gt,&vs.pos[i][j],&vs.pos[i-1][j],&vs.pos[i][j-1]));
        o.add(new Gouraud(&gt,&vs.pos[i-1][j],&vs.pos[i-1][j-1],&vs.pos[i][j-1]));
      }
    }
    
    if (wireframe)
      PPshow();
    else {
    for (register int i=0;i<size;i++)
    for (register int j=0;j<size;j++) {
      vs.pos[i][j].findnormal();
    }
    
    //render(&o,&v);
    rgbmp.clear();
    render(&rgbmp,&o,&v);
    o.freedom();
    }
    
    // PPshow();
    // alt.display();
    
    // Change drivers
    dogeol(1);
    
  }
  
  allegroexit();
  
}
