#include <joeylib.c>

#define scrwid 100
#define scrhei 100

void make(JBmp *to,Map2d<short> *from,int left,int top,int right,int bottom) {
  int size=right-left;
  int cenx=(left+right)/2;
  int ceny=(top+bottom)/2;
  to->bmp[top][cenx]=(to->bmp[top][left]+to->bmp[top][right])/2;
  to->bmp[bottom][cenx]=(to->bmp[bottom][left]+to->bmp[bottom][right])/2;
  to->bmp[ceny][left]=(to->bmp[top][left]+to->bmp[bottom][left])/2;
  to->bmp[ceny][right]=(to->bmp[top][right]+to->bmp[bottom][right])/2;
  int newcol=(to->bmp[top][left]+to->bmp[bottom][left]+to->bmp[top][right]+to->bmp[bottom][right])/4;
  float affect=size/100.0;
  newcol=(int)(newcol+affect*from->pos[cenx][ceny])%256;
  to->bmp[ceny][cenx]=newcol;
  if (size>1) {
    make(to,from,left,top,cenx,ceny);
    make(to,from,left,ceny,cenx,bottom);
    make(to,from,cenx,top,right,ceny);
    make(to,from,cenx,ceny,right,bottom);
  }
}

void main() {
  Map2d<short> *from=new Map2d<short>(scrwid,scrhei);
  JBmp *to=new JBmp(scrwid,scrhei);
  randomise();
  allegrosetup(320,200);
  for (int i=0;i<256;i++) {
    mypalette(i,myRGB::hue((float)i/256));
  }
  for (int x=0;x<scrwid;x++)
  for (int y=0;y<scrhei;y++) {
    from->pos[x][y]=intrnd(-30,30);
  }
  to->bmp[0][0]=myrnd()*256;
  to->bmp[0][scrwid-1]=myrnd()*256;
  to->bmp[scrhei-1][0]=myrnd()*256;
  to->bmp[scrhei-1][scrwid-1]=myrnd()*256;
  do {
    make(to,from,0,0,scrwid-1,scrhei-1);
    to->display();
    for (int x=0;x<scrwid;x++)
    for (int y=0;y<scrhei;y++) {
      from->pos[x][y]+=intrnd(-1,1);
    }
  } while (!key[KEY_ESC]);
}
