#ifndef v2d_H
  #define v2d_H

#include <stdio.h>
#include <math.h>

  class V2d;
   
   V2d operator*(float d,V2d v); // Method

   V2d operator*(V2d v,float d); // Method

   V2d operator/(V2d v,float d); // Method


class V2d //
 {
public:
  float x,y; // Exists

  // static V2d o,i,j;
fillin V2d o; // Variable initialised in .c file
fillin V2d i; // Variable initialised in .c file
fillin V2d j; // Variable initialised in .c file
  
   V2d(); // Method


   V2d(const V2d &o); // Method

  
   V2d(float u,float v); // Method


   V2d(V2d *o); // Method


   V2d(Pixel p); // Method


  static V2d polar(float ang,float rad); // Method

  
  bool operator==(V2d o); // Method

  
  V2d operator+(V2d v); // Method

  
  V2d operator-(V2d v); // Method

  
  void print(); // Method

  
  String str(); // Method

  
  static void print(V2d v); // Method

  
  
  static V2d add(V2d u, V2d v); // Method

  
  
  static V2d mult(float a,V2d v); // Method

  
  
  V2d neg(); // Method

  
  static V2d neg(V2d v); // Method

  
  
  static V2d sub(V2d u, V2d v); // Method

  
  
  static float mod(V2d v); // Method

  
  float mod(); // Method

  
  
  static float radius(V2d v); // Method

  
  
  static V2d norm(V2d v); // Method

  
  V2d normalised(); // Method

  V2d norm(); // Method

  
  void normalise(); // Method

  
  static float dist(V2d u,V2d v); // Method


  float dist(V2d o); // Method

  
  static int equal(V2d u,V2d v); // Method

  
  static float dot(V2d u,V2d v); // Method


  float dot(V2d o); // Method

  
  static float normdot(V2d u,V2d v); // Method

  
  static V2d rotate(V2d v,float ang); // Method

  
//


  void rotate(float ang); // Method


  V2d rotated(float ang); // Method


  //

  
  static V2d random(); // Method


  static V2d randomcircle(); // Method

  
  void changeto(V2d v); // Method

  
  float var(int i); // Method

  
  float getlongestside(); // Method

  
  void chop(float low,float high); // Method

  
  V2d perp(); // Method

  
  String toString(); // Method


  float getx(); // Method


  float gety(); // Method


  static float getx(V2d v); // Method


  static float gety(V2d v); // Method


  static V2d angle(float f); // Method


  float mag(); // Method


  float angle(); // Method


  // Between -pi and +pi
  static float angbetween(V2d a,V2d b); // Method


};

V2d operator*(float d,V2d v); // Method


V2d operator*(V2d v,float d); // Method


V2d operator/(V2d v,float d); // Method



List<V2d> v2dsincircle(int cx,int cy,int rad); // Method



/* Skipping later ::  Pixel::Pixel(V2d v); // Method
*/


class Polar {
public:
  float ang,mag; // Exists
   Polar(float a,float m); // Method

   Polar(V2d v); // Method

  V2d v2d(); // Method

};

V2d operator-(V2d v); // Method


float tofloat(V2d v); // Method


#endif
