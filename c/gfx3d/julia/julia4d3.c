/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


#include <joeylib.c>

// z=z^2+c

#define smallest 0.01
#define forget 0.5

Quaternion current;

int julquat(Quaternion z) {
      int k=0;
      bool over=false;
      do {
        k++;
        z=z*z+current;
        if (k>255)
          over=true;
      } while (!over && !(z.mod()>2.0));
      if (over)
        k=0;
    return k;
  }

Quaternion searchline(Quaternion front,Quaternion back) {
  Quaternion t=front-back;
  if (t.mod()<smallest)
    return back;
  Quaternion mid=(front+back)/2.0;
//  if (mid.c>forget)
//    return Quaternion(0,0,forget,0);
  if (julquat(mid)==0)
    return searchline(front,mid);
  else
    return searchline(mid,back);
}

void main() {
  int scrwid=320;
  int scrhei=200;
  int indent=(scrwid-scrhei)/2;
  float cenx=0;
  float ceny=0;
  float wid=1.0;
  float hei=1.0;
  float left=cenx-wid;
  float right=cenx+wid;
  float top=ceny-hei;
  float bottom=ceny+hei;
  float dslice=0;
  float front=-1.5;
  float back=0.5;
  float scale=2.0/(2.0*front);
  int steps=50;
  JBmp *b=new JBmp(scrwid,scrhei);
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  _farsetsel(screen->seg);
  randomise();
//  Quaternion c=*new Quaternion(floatrnd(-2.0,2.0),floatrnd(-2.0,2.0),floatrnd(-2.0,2.0),floatrnd(-2.0,2.0));
  current=*new Quaternion(floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5));
  do {
  current=current*Quaternion(0.99,0,0,0)+Quaternion(floatrnd(-.05,.05),floatrnd(-.05,.05),floatrnd(-.05,.05),floatrnd(-.05,.05));
  for (int i=0;i<scrhei;i+=2) {
    for (int j=0;j<scrhei;j+=2) {
//      b->setpixelud(indent+i,j,255);
//      b->writetoscreen();
      int col=0;
      float nearx=left+2.0*wid*i/(float)scrhei;
      float neary=top+2.0*hei*j/(float)scrhei;
      float farx=nearx*2.0;
      float fary=neary*2.0;
      float nearz=front;
      float farz=back;
      Quaternion near=Quaternion(nearx,neary,nearz,dslice);
      Quaternion far=Quaternion(farx,fary,farz,dslice);
      Quaternion last=near;
      for (int k=0;k<steps;k++) {
        float dk=(float)k/(float)steps;
        Quaternion next=near*(1.0-dk)+far*dk;
        if (julquat(next)==0) {
          Quaternion res=searchline(last,next);
          col=255.0*(1.0-(res.c-front)/(forget-front));
          break;
        }
        last=next;
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