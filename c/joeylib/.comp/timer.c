#ifndef timer_C
  #define timer_C

  #include <timer.h>

#include <time.h>
#include <stdio.h>

long starttime;
int numframes;
long endtime;
long pausetime;

void pausetimer() {
  printf("Pausing timer.\n");
  #ifdef DOS
  pausetime=uclock();
  #endif
  #ifdef POSIX
    pausetime=clock();
  #endif
}

void unpausetimer() {
  printf("Un-pausing timer.\n");
  long toadd=pausetime-starttime;
  #ifdef DOS
  starttime=uclock()-toadd;
  #endif
  #ifdef POSIX
    starttime=clock()-toadd;
  #endif
}

void starttimer() {
  #ifdef DOS
  starttime=uclock();
  #endif
  #ifdef POSIX
    starttime=clock();
  #endif
  numframes=0;
  endtime=-1;
}

void framedone() {
  numframes++;
}

void savetimer() {
  #ifdef DOS
  endtime=uclock();
  #endif
  #ifdef POSIX
    endtime=clock();
  #endif
}

void displayframespersecond() {
  if (endtime==-1)
    savetimer();
  #ifdef DOS
  printf("%f frames per second.\n",(float)UCLOCKS_PER_SEC*numframes/(endtime-starttime));
  #endif
  #ifdef POSIX
    printf("%f frames per second.\n",(float)numframes/(float)(endtime-starttime)*(float)CLOCKS_PER_SEC);
  #endif
}

void waituntil(float seconds) {
  #ifdef DOS
  while (uclock()-starttime<UCLOCKS_PER_SEC*seconds) {
    // Do nothing
  }
  #endif
}

float gettimer() {
  savetimer();
  #ifdef DOS
    return (float)(endtime-starttime)/(float)UCLOCKS_PER_SEC;
  #endif
  #ifdef POSIX
    return (float)(endtime-starttime)/(float)CLOCKS_PER_SEC;
  #endif
}

#endif
