/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

int size=20;
Map2d<float> alt,drv,to;
Map2d<V3d> vs;
float geolspeed=0.03;
float rotspeed=pi/80.0;
float elevang=pi/7.0;

float strength(int s) {
  return (float)s/(float)size;
}

void geol(int x,int y,float a,float b,int s) {
  alt.pos[x][y]=oldpull((a+b)/2.0,drv.pos[x][y],strength(s));
}

void makemountain(int l,int t,int r,int b) {
  int cx=(l+r)/2;
  int cy=(t+b)/2;
  int s=max(max(diff(cx,r),diff(cy,b)),max(diff(cx,l),diff(cy,t)));
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
  if (s>1) {
    makemountain(l,t,cx,cy);
    makemountain(cx,t,r,cy);
    makemountain(l,cy,cx,b);
    makemountain(cx,cy,r,b);
  }
}

void main(int argc,String *argv) {

  int scrwid=640;
  int scrhei=480;
  allegrosetup(scrwid,scrhei);
  PPsetup(scrwid,scrhei,2.0,2.0);
  mypalette(0,myRGB(0,0,0));
  mypalette(255,myRGB(0,255,0));

  alt=Map2d<float>(size,size,0.0);
  drv=Map2d<float>(size,size,&myrnd);
  to=Map2d<float>(size,size,&myrnd);
  vs=Map2d<V3d>(size,size);

  int time=0;
  
  while (!key[KEY_ESC]) {

    time++;

    // Generate new mountain
    alt.pos[0][0]=drv.pos[0][0];
    alt.pos[size-1][0]=drv.pos[size-1][0];
    alt.pos[0][size-1]=drv.pos[0][size-1];
    alt.pos[size-1][size-1]=drv.pos[size-1][size-1];
    makemountain(0,0,size-1,size-1);

    // Render mountain
    PPclear();
    for (int i=0;i<size;i++)
    for (int j=0;j<size;j++) {
      V3d x=V3d((float)i/(float)size*2.0-1.0,alt.pos[i][j],(float)j/(float)size*2.0-1.0);
      x=x*V3d(1,2,1)-V3d(0,1,0);
      x=V3d::rotate(x,V3d(0,1,0),rotspeed*(float)time);
      x=V3d::rotate(x,V3d(1,0,0),elevang);
      vs.pos[i][j]=x;
      if (i>0)
        PPplotline(x,vs.pos[i-1][j]);
      if (j>0)
        PPplotline(x,vs.pos[i][j-1]);
    }

    PPshow();
    // alt.display();

    // Change drivers
    for (int i=0;i<size;i++)
    for (int j=0;j<size;j++) {
      float f=to.pos[i][j]-drv.pos[i][j];
      int d=mysgn(f);
      drv.pos[i][j]+=geolspeed*d;
      if (myabs(f)<geolspeed)
        to.pos[i][j]=myrnd();
    }

  }

}
