#include <joeylib.c>

#define scrwid 100
#define scrhei 100

int merge(uchar x,uchar y,float amount) {
  int d=myabs(y-x);
  int s=mysgn(y-x);
  if (d>128) {
    d=256-d;
    s=-s;
  }
  return (uchar)(x+s*d*amount);
}

int merge(uchar x,uchar y) {
  return merge(x,y,0.5);
}

void make(JBmp *to,Map2d<short> *from,int left,int top,int right,int bottom) {
  int size=right-left;
  int cenx=(left+right)/2;
  int ceny=(top+bottom)/2;
  to->bmp[top][cenx]=merge(to->bmp[top][left],to->bmp[top][right]);
  to->bmp[bottom][cenx]=merge(to->bmp[bottom][left],to->bmp[bottom][right]);
  to->bmp[ceny][left]=merge(to->bmp[top][left],to->bmp[bottom][left]);
  to->bmp[ceny][right]=merge(to->bmp[top][right],to->bmp[bottom][right]);
  int newcol=merge(merge(to->bmp[top][left],to->bmp[bottom][left]),merge(to->bmp[top][right],to->bmp[bottom][right]));
  float affect=4*size/100.0;
//  newcol=merge(newcol,from->pos[cenx][ceny],affect);
  newcol=(uchar)(newcol+affect*from->pos[cenx][ceny]);
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
    from->pos[x][y]=intrnd(-60,60);
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
      from->pos[x][y]+=intrnd(-5,5);
    }
  } while (!key[KEY_ESC]);
}
