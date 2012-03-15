#include <math.h>
#include <stdlib.h>

#define pi 3.1415926535897932385
#define nate 2.718281828
#define bigfloat 4000

#define TRIGLOOKUPOFF

  float inf=65535.0;

  float infinity=inf;


  float rad2deg=360.0/2.0/pi;


// Not all mod operations allow the right hand constraint
// for this, use mod2
// er but for floats i think mymod2 offsets cutoff by second argument

// -deprecated (by what? - I don't know, so let's carry on using it!)

//


float mysgn(float x);


int sgn(int i);


  class V2d;

 class Pixel{

public:
    int x,y;

   Pixel( );

   Pixel(int a,int b);

  Pixel (V2d v);

  bool operator==(Pixel o);

  Pixel operator+(Pixel p);

  Pixel operator-(Pixel p);

  Pixel operator*(float f);

  Pixel operator/(float f);

  String toString( );

  static bool ishor(int d);

  static int plusormin(int d);

  static Pixel indir(int d);

  static List<Pixel> line(Pixel a,Pixel b);
};


// For backwards compatibility when Pixel wasn't a class
Pixel newPixel(int x,int y);


Pixel operator*(float f,Pixel p);


//


float dist(Pixel a,Pixel b);












float myangle(float x,float y);


float mymod(float a,float b);


float mymod2(float x,float a,float b);


float floatmod(float a,float b);


float floatmod(float a,float b,float c);


//


int intmod(int x,int a,int b);


int intmod2(int x,int a,int b);


int intmod(int x,int b);


float mymod(float a);


float saw(float f);


float saw2(float f);


float chop(float x,float a,float b);


int intchop(int x,int a,int b);


int intchop(int x,int b);


int intchop2(int x,int a,int b);


float chop(float x);


uchar ucharchop(float x);


uchar ucharsaw(float x);


float fromto(float lambda,float from,float to);


float myabs(float f);


float floatabs(float f);


float mypow(float x,float y);


float power(float x,float y);


float myrnd( );


float myrnd2( );


float floatrnd(float l,float r);


int intrnd(int l,int r);


int intrnd(int r);


float magrnd(float m);


float nicernd(float f);


float nicernd( );


float mysquare(float f);


float square(float f);


float mysquaresgn(float f);


// - Procedures to make numbers deviate continuously around around means

float hang(float x,float cen,float damp,float rnd);


float hang(float *x,float cen,float damp,float rnd);


float hang(float *x,float cen,float damp);


 class Dampener{

public:
    float var,cen,damp,force;

   Dampener(float c,float d,float f);

  void perturb( );

  static float hang(float x,float cen,float damp,float rnd);
};


float quantiseto(float t,float x);


// -deprecated nah ballbag!
List<Pixel> pixelsincircle(int cx,int cy,int rad);


List<Pixel> *p2pixelsincircle(int cx,int cy,int rad);


List<Pixel> *p2pixelsincircle(Pixel p,int rad);


List<Pixel> *p2pixelsinrectangle(int l,int t,int r,int b);


int arewithinbounds(int x,int y,int left,int top,int right,int bottom);


float gimmezerofloat( );


int gimmezeroint( );


int gimmelargeint( );


bool gimmetruebool( );


float gaussian(float x);


float gabor(V2d v,V2d d,float size,float freq);


float gaborabs(V2d v,V2d d,float size,float freq);


float min(float a,float b);


float max(float a,float b);


void randomise(int i);


void randomise( );


#ifndef STUPIDUNIX
void randomiserem( );

#endif

 class Complex{

public:
    float x,y;

   Complex(float a,float b);

  Complex operator+(Complex o);

  Complex operator*(Complex o);

  Complex operator*(Complex *o);

  float mod( );
};


 class Quaternion{

public:
    float a,b,c,d;

   Quaternion( );

   Quaternion(float x,float y,float z,float q);

  Quaternion operator+(Quaternion o);

  Quaternion operator*(Quaternion o);

  float mod( );

  Quaternion operator*(float f);

  Quaternion operator/(float f);

  Quaternion operator-(Quaternion o);

  void setto(Quaternion q);

  String toString( );
};


uchar ucharmod(int x);


int ucharmirror(int x);


float plusminus(int x);


float plusminus( );


int intabs(int x);


float diff(float x,float y);


float mydiv(float x,float y);


void swapints(int *x,int *y);


template <class Object>void swap(Object *x,Object *y);


uchar ucharrnd( );


float myrnd(float f);


float deg2rad(float x);


float minus(float a,float b);


float pythag(float x,float y);


float hypotenuse(float x,float y);


bool boolor(bool x,bool y);


bool booland(bool x,bool y);


float wrapdist(float a,float b,float s);


float moddiff(float a,float b,float s);


  int triglookupquant=20000;

  float *sinlookup=new float[triglookupquant];

  float *coslookup=new float[triglookupquant];

  bool trigsetup=false;


void setuptriglookup( );


void setupsinlookup( );


float mysin(float x);


float mycos(float x);


bool boolrnd( );


float floatneg(float f);


bool boolneg(bool b);


float floatmult(float a,float b);


int wrap(int x,int a,int b);


int wrap(int x,int b);


float floatdist(float x,float y);


float pull(float a,float t,float b);


float oldpull(float a,float b,float t);


float tofloat(float f);


float tofloat(int f);


float tofloat(double f);


float ramp(float f);


bool even(int i);


bool odd(int i);


bool alternate(int i);


bool alternate(float d,float r);


#ifndef STUPIDUNIX
void wait(float secs);

#endif

float corrave(List<float> xs);


float corrS(List<float> xs,List<float> ys);


float correlation(List<float> xs,List<float> ys);


float lscorrelation(List<float> xs,List<float> ys);


float correlation(List<Pixel> *ps);


  class V2d;


 class Correlator{

public:
	  List<float> xs;

	  List<float> ys;

	  float *cxy;

	  float *cxx;

	  float *cyy;

	  float *ang;

	 Correlator( );

	void add(V2d p);

	void add(float x,float y);

	 Correlator(List<float> x,List<float> y);

	 Correlator(List<Pixel> *ps);

	float corrxy( );

	float corrxx( );

	float corryy( );

	float correlation( );

	float A( );

	float corrS(List<float> xs,List<float> ys);

	void freedom( );


	float sumx( );

	
	float sumy( );

	
  V2d centroid( );

	float angerror(float );

  V2d ori( );

  float length( );

  float fatness( );

	float angle( );
 // In clock form I believe
	float crossesy( );

	float crossesyxoutliers( );

	float yforx(float x);

	
	float grad( );

	
	float bestgradxoutliers( );

	
	// This method is not actually appropriate because
	// it works out ordered combinations
	// making 1,2,3 and 3,2,1 different!
	List<int> decompose(int n,int excl);

	
	float bestanglexoutliers( );

	
	float bestanglexoutliers(int k);
};


		

String floattoString(float f);


int max(int a,int b);


// a^b=c
// b=log c / log a
float depower(float c,float a);


float invert(float f);


bool angleless(float a,float b);


