#ifndef simpline2d_H
  #define simpline2d_H

class Line2d {
public:
  V2d a,b; // Exists
  float length; // Exists
   Line2d(); // Method

   Line2d(V2d aa,V2d bb); // Method

  void swapends(); // Method

  V2d *findintersection(Line2d o); // Method

};

class Polygon2d {
public:
  List<V2d> vs; // Exists
   Polygon2d(List<V2d> v); // Method

   Polygon2d(Polygon2d *o,int numps, ... ); // Method

//
   Polygon2d(Polygon2d *o,int a,int b); // Method

  Polygon2d maketri(int a,int b,int c); // Method

  float area(); // Method

};

#endif
