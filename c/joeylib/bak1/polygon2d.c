class Region;

class Polygon2d {
public:

  static bool container;
  static bool contained;

  List<V2d> vs;

  Region *regionised;
  Region *smallreg;
  float score;

  String name;

  Polygon2d() {
    regionised=NULL;
    smallreg=NULL;
    score=-1;
  }

  Polygon2d(List<V2d> v) {
    regionised=NULL;
    smallreg=NULL;
    score=-1;
    vs=v;
  }

/*  static Polygon2d (Polygon2d *o,int numps, ... ) {
    va_list ap;
    va_start(ap, numps);
    vs=List<V2d>();
    for (int i = 0; i < numps; i++)
      vs.add(o->vs.num(va_arg(ap, int)));
    va_end(ap);
  }*/

/*  Polygon2d(int numps, ... ) { // Doesn't work!
    va_list ap;
    va_start(ap, numps);
    vs=List<V2d>();
    for (int i = 0; i < numps; i++) {
      V2d tmp=va_arg(ap,V2d);
//      tmp.print();
      vs.add(tmp);
    }
    va_end(ap);
  }*/

/*  Polygon2d(Polygon2d *o,int a,int b) {
    vs=List<V2d>();
    bool finished=false;
    for (int i=a;!finished;i=wrap(i+1,1,o->vs.len)) {
//      printf("%i ",i);
      vs.add(o->vs.num(i));
      if (i==b)
        finished=true;
    }
  }*/

  Polygon2d maketri(int a,int b,int c) {
    List<V2d> tmp;
    tmp.add(vs.num(a));
    tmp.add(vs.num(b));
    tmp.add(vs.num(c));
    return Polygon2d(tmp);
  }

  Polygon2d subpath(int a,int b) {
    List<V2d> tmp;
    bool finished=false;
    for (int i=a;!finished;i=wrap(i+1,1,vs.len)) {
      tmp.add(vs.num(i));
      if (i==b)
        finished=true;
    }
    return Polygon2d(tmp);
  }

  static float area(Polygon2d p) {
    return p.area();
  }

  float area() {
//    printf("Finding area of polygon %i\n",vs.len);
    if (vs.len==3) {
      // Find area of triangle.
      // Split vertically into two
      // Get middle vertically
      vs.sort(&(V2d::gety));
      V2d a=vs.num(1);
      V2d b=vs.num(2);
      V2d c=vs.num(3);
      // Find where vertical line crosses the horizontal of the middle point
      // i = A + k(C-A)
      // i = (l,B.y)
      // l = Ax+k(Cx-Ax)
      // By = Ay+k(Cy-Ay)
      float k=(b.y-a.y)/(c.y-a.y);
      V2d i=a+k*(c-a);
      float wid=floatabs(b.x-i.x);
      float ha=floatabs(a.y-b.y);
      float hc=floatabs(c.y-b.y);
//      a.print(); b.print(); c.print();
//      printf("%f %f %f\n",wid,ha,hc);
      return 1.0/2.0*wid*(ha+hc);
    }
    // If larger than a triangle, need to split
    // Find a line between two vertices that does not cross the perimiter.
    for (int p=1;p<=vs.len;p++)
    for (int q=1;q<=vs.len;q++) {
      // Ensure we don't have the same or adjacent vertices
      if (p!=q && abs(p-q)>1 && abs(p-q)<vs.len) {
//        printf("Checking %i %i\n",p,q);
        Line2d l=Line2d(vs.num(p),vs.num(q));
//        l.a.print(); l.b.print();
        // Check it doesn't cross the perimeter
        bool cross=false;
        for (int i=1;i<=vs.len && !cross;i++) {
//          printf("%i\n",i);
          Line2d tl=Line2d(vs.num(i),vs.numwrap(i+1));
          V2d *v=tl.findintersectionornull(l);
          if (v==NULL)
            cross=true;
//          printf("'\n");
          //free(v);
        }
        if (!cross) {
//          printf("no crossing for %i %i\n",p,q);
          // We make two new polygons out of this one, and find their areas.
          Polygon2d tmpa=this->subpath(p,q);
          Polygon2d tmpb=this->subpath(q,p);
          return tmpa.area()+tmpb.area();
        }
//        printf("Cross\n");
      }
    }
    error("area: Could not split polygon without crossing lines.\n");
  }
  List<Polygon2d> splittotris() {
    List<Polygon2d> ps;
    if (vs.len==3) {
      ps.add(*this);
      return ps;
    }
    for (int p=1;p<=vs.len;p++)
    for (int q=1;q<=vs.len;q++) {
      // Ensure we don't have the same or adjacent vertices
      if (p!=q && abs(p-q)>1 && abs(p-q)<vs.len) {
        Line2d l=Line2d(vs.num(p),vs.num(q));
        // Check it doesn't cross the perimeter
        bool cross=false;
        for (int i=1;i<=vs.len && !cross;i++) {
          Line2d tl=Line2d(vs.num(i),vs.numwrap(i+1));
          V2d *v=tl.findintersectionornull(l);
          if (v==NULL)
            cross=true;
        }
        if (!cross) {
          // We make two new polygons out of this one, and find their areas.
          ps.add(this->subpath(p,q).splittotris());
          ps.add(this->subpath(q,p).splittotris());
          return ps;
        }
      }
    }
    error("splittotris: Could not split polygon without crossing lines.\n");
//    return NULL;
  }
    
  String toString() {
    String s="";
    for (int i=1;i<=vs.len;i++) {
      V2d v=vs.num(i);
      s=Sformat("%s(%i,%i),",s,(int)v.x,(int)v.y);
    }
    return Sleft(s,Slen(s)-1);
  }
  bool contains(V2d v) {
    Line2d toinf=Line2d(v,V2d(10000,v.y));
    int count=0;
    for (int i=1;i<=vs.len;i++) {
      Line2d t=Line2d(vs.num(i),vs.numwrap(i+1));
      if (t.crosses(toinf))
        count++;
    }
    return (intmod(count,2)==1);
  }

  static Polygon2d fromlist(List<V2d> l) {
    return Polygon2d(l);
  }

  void scale(float f) {
    for (int i=1;i<=vs.len;i++)
      vs.replacenum(i,vs.num(i)*f);
  }

  Line2d linefrom(int i) {
    return Line2d(vs.wrapnum(i),vs.wrapnum(i+1));
  }

  V2d num(int i) {
    return vs.wrapnum(i);
  }

  bool crosses(Line2d l) {
    for (int i=1;i<=vs.len;i++) {
//      flibble(linefrom(i),3);
      if (l.crosses(linefrom(i)))
        return true;
//      flibble(linefrom(i),0);
    }
    return false;
  }

  bool overlaps(Polygon2d p) { // does not seem to work
//    return overlaps(&p);

    for (int i=1;i<=p.vs.len;i++)
      if (crosses(p.linefrom(i)))
        return true;
    bool pin=true;
    bool thisin=true;
    for (int i=1;i<=p.vs.len && pin;i++)
      if (!contains(p.vs.num(i)))
        pin=false;
    for (int i=1;i<=vs.len && thisin;i++)
      if (!p.contains(vs.num(i)))
        thisin=false;
    if (pin)
      return container; // this polygon completely contains p
    if (thisin)
      return contained;
    return false;
  }

  virtual bool overlaps(Polygon2d *p);

  bool overlaps(Polygon2d *p,float lenience) { // this has a bias towards being a container and not contained
    return overlaps(p->scaleaboutcentroid(lenience));
  }

  Polygon2d scaleaboutcentroid(float scale) {
    Polygon2d n;
    V2d cen=centroid();
    for (int i=1;i<=vs.len;i++) {
      V2d x=vs.num(i);
      x=x-cen;
      x=x*scale;
      x=x+cen;
      n.add(x);
    }
    return n;
  }

  V2d centroid() {
    List<Polygon2d> ps=splittotris();
    V2d cen=V2d::o;
    float totalarea=0;
    for (int i=1;i<=ps.len;i++) {
      Polygon2d p=ps.num(i);
      float area=p.area();
      cen=cen+area*(p.num(1)+p.num(2)+p.num(3))/3.0;
      totalarea+=area;
    }
    return cen/totalarea;
  }

  void add(V2d v) {
    vs.add(v);
  }

  void freedom() {
    vs.freedom();
  }

  Polygon2d operator*(float f) {
    Polygon2d p;
    for (int i=1;i<=vs.len;i++)
      p.add(vs.num(i)*f);
    return p;
  }

};                  

fillin bool Polygon2d::container=2;
fillin bool Polygon2d::contained=3;
