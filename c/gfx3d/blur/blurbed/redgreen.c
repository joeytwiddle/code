/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


#include "joeylib.c"

#define scrwid 320
#define scrhei 200

typedef struct {
  V3d pos;
  float rad,sgn;
  int c;
} Part;

void newpart(Part *p) {
    p->pos=V3d(magrnd(1),magrnd(1),1);
    p->rad=0.02+myrnd(0.04);
    p->c=16;
    p->sgn=plusminus();
}

void main() {
  allegrosetup(scrwid,scrhei);
  makesplitpalette(&redtowhitepalette,&greentowhitepalette);
  PPsetup(scrwid,scrhei,4);
  JBmp j=JBmp(scrwid,scrhei);
  j.clear();
  JBmp k=JBmp(scrwid,scrhei);
  k.clear();
  JBmp l=JBmp(scrwid,scrhei);
  l.clear();
  List<Part> ps=List<Part>();
  for (int i=1;i<=20;i++) {
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
      if (p->sgn>0)
        j.addcircle(sx,sy,rad,p->c);
      else
        k.addcircle(sx,sy,rad,p->c);
    }
    for (int x=0;x<scrwid;x++)
      for (int y=0;y<scrhei;y++) {
        if (j.bmp[y][x]>=3)
          j.bmp[y][x]-=3;
        if (k.bmp[y][x]>=3)
          k.bmp[y][x]-=3;
        int c=j.bmp[y][x]/2.0;
        int d=k.bmp[y][x]/2.0;
        if (c>d)
          l.bmp[y][x]=c;
        else
          l.bmp[y][x]=128+d;
      }
    l.writetoscreen();
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
