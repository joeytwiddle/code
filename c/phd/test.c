/* This source code is freely distributable under the LGPL
   (lesser GNU public licence).  Please keep this tag with
   this code, and add your own if you contribute.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark | www.changetheworld.org.uk */

#include <joeylib.c>

void main(int argc,String *argv) {
  
  printf("Hello!\n");
  Map2d<int> banana=Map2d<int>(2,2);
  banana.setpos(1,1,5);
  int x=banana.getpos(1,1);
  printf("%i\n",x);
  int y=banana.getpos(-2,-2);
  printf("%i\n",y);

}
