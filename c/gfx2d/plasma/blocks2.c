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

void make(JBmp *to,Map2d<short> *from,int left,int top,int right,int bottom,int col,float power) {
  int size=max(right-left,bottom-top);
  int cenx=(left+right)/2;
  int ceny=(top+bottom)/2;
  int nwcc=from->pos[(left+cenx)/2][(top+ceny)/2]-128;
  int swcc=from->pos[(left+cenx)/2][(bottom+ceny)/2]-128;
  int necc=from->pos[(right+cenx)/2][(top+ceny)/2]-128;
  int secc=from->pos[(right+cenx)/2][(bottom+ceny)/2]-128;
//  int newcoldown=ucharmod(col+power*(from->pos[cenx][(top+ceny)/2]-128));
  int newcol=col;
  float newpow=power*0.5;
    to->bmp[top][left]=newcol;
    to->bmp[top][right]=newcol;
    to->bmp[bottom][left]=newcol;
    to->bmp[bottom][right]=newcol;
  if (size==1) {
//    to->bmp[ceny][cenx]=newcol;
  } else {
    #define ave 1
    make(to,from,left,top,cenx,ceny,ucharmod(col+power*(nwcc+(necc+swcc)/ave)),newpow);
    make(to,from,left,ceny,cenx,bottom,ucharmod(col+power*(swcc+(nwcc+secc)/ave)),newpow);
    make(to,from,cenx,top,right,ceny,ucharmod(col+power*(necc+(nwcc+secc)/ave)),newpow);
    make(to,from,cenx,ceny,right,bottom,ucharmod(col+power*(secc+(swcc+necc)/ave)),newpow);
  }
//  float affect=4*size/100.0;
//  if (size>1) {
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
    from->pos[x][y]=intrnd(0,255);
  }
  to->bmp[0][0]=myrnd()*256;
  to->bmp[0][scrwid-1]=myrnd()*256;
  to->bmp[scrhei-1][0]=myrnd()*256;
  to->bmp[scrhei-1][scrwid-1]=myrnd()*256;
  do {
    make(to,from,0,0,scrwid-1,scrhei-1,0,0.3);
    to->display();
    for (int x=0;x<scrwid;x++)
    for (int y=0;y<scrhei;y++) {
      from->pos[x][y]+=intrnd(-6,5);
    }
  } while (!key[KEY_ESC]);
}
