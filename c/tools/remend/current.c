/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, April 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  if (argc==1) {
    printf("remend <file> : Removes any blank lines at the end of a file.\n");
    exit(0);
  }

  List<String> ls=readlinesfromfile(argv[1]);

  while (ls.len>0 && Seq(ls.num(ls.len),""))
    ls.len--;

  writelinestofile(ls,argv[1]);

}
