#ifndef matrix0_H
  #define matrix0_H

 class V3d;
 class Ori;

class Matrix {
public:
  float** v; // Exists
  V3d operator*(V3d u); // Method

   Matrix(); // Method

  static Matrix rotation(V3d axis,float ang); // Method

  void makerotation(V3d axis,float ang); // Method

  void makeorientation(Ori o); // Method

};

#endif
