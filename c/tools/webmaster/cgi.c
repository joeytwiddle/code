/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

#include <joeylib.c>

#include "getcgi.c"

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  a.done();

  String *vars=getcgivars();
  
}
