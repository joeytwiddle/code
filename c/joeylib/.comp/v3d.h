#ifndef v3d_H
  #define v3d_H

  #include <myrgb.h>
  #include <v2d.h>
  #include <matrix0.h>

#include <stdio.h>
#include <math.h>

 class Ori;
 class Viewpoint;
 class Plane;

/* class Displayable {
   public:
     virtual void print()=0;
   };*/

 class V3d;

V3d operator*(float d,V3d v); // Method

V3d operator*(V3d v,float d); // Method

V3d operator/(V3d v,float d); // Method


class V3d /*: public Displayable*/ {
public:
  float x,y,z; // Exists
  
  fillin V3d origin,o,i,j,k,crazy; // Exists
  
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


  static V3d dimscale(V3d u,V3d v); // Method


  static float dot(V3d u,V3d v); // Method


  static float dot(V3d *u,V3d *v); // Method

  
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

  
  /* V3d breakdown(V3d a,V3d b,V3d c) { // breaks down vector into axes
       // p*a+q*b+r*c=this
       // p=(x-q*b.x-r*c.x)/a.x
       // (x-q*b.x-r*c.x)/a.x*a.y+q*b.y+r*c.y=y
       // q(-b.x/a.x*a.y+b.y)=y-r*c.y-(r*c.x+x)/a.x*a.y
       // q=(y-r*c.y-(r*c.x+x)/a.x*a.y)/(-b.x/a.x*a.y+b.y)
       // r*c.z=z-(x-q*b.x-r*c.x)/a.x*a.z-(y-r*c.y-(r*c.x+x)/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z
       // r*(c.z-c.x/a.x*a.z-(c.y-c.x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z=z-(x-q*b.x)/a.x*a.z-(y-x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z
       // r=z-(x-q*b.x)/a.x*a.z-(y-x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z/((c.z-c.x/a.x*a.z-(c.y-c.x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z)
       float r=z-(x-q*b.x)/a.x*a.z-(y-x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z/((c.z-c.x/a.x*a.z-(c.y-c.x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z);
       float q=(y-r*c.y-(r*c.x+x)/a.x*a.y)/(-b.x/a.x*a.y+b.y);
       float p=(x-q*b.x-r*c.x)/a.x;
       return V3d(p,q,r);
     } */

  V3d operator*(V3d o); // Method


  V3d orient(Viewpoint); // Method

  V3d disorient(Viewpoint); // Method


  V2d dropz(); // Method


  V2d xz(); // Method


  V2d perpproj(); // Method


	static float angBetween(V3d a,V3d b); // Method


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


// fillup extern V3d V3d::origin; // Variable initialised in .c file
// fillup extern V3d V3d::o; // Variable initialised in .c file
// fillup extern V3d V3d::i; // Variable initialised in .c file
// fillup extern V3d V3d::j; // Variable initialised in .c file
// fillup extern V3d V3d::k; // Variable initialised in .c file
// fillup extern V3d V3d::crazy; // Variable initialised in .c file


#endif
