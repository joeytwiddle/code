#include <joeylib.c>

#define scrwid 100
#define scrhei 100

// R+G=Y
// R+B=M
// R+C=0

int merge(int a,int b,float amount) {
  int d=abs(a-b);
  int s=mysgn(a-b);
  if (d>128) {
    d=256-128;
    s=-s;
  }
  return mymod(a+s*d*amount,256);
}

int merge(int a,int b) {
  return merge(a,b,0.5);
}

void make(JBmp *to,JBmp *from,int left,int top,int right,int bottom) {
  int size=right-left;
  int cenx=(left+right)/2;
  int ceny=(top+bottom)/2;
  to->bmp[top][cenx]=merge(to->bmp[top][left],to->bmp[top][right]);
  to->bmp[bottom][cenx]=merge(to->bmp[bottom][left],to->bmp[bottom][right]);
  to->bmp[ceny][left]=merge(to->bmp[top][left],to->bmp[bottom][left]);
  to->bmp[ceny][right]=merge(to->bmp[top][right],to->bmp[bottom][right]);
  int newcol=merge(merge(to->bmp[top][left],to->bmp[bottom][left]),merge(to->bmp[top][right],to->bmp[bottom][right]));
  float affect=size/100.0;
  newcol=merge(newcol,from->bmp[ceny][cenx],affect);
  to->bmp[ceny][cenx]=newcol;
  if (size>1) {
    make(to,from,left,top,cenx,ceny);
    make(to,from,left,ceny,cenx,bottom);
    make(to,from,cenx,top,right,ceny);
    make(to,from,cenx,ceny,right,bottom);
  }
}

void main() {
  JBmp *j=new JBmp(scrwid,scrhei);
  JBmp *k=new JBmp(scrwid,scrhei);
  randomise();
  allegrosetup(320,200);
  for (int i=0;i<256;i++) {
    mypalette(i,myRGB::hue((float)i/256));
  }
  for (int x=0;x<scrwid;x++)
  for (int y=0;y<scrhei;y++) {
    j->bmp[y][x]=myrnd()*255;
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
      int c=j->bmp[y][x];
      c=chop(c+intrnd(-1,1),0,256);
      j->bmp[y][x]=c;
    }
  } while (!key[KEY_ESC]);
}
