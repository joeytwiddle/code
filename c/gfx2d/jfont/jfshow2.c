#include <joeylib.c>

#define scrwid 640
#define scrhei 480

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  String fname=a.argafter("-i","Font file (.jf)");
  float size=scrhei/2.0;
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  randomise();

  JBmp j=JBmp(scrwid,scrhei,0);
  JFont font=JFont(fname);
//  String word="cyberdruid";
  String word=Salphabet();
  int l=1;
  while (!keypressed()) {
//    String c=Smid(word,intrnd(1,Slen(word)),1);
    String c=Smid(word,l,1);
    font.writechar(c,&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    j.writetoscreen();
    j.fadeby(10);
    if (myrnd()<0.1)
      l=(l%Slen(word))+1;
  }

}
