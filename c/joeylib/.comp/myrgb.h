#ifndef myrgb_H
  #define myrgb_H

 class V3d;

class myRGB {
public:
  uchar r,g,b; // Exists
  fillin myRGB white,grey,black; // Exists
  fillin myRGB red,green,blue,yellow,magenta,cyan; // Exists
  fillin myRGB darkred,darkgreen,darkblue,darkyellow,darkmagenta,darkcyan; // Exists
   myRGB(); // Method

  bool operator==(myRGB o); // Method

  //
   myRGB(int x,int y,int z); // Method

   myRGB(uchar x,uchar y,uchar z); // Method

//
   myRGB(double x,double y,double z); // Method

  void print(); // Method

  V3d v3d(); // Method

  void perturb(int amount); // Method

  myRGB operator+(myRGB o); // Method

  myRGB operator/(float f); // Method

  myRGB operator-(myRGB o); // Method

  float brightness(); // Method

  static float distance(myRGB *a,myRGB *b); // Method

  static float difference(myRGB *a,myRGB *b); // Method

  static float huefor(float t,float o); // Method

  static float darkhuefor(float t,float o); // Method

  static float darkhuefor2(float t,float o); // Method

  static myRGB hue(float a); // Method

  static myRGB darkhue(float a); // Method

  static myRGB darkhue2(float a); // Method

  static myRGB random(); // Method

  myRGB dark(); // Method

  myRGB pastel(); // Method

  static myRGB randompastel(); // Method

  static myRGB randomdark(); // Method

  static myRGB randomdarkpastel(); // Method

  myRGB ave(myRGB o); // Method

  myRGB operator*(float a); // Method

  myRGB operator*(myRGB o); // Method

  myRGB darklight(float f); // Method

  String toString(); // Method

};

myRGB operator*(float a,myRGB r); // Method


// fillup extern myRGB myRGB::white; // Variable initialised in .c file
// fillup extern myRGB myRGB::red  ; // Variable initialised in .c file
// fillup extern myRGB myRGB::green; // Variable initialised in .c file
// fillup extern myRGB myRGB::blue ; // Variable initialised in .c file
// fillup extern myRGB myRGB::yellow; // Variable initialised in .c file
// fillup extern myRGB myRGB::magenta; // Variable initialised in .c file
// fillup extern myRGB myRGB::cyan; // Variable initialised in .c file
// fillup extern myRGB myRGB::black; // Variable initialised in .c file
// fillup extern myRGB myRGB::grey; // Variable initialised in .c file

// fillup extern myRGB myRGB::darkred; // Variable initialised in .c file
// fillup extern myRGB myRGB::darkgreen; // Variable initialised in .c file
// fillup extern myRGB myRGB::darkblue; // Variable initialised in .c file
// fillup extern myRGB myRGB::darkyellow; // Variable initialised in .c file
// fillup extern myRGB myRGB::darkmagenta; // Variable initialised in .c file
// fillup extern myRGB myRGB::darkcyan; // Variable initialised in .c file

#endif
