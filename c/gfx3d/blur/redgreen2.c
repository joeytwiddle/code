#include "joeylib.c"

#define scrwid 320
#define scrhei 200

typedef struct {
  V3d pos;
  float rad,sgn;
  int inc,outc;
} Part;

void newpart(Part *p) {
    p->pos=V3d(magrnd(1),magrnd(1),1);
    p->rad=0.02+myrnd(0.04);
    p->sgn=plusminus();
    if (p->sgn>0) {
      p->inc=127;
      p->outc=63;
    } else {
      p->inc=255;
      p->outc=128+63;
    }
}

void main() {
  allegrosetup(scrwid,scrhei);
  makesplitpalette(&redtowhitepalette,&greentowhitepalette);
  PPsetup(scrwid,scrhei,4);
  JBmp j=JBmp(scrwid,scrhei);
  j.clear();
  List<Part> ps=List<Part>();
  for (int i=1;i<=10;i++) {
    Part p;
    newpart(&p);
    ps+p;
  }
  int frame=0;
  do {
    for (int i=1;i<=ps.len;i++) {
      Part *p=ps.p2num(i);
      int sx,sy;
      PPgetscrpos(p->pos,&sx,&sy);
      int sxb,syb;
      PPgetscrpos(p->pos+V3d(p->rad,0,0),&sxb,&syb);
      float rad=sqrt(mysquare(sx-sxb)+mysquare(sy-syb));
      j.filledcircle(sx,sy,rad,p->inc);
//      j.filledcirclenodarker(sx,sy,rad/3,p->inc);
    }
    for (int x=0;x<scrwid;x++)
      for (int y=0;y<scrhei;y++) {
        int k=j.bmp[y][x];
        if (k==128)
          j.bmp[y][x]=0;
        if (k<128)
          j.bmp[y][x]=chop(k-4,0,128);
        else
          j.bmp[y][x]=128+chop(k-4-128,0,128);
      }
    j.writetoscreen();
    Matrix m,n;
    V3d rotaxis=V3d::rotate(V3d(0,1,0),V3d(0,0,1),pi/4.0*sin(2*pi*frame/4000));
    m.makerotation(rotaxis,pi/1000.0);
    n.makerotation(rotaxis,-pi/1000.0);
    for (int i=1;i<=ps.len;i++) {
      Part *p=ps.p2num(i);
      if (p->sgn>0)
        p->pos=m*p->pos;
      else
        p->pos=n*p->pos;
      p->pos=p->pos+V3d(0,0,-0.03);
      if (p->pos.z<-4 || max(myabs(p->pos.x),myabs(p->pos.y))>2)
        newpart(p);
    }
    frame++;
  } while (!key[KEY_ESC]);
}
