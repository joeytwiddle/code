

// redefine Code as List<String>
// put Stuff parsing methods inside the classes!
// put output inside class also





/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, April 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

List<String> ls;
int ln; // Current line number
String l; // Current line
String sl; // Stripped line
String lap; // Needs to be added on end of sl
String fsl; // Functional stripped line (any "..." removed)
int brackets=0;
int bracketslast;

void getline() {
  ln++;
  // printf("Getting %i\n",ln);
  if (ls.inlist(ln))
    l=ls.num(ln);
  else
  l="";
  sl=l;
  lap="";
  // Remove any // onwards
  int i=Sinstr(sl,Sconc("/","/"));
  if (i>0) {
    lap=Sfrom(sl,i);
    sl=Sleft(sl,i-1);
  }
  // fsl: remove any "..."
  fsl=sl;
  int k;
  while ((k=Sinstr(fsl,Schr(34)))>0) {
    fsl=Sfrom(fsl,k+1);
    int j=Sinstr(fsl,Schr(34));
    if (j==0)
      j=Slen(fsl);
    fsl=Sfrom(fsl,j+1);
  }
  bracketslast=brackets;
  brackets=brackets+Scount(fsl,"{")-Scount(fsl,"}");
}

class Arg {
public:
  String type;
  String name;
  Arg() { }
  Arg(String t) {
    type=t;
    name="";
  }
  Arg(String t,String n) {
    type=t;
    name=n;
  }
};

class Stuff {
public:
  virtual String toString() {
    return "just stuff";
  }
  virtual String shortdescr() {
    return "";
  }
};

List<Stuff*> st;

class Code : public Stuff {
public:
  List<String> code;
  Code() { 
  }
  Code(String s) {
    code.add(s);
  }
  void add(String s) {
    code.add(s);
    // printf("Code %s\n",s);
  }
  String toString() {
    String s="";
    for (int i=1;i<=code.len;i++)
    s=Sconc(s,code.num(i),"\n");
    return s;
  }
};

class Method : public Stuff {
public:
  String type;
  String name;
  List<Arg> as;
  Code code;
  Method(String n,String t) {
    name=n;
    type=t;
  }
  String toString() {
    String s="";
    for (int i=1;i<=as.len;i++) {
      s=Sformat("%s%s %s",s,as.num(i).type,as.num(i).name);
      if (i<as.len)
        s=Sconc(s,",");
    }
    return Sconc(type," ",name,"(",s,") {\n",code.toString(),"}\n");
  }
  String shortdescr() {
    return Sformat("method %s\n",name);
  }
  static Stuff *parsemethod() {
    String tmp=Srts(Sbefore(l,"("));
    String type=Sbeforelast(tmp," ");
    String name=Safterlast(tmp," ");
    Method *m=new Method(name,type);
    String tmp2=Srls(Sbefore(Safter(l,"("),")")); // this needs to use { in order to deal with (*fn)
    while (Slen(tmp2)>0) {
      int com=Sinstr(tmp2,",");
      String t;
      if (com>0)
        t=Sleft(tmp2,com-1);
      else
        t=tmp2;
      if (Sinstr(t," ")>0)
        m->as.add(Arg(Sbefore(t," "),Safter(t," ")));
      else
        m->as.add(Arg(t));
      if (com>0)
        tmp2=Sfrom(tmp2,com+1);
      else
        tmp2="";
    }
    // printf("Method %s (type %s) ... %s\n",name,type,l);
    String rst=Safter(l,"{");
    if (Slen(rst)>0)
      m->code.add(rst);
    int bd=bracketslast;
    getline();
    while (brackets>bd) {
      m->code.add(l);
      getline();
    };
    return m;
  }
  
};

Stuff *parseline() {
  // helloworld();
  // printf("%s\n",l);
  int i=Sinstr(l,") {");
  if (i>0 && !Sin(l,"#define") && !Sin(l,"//") && !Sin(l,"/*"))
    return Method::parsemethod();
  else
  return new Code(l);
}

class Class : public Stuff {
public:
  String type;
  String name;
  List<Stuff*> st;
  Code code;
  Class(String a,String b) {
    type=a;
    name=b;
  }
  String toString() {
    String stf="  ";
    for (int i=1;i<=st.len;i++)
    stf=Sconc(stf,st.num(i)->toString());
    return Sconc("class ",name," {\n",stf,"};\n");
  }
  String shortdescr() {
    String s=Sformat("Class %s:\n",name);
    for (int i=1;i<=st.len;i++)
      s=Sconc(s,"  ",st.num(i)->shortdescr());
    return s;
  }
  static Stuff *parseclass() {
    String type=Sgetbefore(sl,"class");
    String name=Srls(Srts(Sgetbefore(Sgetafter(sl,"class"),"{")));
    Class *c=new Class(type,name);
    getline();
    // printf("Class %s (type %s) ... %s\n",name,type,l);
    while (Sinstr(l,"};")==0) {
      c->st.add(parseline());
      getline();
    }
    return c;
  }
};




void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  if (argc<=1) {
    printf("cparser <filename> : Parse a C file\n");
    exit(0);
  }
  
  ls=readlinesfromfile(argv[1]);
  writelinestofile(ls,Sconc(argv[1],".cpbak"));
  
  // Class *curclass=NULL;
  ln=0;
  getline();
  while (ln<ls.len) {
    if (Sinstr(sl,"class")) {
      st.add(Class::parseclass());
    } else {
      st.add(parseline());
    }
    getline();
  }
  
  FILE *fp=fopen("tmp.c","w");
  
  for (int i=1;i<=st.len;i++) {
//    String s=st.num(i)->toString();
    String s=st.num(i)->shortdescr();
    printf("%s",s);
    fprintf(fp,"%s",s);
  }
  
  fclose(fp);
  
}
