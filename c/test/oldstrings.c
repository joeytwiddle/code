//#include <stdlib.h>
//#include <stdio.h>
#include <stdarg.h>
//#include <math.h>

#include <error.c>

#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdarg.h>
#include <fnmatch.h>

#define fart char *

// - All parameters given to farts assume 1 is first character in string
// - Warning: fart a,b; declares a as char* and b as char !



fart Snew(int x) {
  fart n=new char[x+1];
  for (int i=0;i<x+1;i++)
  n[i]='\0';
  return n;
}

int Slen(fart s) {
  return strlen(s);
}

int Sinstr(fart x,fart s) {
  for (int i=0;i<Slen(x)-Slen(s)+1;i++) {
    int j=0;
    while (x[i+j]==s[j]) {
      j++;
      if (j==Slen(s))
        return i+1;
    }
  }
  return 0;
}

int Sinstrlast(fart x,fart s) {
  for (int i=Slen(x)-Slen(s);i>=0;i--) {
    int j=0;
    while (x[i+j]==s[j]) {
      j++;
      if (j==Slen(s))
        return i+1;
    }
  }
  return 0;
}

int badScount(fart x,fart s) {
  fart q=s;
  int c=0;
  int ch;
  while ((ch=Sinstr(x,s))>0) {
    c++;
    q=&(q[ch]);
  }
  return c;
}

int Scount(fart x,fart s) {
  int i=Sinstr(x,s);
  if (i==0)
    return 0;
  else
    return 1+Scount(&x[i],s);
}

fart Sformat(fart f, ... ) {
  va_list vas;
  va_start(vas,f); // Scount(f,"%")
  fart t=new char[256];
  vsprintf(t,f,vas);
  return t;
}

fart myformat(fart f, ... ) {
  va_list vas=va_start(vas,f);
  return Sformat(f,vas);
}

fart Smid(fart f,int s,int l) {
  fart n=new char[l+1];
  for (int i=0;i<l;i++) {
    n[i]=f[s-1+i];
  }
  n[l]='\0';
  return n;
}

fart Smid(fart f,int i) {
  return Smid(f,i,1);
}

fart Sleft(fart s,int i) {
  return Smid(s,1,i);
}

fart Sright(fart s,int i) {
  return Smid(s,Slen(s)-i+1,i);
}

fart Sconcat(fart a,fart b) {
  fart c=new char[Slen(a)+Slen(b)+1];
  for (int i=0;i<Slen(a);i++) {
    c[i]=a[i];
  }
  for (int j=0;j<Slen(b);j++) {
    c[Slen(a)+j]=b[j];
  }
  c[Slen(a)+Slen(b)]='\0';
  return c;
}

fart Sconcd(fart a,fart b) { // Destroys a and b!
	fart ans=Sconcat(a,b);
	free(a);
	free(b);
	return ans;
}

fart Sconc(fart a,fart b) {
  return Sconcat(a,b);
}

fart Sconc(fart a,fart b,fart c) {
  return Sconc(Sconc(a,b),c);
}

fart Sconc(fart a,fart b,fart c,fart d) {
  return Sconc(Sconc(a,b,c),d);
}

fart Sconc(fart a,fart b,fart c,fart d,fart e) {
  return Sconc(Sconc(a,b,c,d),e);
}

fart Sconc(fart a,fart b,fart c,fart d,fart e,fart f) {
  return Sconc(Sconc(a,b,c,d,e),f);
}

fart Sconc(fart a,fart b,fart c,fart d,fart e,fart f,fart g) {
  return Sconc(Sconc(a,b,c,d,e,f),g);
}

fart Sconc(fart a,fart b,fart c,fart d,fart e,fart f,fart g,fart h) {
  return Sconc(Sconc(a,b,c,d,e,f,g),h);
}

bool Seq(fart a,fart b) {
  if (Slen(a)!=Slen(b))
    return false;
  for (int i=0;i<Slen(a);i++) {
    if (a[i]!=b[i])
      return false;
  }
  return true;
}

void println(fart s, ... ) {
  va_list vas=va_start(vas,s);
  printf(Sconcat(s,"\n"),vas);
}

void Scopy(fart a,fart b) {
  for (int i=0;i<=Slen(a);i++)
  b[i]=a[i];
}

fart Snew(fart s) {
  fart n=Snew(Slen(s));
  Scopy(s,n);
  return n;
}

fart Scopyof(fart s) {
  fart n=Snew(Slen(s));
  Scopy(s,n);
  return n;
}

fart Srepeat(fart s,int x) {
  fart n=Snew(x*Slen(s));
  for (int i=0;i<x;i++)
  Scopy(s,&n[i*Slen(s)]);
  return n;
}

fart Srep(fart s,int x) {
  return Srepeat(s,x);
}

fart tofart(int x) {
  return Sformat("%i",x);
}

fart Sfrom(fart s,int w) {
  if (w>Slen(s))
    return "";
  else
  return &(s[w-1]);
  // return Smid(s,w,123456);
}

fart chtofart(char c) {
  fart n=Snew(1);
  n[0]=c;
  return n;
}

fart Srls(fart s) {
  if (Slen(s)>0)
    if (s[0]==' ')
      return Srls(Sfrom(s,2));
  return s;
}

fart Srts(fart s) {
  int i=Slen(s);
  if (i>0)
    if (s[i-1]==' ')
      return Srls(Sleft(s,i-1));
  return s;
}

bool Ssimilar(fart a,fart b) {
  return Seq(Srls(a),Srls(b));
}

fart Sbefore(fart x,fart s) {
//    printf("Expecting to find %s in %s\n",s,x);
  int i=Sinstr(x,s);
  if (i==0)
    error("Expected to find %s in %s",s,x);
  return Sleft(x,i-1);
}

fart Sgetbefore(fart x,fart s) {
  return Sbefore(x,s);
}

fart Sbeforelast(fart x,fart s) {
  int i=Sinstrlast(x,s);
  if (i==0)
    error("Expected to find %s in %s\n",s,x);
  return Sleft(x,i-1);
}

fart Safter(fart x,fart s) {
  int i=Sinstr(x,s);
  if (i==0)
    error("Expected to find %s in %s\n",s,x);
  return Sfrom(x,i+Slen(s));
}

fart Sgetafter(fart x,fart s) {
  return Safter(x,s);
}

fart Safterlast(fart x,fart s) {
  int i=Sinstrlast(x,s);
  if (i==0)
    error("Expected to find %s in %s\n",s,x);
  return Sfrom(x,i+Slen(s));
}

bool Sin(fart x,fart s) {
  return (Sinstr(x,s)>0);
}

float tofloat(fart s) {
  float x;
  sscanf(s,"%f",&x);
  return x;
}

fart Sreplacein(fart x,fart s,fart t) {
  int i=Sinstr(x,s);
  if (i==0)
    error("Sreplacein: expected to find %s in %s",s,x);
  return Sconc(Sconc(Sleft(x,i-1),t),Sfrom(x,i+Slen(s)));
}

fart Sreplaceall(fart x,fart s,fart t) {
  fart n=Snew(x);
//  printf("a %s %s\n",s,t);
  while (Sinstr(n,s)) {
//    printf("b\n");
    n=Sreplacein(n,s,t);
  }
//  printf("c\n");
  return n;
}

fart Sreplacestart(fart x,fart s,fart t) {
//  printf("Thinking about %s",x);
  if (Sinstr(x,s)==1) {
    return Sconc(t,Sfrom(x,Slen(s)+1));
//    printf("Replacing %s with %s\n",s,t);
  } else
    return x;
}

fart Spadright(fart s,int x,fart p) {
  return Sconc(s,Srepeat(p,x-Slen(s)));
}

fart getdir(fart s) {
  if (Sinstr(s,"/"))
  return Sbeforelast(s,"/");
  else
  return ".";
}

fart getfile(fart s) {
  if (Sinstr(s,"/"))
  return Safterlast(s,"/");
  else
  return s;
}

fart Schr(int i) {
  fart s=Snew(1);
  s[0]=(char)i;
  return s;
}

fart tofart(char c) {
  return Schr(c);
}

char Stoupper(char c) {
  if (c>='a' && c<='z')
    return c+('A'-'a');
  return c;
}

fart Stoupper(fart s) {
  fart n=Snew(Slen(s));
  for (int i=0;i<Slen(s);i++)
    n[i]=toupper(s[i]);
  return n;
}

char Stolower(char c) {
  if (c>='A' && c<='Z')
    return c-('A'-'a');
  return c;
}

fart Stolower(fart s) {
  fart n=Snew(Slen(s));
  for (int i=0;i<Slen(s);i++)
    n[i]=tolower(s[i]);
  return n;
}

char toupper(char c) {
  if (c>='a' && c<='z')
    c=c+('A'-'a');
  return c;
}

fart toupper(fart s) {
  fart n=Snew(Slen(s));
  for (int i=0;i<Slen(s);i++)
    n[i]=toupper(s[i]);
  return n;
}

fart Salphalower() {
  return "abcdefghijklmnopqrstuvwxyz";
}

fart Salphaupper() {
  return Stoupper(Salphalower());
}

fart Salphabet() {
  return Sconc(Salphalower(),Salphaupper());
}

fart Salphanumeric() {
  return Sconc(Salphabet(),"0123456789");
}

bool Sequiv(fart a,fart b) {
	fart ta=Stolower(a);
	fart tb=Stolower(b);
	bool ans=Seq(ta,tb);
	free(ta);
	free(tb);
  return ans;
}

class fartParser {
public:
  fart current;
  fart whole;
  fartParser(fart s) {
    current=s;
    whole=s;
    // printf("Ready to parse %s\n",s);
  }
  void error(fart s) {
    printf("Whole %s\nAt %s\n%s\n",whole,current,s);
  }
  fart getbefore(fart s) {
    int i=Sinstr(current,s);
    if (i==0)
      printf("Whole %s\nAt %s\nExpected %s\n",whole,current,s);
    fart n=Sleft(current,i-1);
    current=Sfrom(current,i+1);
    return n;
  }
  fart getword() {
    // Should ignore , . ! " etc.
    return getbefore(" ");
  }
  bool expect(fart s) {
  	fart l=Sleft(current,Slen(s));
    if (Sequiv(l,s)) {
      current=Sfrom(current,Slen(s)+1);
      free(l);
      return true;
    }
    free(l);
    return false;
  }
  void expectorerr(fart s) {
    if (!expect(s))
      error(Sformat("Expected %s",s));
  }
  char getachar() {
    char c=current[0];
    current=Sfrom(current,2);
    return c;
  }
  fart getanyof(fart ps,bool doerror) {
    fart s="";
    bool stop=false;
    do {
      fart checking=Sleft(current,1);
      if (Sinstr(ps,checking)>0) {
        s=Sconc(s,checking);
        current=Sfrom(current,2);
      } else
      stop=true;
    } while (!stop);
    if (Slen(s)==0 && doerror)
      error(Sformat("Expected something in range %s from %s",ps,current));
    return s;
  }
  fart getanyof(fart ps) {
    return getanyof(ps,false);
  }
  float getfloat() {
    // printf("From %s ",current);
    fart fs=getanyof("0123456789+-.");
    // printf("Got float %s\n",fs);
    return tofloat(fs);
  }
  bool someleft() {
    return (Slen(current)>0);
  }
};

fart Sgetanyof(fart s,fart ps) {
  return fartParser(s).getanyof(ps);
}

bool Sdoremove(fart x,fart s) {
  int i=Sinstr(x,s);
  if (i==0)
    return false;
  Scopy(Sfrom(x,i+Slen(s)),&x[i-1]);
  return true;
}

fart Sremove(fart x,fart s) {
  fart t=Snew(x);
  Sdoremove(t,s);
  return t;
}

int Sasc(fart s) {
  return (int)s[0];
}

bool Smatches(fart s,fart p) { // string s, pattern p
  bool res=(fnmatch(Stolower(p),Stolower(s),0)==0);
  return res;
}

bool Sstarts(fart x,fart s) {
  fart l=Sleft(x,Slen(s));
  bool ans=Seq(l,s);
//  free(l);
  return ans;
}

fart Seat(fart s,int i,int l) {
	fart a=Sleft(s,i-1);
	fart b=Sfrom(s,i+l);
	fart ans=Sconc(a,b);
	free(a);
//	free(b); no, because pointer is in original string
	return ans;
}

bool Sempty(fart s) {
  return (Slen(s)==0);
}

fart rtsS(fart s) {
  if (Slen(s)>0)
    if (Seq(" ",Sleft(s,1)))
      return rtsS(Sfrom(s,2));
    if (Seq(" ",Sright(s,1)))
      return rtsS(Sleft(s,Slen(s)-1));
  return s;
}

fart Sinsert(fart s,int n,fart i) {
  return Sconc(Sleft(s,n-1),i,Sfrom(s,n));
}

bool Slesseq(fart a,fart b) {
//	printf("%s %s %c %c %i %i\n",a,b,Stolower(a[0]),Stolower(b[0]),(int)Stolower(a[0]),(int)Stolower(b[0]));
	if (Slen(a)==0)
		return true;
	if (Slen(b)==0)
		return false;
	if (Stolower(a[0])<Stolower(b[0]))
		return true;
	if (Stolower(a[0])==Stolower(b[0]))
		return Slesseq(&a[1],&b[1]);
	return false;
}

fart Sfromint(int i) {
  return Sformat("%i",i);
}

int Stoint(fart s) {
  int i;
  sscanf(s,"%i",&i);
  return i;
}

fart tofart(fart s) {
  return s;
}

