#ifndef stringsa_H
  #define stringsa_H

#include <stdio.h>
#include <stdarg.h>

#define String char *

// - All parameters given to Strings assume 1 is first character in string

//

String Snew(int x); // Method


int Slen(String s); // Method


int Sinstr(String x,String s); // Method


int Scount(String x,String s); // Method



String Sformat(String f, ... ); // Method


String myformat(String f, ... ); // Method


String Smid(String f,int s,int l); // Method


String Sleft(String s,int i); // Method


String Sright(String s,int i); // Method


String Sconcat(String a,String b); // Method


String Sconc(String a,String b); // Method


bool Seq(String a,String b); // Method


void println(String s, ... ); // Method


void Scopy(String a,String b); // Method


String Srepeat(String s,int x); // Method


String toString(int x); // Method


#endif
