#ifndef viseng_H
  #define viseng_H

#define scrwid 320
#define scrhei 200

extern float fogdepth; // Variable initialised in .c file

extern float turnability; // Variable initialised in .c file
extern float forcevel; // Variable initialised in .c file
extern float markerrange; // Variable initialised in .c file
extern int taillen; // Variable initialised in .c file

extern float partrad; // Variable initialised in .c file

extern V3d pos; // Variable initialised in .c file
extern Ori ori; // Variable initialised in .c file

  extern V3d vel; // Variable initialised in .c file
  extern V3d acc; // Variable initialised in .c file
  extern float droll; // Variable initialised in .c file
  extern float dyaw; // Variable initialised in .c file
  extern float dpitch; // Variable initialised in .c file

extern Octree<V3d> *octree; // Variable initialised in .c file

extern JBmp jbmp; // Variable initialised in .c file

void plotsphere(V3d cen,int c); // Method


void plotline(V3d a,V3d b,float i); // Method


V3d prepare(V3d v); // Method


void line(V3d a,V3d b); // Method


void plotscene(); // Method
    

void movement(); // Method


void visengfly(); // Method


#endif
