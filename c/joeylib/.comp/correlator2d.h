#ifndef correlator2d_H
  #define correlator2d_H

extern int Correlator2dMethod; // Variable initialised in .c file // 1 = over-exclusive, 2 = over-inclusive

class Correlator2d {
public:
  List<V2d> points; // Exists // Points from which to find a line of best fit
  List<float> weights; // Exists // Weightings of the points

  int min; // Exists
  int maxposs; // Exists

  boolean made; // Exists
  // Redundant information (only exists after processing above)
  float bestang; // Exists
  List<int> pointskept; // Exists
  V2d centroid; // Exists
  float total; // Exists
  float besterr; // Exists

   Correlator2d(); // Method

   Correlator2d(List<V2d> pts); // Method


  void add(V2d v,float w); // Method

  void add(V2d v); // Method

  void add(Pixel p); // Method


  //

  void make(); // Method


  void addposs(List<int> n,List<List<int> > *poss,List<V2d> *cens,List<float> *totals); // Method


  void remake(); // Method


  float totalfor(List<int> ps); // Method


  V2d centroidfor(List<int> ps); // Method


  boolean tryset(List<int> ps); // Method


  boolean tryset(List<int> ps,V2d cen,float total); // Method


  float angle(); // Method

  float error(); // Method

  Line2d line(); // Method


  float crossesy(); // Method


  void remakebruteforce(); // Method


    bool used(int i); // Method


    // See drawCorrelator2d(c) in region.c

};





//


Map2d<bool> drawCorrelator2d(Correlator2d cc); // Method


RGBmp drawCorrelator2dInColour(Correlator2d cc); // Method


#endif
