#ifndef volrend_H
  #define volrend_H

  #include <v3d.h>
  #include <jbmp.h>

class VolRend {
public:
  Map2d<float> *rend; // Exists
  JBmp *b; // Exists
  float crazy; // Exists
  float smallest; // Exists
  bool (*invol)(V3d); // Exists
  int steps,coldifftol,scrwid,scrhei,res; // Exists
  float back; // Exists
  
  V3d searchline(V3d front,V3d back,bool wanted); // Method

  
  float getdepth(int i,int j); // Method

  
  int getcol(int rx,int ry); // Method

  
  void fillsquare(int x,int y,int w,int h); // Method

  
   VolRend(int w,int h,bool (*f)(V3d),int cdt,float jump,int s,float acc,float ba,int r); // Method

  
};

#endif
