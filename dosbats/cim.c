/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, April 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);

  if (argc==1)
    error("jshow <filename> : display image");

  RGBmp *r=RGBmp::readfile(argv[1]);

  r->display();

  exit(0);

  printf("1 %s ",argv[1]);
  Map2d<float> *m=Map2d<float>::readfile(argv[1]);
  printf("2");
  m->display();
  printf("3");

}
