#ifndef V2DA_H
  #define V2DA_H

#include <stdio.h>
#include <math.h>

//
				//

class V2d {
public:
  float x,y; // Exists
  
   V2d(); // Method

  
   V2d(float u,float v); // Method


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

  
  V2d norm(); // Method

  
  void normalise(); // Method

  
  static float dist(V2d u,V2d v); // Method

  
  static int equal(V2d u,V2d v); // Method

  
  static float dot(V2d u,V2d v); // Method

  
  static float normdot(V2d u,V2d v); // Method

  
  static V2d rotate(V2d v,float ang); // Method

  
  V2d rotate(float ang); // Method

  
  //
  
  static V2d random(); // Method

  
  void changeto(V2d v); // Method

  
  float var(int i); // Method

  
  float getlongestside(); // Method

  
  void chop(float low,float high); // Method


};

V2d operator*(float d,V2d v); // Method


V2d operator*(V2d v,float d); // Method


V2d operator/(V2d v,float d); // Method


List<V2d> v2dsincircle(int cx,int cy,int rad); // Method






                                                                                                 
                                                                                                 

#endif
