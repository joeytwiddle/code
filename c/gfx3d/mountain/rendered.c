/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

int size;
Map2d<float> alt,drv,vel;
Map2d<V3d> vs;

void dogeol(int n) {
    for (int i=0;i<size;i++)
    for (int j=0;j<size;j++)
      for (int k=1;k<=n;k++) {
      hang(&drv.pos[i][j],0.5,0.998,vel.pos[i][j]);
      hang(&vel.pos[i][j],0,0.99,0.01);
    }
}

float strength(int s) {
  return (float)s/(float)size;
}

void geol(int x,int y,float a,float b,int s) {
  alt.pos[x][y]=oldpull((a+b)/2.0,drv.pos[x][y],strength(s));
//  alt.pos[x][y]=oldpull((a+b)/2.0,0.5*mysquaresgn(drv.pos[x][y]),strength(s));
//  alt.pos[x][y]=1.0;
}

void makemountain(int l,int t,int r,int b) {
  int cx=(l+r)/2;
  int cy=(t+b)/2;
//  int s=max(max(diff(cx,r),diff(cy,b)),max(diff(cx,l),diff(cy,t)));
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
//  alt.display();
  if (s>0) {
    makemountain(l,t,cx,cy);
    makemountain(cx,t,r,cy);
    makemountain(l,cy,cx,b);
    makemountain(cx,cy,r,b);
  }
}

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  size=a.intafter("-gs","grid size",20);
  float rotspeed=a.floatafter("-rs","rotation speed",pi/80.0);
  float elevang=a.floatafter("-ea","elevation angle",pi/7.0);
  float waterlevel=a.floatafter("-wl","water level",-0.2);
  bool funky=a.argexists("-f","funky");
  bool groovy=a.argexists("-g","groovy");
  a.done();

  int scrwid=800;
  int scrhei=600;
  //printf("z\n");
  allegrosetup(scrwid,scrhei);
  //printf("a\n");
  PPsetup(scrwid,scrhei,4.0,4.0);
  //printf("b\n");
  mypalette(0,myRGB(0,0,0));
  mypalette(255,myRGB(0,255,0));
  makepalette(&greypalette);

  alt=Map2d<float>(size,size,0.0);
  randomise();
  drv=Map2d<float>(size,size,&myrnd);
  vel=Map2d<float>(size,size,0.0);
  vs=Map2d<V3d>(size,size);

  int time=0;

  dogeol(300);
  
  while (!keypressed()) {
  //printf("d\n");

    time++;

    // Generate new mountain
    alt.pos[0][0]=drv.pos[0][0];
    alt.pos[size-1][0]=drv.pos[size-1][0];
    alt.pos[0][size-1]=drv.pos[0][size-1];
    alt.pos[size-1][size-1]=drv.pos[size-1][size-1];
    makemountain(0,0,size-1,size-1);

    // Render mountain
    PPclear();
    Matrix ma=Matrix::rotation(V3d(0,1,0),rotspeed*(float)time);
    Matrix mb=Matrix::rotation(V3d(1,0,0),elevang);
    //printf("!");
    Octree<Renderable*> o=Octree<Renderable*>(V3d::o,2.0,&getpos);
    for (int i=0;i<size;i++)
    for (int j=0;j<size;j++) {
      //printf(">");
      V3d x=V3d((float)i/(float)size*2.0-1.0,alt.pos[i][j],(float)j/(float)size*2.0-1.0);
//      x.y=x.y*ramp((float)i/(float)size*2.0);
      x=x*V3d(1,2,1)-V3d(0,1,0);
      bool wat;
      if (x.y<waterlevel) {
        x.y=waterlevel;
        wat=true;
      } else
        wat=false;
      x=ma*x;
      x=mb*x;
      vs.pos[i][j]=x;
      if (i>0 && j>0) {
        o.add(new Triangle3d(x,vs.pos[i-1][j],vs.pos[i][j-1]));
        o.add(new Triangle3d(vs.pos[i-1][j],vs.pos[i][j-1],vs.pos[i-1][j-1]));
      }
    }

    Ori ori;
    Viewpoint v=Viewpoint(V3d(0,0,-4),ori);
//    Viewpoint v=Viewpoint(mb*(ma*V3d(0,0,-4)),mb*(ma*ori));
    render(&o,&v);
    o.freedom();

    // PPshow();
    // alt.display();

    // Change drivers
    dogeol(1);

  }

}
