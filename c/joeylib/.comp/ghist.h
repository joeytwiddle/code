#ifndef ghist_H
  #define ghist_H

class GHist {
public:
  Map1d<int> m; // Exists
  int total; // Exists
   GHist(); // Method

   GHist(int s); // Method

  void addocc(float f); // Method

  int difference(GHist *o); // Method

  float variance(); // Method

  float glvariance(); // Method

  void smooth(int r); // Method

  void freedom(); // Method

};

float tofloat(GHist g); // Method


float tofloat(GHist *g); // Method

#endif
