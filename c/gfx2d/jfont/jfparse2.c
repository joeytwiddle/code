#include <joeylib.c>

float tofloat(String s) {
  float x;
  sscanf("%f",s,&x);
  return x;
}

class StringParser {
public:
  String current;
  String whole;
  StringParser(String s) {
    current=s;
    whole=s;
//    printf("Ready to parse %s\n",s);
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
  char getchar() {
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
//    printf("From %s ",current);
    String fs=getanyof("0123456789+-.");
 //  printf("Got float %s\n",fs);
    return tofloat(fs);
  }
  bool someleft() {
    return (Slen(current)>0);
  }
};

class FontBit {
};

class FontChar {
public:
  FontChar() {
  }
  String name;
  List<FontBit> bs;
  FontChar(String n) {
    name=n;
  }
};

class FLine : public FontBit {
public:
  float xa,xb,ya,yb,w;
  FLine(float a,float b,float c,float d,float e) {
    xa=a;
    xb=c;
    ya=b;
    yb=d;
    w=e;
  }
};

class FArc : FontBit {
public:
  float x,y,r,aa,ab;
  FArc(float a,float b,float c) {
    x=a;
    y=b;
    r=c;
    aa=0;
    ab=2*pi;
  }
};

void main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String fname=a.argafter("-i","Font file (.jf)");
  float fw=a.floatafter("-fw","Font width",0.1);
  // We parse from
  List<String> lines=parsefileintolines(fname);
  // into
  List<FontChar> fcs=List<FontChar>();
  for(int i=1;i<=lines.len;i++) {
    String current=lines.num(i);
//    printf("Parsing line %s\n",current);
    StringParser s=StringParser(current);
    String character=s.getbefore(":");
//    printf("Reading character %s ...",character);
    FontChar fc=FontChar(character);
    while (s.someleft()) {
      char whatisit=s.getchar();
      switch (whatisit) {
        case 'L'
          float xa=tofloat(s.getbefore(","));
          float ya=tofloat(s.getbefore("_"));
          bool keepgoing=true;
          while (keepgoing) {
            float xb=tofloat(s.getbefore(","));
            float yb=s.getfloat();
            fc.bs.add(FLine(xa,ya,xb,yb,fw));
            xa=xb; ya=yb;
//            printf("one line added\n");
//            printf("From %s we pick off ",s.current);
            char c=s.getchar();
//            printf("%c, to get %s\n",c,s.current);
            if (c==';')
              keepgoing=false;
            else
              if (c!='_')
                s.error(Sformat("Expected _, got %c",c));
          };
          break;
        case 'C': String type=s.getanyof("TBLRNESW");
          float x=tofloat(s.getbefore(","));
          float y=tofloat(s.getbefore(","));
          float r=tofloat(s.getbefore(";"));
          FArc fa=FArc(x,y,r);
          if (Seq(type,"T")) {
            fa.aa=0;
            fa.ab=2*pi;
          }
          if (Seq(type,"T")) {
           fa.aa=-pi/2.0;
           fa.ab=pi/2.0;
          }
          if (Seq(type,"B")) {
            fa.aa=pi/2.0;
            fa.ab=2*pi-pi/2.0;
          }
          if (Seq(type,"L")) {
            fa.aa=-pi;
            fa.ab=0;
          }
          if (Seq(type,"R")) {
            fa.aa=0;
           fa.ab=pi;
          }
          if (Seq(type,"NW")) {
            fa.aa=-pi/2.0;
            fa.ab=0;
          }
          if (Seq(type,"NE")) {
            fa.aa=0;
            fa.ab=pi/2.0;
          }
          if (Seq(type,"SE")) {
            fa.aa=pi/2.0;
            fa.ab=pi;
          }
          if (Seq(type,"SW")) {
            fa.aa=-pi;
            fa.ab=-pi/2.0;
          }
          fc.bs.add(fa);
          break;
        case '+':
          String search=s.getbefore(";");
          int find=0;
          for (int j=1;j<i && find==0;j++) {
            if (Seq(search,fcs.p2num(j)->name))
              find=j;
          }
          if (find==0)
//            printf("You haven't defined character %s before %s",find,character);
          fc.bs.add(fcs.p2num(j)->bs);
          break;
        default:
//          printf("What kind of FontBit is a %s?",toString(whatisit));
          break;

      }
    };
    fcs.add(fc);
    printf(" has %i bits\n",fc.bs.len);
  }
}
