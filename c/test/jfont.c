#include <joeylib.c>

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  a.done();

  RGBmp r=RGBmp(1000,300,myRGB(255,255,255));
  JFont j=JFont();
  for (int i=1;i<=26;i++) {
    String s="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    String s2="abcdefghijklmnopqrstuvwxyz";
    j.centerString(s,&r,100,25,myRGB(0,0,0));
    j.centerString(s2,&r,200,25,myRGB(0,0,0));
  }
  r.writefile("jf.bmp");
 
}
