#ifndef rectangle3d_H
  #define rectangle3d_H

  #include <plane.h>

class Rectangle3d : public Plane {
public:
  V3d right,down; // Exists

   Rectangle3d(); // Method


   Rectangle3d(V3d topleft,V3d r,V3d d); // Method


  void setupRectangle3d(V3d topleft,V3d r,V3d d); // Method


  bool inrectangle(V3d v); // Method


  V2d projectDown(V3d v); // Method

  
};

#endif
