// To be done
// Gets confused by email containing a forwarded email with header

#include <joeylib.c>

List<String> ls;

int gi;

int nextstart(int n) {
  n++;
  if (n>ls.len)
    return 0;
  if (Sstarts(ls.num(n),"From ")) {
    int state=0;
    for (int i=n;state==0 && i<=ls.len;i++) {
      if (Sstarts(ls.num(i),"To: ")) {
        gi=i;
        if (n>1) {
        String s=ls.num(n-1);
        printf("%i. ",(int)s[Slen(s)-1]);
      }
      return n;
    }
    if (Slen(ls.num(i))==0)
      state=-1;
    }
  }
  return nextstart(n);
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  String fname=a.getarg(1);
  a.done();
  
  ls=readlinesfromfile(fname);
  
  int i=nextstart(0);
  while (i>0 && i<ls.len) {
    int j=nextstart(gi);
    if (j==0)
      j=ls.len+1;
    List<String> nls;
    for (int k=i;k<j;k++)
      nls.add(ls.num(k));
    writelinestofile(nls,getnextfilename("txt"));
    i=j;
  }
    
}
