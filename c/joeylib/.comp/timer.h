#ifndef timer_H
  #define timer_H

#include <time.h>
#include <stdio.h>

extern long starttime; // Exists
extern int numframes; // Exists
extern long endtime; // Exists
extern long pausetime; // Exists

// utime seems to perform worse!
// #ifdef DOS
//   #define clock uclock
//   #define CLOCKS_PER_SEC UCLOCKS_PER_SEC
// #endif

void pausetimer(); // Method


void unpausetimer(); // Method


void starttimer(); // Method


void framedone(); // Method


void savetimer(); // Method


void displayframespersecond(); // Method


void waituntil(float seconds); // Method


float gettimer(); // Method


#endif
