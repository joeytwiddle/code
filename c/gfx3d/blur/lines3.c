#include "joeylib.c"

#define scrwid 320
#define scrhei 200

#define fade 1

#define numparts 20

#define line 3

typedef struct {
  V3d pos;
  float rad,sgn;
  int inc,outc;
} Part;

void newparts(List<Part> *l) {
  V3d a=V3d(magrnd(1),magrnd(1),magrnd(5));
  V3d b=a+V3d(magrnd(1),magrnd(1),magrnd(0.4));;
  float sgn=plusminus();
  float rad=0.005+myrnd(0.005);
  for (int i=1;i<=line;i++) {
    Part p;
    float thru=(float)(i-1)/(line-1);
    p.pos=a+thru*(b-a);
    p.rad=rad;
    p.sgn=sgn;
    if (p.sgn>0) {
      p.inc=127;
      p.outc=63;
    } else {
      p.inc=255;
      p.outc=128+63;
    }
    l->add(p);
  }
}

void main() {
  allegrosetup(scrwid,scrhei);
  makesplitpalette(&redtowhitepalette,&greentowhitepalette);
  randomise();
  PPsetup(scrwid,scrhei,4);
  JBmp j=JBmp(scrwid,scrhei);
  j.clear();
  List<Part> ps=List<Part>();
  for (int i=1;i<=numparts/line;i++) {
    newparts(&ps);
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
      j.filledcirclenodarker(sx,sy,rad,p->outc);
      j.filledcirclenodarker(sx,sy,rad/3,p->inc);
//      j.shadedcirclenodarker(sx,sy,rad,p->inc,p->outc);
    }
    for (int x=0;x<scrwid;x++)
      for (int y=0;y<scrhei;y++) {
        int k=j.bmp[y][x];
        if (k>=128+fade)
          j.bmp[y][x]-=fade;
        else
          if (k<128)
            if (k>=fade)
              j.bmp[y][x]-=fade;
//            j.bmp[y][x]=intchop(k-fade,0,128);
//          j.bmp[y][x]=128+intchop(k-fade-128,0,128);
        if (k>=128 && k<=128+3)
          j.bmp[y][x]=0;
      }
    j.writetoscreen();
    Matrix m,n;
    V3d rotaxis=V3d::rotate(V3d(0,1,0),V3d(0,0,1),pi/4.0*sin(2*pi*frame/1000));
    m.makerotation(rotaxis,pi/1000.0);
    n.makerotation(rotaxis,-pi/1000.0);
    for (int i=1;i<=ps.len;i++) {
      Part *p=ps.p2num(i);
      if (p->sgn>0)
        p->pos=m*p->pos;
      else
        p->pos=n*p->pos;
      p->pos=p->pos+V3d(0,0,-0.03);
      if ((p->pos.z<-4 || max(myabs(p->pos.x),myabs(p->pos.y))>2)) {
        ps.removenum(i);
        if (ps.len<numparts-line)
          newparts(&ps);
      }
    }
    frame++;
  } while (!key[KEY_ESC]);
}

