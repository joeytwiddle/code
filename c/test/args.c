#include <joeylib.c>

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  String s=a.argafter("-fart","Fart","fart0");
  bool t=a.argexists("-smell","make it smell");
  while (a.argsleft())
    printf("\"%s\"\n",a.getarg());
  a.done();
  
}
