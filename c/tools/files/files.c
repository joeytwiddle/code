#include <joeylib.c>

void expect(String a,String b) {
  if (!Seq(a,b))
    error("Got %s expected %s",b,a);
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  String dir=a.argor("directory",".");
  a.done();

  DirRec d=DirRec(dir);
  d.displayratio();
  
}
