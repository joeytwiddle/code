#ifndef plane3d_C
  #define plane3d_C

  #include <plane3d.h>

// See plane.c instead
// Merge one day!

// Starts of class Plane3d


  // Variable declared in .h file
  // Variable declared in .h file

   Plane3d::Plane3d(V3d c,V3d n) {
    cen=c;
    nor=n;
  }

  V3d Plane3d::intersection(Line3d l) {
    return l.intersection(this);
  }

  float Plane3d::distAbove(V3d v) { // should be part of V3d!
    return v.distAbove(*this);
  }

// End class 


#endif
