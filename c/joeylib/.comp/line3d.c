#ifndef line3d_C
  #define line3d_C

  #include <line3d.h>

// Starts of class Line3d


  // Variable declared in .h file
   Line3d::Line3d() { }
   Line3d::Line3d(V3d aa,V3d bb) {
    a=aa;
    b=bb;
  }
   Line3d::Line3d(Line2d l) {
    a=V3d(l.a.x,l.a.y,0);
    b=V3d(l.b.x,l.b.y,0);
  }
   Line3d::Line3d(const Line3d &l) {
    a=V3d(l.a.x,l.a.y,l.a.z);
    b=V3d(l.b.x,l.b.y,l.b.z);
  }
  Line3d Line3d::orient(Viewpoint v) {
    return Line3d(a.orient(v),b.orient(v));
  }
  void Line3d::swapends() {
    V3d t=a;
    a=b;
    b=t;
  }
  V3d *Line3d::intersection(Line3d o) {
    //printf("\nTrying to find intersection %s-%s with %s-%s\n",a.toString(),b.toString(),o.a.toString(),o.b.toString());
    Line2d la=Line2d(a.dropz(),b.dropz());
    Line2d lb=Line2d(o.a.dropz(),o.b.dropz());
    float f=la.findintersectionnum(lb);
    V3d lah=a+f*(b-a);
    float g=lb.findintersectionnum(la);
    V3d lbh=o.a+g*(o.b-o.a);
    //printf("Intersection (%f) %s == %s (%f) ?\n",f,lah.toString(),lbh.toString(),g);
    return new V3d(lah.x,lah.y,lah.z);
    if (lah==lbh)
      return new V3d(lah.x,lah.y,lah.z);
    //printf("Intersection failed.\n");
      return NULL;
  }
  V3d Line3d::intersect(Line3d o) {
    return *intersection(o);
  }

  /* Skipping declared method V3d Line3d::intersection(Plane p) */


  String Line3d::toString() {
    return Sformat("%s-%s",a.toString(),b.toString());
  }

// End class 


#endif
