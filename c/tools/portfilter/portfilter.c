#include <joeylib.c>

void expect(String a,String b) {
  if (!Seq(a,b))
    error("Got %s expected %s",b,a);
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  String app=a.argor("append","tar cfz hwiport.tgz");
  String dir=a.argor("directory",".");
  a.done();

  DirRec d=DirRec(dir);
  d.portfilter();
  printf("%s",app);
  d.portdisplay("");
  printf("\n");
  
}
