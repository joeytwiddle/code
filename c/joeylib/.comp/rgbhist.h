#ifndef rgbhist_H
  #define rgbhist_H

class RGBHistogram {
public:
  int quant,numsamps; // Exists
  uchar ***rgb; // Exists
  
   RGBHistogram(); // Method

  
   RGBHistogram(int q); // Method

  
  float correlation(); // Method


    //

  
  float variance(); // Method

  
  void addocc(myRGB *c); // Method

  //

  
  RGBHistogram *normsmoothed(int radius); // Method

  
  // Gaussian
  RGBHistogram *smoothed(int radius); // Method

  static float difference(RGBHistogram a,RGBHistogram b); // Method

  void display(); // Method

  int largest(); // Method

  int maximumat(int i,int j,int k); // Method

  int eightmaximumat(int i,int j,int k); // Method

  int fourmaximumat(int i,int j,int k); // Method

};

float tofloat(RGBHistogram r); // Method


#endif
