#ifndef stringsa_C
  #define stringsa_C

  #include <stringsa.h>

#include <stdio.h>
#include <stdarg.h>

#define String char *

// - All parameters given to Strings assume 1 is first character in string

/* String operator+(String x,double f) {
     String t;
     sprintf("%f",t,f)
     return x+t;
   }
*/

String Snew(int x) {
  String n=new char[x+1];
  for (int i=0;i<x+1;i++)
  n[i]='\0';
  return n;
}

int Slen(String s) {
  return strlen(s);
}

int Sinstr(String x,String s) {
  for (int i=0;i<Slen(x)-Slen(s);i++) {
    int j=0;
    while (x[i+j]==s[j]) {
      j++;
      if (j==Slen(s))
        return i+1;
    }
  }
  return 0;
}

int Scount(String x,String s) {
  String q=s;
  int c,ch;
  while ((ch=Sinstr(x,s))>0) {
    c++;
    q=&(q[ch]);
  }
  return c;
}


String Sformat(String f, ... ) {
  va_list vas;
  va_start(vas,f); // Scount(f,"%")
  String t=new char[256];
  vsprintf(t,f,vas);
  return t;
}

String myformat(String f, ... ) {
  va_list vas=va_start(vas,f);
  Sformat(f,vas);
}

String Smid(String f,int s,int l) {
  String n=new char[l+1];
  for (int i=0;i<l;i++) {
    n[i]=f[s-1+i];
  }
  n[l]='\0';
  return n;
}

String Sleft(String s,int i) {
  return Smid(s,1,i);
}

String Sright(String s,int i) {
  return Smid(s,Slen(s)-i+1,i);
}

String Sconcat(String a,String b) {
  String c=new char[Slen(a)+Slen(b)+1];
  for (int i=0;i<Slen(a);i++) {
    c[i]=a[i];
  }
  for (int j=0;j<Slen(b);j++) {
    c[Slen(a)+j]=b[j];
  }
  c[Slen(a)+Slen(b)]='\0';
  return c;
}

String Sconc(String a,String b) {
  return Sconcat(a,b);
}

bool Seq(String a,String b) {
  if (Slen(a)!=Slen(b))
    return false;
  for (int i=0;i<Slen(a);i++) {
    if (a[i]!=b[i])
      return false;
  }
  return true;
}

void println(String s, ... ) {
  va_list vas=va_start(vas,s);
  printf(Sconcat(s,"\n"),vas);
}

void Scopy(String a,String b) {
  for (int i=0;i<Slen(a);i++)
  b[i]=a[i];
}

String Srepeat(String s,int x) {
  String n=Snew(x*Slen(s));
  for (int i=0;i<x;i++)
  Scopy(s,&n[i*Slen(s)]);
  return n;
}

String toString(int x) {
  return Sformat("%i",x);
}

#endif
