/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

#include <joeylib.c>

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  float angle=a.floatafter("-a","angle",0);
  float size=a.floatafter("-s","size",20);
  float freq=a.floatafter("-f","frequency",1);
  String iname=a.getarg("filename");
  String oname=a.argor("output filename","gaborout.bmp");
  a.done();

  Map2d<float> *image=Map2d<float>::readfile(iname);
  Map2d<float> *output=image->filterby(Map2d<float>::gabor(V2d::angle(angle),size,freq));
  output->writefile(oname);
  
}
