#ifndef polygon3d_C
  #define polygon3d_C

  #include <polygon3d.h>

/*class Polygon3d {
public:
  Polygon2d pol;
  Plane pla;
  Polygon3d() { }
  Polygon3d(V3d a,V3d b,V3d c) {
    pla=Plane((a+b+c)/3.0,V3d::normcross(a-b,a-c));
    pol.vs.add(pla.orientor().disorient(a).dropz());
    pol.vs.add(pla.orientor().disorient(b).dropz());
  }
  Line3d *intersection(Plane p) {
    Line2d plni=pla.intersection(p);
    List<V2d> vs;
    for (int i=1;i<=pol.vs.len;i++) {
      Line2d l=pol.linefrom(i);
      V2d *v=l.findintersectionornull(plni);
      if (v!=NULL)
        vs.add(*v);
    }
    if (vs.len>=2) {
      Line2d l=Line2d(vs.num(1),vs.num(2));
      Line3d r=Line3d(l).orient(pla.orientor());
      return new Line3d(r);
    }
    return NULL;
  }
};*/


// Starts of class Polygon3d


  // Variable declared in .h file

   Polygon3d::Polygon3d() { }

   Polygon3d::Polygon3d(V3d a,V3d b,V3d c) {
    vs+a;
    vs+b;
    vs+c;
  }

  V3d Polygon3d::centre() {
    V3d x=V3d(0,0,0);
    for (int i=1;i<=vs.len;i++)
      x=x+vs.num(i);
    return x;
  }

  Line3d *Polygon3d::intersectionheight(float y) {
//    printf("Looking for intersection height %f in %s\n",y,toString());
    List<V3d> es;
    for (int i=1;i<=vs.len;i++) {
      Line3d l=linefrom(i);
      if (l.a.y>l.b.y)
        l.swapends();
      if (l.a.y<y && l.b.y>y) {
        V3d v=l.a+(y-l.a.y)/(l.b.y-l.a.y)*(l.b-l.a);
        es+v;
      }
    }
    if (es.len==2) {
      Line3d *n=new Line3d(es.num(1),es.num(2));
      es.freedom();
      return n;
    }
    return NULL;
  }

  void Polygon3d::getintersectionheight(float y,Line3d *l,bool *s) {
//    printf("Looking for intersection height %f in %s\n",y,toString());
    List<V3d> es;
    for (int i=1;i<=vs.len;i++) {
      Line3d l=linefrom(i);
      if (l.a.y>l.b.y)
        l.swapends();
      if (l.a.y<y && l.b.y>y) {
        V3d v=l.a+(y-l.a.y)/(l.b.y-l.a.y)*(l.b-l.a);
        es+v;
      }
    }
    if (es.len==2) {
      *s=true;
      *l=Line3d(es.num(1),es.num(2));
      es.freedom();
    } else
      *s=false;
  }

  Line3d *Polygon3d::addifintersectionheight(float y,List<Line3d> *ls) {
    List<V3d> es;
    for (int i=1;i<=vs.len;i++) {
      Line3d l=linefrom(i);
      if (l.a.y>l.b.y)
        l.swapends();
      if (l.a.y<y && l.b.y>y) {
        V3d v=l.a+(y-l.a.y)/(l.b.y-l.a.y)*(l.b-l.a);
        es+v;
      }
    }
    if (es.len==2) {
      ls->add(Line3d(es.num(1),es.num(2)));
      es.freedom();
    }
  }

  Line3d Polygon3d::linefrom(int i) {
    return Line3d(vs.num(i),vs.wrapnum(i+1));
  }

  String Polygon3d::toString() {
    return Sformat("Polygon %s,%s,%s",vs.num(1).toString(),vs.num(2).toString(),vs.num(3).toString());
  }

  void Polygon3d::freedom() {
    vs.freedom();
  }

// End class 


#endif
