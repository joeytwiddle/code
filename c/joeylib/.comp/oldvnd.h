#ifndef oldvnd_H
  #define oldvnd_H

class Vnd {
public:
  int dim; // Exists
  float *ds; // Exists
   Vnd(); // Method

   Vnd(int ss); // Method

//

  Vnd operator+(Vnd o); // Method

  Vnd operator*(float f); // Method

};

float tofloat(Vnd v); // Method


#endif
