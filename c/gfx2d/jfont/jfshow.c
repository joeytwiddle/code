#include <joeylib.c>

#define scrwid 640
#define scrhei 480

void main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String fname=a.argafter("-i","Font file (.jf)");
  List<FontChar> fcs=parsefontchars(fname);
  JBmp j=JBmp(scrwid,scrhei,0);
  float size=scrhei/2.0;
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  randomise();
  while (!keypressed()) {
    int c=myrnd()*fcs.len+1;
    fcs.num(c).writetojbmp(&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,myrnd()*255);
    j.writetoscreen();
    j.fadeby(3);
//    j.smooth(1);
//    if (myrnd()>0.999)
  //    j.clear(0);
  }
}
