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

class Sphere : public Renderable {
public:
  float rad;
  Sphere(V3d c,float r) {
    cen=c;
    rad=r;
    size=rad;
  }
  Renderable translated(V3d t) {
    return Sphere(cen+t,rad);
  }
  Renderable rotated(V3d ax,float ang) {
    return Sphere(cen.rotated(ax,ang),rad);
  }
  Renderable operator*(Matrix m) {
    return Sphere(m*cen,rad);
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

class Triangle3d : public Renderable, public Polygon3d {
public:
  V3d a,b,c;
  Triangle3d(V3d aa,V3d bb,V3d cc) {
    a=aa;
    b=bb;
    c=cc;
    vs.add(a);
    vs.add(b);
    vs.add(c);
    cen=(aa+bb+cc)/3.0;
    size=max(max((a-cen).mod(),(b-cen).mod()),(c-cen).mod());
  }
  V3d normal() {
    return V3d::normcross(b-a,c-a);
  }
  Renderable translated(V3d t) {
    return Triangle3d(a+t,b+t,c+t);
  }
  Renderable rotated(V3d ax,float ang) {
    return Triangle3d(a.rotated(ax,ang),b.rotated(ax,ang),c.rotated(ax,ang));
  }
  Renderable operator*(Matrix m) {
    return Triangle3d(m*a,m*b,m*c);
  }
  void render(JBmp *j,Viewpoint *v) {
    int xa,xb,xc,ya,yb,yc;
    if (PPgetscrposnoadd(v->bring(a),&xa,&ya) &&
        PPgetscrposnoadd(v->bring(b),&xb,&yb) &&
        PPgetscrposnoadd(v->bring(c),&xc,&yc)) {
      V3d n=normal().disorientate(v->ori);
      int c=ucharchop(255*myabs(V3d::normdot(n,V3d(-1,2,-1))));
      j->filltri(xa,ya,xb,yb,xc,yc,c);
//      j->filltri(xa,ya,xb,yb,xc,yc,ucharchop(255-255*v->bring(cen).z/8.0));
    }
  }
};

class Gouraud;

class Corner {
public:
  V3d pos,normal;
  List<Gouraud*> gs;
  int col;
  Corner() { }
  Corner(V3d x) {
    pos=x;
  }
  Corner(V3d x,V3d n) {
    pos=x;
    normal=n;
  }
  void findnormal();
};

float tofloat(Corner c) {
  return tofloat(c.pos);
}

class Gouraud : public Renderable {
public:
  Corner *a,*b,*c;
  Gouraud(Corner *aa,Corner *bb,Corner *cc) {
    a=aa;
    b=bb;
    c=cc;
//    printf("Adding gouraud %s %s %s\n",aa->pos.toString(),b->pos.toString(),b->pos.toString());
    a->gs.add(this);
    b->gs.add(this);
    c->gs.add(this);
    cen=(a->pos+b->pos+c->pos)/3.0;
    size=max(max((a->pos-cen).mod(),(b->pos-cen).mod()),(c->pos-cen).mod());
  }
  void render(JBmp *j,Viewpoint *v) {
    int xa,xb,xc,ya,yb,yc;
    if (PPgetscrposnoadd(v->bring(a->pos),&xa,&ya) &&
        PPgetscrposnoadd(v->bring(b->pos),&xb,&yb) &&
        PPgetscrposnoadd(v->bring(c->pos),&xc,&yc)) {
      V3d n=normal().disorientate(v->ori);
      int ca=a->col;
      int cb=b->col;
      int cc=c->col;
//      j->filltri(xa,ya,xb,yb,xc,yc,ca);
      j->intertri(xa,ya,xb,yb,xc,yc,ca,cb,cc);
//      j->line(xa,ya,xb,yb,ca); j->line(xb,yb,xc,yc,cb); j->line(xc,yc,xa,ya,cc);
    }
  }
  V3d normal() {
    return V3d::normcross(b->pos-a->pos,c->pos-b->pos);
  }
};

  void Corner::findnormal() {
    normal=V3d::o;
    for (int i=1;i<=gs.len;i++)
      normal=normal+gs.num(i)->normal();
    if (gs.len>0)
      normal.normalise();
    col=ucharchop(255*myabs(V3d::normdot(normal,V3d(-1,2,-1))));
//    col=col*(float)gs.len/6.0;
  }

V3d getpos(Renderable *r) {
  return r->cen;
}

void render(Octree<Renderable*> *o,Viewpoint *v) {
  PPscr.clear();
  Frustrum f=Frustrum(v->pos,v->ori,0.1,20);
//   List<Renderable*> *ol=o->getclippedlist(f);
  OrderedList<Renderable*> *ol=o->getorderedlist(f);
  for (int i=1;i<=ol->len;i++) {
//    printf(".");
    Renderable *r=ol->num(ol->len+1-i);
    r->render(&PPscr,v);
//    free(r); Assumed to be done by destroy(octree)
  }
  destroystruct(ol); // don't destroy elements, as above
  PPshow();
}
