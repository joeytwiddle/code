#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#ifndef CYGWIN
#include <fnmatch.h>
#endif

#define String char *

// - All parameters given to Strings assume 1 is first character in string
// - Warning: String a,b; declares a as char* and b as char !

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

int Sinstr(String x,String s,int start) {
	int xlen=Slen(x);
	int slen=Slen(s);
	for (int i=start-1;i<xlen-slen+1;i++) {
		int j=0;
		while (x[i+j]==s[j]) {
			j++;
			if (j==slen)
				return i+1;
		}
	}
	return 0;
}

int Sinstr(String x,String s) {
	return Sinstr(x,s,1);
}

int Sinstr(String x,char c) {
	for (int i=0;i<Slen(x);i++) {
		if (x[i]==c)
			return i+1;
	}
	return 0;
}

int Sinstrlast(String x,String s) {
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

int badScount(String x,String s) {
	String q=s;
	int c=0;
	int ch;
	while ((ch=Sinstr(x,s))>0) {
		c++;
		q=&(q[ch]);
	}
	return c;
}


int Scount(String x,String s) {
	int i=Sinstr(x,s);
	if (i==0)
		return 0;
	else
		return 1+Scount(&x[i],s);
}

String Sformat(String f, ... ) {
	va_list vas;
	va_start(vas,f); // Scount(f,"%")
	String t=new char[256];
	vsprintf(t,f,vas);
	return t;
}

String myformat(String f, ... ) {
	va_list vas;
	va_start(vas,f);
	return Sformat(f,vas);
}

String Smid(String f,int s,int l) {
	String n=new char[l+1];
	for (int i=0;i<l;i++) {
		n[i]=f[s-1+i];
	}
	n[l]='\0';
	return n;
}

String Smid(String f,int i) {
	return Smid(f,i,1);
}

// *** Sick - all my String functions work from 1 onwards!

String Ssub(String s,int x,int y) {
	return Smid(s,x,y-x);
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

String Sconcd(String a,String b) { // Destroys a and b!
	String ans=Sconcat(a,b);
	free(a);
	free(b);
	return ans;
}

String Sconc(String a,String b) {
	return Sconcat(a,b);
}

String Sconc(String a,String b,String c) {
	return Sconc(Sconc(a,b),c);
}

String Sconc(String a,String b,String c,String d) {
	return Sconc(Sconc(a,b,c),d);
}

String Sconc(String a,String b,String c,String d,String e) {
	return Sconc(Sconc(a,b,c,d),e);
}

String Sconc(String a,String b,String c,String d,String e,String f) {
	return Sconc(Sconc(a,b,c,d,e),f);
}

String Sconc(String a,String b,String c,String d,String e,String f,String g) {
	return Sconc(Sconc(a,b,c,d,e,f),g);
}

String Sconc(String a,String b,String c,String d,String e,String f,String g,String h) {
	return Sconc(Sconc(a,b,c,d,e,f,g),h);
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
	va_list vas;
	va_start(vas,s);
	printf(Sconcat(s,"\n"),vas);
}

void Scopy(String a,String b) {
	for (int i=0;i<=Slen(a);i++)
		b[i]=a[i];
}

String Snew(String s) {
	String n=Snew(Slen(s));
	Scopy(s,n);
	return n;
}

String Scopyof(String s) {
	String n=Snew(Slen(s));
	Scopy(s,n);
	return n;
}

String Srepeat(String s,int x) {
	String n=Snew(x*Slen(s));
	for (int i=0;i<x;i++)
		Scopy(s,&n[i*Slen(s)]);
	return n;
}

String Srep(String s,int x) {
	return Srepeat(s,x);
}

String toString(int x) {
	return Sformat("%i",x);
}

	String Sfrom(String s,int w) {
		if (w>Slen(s))
			return "";
		else
			return &(s[w-1]);
		// return Smid(s,w,123456);
	}

String chtoString(char c) {
	String n=Snew(1);
	n[0]=c;
	return n;
}

	String Srls(String s) {
		if (Slen(s)>0)
			if (s[0]==' ')
				return Srls(Sfrom(s,2));
		return s;
	}

String Srts(String s) {
	int i=Slen(s);
	if (i>0)
		if (s[i-1]==' ')
			return Srls(Sleft(s,i-1));
	return s;
}

String Sbefore(String x,String s) {
	//    printf("Expecting to find %s in %s\n",s,x);
	int i=Sinstr(x,s);
	if (i==0)
		error("Expected to find %s in %s",s,x);
	return Sleft(x,i-1);
}

String Sgetbefore(String x,String s) {
	return Sbefore(x,s);
}

String Sbeforelast(String x,String s) {
	int i=Sinstrlast(x,s);
	if (i==0)
		error("Expected to find %s in %s\n",s,x);
	return Sleft(x,i-1);
}

String Safter(String x,String s) {
	int i=Sinstr(x,s);
	if (i==0)
		error("Expected to find %s in %s\n",s,x);
	return Sfrom(x,i+Slen(s));
}

String Sgetafter(String x,String s) {
	return Safter(x,s);
}

String Safterlast(String x,String s) {
	int i=Sinstrlast(x,s);
	if (i==0)
		error("Expected to find %s in %s\n",s,x);
	return Sfrom(x,i+Slen(s));
}

String Safterlastorall(String x,String s) {
	int i=Sinstrlast(x,s);
	if (i==0)
		return x;
	return Sfrom(x,i+Slen(s));
}

bool Sin(String x,String s) {
	return (Sinstr(x,s)>0);
}

float tofloat(String s) {
	float x;
	sscanf(s,"%f",&x);
	return x;
}

String Sreplacein(String x,String s,String t) {
	int i=Sinstr(x,s);
	if (i==0)
		error("Sreplacein: expected to find %s in %s",s,x);
	return Sconc(Sleft(x,i-1),t,Sfrom(x,i+Slen(s)));
}

String Sreplaceall(String x,String s,String t) {
	String n=Snew(x);
	int sofar=0;
	int i;
	while ((i=Sinstr(n,s,sofar))>0) {
		n=Sconc(Sleft(n,i-1),t,Sfrom(n,i+Slen(s)));
		sofar=i+Slen(t);
	}
	return n;
}

String Sreplace(String x,String s,String t) {
	return Sreplaceall(x,s,t);
}

String Sreplacestart(String x,String s,String t) {
	//  printf("Thinking about %s",x);
	if (Sinstr(x,s)==1) {
		return Sconc(t,Sfrom(x,Slen(s)+1));
		//    printf("Replacing %s with %s\n",s,t);
	} else
		return x;
}

String Spadright(String s,int x,String p) {
	return Sconc(s,Srepeat(p,x-Slen(s)));
}

String getdir(String s) {
	if (Sinstr(s,"/"))
		return Sbeforelast(s,"/");
	else
		return ".";
}

String getfile(String s) {
	if (Sinstr(s,"/"))
		return Safterlast(s,"/");
	else
		return s;
}

String Schr(int i) {
	String s=Snew(1);
	s[0]=(char)i;
	return s;
}

String toString(char c) {
	return Schr(c);
}

/*char toupper(char c) {
  if (c>='a' && c<='z')
  c=c+('A'-'a');
  return c;
  }

  String toupper(String s) {
  String n=Snew(Slen(s));
  for (int i=0;i<Slen(s);i++)
  n[i]=toupper(s[i]);
  return n;
  }*/

char Stoupper(char c) {
	if (c>='a' && c<='z')
		return c+('A'-'a');
	return c;
}

String Stoupper(String s) {
	String n=Snew(Slen(s));
	for (int i=0;i<Slen(s);i++)
		n[i]=Stoupper(s[i]);
	return n;
}

char Stolower(char c) {
	if (c>='A' && c<='Z')
		return c-('A'-'a');
	return c;
}

String Stolower(String s) {
	String n=Snew(Slen(s));
	for (int i=0;i<Slen(s);i++)
		n[i]=Stolower(s[i]);
	return n;
}

String Salphalower() {
	return "abcdefghijklmnopqrstuvwxyz";
}

String Salphaupper() {
	return Stoupper(Salphalower());
}

String Salphabet() {
	return Sconc(Salphalower(),Salphaupper());
}

String Salphanumeric() {
	return Sconc(Salphabet(),"0123456789");
}

bool Sequiv(String a,String b) {
	/*      String ta=Stolower(a);
			  String tb=Stolower(b);
			  bool ans=Seq(ta,tb);
	//      printf("Comparing %s with %s answer %i\n",ta,tb,ans);
	free(ta);
	free(tb);
	return ans;*/
	if (Slen(a)!=Slen(b))
		return false;
	for (int i=0;i<Slen(a);i++) {
		if (Stolower(a[i])!=Stolower(b[i]))
			return false;
	}
	return true;
}

bool Ssimilar(String a,String b) {
	return Sequiv(Srls(a),Srls(b));
}

class StringParser {
	public:
		String current;
		String whole;
		StringParser(String s) {
			current=s;
			whole=s;
			// printf("Ready to parse %s\n",s);
		}
		void error(String s) {
			printf("Whole %s\nAt %s\n%s\n",whole,current,s);
		}
		String getbefore(String s) {
			int i=Sinstr(current,s);
			if (i==0)
				printf("Whole %s\nAt %s\nExpected %s\n",whole,current,s);
			String n=Sleft(current,i-1);
			current=Sfrom(current,i+1);
			return n;
		}
		String getword() {
			// Should ignore , . ! " etc.
			return getbefore(" ");
		}
		bool expect(String s) {
			String l=Sleft(current,Slen(s));
			if (Sequiv(l,s)) {
				current=Sfrom(current,Slen(s)+1);
				free(l);
				return true;
			}
			free(l);
			return false;
		}
		void expectorerr(String s) {
			if (!expect(s))
				error(Sformat("Expected %s",s));
		}
		char getachar() {
			char c=current[0];
			current=Sfrom(current,2);
			return c;
		}
		String getanyof(String ps,bool doerror) {
			String s="";
			bool stop=false;
			do {
				String checking=Sleft(current,1);
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
		String getanyof(String ps) {
			return getanyof(ps,false);
		}
		float getfloat() {
			// printf("From %s ",current);
			String fs=getanyof("0123456789+-.");
			// printf("Got float %s\n",fs);
			return tofloat(fs);
		}
		bool someleft() {
			return (Slen(current)>0);
		}
};

String Sgetanyof(String s,String ps) {
	return StringParser(s).getanyof(ps);
}

bool Sdoremove(String x,String s) {
	int i=Sinstr(x,s);
	if (i==0)
		return false;
	Scopy(Sfrom(x,i+Slen(s)),&x[i-1]);
	return true;
}

String Sremove(String x,String s) {
	String t=Snew(x);
	Sdoremove(t,s);
	return t;
}

int Sasc(String s) {
	return (int)s[0];
}

#ifndef CYGWIN
bool Smatches(String s,String p) { // string s, pattern p
	/*  String g=Stolower(s);
		 String f=Stolower(p);
		 bool isit=(fnmatch(f,g,FNM_CASEFOLD)==0);
		 if (isit)
		 printf(">%s matches %s<\n",g,f);
		 else
		 printf(">%s does not match %s<\n",g,f);
		 return isit;*/
	bool res=(fnmatch(Stolower(p),Stolower(s),0)==0);
	//  if (!res)
	//    printf("%s !match %s\n",s,p);
	return res;
}
#endif

bool Sstarts(String x,String s) {
	int slen=Slen(s);
	if (Slen(x)<slen)
		return false;
	for (int i=0;i<slen;i++) {
		if (x[i]!=s[i])
			return false;
	}
	return true;
	// String l=Sleft(x,Slen(s));
	// bool ans=Seq(l,s);
	// //  free(l);
	// return ans;
}

String Seat(String s,int i,int l) {
	String a=Sleft(s,i-1);
	String b=Sfrom(s,i+l);
	String ans=Sconc(a,b);
	free(a);
	//      free(b); no, because pointer is in original string
	return ans;
}

bool Sempty(String s) {
	return (Slen(s)==0);
}

	String rtsS(String s) {
		if (Slen(s)>0)
			if (Seq(" ",Sleft(s,1)))
				return rtsS(Sfrom(s,2));
		if (Seq(" ",Sright(s,1)))
			return rtsS(Sleft(s,Slen(s)-1));
		return s;
	}

String Sinsert(String s,int n,String i) {
	return Sconc(Sleft(s,n-1),i,Sfrom(s,n));
}

bool Slesseq(String a,String b) {
	//      printf("%s %s %c %c %i %i\n",a,b,Stolower(a[0]),Stolower(b[0]),(int)Stolower(a[0]),(int)Stolower(b[0]));
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

String Sfromint(int i) {
	return Sformat("%i",i);
}

String Sfromfloat(float i) {
	return Sformat("%f",i);
}

int Stoint(String s) {
	int i;
	sscanf(s,"%i",&i);
	return i;
}

float Stofloat(String s) {
	float i;
	sscanf(s,"%f",&i);
	return i;
}

String toString(String s) {
	return s;
}

String StoCformat(String s) {
	//  printf("%s\n",s);
	s=Sreplaceall(s,"\\","\\\\");
	s=Sreplaceall(s,"\"","\\\"");
	//  s=Sreplaceall(s,"(","\\(");
	//  s=Sreplaceall(s,")","\\)");
	//  printf("%s\n",s);
	return s;
}

String Stoechoformat(String s) {
	return StoCformat(s);
}

float floatFromString(String s) {
	float f;
	sscanf(s,"%f",&f);
	return f;
}

int Sfirstnotin(String s,String cs) {
	for (int i=0;i<Slen(s);i++) {
		char c=s[i];
		if (Sinstr(cs,c)==0) {
			//for (int j=0;j<Slen(cs);j++)
			//printf("%i ",(int)cs[j]);
			//printf("got %i at %i\n",(int)c,i);
			return i;
		}
	}
	return Slen(s);
}

int Slastnotin(String s,String cs) {
	for (int i=Slen(s)-1;i>=0;i--) {
		char c=s[i];
		if (Sinstr(cs,c)==0) {
			//for (int j=0;j<Slen(cs);j++)
			//printf("%i ",(int)cs[j]);
			//printf("got %i at %i\n",(int)c,i);
			return i;
		}
	}
	return -1;
}

String Strim(String s) {
	int firstnonspace=Sfirstnotin(s," \t");
	int lastnonspace=Slastnotin(s," \t");
	String answer = (
			firstnonspace>lastnonspace
			? Snew("")
			: Ssub(s,firstnonspace+1,lastnonspace+2)
			);
	// printf("%i-%i/%i\n>%s<\n>%s<\n",firstnonspace,lastnonspace,Slen(s),s,answer);
	return answer;
}

