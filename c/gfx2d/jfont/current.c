#include <joeylib.c>

#define scrwid 320
#define scrhei 200

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  String fname=a.argafter("-i","Font file (.jf)");
  int blur=a.intafter("-b","blurring",1);
  float size=scrhei/2.0;
//  float size=scrhei/2.0*5;

  JBmp j=JBmp(scrwid,scrhei,0);
  j.setupfastsmooth(blur);
  JFont font=JFont(fname);
//  String word="retro asymptotic cyberdruid";
  String word=Salphabet();
  int l=1;
//  exit(0);
  waitforkeypress();
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  randomise();
  while (!keypressed()) {
//    String c=Smid(word,intrnd(1,Slen(word)),1);
    String c=Smid(word,l,1);
    if (myrnd()<0.2)
    font.writeString(Smid(word,intrnd(1,Slen(word)),1),&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    if (myrnd()<0.07)
    font.centerString("0123456789",&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    if (myrnd()<0.07)
    font.centerString("Zretro",&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    if (myrnd()<0.07)
    font.centerString("asymZptotic",&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    if (myrnd()<0.07)
    font.centerString("cyberZdruid",&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    if (myrnd()<0.07)
    font.centerString(Smid(Salphanumeric(),intrnd(1,26),intrnd(1,10)),&j,myrnd()*scrwid,myrnd()*scrhei,0.5+0.5*myrnd()*size,255);
    j.fadeby(10);
    j.fastsmooth();
    j.writetoscreen();
    //l=(l%Slen(word))+1;
  }

}
