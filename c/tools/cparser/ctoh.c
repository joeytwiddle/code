/* This source code is freely distributable under the LGPL
   (lesser GNU public licence).  Please keep this tag with
   this code, and add your own if you contribute.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark | www.changetheworld.org.uk */

#define QUIET

#include <joeylib.c>

class OutputStream {
public:
  FILE *fp;
  OutputStream(String fname) {
    fp=fopen(fname,"w");
    if (fp==NULL)
      error("Could not open OutputStream to file %s",fname);
  }
  write(char c) {
    fputc(c,fp);
  }
  write(String s) {
    fputs(s,fp);
  }
  writeln(String s) {
    write(Sformat("%s\n",s));
  }
  close() {
    fclose(fp);
  }
};

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  String fname=a.getarg("name of .c file");
  String oname=a.getarg("name of .h output file");
  a.done();

  List<String> ls=readlinesfromfile(fname);
  OutputStream os=OutputStream(oname);
  int brackets=0;
  bool inclass=false;
  bool comment=false;
  for (int i=1;i<=ls.len;i++) {
    String s=ls.num(i);
    if (Sstarts(s,"class")) {
      os.writeln(s);
      inclass=true;
    } else
    for (int j=0;j<Slen(s);j++) {
      if (Sstarts(&s[j],"//")) {
        j=Slen(s);
      } else if (Sstarts(&s[j],"/*")) {
        comment=true;
      } else if (Sstarts(&s[j],"*/")) {
        comment=false;
        j++;
      } else {
        char c=s[j];
        if (c=='{') {
          if (!comment && brackets<1)
            os.writeln(";");
          brackets++;
        }
        if (!comment && brackets<1)
          os.write(c);
        if (c=='}') {
          brackets--;
          if (brackets<0) {
            if (inclass) {
              inclass=false;
              brackets=0;
            } else
              error("brackets<0 but not in class!");
          }
        }
      }
    }
    if (!comment && brackets<1)
      os.write('\n');
  }
  os.close();

}
