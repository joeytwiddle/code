#include <joeylib.c>

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  a.done();
  
  randomise();
  
  int scrwid=320;
  int scrhei=200;
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  PPsetup(scrwid,scrhei,2.0);
  JBmp img=JBmp(scrwid,scrhei);
  
}
