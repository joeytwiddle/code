#include <joeylib.c>

void main(int argc,String *argv) {
  
  if (argc!=2)
    error("Syntax: bat2sh <fname.bat>");
  
  String fname=Snew(argv[1]);
  List<String> ls=readlinesfromfile(fname);
  printf("Read\n");
  List<String> ns;
  for (int i=1;i<=ls.len;i++) {
    String s=ls.num(i);
    printf("%s",s);
    s=Sreplaceall(s,"%","$");
    s=Sreplaceall(s," \\"," /mnt/filfirin/");
    s=Sreplaceall(s,"\\","/");
    s=Sreplacestart(Srls(s),"@","");
    s=Sreplacestart(Srls(s),"rem ","#");
    s=Sreplacestart(Srls(s),"call ","");
    printf(" -> %s\n",s);
    ns.add(s);
  }
  fname=Stolower(fname);
  if (Seq(Sright(fname,4),".bat"))
    fname=Sleft(fname,Slen(fname)-4);
  writelinestofile(ns,fname);
  system(Sformat("chmod a+x %s",fname));

}
