class Line2d {
public:
  V2d a,b;
  float length;
  Line2d() { }
  Line2d(V2d aa,V2d bb) {
    a=aa;
    b=bb;
    length=V2d::dist(a,b);
  }
  void swapends() {
    V2d tmp=a;
    a=b;
    b=tmp;
  }
  V2d *findintersection(Line2d o) {
//    printf("Looking for intersection\n");
    // i=A+k(B-A)
    // i=OA+l(OB-OA)
    // Ax+k(Bx-Ax)=OAx+l(OBx-OAx)
    // Ay+k(By-Ay)=OAy+l(OBy-OAy)
    // k=(OAx+l(OBx-OAx)-Ax)/(Bx-Ax)
    // Ay+(OAx+l(OBx-OAx)-Ax)/(Bx-Ax)*(By-Ay)=OAy+l(OBy-OAy)
    // Ay+(By-Ay)/(Bx-Ax)*(OAx-Ax)+l*(By-Ay)/(Bx-Ax)*(OBx-OAx)=OAy+l(OBy-OAy)
    // l*{ OAy-OBy + (By-Ay)/(Bx-Ax)*(OBx-OAx) } = OAy-Ay-(By-Ay)/(Bx-Ax)*(OAx-Ax)
    float l=(o.a.y-a.y-(b.y-a.y)/(b.x-a.x)*(o.a.x-a.x))/(o.a.y-o.b.y+(b.y-a.y)/(b.x-a.x)*(o.b.x-o.a.x));
    // Is intersection beyond end of line?
    if (l<0 || l>1.0)
      return NULL;
    return new V2d(o.a+l*(o.b-o.a));
  }
};

class Polygon2d {
public:
  List<V2d> vs;
  Polygon2d(List<V2d> v) {
    vs=v;
  }
  Polygon2d(Polygon2d *o,int numps, ... ) {
    va_list ap;
    va_start(ap, numps);
    vs=List<V2d>();
    for (int i = 0; i < numps; i++)
      vs.add(o->vs.num(va_arg(ap, int)));
    va_end(ap);
  }
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
  Polygon2d(Polygon2d *o,int a,int b) {
    vs=List<V2d>();
    bool finished=false;
    for (int i=a;!finished;i=wrap(i+1,1,o->vs.len)) {
//      printf("%i ",i);
      vs.add(o->vs.num(i));
      if (i==b)
        finished=true;
    }
  }
  Polygon2d maketri(int a,int b,int c) {
    return Polygon2d(this,3,a,b,c);
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
          V2d *v=tl.findintersection(l);
          if (v==NULL)
            cross=true;
//          printf("'\n");
          //free(v);
        }
        if (!cross) {
//          printf("no crossing for %i %i\n",p,q);
          // We make two new polygons out of this one, and find their areas.
          Polygon2d tmpa=Polygon2d(this,p,q);
          Polygon2d tmpb=Polygon2d(this,q,p);
          return tmpa.area()+tmpb.area();
        }
//        printf("Cross\n");
      }
    }
    error("Could not split polygon without crossing lines.\n");
  }
};
