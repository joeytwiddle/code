#ifndef maths_H
  #define maths_H

#include <math.h>
#include <stdlib.h>

#define pi 3.1415926535897932385
#define nate 2.718281828
#define bigfloat 4000

#define TRIGLOOKUPOFF

#define bit uchar

extern float inf; // Variable initialised in .c file
extern float infinity; // Variable initialised in .c file

extern float rad2deg; // Variable initialised in .c file

// mod2 allows you to specify lower and upper bounds


// Binary operations

bit bingetbit(int b,int n); // Method



// Simple maths

float mysgn(float x); // Method


int sgn(int i); // Method


 class V2d;

class Pixel {
public:
  int x,y; // Exists
   Pixel(); // Method

   Pixel(int a,int b); // Method

  Pixel (V2d v); // Method

  bool operator==(Pixel o); // Method

  Pixel operator+(Pixel p); // Method

  Pixel operator-(Pixel p); // Method

  Pixel operator*(float f); // Method

  Pixel operator/(float f); // Method

  String toString(); // Method

  static bool ishor(int d); // Method

  static int plusormin(int d); // Method

  static Pixel indir(int d); // Method

  static List<Pixel> line(Pixel a,Pixel b); // Method

};

// For backwards compatibility when Pixel wasn't a class
Pixel newPixel(int x,int y); // Method


Pixel operator*(float f,Pixel p); // Method


//


float dist(Pixel a,Pixel b); // Method












float myangle(float x,float y); // Method


float mymod(float a,float b); // Method


float mymod(float x,float a,float b); // Method


float modtwopi(float a); // Method


float mymod2(float x,float a,float b); // Method


float floatmod(float a,float b); // Method


float floatmod(float a,float b,float c); // Method


//


int intmod(int x,int a,int b); // Method


int intmod2(int x,int a,int b); // Method


int intmod(int x,int b); // Method


float mymod(float a); // Method


float saw(float f); // Method


float saw2(float f); // Method


float chop(float x,float a,float b); // Method


int intchop(int x,int a,int b); // Method


int intchop(int x,int b); // Method


int intchop2(int x,int a,int b); // Method


float chop(float x); // Method


uchar ucharchop(float x); // Method


uchar ucharsaw(float x); // Method


float fromto(float lambda,float from,float to); // Method


float myabs(float f); // Method


float floatabs(float f); // Method


float mypow(float x,float y); // Method


float power(float x,float y); // Method


float myrnd(); // Method


float myrnd2(); // Method


float floatrnd(float l,float r); // Method


int intrnd(int l,int r); // Method


int intrnd(int r); // Method


float magrnd(float m); // Method


float nicernd(float f); // Method


float nicernd(); // Method


float mysquare(float f); // Method


float square(float f); // Method


float mysquaresgn(float f); // Method


// - Procedures to make numbers deviate continuously around around means

float hang(float x,float cen,float damp,float rnd); // Method


float hang(float *x,float cen,float damp,float rnd); // Method


float hang(float *x,float cen,float damp); // Method


class Dampener {
public:
  float var,cen,damp,force; // Exists
   Dampener(float c,float d,float f); // Method

  void perturb(); // Method

  static float hang(float x,float cen,float damp,float rnd); // Method

};

float quantiseto(float t,float x); // Method


// -deprecated nah ballbag!
List<Pixel> pixelsincircle(int cx,int cy,int rad); // Method


List<Pixel> *p2pixelsincircle(int cx,int cy,int rad); // Method


List<Pixel> *p2pixelsincircle(Pixel p,int rad); // Method


List<Pixel> *p2pixelsinrectangle(int l,int t,int r,int b); // Method


int arewithinbounds(int x,int y,int left,int top,int right,int bottom); // Method


float gimmezerofloat(); // Method


int gimmezeroint(); // Method


int gimmelargeint(); // Method


bool gimmetruebool(); // Method


float gaussian(float x); // Method


float gabor(V2d v,V2d d,float size,float freq); // Method


float gaborabs(V2d v,V2d d,float size,float freq); // Method


float min(float a,float b); // Method


float max(float a,float b); // Method


void randomise(int i); // Method


void randomise(); // Method


#ifndef STUPIDUNIX
void randomiserem(); // Method

#endif

class Complex {
public:
  float x,y; // Exists
   Complex(float a,float b); // Method

  Complex operator+(Complex o); // Method

  Complex operator*(Complex o); // Method

  Complex operator*(Complex *o); // Method

  float mod(); // Method

};

class Quaternion {
public:
  float a,b,c,d; // Exists
   Quaternion(); // Method

   Quaternion(float x,float y,float z,float q); // Method

  Quaternion operator+(Quaternion o); // Method

  Quaternion operator*(Quaternion o); // Method

  float mod(); // Method

  Quaternion operator*(float f); // Method

  Quaternion operator/(float f); // Method

  Quaternion operator-(Quaternion o); // Method

  void setto(Quaternion q); // Method

  String toString(); // Method

};

uchar ucharmod(int x); // Method


int ucharmirror(int x); // Method


float plusminus(int x); // Method


float plusminus(); // Method


int intabs(int x); // Method


float diff(float x,float y); // Method


float mydiv(float x,float y); // Method


void swapints(int *x,int *y); // Method


template <class Object>
void swap(Object *x,Object *y); // Method


uchar ucharrnd(); // Method


float myrnd(float f); // Method


float deg2rad(float x); // Method


float minus(float a,float b); // Method


float pythag(float x,float y); // Method


float hypotenuse(float x,float y); // Method


bool boolor(bool x,bool y); // Method


bool booland(bool x,bool y); // Method


float wrapdist(float a,float b,float s); // Method


float moddiff(float a,float b,float s); // Method


extern int triglookupquant; // Variable initialised in .c file
extern float *sinlookup; // Variable initialised in .c file
extern float *coslookup; // Variable initialised in .c file
extern bool trigsetup; // Variable initialised in .c file

void setuptriglookup(); // Method


void setupsinlookup(); // Method


float mysin(float x); // Method


float mycos(float x); // Method


bool boolrnd(); // Method


float floatneg(float f); // Method


bool boolneg(bool b); // Method


float floatmult(float a,float b); // Method


int wrap(int x,int a,int b); // Method


int wrap(int x,int b); // Method


float floatdist(float x,float y); // Method


float pull(float a,float t,float b); // Method


float oldpull(float a,float b,float t); // Method


float tofloat(float f); // Method


float tofloat(int f); // Method


//


//


float ramp(float f); // Method


bool even(int i); // Method


bool odd(int i); // Method


bool alternate(int i); // Method


bool alternate(float d,float r); // Method


#ifndef STUPIDUNIX
// void wait(float secs) {
  // int t=time(NULL);
  // while (time(NULL)<t+secs) { };
// }
void wait(time_t secs); // Method

#endif

float corrave(List<float> xs); // Method


float corrS(List<float> xs,List<float> ys); // Method


float correlation(List<float> xs,List<float> ys); // Method


float lscorrelation(List<float> xs,List<float> ys); // Method


float correlation(List<Pixel> *ps); // Method


//template <class Obj>
//extern class Map2d;

 class Line2d;

class Correlator {
public:
	List<float> xs; // Exists
	List<float> ys; // Exists
	List<float> ws; // Exists
	float *cxy; // Exists
	float *cxx; // Exists
	float *cyy; // Exists
	float *ang; // Exists
	 Correlator(); // Method

	// I have had to implement drawCorrelator();
//	Map2d<float> draw();
	void add(V2d p); // Method

	void add(V2d p,float w); // Method

	void add(List<V2d> v); // Method

	void add(float x,float y); // Method

	void add(float x,float y,float w); // Method

	 Correlator(List<float> x,List<float> y); // Method

	 Correlator(List<Pixel> *ps); // Method

	float totalweight(); // Method

	float corrxy(); // Method

	float corrxx(); // Method

	float corryy(); // Method

	float correlation(); // Method

	float A(); // Method

	float corrS(List<float> xs,List<float> ys); // Method

	void freedom(); // Method


	float sumx(); // Method

	
	float sumy(); // Method

	
  V2d centroid(); // Method

	float angerror(float); // Method

	float error(); // Method

  V2d ori(); // Method

  Line2d line(); // Method

  float length(); // Method

  float fatness(); // Method

	float angle(); // Method
 // In clock form I believe
	float crossesy(); // Method

	float crossesyxoutliers(); // Method

	float yforx(float x); // Method

	
	float grad(); // Method

	
	float bestgradxoutliers(); // Method

	
	// This method is not actually appropriate because
	// it works out ordered combinations
	// making 1,2,3 and 3,2,1 different!
	List<int> decompose(int n,int excl); // Method

	
	float bestanglexoutliers(); // Method

	
	float bestanglexoutliers(int k); // Method


};

		

String floattoString(float f); // Method


int max(int a,int b); // Method


// a^b=c
// b=log c / log a
float depower(float c,float a); // Method


float invert(float f); // Method


//


bool angleless(float a,float b); // Method


template<class Object>
Object *p2newtemplate(Object o); // Method


float *p2newfloat(float f); // Method


// ang and bang may not be more than 2pi apart
float anglebetween(float ang,float bang); // Method


int factorial(int n); // Method


#endif
