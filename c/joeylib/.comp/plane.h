#ifndef plane_H
  #define plane_H

  #include <line3d.h>

class Plane {
public:
  V3d pos,nor; // Exists // nor must be normalised or distances will not work properly
   Plane(); // Method

   Plane(V3d a,V3d b); // Method

  void setupPlane(V3d a,V3d b); // Method

  float distto(V3d v); // Method

  bool onplane(V3d v); // Method

  Viewpoint orientor(); // Method

  Plane disorient(Viewpoint v); // Method

  Plane bring(Viewpoint v); // Method

  Plane orient(Viewpoint v); // Method

  Line2d intersection(Plane p); // Method


  V3d intersection(Line3d l); // Method


  V3d intersect(Line3d l); // Method


  float distAbove(V3d v); // Method


  static Plane fromLines(Line3d a,Line3d b); // Method


  String toString(); // Method


};

/* Skipping later :: float V3d::distAbove(Plane p); // Method
*/


/* Skipping later :: V3d Line3d::intersection(Plane p); // Method
*/


#endif
