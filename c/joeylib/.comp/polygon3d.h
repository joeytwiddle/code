#ifndef polygon3d_H
  #define polygon3d_H

//



class Polygon3d {
public:
  List<V3d> vs; // Exists

   Polygon3d(); // Method


   Polygon3d(V3d a,V3d b,V3d c); // Method


  V3d centre(); // Method


  Line3d *intersectionheight(float y); // Method


  void getintersectionheight(float y,Line3d *l,bool *s); // Method


  Line3d *addifintersectionheight(float y,List<Line3d> *ls); // Method


  Line3d linefrom(int i); // Method


  String toString(); // Method


  void freedom(); // Method


};

#endif
