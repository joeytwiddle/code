#ifndef maths2_H
  #define maths2_H

//RGBmp *OUTLIERIMAGE;
extern List<V2d> Excled; // Exists

myRGB pull(myRGB a,float t,myRGB b); // Method


/* Skipping later :: V2d Correlator::centroid(); // Method
*/


/* Skipping later :: float Correlator::angerror(float a); // Method
*/


/* Skipping later :: V2d Correlator::ori(); // Method
*/


/* Skipping later :: Line2d Correlator::line(); // Method
*/


/* Skipping later :: float Correlator::length(); // Method
*/


// Not compensated for weighting
/* Skipping later :: float Correlator::fatness(); // Method
*/


  /* Skipping later :: void Correlator::add(V2d p); // Method
*/


  /* Skipping later :: void Correlator::add(V2d p,float w); // Method
*/


  /* Skipping later :: void Correlator::add(List<V2d> ls); // Method
*/

	
	/* Skipping later :: float Correlator::angle(); // Method
*/


  /* Skipping later :: float Correlator::error(); // Method
*/


  // None of the rest has been compensated for weighting
	/* Skipping later :: float Correlator::crossesy(); // Method
*/

	
	/* Skipping later :: float Correlator::crossesyxoutliers(); // Method
*/

	
	/* Skipping later :: float Correlator::yforx(float x); // Method
*/


/* Skipping later :: float Correlator::bestanglexoutliers(int k); // Method
*/


  // See drawCorrelator(c) in map2d.c


float gabor(V2d v,V2d d,float size,float freq); // Method


float gaborabs(V2d v,V2d d,float size,float freq); // Method






// A circle that maps to the whole R^2 plane.
// Points at radius 1 on the circle map to infinity on the plane.

extern float maxinfcir; // Variable initialised in .c file

  V2d infcircletoplane(V2d v); // Method


  V2d planetoinfcircle(V2d v); // Method



#endif
