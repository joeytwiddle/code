/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


#include <joeylib.c>

// z=z^2+c

int julquat(Quaternion c,Quaternion z) {
      int k=0;
      bool over=false;
      do {
        k++;
        z=z*z+c;
        if (k>255)
          over=true;
      } while (!over && !(z.mod()>2.0));
      if (over)
        k=0;
    return k;
  }

void main() {
  int scrwid=320;
  int scrhei=200;
  int indent=(scrwid-scrhei)/2;
  float cenx=0;
  float ceny=0;
  float wid=0.75;
  float hei=0.75;
  float left=cenx-wid;
  float right=cenx+wid;
  float top=ceny-hei;
  float bottom=ceny+hei;
  float dslice=0;
  float front=-1.5;
  float back=-0.5;
  float scale=2.0/(back-front);
  int steps=250;
  JBmp *b=new JBmp(scrwid,scrhei);
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  _farsetsel(screen->seg);
  randomise();
//  Quaternion c=*new Quaternion(floatrnd(-2.0,2.0),floatrnd(-2.0,2.0),floatrnd(-2.0,2.0),floatrnd(-2.0,2.0));
  Quaternion c=*new Quaternion(floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5));
  do {
  c=c*Quaternion(0.99,0,0,0)+Quaternion(floatrnd(-.05,.05),floatrnd(-.05,.05),floatrnd(-.05,.05),floatrnd(-.05,.05));
  dslice=c.d;
  for (int i=0;i<scrhei;i+=2) {
    for (int j=0;j<scrhei;j+=2) {
      int col=0;
      for (int k=0;k<steps && col==0;k++) {
        float x=left+2.0*wid*i/(float)scrhei;
        float y=top+2.0*hei*j/(float)scrhei;
        float z=front+(back-front)*k/(float)steps;
        float scrz=(z-front)*scale+1.0;
        x=x/z; y=y/z;
        int r=julquat(Quaternion(x,y,z,dslice),Quaternion(x,y,z,dslice));
        if (r==0)
          col=255.0*(1.0-(float)k/(float)steps);
      }
      b->setpixel(indent+i,j,col);
      b->setpixel(indent+i+1,j,col);
      b->setpixel(indent+i,j+1,col);
      b->setpixel(indent+i+1,j+1,col);
      if (key[KEY_SPACE])
        exit(0);
    }
    b->writetoscreen();
  }
  } while (!key[KEY_SPACE]);
  allegro_exit();
}
