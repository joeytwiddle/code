/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


#include <joeylib.c>


main()
{
  starttimer();
  for (int i=0;i<100000000;i++)
  {
    i++;
  };
  setframedone();
  printoutframespersecond();

  for (register int p=0;p<100000000;p++)
  {
    p++;
  };
  
  setframedone();
  printoutframespersecond();
   
 };

