#ifndef v3d_H
  #define v3d_H

#include <stdio.h>
#include <math.h>

 class Ori;
 class Viewpoint;
 class Plane;

//


 class V3d;

V3d operator*(float d,V3d v); // Method

V3d operator*(V3d v,float d); // Method

V3d operator/(V3d v,float d); // Method


class V3d //
 {
public:
  float x,y,z; // Exists
  
fillin V3d origin; // Variable initialised in .c file
fillin V3d o; // Variable initialised in .c file
fillin V3d i; // Variable initialised in .c file
fillin V3d j; // Variable initialised in .c file
fillin V3d k; // Variable initialised in .c file
fillin V3d crazy; // Variable initialised in .c file
  // static V3d origin,o,i,j,k,crazy;
  
   V3d(); // Method

  
   V3d(float u,float v,float w); // Method

  
  myRGB myrgb(); // Method

  
  myRGB rgb(); // Method

  
  bool operator==(V3d b); // Method


  bool equals(V3d b); // Method

  
  V3d operator+(myRGB r); // Method

  
  V3d operator+(V3d v); // Method

  
  V3d operator-(V3d v); // Method

  
  V3d operator-(); // Method

  
  void print(); // Method

  
  String str(); // Method

  
  String toString(); // Method

  
  static void print(V3d v); // Method

  
  
  static V3d add(V3d u, V3d v); // Method

  
  
  static V3d mult(float a,V3d v); // Method

  
  
  V3d neg(); // Method

  
  static V3d neg(V3d v); // Method

  
  
  static V3d sub(V3d u, V3d v); // Method

  
  
  static float mod(V3d v); // Method

  
  static float smod(V3d v); // Method

  
  float mod(); // Method

  
  float mag(); // Method

  
  
  static float radius(V3d v); // Method

  
  
  static V3d norm(V3d v); // Method

  
  V3d norm(); // Method

  
  V3d normal(); // Method

  
  V3d normalised(); // Method

  
  void normalise(); // Method

  
  static float dist(V3d u,V3d v); // Method

  
  static int equal(V3d u,V3d v); // Method

  
  static float dot(V3d u,V3d v); // Method

  
  static V3d dimscale(V3d u,V3d v); // Method

  
  static float normdot(V3d u,V3d v); // Method

  
  static float normdot(V3d *u,V3d *v); // Method

  
  static V3d cross(V3d u,V3d v); // Method

  
  static V3d normcross(V3d u,V3d v); // Method

  
  static V3d oldrotate1(V3d v,V3d axis,float ang); // Method

  
  static V3d oldrotate(V3d v,V3d axis,float ang); // Method

  
  #ifdef ALLEGRO
  static V3d rotatea(V3d v,V3d axis,float ang); // Method

  #endif
  
  static V3d rotate(V3d v,V3d axis,float ang); // Method

  
  V3d rotate(V3d axis,float ang); // Method

  
  V3d rotated(V3d axis,float ang); // Method

  
  #ifdef ALLEGRO
  static V3d rotatelista(List<V3d> l,V3d axis,float ang); // Method

  #endif
  
  static V3d rotatelist(List<V3d> l,V3d axis,float ang); // Method

  
  static V3d random(); // Method

  
  static V3d random(float f); // Method

  
  static V3d randomvolume(); // Method

  
  static V3d randomvolume(float f); // Method

  
  void changeto(V3d v); // Method

  
  void setto(V3d v); // Method

  
  void operator=(V3d v); // Method

  
  float var(int i); // Method

  
  float getlongestside(); // Method

  
  void chop(float low,float high); // Method

  
  static V3d orientate(V3d v,Ori ori); // Method

  static V3d qorientate(V3d v,Ori ori); // Method

  static V3d disorientate(V3d v,Ori ori); // Method

  V3d orientate(Ori ori); // Method

  V3d disorientate(Ori ori); // Method

  
  //


  V3d operator*(V3d o); // Method


  V3d orient(Viewpoint); // Method

  V3d disorient(Viewpoint); // Method


  V2d dropz(); // Method


  V2d xz(); // Method


  V2d perpproj(); // Method


  float distAbove(Plane p); // Method
 // -ve if below in terms of plane's normal

};

V3d operator*(float d,V3d v); // Method


V3d operator*(V3d v,float d); // Method


V3d operator/(V3d v,float d); // Method


/* Skipping later :: V3d myRGB::v3d(); // Method
*/



// myRGB myRGB::operator*(myRGB o) {
  // return V3d::dimscale(o.v3d(),this->v3d()).rgb();
// }

float tofloat(V3d v); // Method


float V3dtofloat(V3d v); // Method


float tofloat(myRGB r); // Method


V3d hang(V3d x,V3d cen,float damp,float rnd); // Method


V2d hang(V2d x,V2d cen,float damp,float rnd); // Method


V2d hang(V2d *x,V2d cen,float damp,float rnd); // Method


V3d pull(V3d a,float t,V3d b); // Method


/* Skipping later :: myRGB myRGB::ave(myRGB o); // Method
*/


#endif
