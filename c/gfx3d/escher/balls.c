#include <joeylib.c>

#define scrwid 640
#define scrhei 480

typedef struct {
  float x,y,z,r,u,v,rv,s;
  int c;
} Tuple;

bool sort(Tuple t,Tuple r) {
  return (t.z>r.z);
}

void main() {
  allegrosetup(scrwid,scrhei);
  mypalette(0,0,0,0);
  mypalette(1,1,0,0);
  mypalette(2,1,1,0);
  mypalette(3,0,1,0);
  mypalette(4,0,1,1);
  mypalette(5,0,0,1);
  mypalette(6,1,0,1);
  mypalette(7,1,1,1);
  List<Tuple> os=List<Tuple>(10);
  for (int i=0;i<4;i++) {
    Tuple t;
    t.x=myrnd()*scrwid;
    t.y=myrnd()*scrhei;
    t.z=myrnd();
    t.z=0;
    t.r=myrnd()*2*pi;
    t.u=t.v=t.rv=0;
    t.s=20+myrnd()*60;
    t.c=intrnd(1,6);
    os.add(t);
  }
  JBmp j=JBmp(scrwid,scrhei);
  do {
    j.clear();
    os.sort(&sort);
    for (int i=1;i<=os.len;i++) {
      Tuple *t=os.p2num(i);
      j.filledcircle(t->x,t->y,t->s,t->c);
    }
    j.writetoscreen();
    for (int i=1;i<=os.len;i++) {
      Tuple *t=os.p2num(i);
      t->u=t->u*.99+magrnd(2);
      t->v=t->v*.99+magrnd(2);
      t->x=scrwid/2+(t->x-scrwid/2)*.9999+t->u;
      t->y=scrhei/2+(t->y-scrhei/2)*.9999+t->v;
      if (t->x<0)
        t->u=myabs(t->u);
      if (t->x>scrwid-1)
        t->u=-myabs(t->u);
      if (t->y<0)
        t->v=myabs(t->v);
      if (t->y>scrhei-1)
        t->v=-myabs(t->v);
      t->rv=t->rv*.99+magrnd(0.02);
      t->r=t->r+t->rv;
      t->s=t->s+magrnd(1);
      if (t->s<20)
        t->s=20;
//      t->z=t->z+magrnd(0.1);
    }
    for (int i=1;i<=os.len;i++) {
      // Check for collision
      Tuple *t=os.p2num(i);
      bool collision=false;
      for (int j=1;j<=os.len && !collision;j++) {
        if (j!=i) {
          Tuple *r=os.p2num(j);
          if (sqrt(mysquare(t->x-r->x)+mysquare(t->y-r->y))<=t->s+r->s+2)
            collision=true;
        }
      }
      if (!collision)
        t->z=myrnd();
    }
  } while (!key[KEY_ESC]);
}
