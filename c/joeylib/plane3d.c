// See plane.c instead
// Merge one day!

class Plane3d {
public:
  V3d cen;
  V3d nor;

  Plane3d(V3d c,V3d n) {
    cen=c;
    nor=n;
  }

  V3d intersection(Line3d l) {
    return l.intersection(this);
  }

  float distAbove(V3d v) { // should be part of V3d!
    return v.distAbove(*this);
  }

};
