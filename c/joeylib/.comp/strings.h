#ifndef strings_H
  #define strings_H

#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <fnmatch.h>

#define String char *

// - All parameters given to Strings assume 1 is first character in string
// - Warning: String a,b; declares a as char* and b as char !

//

String Snew(int x); // Method


int Slen(String s); // Method


int Sinstr(String x,String s,int start); // Method


int Sinstr(String x,String s); // Method


int Sinstr(String x,char c); // Method


int Sinstrlast(String x,String s); // Method


int badScount(String x,String s); // Method



int Scount(String x,String s); // Method


String Sformat(String f, ... ); // Method


String myformat(String f, ... ); // Method


String Smid(String f,int s,int l); // Method


String Smid(String f,int i); // Method


// *** Sick - all my String functions work from 1 onwards!

String Ssub(String s,int x,int y); // Method


String Sleft(String s,int i); // Method


String Sright(String s,int i); // Method


String Sconcat(String a,String b); // Method


String Sconcd(String a,String b); // Method


String Sconc(String a,String b); // Method


String Sconc(String a,String b,String c); // Method


String Sconc(String a,String b,String c,String d); // Method


String Sconc(String a,String b,String c,String d,String e); // Method


String Sconc(String a,String b,String c,String d,String e,String f); // Method


String Sconc(String a,String b,String c,String d,String e,String f,String g); // Method


String Sconc(String a,String b,String c,String d,String e,String f,String g,String h); // Method


bool Seq(String a,String b); // Method


void println(String s, ... ); // Method


void Scopy(String a,String b); // Method


String Snew(String s); // Method


String Scopyof(String s); // Method


String Srepeat(String s,int x); // Method


String Srep(String s,int x); // Method


String toString(int x); // Method


String Sfrom(String s,int w); // Method


String chtoString(char c); // Method


String Srls(String s); // Method


String Srts(String s); // Method


String Sbefore(String x,String s); // Method


String Sgetbefore(String x,String s); // Method


String Sbeforelast(String x,String s); // Method


String Safter(String x,String s); // Method


String Sgetafter(String x,String s); // Method


String Safterlast(String x,String s); // Method


String Safterlastorall(String x,String s); // Method


bool Sin(String x,String s); // Method


float tofloat(String s); // Method


String Sreplacein(String x,String s,String t); // Method


String Sreplaceall(String x,String s,String t); // Method


String Sreplace(String x,String s,String t); // Method


String Sreplacestart(String x,String s,String t); // Method


String Spadright(String s,int x,String p); // Method


String getdir(String s); // Method


String getfile(String s); // Method


String Schr(int i); // Method


String toString(char c); // Method


//

char Stoupper(char c); // Method


String Stoupper(String s); // Method


char Stolower(char c); // Method


String Stolower(String s); // Method


String Salphalower(); // Method


String Salphaupper(); // Method


String Salphabet(); // Method


String Salphanumeric(); // Method


bool Sequiv(String a,String b); // Method


bool Ssimilar(String a,String b); // Method


class StringParser {
public:
  String current; // Exists
  String whole; // Exists
   StringParser(String s); // Method

  void error(String s); // Method

  String getbefore(String s); // Method

  String getword(); // Method

  bool expect(String s); // Method

  void expectorerr(String s); // Method

  char getachar(); // Method

  String getanyof(String ps,bool doerror); // Method

  String getanyof(String ps); // Method

  float getfloat(); // Method

  bool someleft(); // Method

};

String Sgetanyof(String s,String ps); // Method


bool Sdoremove(String x,String s); // Method


String Sremove(String x,String s); // Method


int Sasc(String s); // Method


bool Smatches(String s,String p); // Method


bool Sstarts(String x,String s); // Method


String Seat(String s,int i,int l); // Method


bool Sempty(String s); // Method


String rtsS(String s); // Method


String Sinsert(String s,int n,String i); // Method


bool Slesseq(String a,String b); // Method


String Sfromint(int i); // Method


String Sfromfloat(float i); // Method


int Stoint(String s); // Method


int Stofloat(String s); // Method


String toString(String s); // Method


String StoCformat(String s); // Method


String Stoechoformat(String s); // Method


float floatFromString(String s); // Method


int Sfirstnotin(String s,String cs); // Method


int Slastnotin(String s,String cs); // Method


String Strim(String s); // Method



#endif
