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
  String pat=a.argor("pattern",".");
  a.done();

  List<String> ls=getfilesrecursive(pat);
  printf("getfilesrecursive: %i\n%s\n",ls.len,ls.toString());

  ls=getfilesindir(pat);
  printf("getfilesindir: %i\n%s\n",ls.len,ls.toString());

  ls=getfiles(pat);
  printf("getfiles: %i\n%s\n",ls.len,ls.toString());

}
