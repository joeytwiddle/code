/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#define numvs 4

#include <joeylib.c>

void main(int argc,String *argv) {

  int scrwid=320;
  int scrhei=200;
  allegrosetup(scrwid,scrhei);

  JFont font=JFont("cool.jf");

  JBmp j=JBmp(scrwid,scrhei);
//  while (!key[KEY_ESC]) {
  while (true) {
    j.clear();
    List<V2d> vs;
    for (int i=1;i<=numvs;i++)
      vs.add(V2d(myrnd()*scrwid,myrnd()*scrhei));
    for (int i=1;i<=numvs;i++)
      j.opencircle(vs.num(i),4,255);
    List<float> xs;
    List<float> ys;
    for (int i=1;i<=numvs;i++) {
      xs.add(vs.num(i).x);
      ys.add(vs.num(i).y);
    }
    float corr=correlation(xs,ys);
    font.writeString(Sformat("%f",corr),&j,20,20,10,255);
    j.display();
  };

}
