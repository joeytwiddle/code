#include <joeylib.c>

#define scrwid 100
#define scrhei 100

void make(RGBmp *to,RGBmp *from,int left,int top,int right,int bottom) {
  int size=right-left;
  int cenx=(left+right)/2;
  int ceny=(top+bottom)/2;
  to->bmp[top][cenx]=to->bmp[top][left]/2+to->bmp[top][right]/2;
  to->bmp[bottom][cenx]=to->bmp[bottom][left]/2+to->bmp[bottom][right]/2;
  to->bmp[ceny][left]=to->bmp[top][left]/2+to->bmp[bottom][left]/2;
  to->bmp[ceny][right]=to->bmp[top][right]/2+to->bmp[bottom][right]/2;
  myRGB newcol=(to->bmp[top][left]/2+to->bmp[bottom][left]/2)/2+(to->bmp[top][right]/2+to->bmp[bottom][right]/2)/2;
  float affect=size/100.0;
  affect=0;
  newcol=newcol*(1.0-affect)+from->bmp[ceny][cenx]*affect;
  to->bmp[ceny][cenx]=newcol;
  if (size>1) {
    make(to,from,left,top,cenx,ceny);
    make(to,from,left,ceny,cenx,bottom);
    make(to,from,cenx,top,right,ceny);
    make(to,from,cenx,ceny,right,bottom);
  }
}

void main() {
  RGBmp *j=new RGBmp(scrwid,scrhei);
  RGBmp *k=new RGBmp(scrwid,scrhei);
  randomise();
//  setcolordepth(16);
//  allegrosetup(320,200);
  for (int x=0;x<scrwid;x++)
  for (int y=0;y<scrhei;y++) {
    j->bmp[y][x]=myRGB(myrnd(),myrnd(),myrnd());
  }
  do {
    k->bmp[0][0]=j->bmp[0][0];
    k->bmp[0][scrwid-1]=j->bmp[0][scrwid-1];
    k->bmp[scrhei-1][0]=j->bmp[scrhei-1][0];
    k->bmp[scrhei-1][scrwid-1]=j->bmp[scrhei-1][scrwid-1];
    make(k,j,0,0,scrwid-1,scrhei-1);
    k->display();
    for (int x=0;x<scrwid;x++)
    for (int y=0;y<scrhei;y++) {
    }
  } while (!key[KEY_ESC]);
}
