#ifndef viewpnt_H
  #define viewpnt_H

  #include <ori.h>

class Viewpoint {
public:
  V3d pos; // Exists
  Ori ori; // Exists
   Viewpoint(); // Method

   Viewpoint(V3d p,Ori o); // Method

   Viewpoint(V3d from,V3d to,V3d up); // Method

  V3d orient(V3d x); // Method

  V3d disorient(V3d v); // Method

  V3d bring(V3d v); // Method

  static Viewpoint fromto(V3d from,V3d to,V3d up); // Method

};

/* Skipping later :: V3d V3d::orient(Viewpoint v); // Method
*/


/* Skipping later :: V3d V3d::disorient(Viewpoint v); // Method
*/


#endif
