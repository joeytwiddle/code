/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

JBmp j=JBmp(320,200);

void flibble(Line2d l,int c) {
  j.thickline(l,3,c);
}

void fobble(V2d x) {
  j.opencircle(x,10,15);
}

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);

  int scrwid=320;
  int scrhei=200;
  allegrosetup(scrwid,scrhei);
  PPsetup(scrwid,scrhei,2.0);

  randomise();

  while (!key[KEY_ESC]) {
    Polygon2d a;
    Polygon2d b;
    for (int i=1;i<=intrnd(3,4);i++)
      a.add(V2d(myrnd()*320,myrnd()*200));
    for (int i=1;i<=intrnd(3,4);i++)
      b.add(V2d(myrnd()*320,myrnd()*200));
    j.clear();
    j.fillpoly(a,5);
    j.fillpoly(b,4);
    bool o=a.overlaps(b);
    j.writetoscreen();
    if (o)
      mypalette(0,myRGB(0,0,0));
    else
      mypalette(0,myRGB(255,255,255));
    if (o==Polygon2d::container || o==Polygon2d::contained) {
      mypalette(0,myRGB(0,255,0));
      while (!key[KEY_ESC]) { };
    }
    while (!key[KEY_SPACE] && !key[KEY_ESC]) { };
    a.freedom();
    b.freedom();
  }

}
