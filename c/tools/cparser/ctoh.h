

#include <joeylib.c>

class OutputStream {

public:
  FILE *fp;
  OutputStream(String fname) ;

  write(char c) ;

  write(String s) ;

  writeln(String s) ;

  close() ;

};

void main(int argc,String *argv) ;


