#ifndef frustrum_H
  #define frustrum_H

  #include <globals.h>

class Frustrum {
public:
  V3d cen; // Exists
  Ori ori; // Exists
  Plane left,right,top,bottom; // Exists
  float nearclip; // Exists
  float farclip; // Exists
   Frustrum(V3d a,Ori x,float c,float d); // Method

  int include(V3d v,float size); // Method

};

#endif
