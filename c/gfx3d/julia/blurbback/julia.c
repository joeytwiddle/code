/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


#include <joeylib.c>

// z=z^2+c

void main() {
  int scrwid=320;
  int scrhei=200;
  float cenx=0;
  float ceny=0;
  float wid=2.0;
  float hei=1.5;
  float left=cenx-wid;
  float right=cenx+wid;
  float top=ceny-hei;
  float bottom=ceny+hei;
  JBmp *b=new JBmp(scrwid,scrhei);
  allegrosetup(scrwid,scrhei);
//  makepalette(&greypalette);
  _farsetsel(screen->seg);
  randomise();
  Complex c=Complex(floatrnd(-4.0,4.0),floatrnd(-4.0,4.0));
  do {
  c=c*Complex(0.99,0)+Complex(floatrnd(-.5,.5),floatrnd(-.5,.5));
  for (int i=0;i<scrwid;i+=2) {
    for (int j=0;j<scrhei;j+=2) {
      float x=left+2.0*wid*i/(float)scrwid;
      float y=top+2.0*hei*j/(float)scrhei;
      Complex z=Complex(x,y);
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
      b->setpixel(i,j,k);
      b->setpixel(i+1,j,k);
      b->setpixel(i,j+1,k);
      b->setpixel(i+1,j+1,k);
      if (key[KEY_SPACE])
        break;
    }
  }
  b->writetoscreen();
  } while (!key[KEY_SPACE]);
  allegro_exit();
}
