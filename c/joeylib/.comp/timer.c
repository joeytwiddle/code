#ifndef timer_C
  #define timer_C

  #include <timer.h>

#include <time.h>
#include <stdio.h>

long starttime;
int numframes;
long endtime;
long pausetime;

// utime seems to perform worse!
// #ifdef DOS
//   #define clock uclock
//   #define CLOCKS_PER_SEC UCLOCKS_PER_SEC
// #endif

void pausetimer() {
  printf("Pausing timer.\n");
    pausetime=clock();
}

void unpausetimer() {
  printf("Un-pausing timer.\n");
  long toadd=pausetime-starttime;
    starttime=clock()-toadd;
}

void starttimer() {
    starttime=clock();
  numframes=0;
  endtime=-1;
}

void framedone() {
  numframes++;
}

void savetimer() {
    endtime=clock();
}

void displayframespersecond() {
  if (endtime==-1)
    savetimer();
    printf("%f frames per second.\n",(float)numframes/(float)(endtime-starttime)*(float)CLOCKS_PER_SEC);
}

void waituntil(float seconds) { // Wot exacly is seconds?  Is starttime expected set?!
  #ifdef DOS
  while (uclock()-starttime<UCLOCKS_PER_SEC*seconds) {
    // Do nothing
		// Should release processor
  }
  #endif
}

float gettimer() {
  savetimer();
    return (float)(endtime-starttime)/(float)CLOCKS_PER_SEC;
}

#endif
