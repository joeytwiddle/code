#include <joeylib.c>

#define scrwid 100
#define scrhei 100

#define ss 6
//#define cs ((int)(pow(ss,9))
#define cs ss*ss*ss*ss*ss*ss*ss*ss*ss

void main() {
  JBmp *j=new JBmp(scrwid,scrhei);
  JBmp *k=new JBmp(scrwid,scrhei);
  JBmp *tmp;
  short *change=new short[cs];
  srand((int)time(NULL));
  for (int i=0;i<cs;i++) {
    change[i]=(i/ss/ss/ss/ss)%ss;
    if (myrnd()>.8)
      change[i]=(change[i]-1+ss)%ss;
//      change[i]=myrnd()*ss;
  }
  for (int x=0;x<scrwid;x++)
  for (int y=0;y<scrhei;y++) {
    j->bmp[y][x]=myrnd()*ss;
//    j->bmp[y][x]=0;
  }
  allegrosetup(320,200);
  for (int i=0;i<ss;i++) {
//    mypalette(i,myrnd(),myrnd(),myrnd());
    mypalette(i,(float)i/ss,(float)i/ss,(float)i/ss);
//    mypalette(i,myRGB::hue((float)i/ss));
  }
  do {
    j->writetoscreen();
    tmp=k;
    k=j;
    j=tmp;
    for (int x=0;x<scrwid;x++)
    for (int y=0;y<scrhei;y++) {
      // Collect neighbours
      int c=0;
      int n=0;
      for (int dx=-1;dx<=1;dx++)
      for (int dy=-1;dy<=1;dy++) {
        n=n+pow(ss,c)*k->pointat(x+dx,y+dy);
        c++;
      }
      j->bmp[y][x]=change[n];
    }
  } while (!key[KEY_ESC]);
}
