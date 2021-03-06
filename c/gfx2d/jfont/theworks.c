#include <joeylib.c>

#define scrwid 320
#define scrhei 200

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  String fname=a.argafter("-i","Font file (.jf)");
//  float size=scrhei/2.0;
  float size=scrhei/2.0*5;
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  randomise();

  JBmp j=JBmp(scrwid,scrhei,0);
  JFont font=JFont(fname);
//  String word="retro asymptotic cyberdruid";
  String word=Salphabet();
  int l=1;
  while (!keypressed()) {
//    String c=Smid(word,intrnd(1,Slen(word)),1);
    String c=Smid(word,l,1);
    if (myrnd()<0.07)
    font.writeString(Smid(word,intrnd(1,Slen(word)),1),&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    if (myrnd()<0.07)
    font.centerString("retro",&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    if (myrnd()<0.07)
    font.centerString("asymptotic",&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    if (myrnd()<0.07)
    font.centerString("cyber druid",&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    j.writetoscreen();
    j.fadeby(10);
    j.smooth(2);
    if (myrnd()<0.1)
      l=(l%Slen(word))+1;
  }

}
