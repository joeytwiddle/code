#ifndef line3d_H
  #define line3d_H

class Line3d {
public:
  V3d a,b; // Exists
   Line3d(); // Method

   Line3d(V3d aa,V3d bb); // Method

   Line3d(Line2d l); // Method

   Line3d(const Line3d &l); // Method

  Line3d orient(Viewpoint v); // Method

  void swapends(); // Method

  V3d *intersection(Line3d o); // Method

  V3d intersect(Line3d o); // Method


  V3d intersection(Plane p); // Method


  String toString(); // Method


};

#endif
