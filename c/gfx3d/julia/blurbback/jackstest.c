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

