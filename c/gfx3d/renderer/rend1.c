#include <joeylib.c>

class Renderable {
public:
  V3d cen;
  float size;
  virtual V3d pos() {
    return cen;
  }
  virtual Renderable translated(V3d t) {
  }
  virtual Renderable rotated(V3d ax,float ang) {
  }
  virtual Renderable operator*(Matrix m) {
  }
  virtual void render(JBmp *j,Viewpoint *v) {
  }
};

Renderable operator*(Matrix m,Renderable r) {
  return r*m;
}

class Circle : public Renderable {
public:
  float rad;
  Circle(V3d c,float r) {
    cen=c;
    rad=r;
    size=rad;
  }
  Renderable translated(V3d t) {
    return Circle(cen+t,rad);
  }
  Renderable rotated(V3d ax,float ang) {
    return Circle(cen.rotated(ax,ang),rad);
  }
  Renderable operator*(Matrix m) {
    return Circle(m*cen,rad);
  }
  void render(JBmp *j,Viewpoint *v) {
    int x,y;
    float p=PPgetunitnoadd(v->bring(cen));
    if (p) {
      float r=rad*p;
      PPgetscrposnoadd(v->bring(cen),&x,&y);
      j->filledcircle(x,y,r,255);
    }
  }
};

class Polygon : public Renderable {
public:
  V3d a,b,c;
  Polygon(V3d aa,V3d bb,V3d cc) {
    a=aa;
    b=bb;
    c=cc;
    cen=(aa+bb+cc)/3.0;
    size=max(max((a-cen).mod(),(b-cen).mod()),(c-cen).mod());
  }
  V3d normal() {
    return V3d::normcross(b-a,c-a);
  }
  Renderable translated(V3d t) {
    return Polygon(a+t,b+t,c+t);
  }
  Renderable rotated(V3d ax,float ang) {
    return Polygon(a.rotated(ax,ang),b.rotated(ax,ang),c.rotated(ax,ang));
  }
  Renderable operator*(Matrix m) {
    return Polygon(m*a,m*b,m*c);
  }
  void render(JBmp *j,Viewpoint *v) {
    int xa,xb,xc,ya,yb,yc;
    if (PPgetscrposnoadd(v->bring(a),&xa,&ya) &&
        PPgetscrposnoadd(v->bring(b),&xb,&yb) &&
        PPgetscrposnoadd(v->bring(c),&xc,&yc))
      j->filltri(xa,ya,xb,yb,xc,yc,ucharchop(255-255*v->bring(cen).z/8.0));
  }
};

V3d getpos(Renderable *r) {
  return r->cen;
      printg(Sformat("%f,%f,%f",r->cen.x,r->cen.y,r->cen.z),2);
}

void main() {
  int scrwid=320;
  int scrhei=200;
  PPsetup(scrwid,scrhei,1.5);
  Octree<Renderable*> o=Octree<Renderable*>(V3d::o,10.0,&getpos);
  for (int i=0;i<=10000;i++) {
    if (myrnd()<0.5)
      o.add(new Circle(5.0*V3d::random(),myrnd()*0.1));
    else {
      V3d tmp=5.0*V3d::random();
      o.add(new Polygon(tmp+V3d::random(0.4),tmp+V3d::random(0.4),tmp+V3d::random(0.4)));
    }
  }
  Ori ori=Ori();
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  do {
    JBmp j=JBmp(scrwid,scrhei,0);
    Frustrum f=Frustrum(V3d::o,ori,0.1,20);
//    OrderedList<Renderable*> *ol=o.getorderedlist(f);
    List<Renderable*> *ol=o.getclippedlist(f);
    Viewpoint v=Viewpoint(V3d::o,ori);
    for (int i=1;i<=ol->len;i++) {
      Renderable *r=ol->num(ol->len+1-i);
      r->render(&j,&v);
//      printg(Sformat("%f,%f,%f",r->cen.x,r->cen.y,r->cen.z),1);
    }
    j.writetoscreen();
    printg(Sformat("%i %i",o.totalobjs,ol->len),0);
    ori.roll(0.004);
    ori.yaw(0.008);
  } while (!key[KEY_SPACE]);
}
