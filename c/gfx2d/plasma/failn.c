#include <joeylib.c>

#define scrwid 100
#define scrhei 100

#define cornrnd 6

uchar merge(uchar x,uchar y,float amount) {
  return (uchar)(x+amount*(y-x));
  int d=myabs(y-x);
  int s=mysgn(y-x);
  if (d>128) {
    d=256-d;
    s=-s;
  }
  return (uchar)chop(x+s*d*amount,0,256);
}

uchar merge(uchar x,uchar y) {
  return merge(x,y,0.5);
}

void make(JBmp *rto,Map2d<int> *to,Map2d<short> *from,int left,int top,int right,int bottom) {
  int size=right-left;
  int cenx=(left+right)/2;
  int ceny=(top+bottom)/2;
  to->pos[cenx][top]=merge(to->pos[left][top],to->pos[right][top]);
  to->pos[cenx][bottom]=merge(to->pos[left][bottom],to->pos[right][bottom]);
  to->pos[left][ceny]=merge(to->pos[left][top],to->pos[left][bottom]);
  to->pos[right][ceny]=merge(to->pos[right][top],to->pos[right][bottom]);
  int newcol=merge(merge(to->pos[left][top],to->pos[left][bottom]),merge(to->pos[right][top],to->pos[right][bottom]));
  float affect=4*size/100.0;
//  newcol=merge(newcol,from->pos[cenx][ceny],affect);
  newcol=ucharmod(newcol+affect*(from->pos[cenx][ceny]-128));
  to->pos[cenx][ceny]=newcol;
  rto->bmp[ceny][cenx]=ucharmod(newcol);
  if (size>1) {
    make(rto,to,from,left,top,cenx,ceny);
    make(rto,to,from,left,ceny,cenx,bottom);
    make(rto,to,from,cenx,top,right,ceny);
    make(rto,to,from,cenx,ceny,right,bottom);
  }
}

void main() {
  JBmp *from=new JBmp(scrwid,scrhei);
  JBmp *rto=new JBmp(scrwid,scrhei);
  Map2d<int> *to=new Map2d<int>(scrwid,scrhei);
  randomise();
  allegrosetup(320,200);
  for (int i=0;i<256;i++) {
    mypalette(i,myRGB::hue((float)i/256));
  }
  for (int x=0;x<scrwid;x++)
  for (int y=0;y<scrhei;y++) {
    from->pos[x][y]=ucharrnd();
  }
  to->pos[0][0]=myrnd()*256;
  to->pos[scrwid-1][0]=myrnd()*256;
  to->pos[0][scrhei-1]=myrnd()*256;
  to->pos[scrwid-1][scrhei-1]=myrnd()*256;
  do {
    // make(to,from,0,0,scrwid-1,scrhei-1);
    from->display();
    for (int x=0;x<scrwid;x++)
    for (int y=0;y<scrhei;y++) {
      // Find outer neighbours
      int left=0;
      int right=scrwid;
      int top=0;
      int bottom=scrhei;
      do {
        int cx=(left+right)/2;
        int cy=(top+bottom)/2;
        bool done=true;
        if (cx!=x) {
          done=false;
          if (x<cx)
            right=cx;
          else
            left=cx;
        }
        if (cy!=y) {
          done=false;
          if (y<cy)
            bottom==cy;
          else
            top=cy;
        }
      } while (!done);
      int tmp=(from->bmp[top][left]+from->bmp[top][right]+from->bmp[bottom][left]+from->bmp[bottom][right])/4;
      to->pos[x][y]=tmp+affect*ucharmirror(from->bmp[y][x]-tmp)+(1.0-affect)*
    }
  } while (!key[KEY_ESC]);
}
