/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

String stype;

bool satisfactory(myRGB r) {
  if (Seq(stype,"cluster"))
    return (r.r>r.g && r.r>r.b);
  if (Seq(stype,"boolean"))
    return (r.r>0.9 && r.g>0.9 && r.b>0.9);
  return true;
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  stype=a.argafter("-t","segmentation type","cluster");
  a.comment("cluster = choose red, boolean = choose white");
  String iname=a.arg("image file");
  String sname=a.argor("segmentation file","none");
  a.done();
  
  RGBmp *spic=RGBmp::readfile(sname);
  Map2d<bool> bpic=Map2d<bool>(spic->width,spic->height);
  for (int x=0;x<spic->width;x++)
  for (int y=0;y<spic->height;y++)
    bpic.pos[x][y]=satisfactory(spic->bmp[x][y]);
  
  bpic.writefile("bool.bmp");
  
}
