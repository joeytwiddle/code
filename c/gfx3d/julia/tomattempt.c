
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
  int maxiters=20;
  JBmp *b=new JBmp(scrwid,scrhei);
  PALETTE p;
  allegrosetup(scrwid,scrhei);
//  makepalette(&greypalette);
  mypalette(0,0,0,0);
  p[0]=newallegRGB(0,0,0);
  p[255]=newallegRGB(1,1,1);
  set_palette(p);
  _farsetsel(screen->seg);
  randomise();
  Complex c=Complex(floatrnd(-4.0,4.0),floatrnd(-4.0,4.0));
  float t=myrnd()*10000.0;
  starttimer();
  do {
//  c=c*Complex(0.99,0)+Complex(floatrnd(-.5,.5),floatrnd(-.5,.5));
  t+=0.01;
//  c=Complex(0.7*sin(7.0*t)+0.2*sin(t*17),0.7*cos(7.0*t)+0.2*cos(t*17));
  float rad=0.8+0.1*sin(t*11);
  c=Complex(rad*sin(2.0*t),rad*cos(2.0*t));
//  b->setpixel(scrwid/2+scrhei/2*c.x,scrhei/2+scrhei/2*c.y,255);
  for (int i=0;i<=scrwid/2;i+=2) {
    for (int j=0;j<scrhei;j+=2) {
      float x=left+2.0*wid*i/(float)scrwid;
      float y=top+2.0*hei*j/(float)scrhei;
      Complex z=Complex(x,y);
      int k=0;
      bool over=false;
      do {
        k++;
        z=z*&z+c;
        if (k>maxiters)
          over=true;
      } while (!over && !(z.mod()>2.0));
      if (over)
        k=0;
      b->setpixel(i,j,k);
      b->setpixel(i+1,j,k);
      b->setpixel(i,j+1,k);
      b->setpixel(i+1,j+1,k);
      b->setpixel(scrwid-i,scrhei-j,k);
      b->setpixel(scrwid-i+1,scrhei-j,k);
      b->setpixel(scrwid-i,scrhei-j+1,k);
      b->setpixel(scrwid-i+1,scrhei-j+1,k);
      if (key[KEY_SPACE])
        break;
    }
  }
    for (int i=1;i<=maxiters;i++) {
    //   mypalette(i,myRGB::hue(t+(float)i/40.0)*(0.2+0.8*(float)i/(float)maxiters));
      myRGB r=myRGB::hue(t+(float)i/50.0)*((float)i/(float)maxiters);
      p[i]=newallegRGB(r);
    }
    //set_palette(p);
    set_palette_range(p,1,maxiters,0);
  b->writetoscreen();
    framedone();
  } while (!key[KEY_SPACE]);
  savetimer();
  allegro_exit();
  displayframespersecond();
}
