/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, April 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  
//  a.showargs();

  if (argc<=2)
    error("cim <filename> <filename> (g/c) : convert image");

  RGBmp *r=RGBmp::readfile(a.arg(1));

//  a.showargs();

  r->writefile(a.arg(2));

  exit(0);

  printf("1 %s ",argv[1]);
  Map2d<float> *m=Map2d<float>::readfile(argv[1]);
  printf("2");
  m->display();
  printf("3");

}
