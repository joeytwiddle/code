#ifndef matrix0_H
  #define matrix0_H

  #include <globals.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <stdarg.h>
  #include <math.h>
  #include <time.h>
  #include <mem.h>
  #include <error.h>
  #include <strings.h>
  #include <list.h>
  #include <ordtree.h>
  #include <list2.h>
  #include <strings2.h>
  #include <progmon.h>
  #include <maths.h>
  #include <timer.h>
  #include <files.h>
  #include <htmler.h>
  #include <myrgb.h>
  #include <myalleg.h>
  #include <vnd.h>
  #include <v2d.h>
  #include <line2d.h>
  #include <polygon2d.h>

 class V3d;
 class Ori;

class Matrix {
public:
  // float** v;
	float v[3][3]; // Exists // = { {0,0,0}, {0,0,0}, {0,0,0} } ;
  V3d operator*(V3d u); // Method

   Matrix(); // Method

  static Matrix rotation(V3d axis,float ang); // Method

  void makerotation(V3d axis,float ang); // Method

  void makeorientation(Ori o); // Method

};

#endif
