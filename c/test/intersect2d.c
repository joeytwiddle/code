/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, April 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

  int scrwid=640;
  int scrhei=400;
//  allegrosetup(scrwid,scrhei);
  class JBmp;
  JBmp *j;

  class V2d;
  void opencircle(V2d a,int b,int c);
  void line(V2d a,V2d b,int c);

#include <joeylib.c>

  void opencircle(V2d a,int b,int c) {
    j->opencircle(a,b,c);
  }

  void line(V2d a,V2d b,int c) {
    j->line(a,b,c);
  }

void main(int argc,String *argv) {

  j=new JBmp(scrwid,scrhei);

  for (int i=1;i<=5;i++) {
    j->clear();
    V2d a=V2d(myrnd()*scrwid,myrnd()*scrhei);
    V2d b=V2d(myrnd()*scrwid,myrnd()*scrhei);
    V2d c=V2d(myrnd()*scrwid,myrnd()*scrhei);
    V2d d=V2d(myrnd()*scrwid,myrnd()*scrhei);
    Line2d p=Line2d(a,b);
    Line2d q=Line2d(c,d);
//    Line2d q=Line2d(a+V2d(20,20),b+V2d(20,20));
    j->thickline(p,2,15);
    j->thickline(q,2,15);
    V2d x=p.findintersection(q);
    j->opencircle(x,10,15);
//    j.line(x,p.a,7);
//    j.line(x,q.a,7);
//    j.writetoscreen();
    j->tomap().hueify().writefile(getnextfilename("intersect","bmp"));
//    waitforkeypress();
  }

}
