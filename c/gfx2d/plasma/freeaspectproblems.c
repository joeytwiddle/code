#include <joeylib.c>

int toppow=7;

int scrwid=200;
int scrhei=pow(2,toppow)+1;

#define cornrnd 6
float smoothness=6.0;
float unfunkiness=5.0;

int merge(int x,int y,float amount) {
  return (int)(x+amount*(y-x));
}

int merge(int x,int y) {
  return merge(x,y,0.5);
}

void make(JBmp *rto,Map2d<int> *to,Map2d<uchar> *from,int left,int top,int w,int h) {
  int right=left+w;
  int bottom=top+h;
  int size=min(w,h);
  int cenx=left+w/2;
  int ceny=top+h/2;
  to->pos[cenx][top]=merge(to->pos[left][top],to->pos[right][top]);
  to->pos[cenx][bottom]=merge(to->pos[left][bottom],to->pos[right][bottom]);
  to->pos[left][ceny]=merge(to->pos[left][top],to->pos[left][bottom]);
  to->pos[right][ceny]=merge(to->pos[right][top],to->pos[right][bottom]);
  int newcol=(to->pos[left][top]+to->pos[left][bottom]+to->pos[right][top]+to->pos[right][bottom])/4;
  float affect=0.01+2.0*(float)size/(float)scrwid;
//  newcol=merge(newcol,from->pos[cenx][ceny]-128,affect);
  //newcol=newcol+affect*(from->pos[cenx][ceny]-128);

  // 2^power=wid
  // power=ln wid/ln 2
  float power=log(0.5*w+0.5*h)/log(2)+1.0;
//  float power=p;
  float i = pow(smoothness,unfunkiness - power);
  newcol=((float)from->pos[cenx][ceny] + i * (float)newcol) / (i + 1.0);

  if (to->pos[cenx][ceny]==0)
    to->pos[cenx][ceny]=newcol;
  rto->bmp[ceny][cenx]=newcol;
  rto->bmp[ceny][scrwid+cenx]=from->pos[cenx][ceny];
  if (size>1) {
    make(rto,to,from,left,top,w/2,h/2);
    make(rto,to,from,left,ceny,w/2,h/2);
    make(rto,to,from,cenx,top,w/2,h/2);
    make(rto,to,from,cenx,ceny,w/2,h/2);
  }
}

void main() {
  Map2d<uchar> *from=new Map2d<uchar>(scrwid,scrhei);
  Map2d<uchar> *dest=new Map2d<uchar>(scrwid,scrhei);
  JBmp *rto=new JBmp(scrwid*2,scrhei);
  Map2d<int> *to=new Map2d<int>(scrwid,scrhei,0);
  randomise();
  allegrosetup(320,200);
  for (int i=0;i<256;i++) {
    mypalette(i,myRGB::hue(mymod(4.0*(float)i/256.0)));
  }
  for (int x=0;x<scrwid;x++)
  for (int y=0;y<scrhei;y++) {
    from->pos[x][y]=intrnd(512+256);
    dest->pos[x][y]=intrnd(512+256);
  }
  do {
    to->clear(0);
    to->pos[0][0]=from->pos[0][0];
    to->pos[scrwid-1][0]=from->pos[scrwid-1][0];
    to->pos[0][scrhei-1]=from->pos[0][scrhei-1];
    to->pos[scrwid-1][scrhei-1]=from->pos[scrwid-1][scrhei-1];
    make(rto,to,from,0,0,scrwid-1,scrhei-1);
    rto->display();
    for (int x=0;x<scrwid;x++)
    for (int y=0;y<scrhei;y++) {
      // from->pos[x][y]=ucharchop(from->pos[x][y]+magrnd(10));
      int speed=3;
      if (from->pos[x][y]<=dest->pos[x][y]-speed)
        from->pos[x][y]+=speed;
      else
        if (from->pos[x][y]>=dest->pos[x][y]+speed)
          from->pos[x][y]-=speed;
        else
          dest->pos[x][y]=intrnd(512+256);
    }
    
  } while (!key[KEY_ESC]);
}
