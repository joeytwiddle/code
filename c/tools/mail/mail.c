#include <joeylib.c>

// I expect there are still problems when a header is included as a
// forwarded mail (and not indented with > )

#include "email.c"

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  String fname=a.getarg(1);
  a.done();

	List<String> ls;
  ls=mail2list(fname);

  String destdir=Sformat("O%s",fname);
  system(Sformat("mkdir %s",destdir));
  system(Sformat("rm %s/*",destdir));

  int i=1;
  while (i>0 && i<=ls.len) {
    Email e=Email();
    int j=e.parsefrom(&ls,i);
//    printf("Found email:\n");
//    for (int z=1;z<=e.content.len;z++)
//      printf("%s\n",e.content.num(z));
    writelinestofile(e.whole(),Sformat("%s/%s",destdir,getnextfilename("txt")));
    i=j;
  }

}
