#include <joeylib.c>

#define scrwid 100
#define scrhei 100

myRGB ave(myRGB x,myRGB y) {
  return x/2+y/2;
}

myRGB adjust(myRGB x,myRGB y,float a) {
  return x*(1.0-a)+y*a;
}

void make(RGBmp *to,RGBmp *from,int left,int top,int right,int bottom) {
  int size=right-left;
  int cenx=(left+right)/2;
  int ceny=(top+bottom)/2;
//  float affect=size/100.0;
//  float affect=sqrt(size/100.0);
//  float affect=mysquare(size/100.0);
//  float affect=pow(size/100.0,0.75);
  float affect=0.5*sqrt(size/100.0);
/*  to->bmp[top][cenx]=adjust(ave(to->bmp[top][left],to->bmp[top][right]),from->bmp[top][cenx],affect);
  to->bmp[bottom][cenx]=adjust(ave(to->bmp[bottom][left],to->bmp[bottom][right]),from->bmp[bottom][cenx],affect);
  to->bmp[ceny][left]=adjust(ave(to->bmp[top][left],to->bmp[bottom][left]),from->bmp[ceny][left],affect);
  to->bmp[ceny][right]=adjust(ave(to->bmp[top][right],to->bmp[bottom][right]),from->bmp[ceny][right],affect);*/
  to->bmp[top][cenx]=ave(to->bmp[top][left],to->bmp[top][right]);
  to->bmp[bottom][cenx]=ave(to->bmp[bottom][left],to->bmp[bottom][right]);
  to->bmp[ceny][left]=ave(to->bmp[top][left],to->bmp[bottom][left]);
  to->bmp[ceny][right]=ave(to->bmp[top][right],to->bmp[bottom][right]);
  myRGB newcol=ave(ave(to->bmp[top][cenx],to->bmp[bottom][cenx]),ave(to->bmp[ceny][right],to->bmp[ceny][left]));
  newcol=adjust(newcol,from->bmp[ceny][cenx],affect);
  to->bmp[ceny][cenx]=newcol;
  if (size>2) {
    make(to,from,left,top,cenx,ceny);
    make(to,from,left,ceny,cenx,bottom);
    make(to,from,cenx,top,right,ceny);
    make(to,from,cenx,ceny,right,bottom);
  }
}

void main() {
  RGBmp *j=new RGBmp(scrwid,scrhei);
  RGBmp *k=new RGBmp(scrwid,scrhei);
  set_color_depth(16);
  allegrosetup(320,200);
  randomise();
  for (int x=0;x<scrwid;x++)
  for (int y=0;y<scrhei;y++) {
    j->bmp[y][x]=*new myRGB(myrnd(),myrnd(),myrnd());
  }
  do {
    k->bmp[0][0]=j->bmp[0][0];
    k->bmp[0][scrwid-1]=j->bmp[0][scrwid-1];
    k->bmp[scrhei-1][0]=j->bmp[scrhei-1][0];
    k->bmp[scrhei-1][scrwid-1]=j->bmp[scrhei-1][scrwid-1];
    make(k,j,0,0,scrwid-1,scrhei-1);
    k->writetoscreen();
    for (int x=0;x<scrwid;x++)
    for (int y=0;y<scrhei;y++) {
      j->bmp[y][x].perturb(60);
    }
  } while (!key[KEY_ESC]);
}
