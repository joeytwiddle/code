#ifndef jcurses_C
  #define jcurses_C

  #include <jcurses.h>

#include <curses.h>

boolean getyesno() {
  int k;
  while ((k=getch())==ERR) { }
  if (k=='Y' || k=='y')
    return true;
  if (k=='N' || k=='n')
    return false;
  error("getyesno, got char = %i '%c'\n",(int)k,k);
  return false;
}

#endif
