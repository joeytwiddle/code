#ifndef maths_C
  #define maths_C

  #include <maths.h>

#include <math.h>
#include <stdlib.h>

#define pi 3.1415926535897932385
#define nate 2.718281828
#define bigfloat 4000

#define TRIGLOOKUPOFF

#define bit uchar

float inf=65535.0;
float infinity=inf;

float rad2deg=360.0/2.0/pi;

// mod2 allows you to specify lower and upper bounds


// Binary operations

bit bingetbit(int b,int n) {
  return (n >> b) & 0x0001;
}


// Simple maths

float mysgn(float x) {
  if (x>0)
    return 1;
  if (x<0)
    return -1;
  return 0;
}

int sgn(int i) {
  return ( i>0 ? 1 : ( i<0 ? -1 : 0 ) );
}

// Class declaration of existence: V2d


// Starts of class Pixel


  // Variable declared in .h file
   Pixel::Pixel() {
  }
   Pixel::Pixel(int a,int b) {
    x=a;
    y=b;
  }
  /* Skipping declared method Pixel Pixel::(V2d v) */

  bool Pixel::operator==(Pixel o) {
    return (x==o.x && y==o.y);
  }
  Pixel Pixel::operator+(Pixel p) {
    return Pixel(x+p.x,y+p.y);
  }
  Pixel Pixel::operator-(Pixel p) {
    return Pixel(x-p.x,y-p.y);
  }
  Pixel Pixel::operator*(float f) {
    return Pixel(x*f,y*f);
  }
  Pixel Pixel::operator/(float f) {
    return *this*(1.0/f);
  }
  String Pixel::toString() {
    return Sformat("(%i,%i)",x,y);
  }
   bool Pixel::ishor(int d) {
    return ((d % 2)==1);
  }
   int Pixel::plusormin(int d) {
    return ( (d%4)<=1 ? +1 : -1 );
  }
   Pixel Pixel::indir(int d) {
    return Pixel( ( ishor(d) ? plusormin(d) : 0 ) ,
                  ( ishor(d) ? 0 : plusormin(d) ) );
  }
   List<Pixel> Pixel::line(Pixel a,Pixel b) {
//printf("a\n");
    List<Pixel> ls;
    if (a==b) {
      ls.add(a);
//printf("b\n");
      return ls;
    }
    Pixel diff=b-a;
    if (abs(diff.x)>abs(diff.y)) {
      int diffm=abs(diff.x);
      for (int i=0;i<=diffm;i++) {
//        printf("%i/%i\n",i,diffm);
//        Pixel p=a+diff*((float)i/(float)diffm);
        Pixel p=Pixel(a.x+mysgn(diff.x)*i,a.y+diff.y*((float)i/(float)diffm));
        ls.add(p);
      }
    } else {
      int diffm=abs(diff.y);
      for (int i=0;i<=diffm;i++) {
//        printf(" %i/%i\n",i,diffm);
//        Pixel p=a+diff*((float)i/(float)diffm);
        Pixel p=Pixel(a.x+diff.x*((float)i/(float)diffm),a.y+mysgn(diff.y)*i);
        ls.add(p);
      }
    }
//printf("c\n");
    return ls;
  }
// End class 


// For backwards compatibility when Pixel wasn't a class
Pixel newPixel(int x,int y) {
  return Pixel(x,y);
}

Pixel operator*(float f,Pixel p) {
  return p*f;
}

/*
Pixel operator+(Pixel a,Pixel b) {
  return newPixel(a.x+b.x,a.y+b.y);
}

Pixel operator/(Pixel a,float b) {
  return newPixel((int)((float)a.x/b),(int)((float)a.y/b));
}

bool operator==(Pixel a,Pixel b) {
  return ((a.x==b.x && a.y==b.y) || (a.x==b.y && a.y==b.x));
}*/

float dist(Pixel a,Pixel b) {
  Pixel c=a-b;
  return sqrt(c.x*c.x+c.y*c.y);
}











float myangle(float x,float y) { // returns (-pi,pi)
  float ang;
  ang=atan2(x,y);
  return ang;
}

float mymod(float a,float b) {
  while (a<0 || a>=b) {
    if (a<0)
      a=a+b;
    if (a>=b)
      a=a-b;
  }
  return a;
}

float mymod(float x,float a,float b) {
  while (x<a || x>=b) {
    if (x<a)
      x=x+(b-a);
    if (x>=b)
      x=x-(b-a);
  }
  return x;
}

float modtwopi(float a) {
  return mymod(a,2.0*pi);
}

float mymod2(float x,float a,float b) {
  while (x<a || x>=b) {
    if (x<a)
      x=x+(b-a);
    if (x>=b)
      x=x-(b-a);
  }
  return x;
}

float floatmod(float a,float b) {
  return mymod(a,b);
}

float floatmod(float a,float b,float c) {
  return mymod2(a,b,c);
}

/* int intmod(int x,int a,int b) { // Don't know if this is valid
  float f=((float)x-(float)a)/(float)(b-a);
  return (int)(f*(b-a));
} */

int intmod(int x,int a,int b) { 
  if (x<a)
    return intmod(x+(b-a),a,b);
  if (x>=b)
    return intmod(x-(b-a),a,b);
  return x;
}

int intmod2(int x,int a,int b) {
  if (x<a)
    return intmod2(x+(b-a+1),a,b);
  if (x>b)
    return intmod2(x-(b-a+1),a,b);
  return x;
}

int intmod(int x,int b) {
  return intmod(x,0,b);
  // return (x % b);
}

float mymod(float a) {
  return mymod(a,1);
}

float saw(float f) {
  return mymod(f);
}

float saw2(float f) {
  float d=mymod(f,2.0);
  if (d<=1.0)
    return d;
  else
  return 2.0-d;
}

float chop(float x,float a,float b) {
  if (x<a)
    x=a;
  if (x>b)
    x=b;
  return x;
}

int intchop(int x,int a,int b) {
  if (x<a)
    return a;
  if (x>b)
    return b;
  return x;
}

int intchop(int x,int b) {
  return intchop(x,0,b);
}

int intchop2(int x,int a,int b) {
  if (x<a)
    return a;
  if (x>=b)
    return b;
  return x;
}

float chop(float x) {
  return chop(x,0,1);
}

uchar ucharchop(float x) {
  return (uchar)chop(x,0,255);
}

uchar ucharsaw(float x) {
  return (uchar)mymod(x,256);
}

float fromto(float lambda,float from,float to) {
  return from+lambda*(to-from);
}

float myabs(float f) {
  if (f<0)
    return -f;
  else
  return f;
}

float floatabs(float f) {
  return myabs(f);
}

float mypow(float x,float y) {
  return pow(x,y);
}

float power(float x,float y) {
  return pow(x,y);
}

float myrnd() {
  /* - Returns random float in range [0,1)
     Spaceing between choices is 1/RAND_MAX */
  return (float)rand()/RAND_MAX;
}

float myrnd2() {
  /* - Returns random float in range [0,1]
     Spaceing between choices is 1/(RAND_MAX-1) */
  return (float)rand()/(RAND_MAX-1);
}

float floatrnd(float l,float r) {
  return l+myrnd2()*(r-l);
}

int intrnd(int l,int r) {
  int x=(int)(l+myrnd()*(r-l+1));
  if (x>r)
    x=r;
  return x;
}

int intrnd(int r) {
  return intrnd(0,r);
}

float magrnd(float m) {
  return -m+2.0*m*myrnd2();
}

float nicernd(float f) {
  return -f+2.0*f*myrnd2();
}

float nicernd() {
  return nicernd(1);
}

float mysquare(float f) {
  return f*f;
}

float square(float f) {
  return f*f;
}

float mysquaresgn(float f) {
  return mysgn(f)*f*f;
}

// - Procedures to make numbers deviate continuously around around means

float hang(float x,float cen,float damp,float rnd) {
  return cen+(x-cen)*damp+magrnd(rnd);
}

float hang(float *x,float cen,float damp,float rnd) {
  *x=hang(*x,cen,damp,rnd);
}

float hang(float *x,float cen,float damp) {
  *x=hang(*x,cen,damp,0);
}

// Starts of class Dampener


  // Variable declared in .h file
   Dampener::Dampener(float c,float d,float f) {
    var=c;
    cen=c;
    damp=d;
    force=f;
  }
  void Dampener::perturb() {
    var=cen+(var-cen)*damp+floatrnd(-force,force);
  }
   float Dampener::hang(float x,float cen,float damp,float rnd) {
    return hang(x,cen,damp,rnd);
  }
// End class 


float quantiseto(float t,float x) {
  int y=(int)(x/t);
  return (float)y*t;
}

// -deprecated nah ballbag!
List<Pixel> pixelsincircle(int cx,int cy,int rad) {
  List<Pixel> l;
  for (int x=-rad;x<=rad;x++) {
    int h=(int)sqrt(rad*rad-x*x);
    for (int y=-h;y<=h;y++) {
      Pixel p=newPixel(cx+x,cy+y);
      l+p;
    }
  }
  return l;
}

List<Pixel> *p2pixelsincircle(int cx,int cy,int rad) {
//  printf("P0\n");
  List<Pixel> *l=new List<Pixel>(pi*rad*rad);
//  printf("P1\n");
  for (int x=-rad;x<=rad;x++) {
//    printf("P2\n");
    int h=(int)sqrt(rad*rad-x*x);
//    printf("P3\n");
    for (int y=-h;y<=h;y++) {
//      printf("P4\n");
      Pixel p=Pixel(cx+x,cy+y);
//      printf("P5\n");
      l->add(p);
    }
//    printf("P6\n");
  }
  return l;
}

List<Pixel> *p2pixelsincircle(Pixel p,int rad) {
//  printf("P\n");
  return p2pixelsincircle(p.x,p.y,rad);
}

List<Pixel> *p2pixelsinrectangle(int l,int t,int r,int b) {
  List<Pixel> *ps=new List<Pixel>();
  for (int i=l;i<=r;i++)
  for (int j=t;j<=b;j++)
    ps->add(Pixel(i,j));
  return ps;
}

int arewithinbounds(int x,int y,int left,int top,int right,int bottom) {
  return (x>=left && x<right && y>=top && y<bottom);
}

float gimmezerofloat() {
  return 0;
}

int gimmezeroint() {
  return 0;
}

int gimmelargeint() {
  return 99999999;
}

bool gimmetruebool() {
  return true;
}

float gaussian(float x) {
  // if (x<0.00000001)
  // return 1;
  // if (x>10000000.0)
  // return 0;
  return pow(nate,-mysquare(1.5*x));
}

/* Skipping declared method float gabor(V2d v,V2d d,float size,float freq) */


/* Skipping declared method float gaborabs(V2d v,V2d d,float size,float freq) */


float min(float a,float b) {
  if (a<b)
    return a;
  else
  return b;
}

float max(float a,float b) {
  if (a>b)
    return a;
  else
  return b;
}

void randomise(int i) {
//  #ifdef DOS
  srand(i);
//  #endif
}

void randomise() {
#ifndef STUPIDUNIX
  int i=(int)time(NULL);
  randomise(i);
#endif
}

#ifndef STUPIDUNIX
void randomiserem() {
  int i=(int)time(NULL);
  randomise(i);
  FILE* f=fopen("lastrndseed","w");
  fprintf(f,"%i",i);
  fclose(f);
}
#endif

// Starts of class Complex


  // Variable declared in .h file
   Complex::Complex(float a,float b) {
    x=a;
    y=b;
  }
  Complex Complex::operator+(Complex o) {
    return Complex(x+o.x,y+o.y);
  }
  Complex Complex::operator*(Complex o) {
    // (x+iy)*(ox+ioy)
    // = x*ox-y*oy+i(ox*y+oy*x)
    return Complex(x*o.x-y*o.y,x*o.y+y*o.x);
  }
  Complex Complex::operator*(Complex *o) {
    // is this more efficient?
    /* According to experimental results this is the case
       Isn't C++ a bit stupid?
    */
    // (x+iy)*(ox+ioy)
    // = x*ox-y*oy+i(ox*y+oy*x)
    return Complex(x*o->x-y*o->y,x*o->y+y*o->x);
  }
  float Complex::mod() {
    return sqrt(x*x+y*y);
  }
// End class 


// Starts of class Quaternion


  // Variable declared in .h file
   Quaternion::Quaternion() {
  }
   Quaternion::Quaternion(float x,float y,float z,float q) {
    a=x; b=y; c=z; d=q;
  }
  Quaternion Quaternion::operator+(Quaternion o) {
    return Quaternion(a+o.a,b+o.b,c+o.c,d+o.d);
  }
  Quaternion Quaternion::operator*(Quaternion o) {
    // (a+ib+jc+kd)*(oa+iob+joc+kod)
    // = a*oa+i(a*ob)+j(a*oc)+k(a*od)
    // + i(b*oa)-b*ob+ij(b*oc)+ik(b*od)
    // + j(c*oa)+ji(c*ob)-c*oc+jk(c*od)
    // + k(d*oa)+ki(d*ob)+kj(d*oc)-d*od
    // = a*oa-b*ob-c*oc-d*od
    // + i(a*ob+b*oa+c*od-d*oc)
    // + j(a*oc+c*oa-b*od+d*ob)
    // + k(a*od+d*oa+b*oc-c*ob)
    return Quaternion(a*o.a-b*o.b-c*o.c-d*o.d,
    a*o.b+b*o.a+c*o.d-d*o.c,
    a*o.c+c*o.a-b*o.d+d*o.b,
    a*o.d+d*o.a+b*o.c-c*o.b);
  }
  float Quaternion::mod() {
    return sqrt(a*a+b*b+c*c+d*d);
  }
  Quaternion Quaternion::operator*(float f) {
    return Quaternion(a*f,b*f,c*f,d*f);
  }
  Quaternion Quaternion::operator/(float f) {
    return Quaternion(a/f,b/f,c/f,d/f);
  }
  Quaternion Quaternion::operator-(Quaternion o) {
    return *this+o*-1;
  }
  void Quaternion::setto(Quaternion q) {
    a=q.a; b=q.b; c=q.c; d=q.d;
  }
  String Quaternion::toString() {
    return Sformat("(%f,%f,%f,%f)",a,b,c,d);
  }
// End class 


uchar ucharmod(int x) {
  return (uchar)mymod(x,256);
}

int ucharmirror(int x) {
  return (int)ucharmod(ucharmod(x)+127)-127;
}

float plusminus(int x) {
  if (x%2==0)
    return -1;
  else
  return 1;
}

float plusminus() {
  return plusminus(myrnd()*2);
}

int intabs(int x) {
  return myabs(x);
}

float diff(float x,float y) {
  return myabs(y-x);
}

float mydiv(float x,float y) {
  return (float)((int)(x/y))*y;
}

void swapints(int *x,int *y) {
  int tmp=*x;
  *x=*y;
  *y=tmp;
}

template <class Object>
void swap(Object *x,Object *y) {
  Object tmp=*x;
  *x=*y;
  *y=tmp;
}

uchar ucharrnd() {
  return myrnd()*256;
}

float myrnd(float f) {
  return myrnd()*f;
}

float deg2rad(float x) {
  return 2*pi*x/360.0;
}

float minus(float a,float b) {
  return a-b;
}

float pythag(float x,float y) {
  return sqrt(x*x+y*y);
}

float hypotenuse(float x,float y) {
  return pythag(x,y);
}

bool boolor(bool x,bool y) {
  return (x || y);
}

bool booland(bool x,bool y) {
  return (x && y);
}

float wrapdist(float a,float b,float s) {
  float c=diff(floatmod(a,0,s),floatmod(b,0,s));
  if (c>s/2)
    c=s-c;
  return c;
}

float moddiff(float a,float b,float s) {
  return wrapdist(a,b,s);
}

int triglookupquant=20000;
float *sinlookup=new float[triglookupquant];
float *coslookup=new float[triglookupquant];
bool trigsetup=false;

void setuptriglookup() {
  if (!trigsetup) {
    trigsetup=true;
  for (int i=0;i<triglookupquant;i++) {
    sinlookup[i]=sin(2*pi*i/(float)triglookupquant);
    coslookup[i]=cos(2*pi*i/(float)triglookupquant);
  }
  }
}

void setupsinlookup() { //- deprecated
  setuptriglookup();
}

float mysin(float x) {
#ifdef TRIGLOOKUPOFF
  return sin(x);
#endif
  setuptriglookup();
  float y=mymod(x,2*pi);
  return sinlookup[(int)(y*triglookupquant/2/pi)];
}

float mycos(float x) {
#ifdef TRIGLOOKUPOFF
  return cos(x);
#endif
  setuptriglookup();
  float y=mymod(x,2*pi);
  return coslookup[(int)(y*triglookupquant/2/pi)];
}

bool boolrnd() {
  return (myrnd()>0.5);
}

float floatneg(float f) {
  return -f;
}

bool boolneg(bool b) {
  return !b;
}

float floatmult(float a,float b) {
  return a*b;
}

int wrap(int x,int a,int b) {
//  printf("Wrapping %i in (%i-%i) to %i\n",x,a,b,intmod2(x,a,b));
  return intmod2(x,a,b);
}

int wrap(int x,int b) {
  return wrap(x,1,b);
}

float floatdist(float x,float y) {
  return diff(x,y);
}

float pull(float a,float t,float b) {
  return a*(1.0-t)+b*t;
}

float oldpull(float a,float b,float t) {
  return pull(a,t,b);
}

float tofloat(float f) {
  return f;
}

float tofloat(int f) {
  return f;
}

/* float tofloat(double f) {
  return f;
} */

/* Causes problems, eg. map2d<bool> only writes 0s!
template<class Obj>
float tofloat(Obj f) {
  return 0.0;
}*/

float ramp(float f) {
  if (f<0)
    return 0;
  if (f>1)
    return 1;
  return f;
}

bool even(int i) {
  return ((abs(i) % 2)==0);
}

bool odd(int i) {
  return (!even(i));
}

bool alternate(int i) {
  if (i>=0)
    return even(i);
  else
    return odd(i);
}

bool alternate(float d,float r) {
  int i=d/r;
  if (d>=0)
    return even(i);
  else
    return odd(i);
}

#ifndef STUPIDUNIX
// void wait(float secs) {
  // int t=time(NULL);
  // while (time(NULL)<t+secs) { };
// }
void wait(time_t secs) {
  time_t t=time(NULL);
  while (time(NULL)<t+secs) { };
}
#endif

float corrave(List<float> xs) {
  float s=0;
  for (int i=1;i<=xs.len;i++)
    s+=xs.num(i);
  return s/(float)xs.len;
}

float corrS(List<float> xs,List<float> ys) {
  float xa=corrave(xs);
  float ya=corrave(ys);
  float s=0;
  for (int i=1;i<=xs.len;i++)
    s+=(xs.num(i)-xa)*(ys.num(i)-ya);
  return s/(float)xs.len;
}

float correlation(List<float> xs,List<float> ys) {
  return corrS(xs,ys)/sqrt(corrS(xs,xs)*corrS(ys,ys));
}

float lscorrelation(List<float> xs,List<float> ys) {
  return corrS(xs,ys)/sqrt(corrS(xs,xs)*corrS(ys,ys));
}

float correlation(List<Pixel> *ps) {
	List<float> xs,ys;
	for (int i=1;i<=ps->len;i++) {
		xs.add(ps->num(i).x);
		ys.add(ps->num(i).y);
	}
	float c=correlation(xs,ys);
	xs.freedom(); ys.freedom();
	return c;
}

//template <class Obj>
//extern class Map2d;

// Class declaration of existence: Line2d


// Starts of class Correlator


	// Variable declared in .h file
	// Variable declared in .h file
	// Variable declared in .h file
	// Variable declared in .h file
	// Variable declared in .h file
	// Variable declared in .h file
	// Variable declared in .h file
	 Correlator::Correlator() {
	  cxy=cxx=cyy=ang=NULL;
	}
	// I have had to implement drawCorrelator();
//	Map2d<float> draw();
	/* Skipping declared method void Correlator::add(V2d p) */

	/* Skipping declared method void Correlator::add(V2d p,float w) */

	/* Skipping declared method void Correlator::add(List<V2d> v) */

	void Correlator::add(float x,float y) {
	  add(x,y,1.0);
  }
	void Correlator::add(float x,float y,float w) {
	  xs.add(x);
	  ys.add(y);
	  ws.add(w);
  }
	 Correlator::Correlator(List<float> x,List<float> y) {
	  cxy=cxx=cyy=ang=NULL;
		xs=x;
		ys=y;
		ws=List<float>(xs.len,1.0);
	}
	 Correlator::Correlator(List<Pixel> *ps) {
	  cxy=cxx=cyy=ang=NULL;
		for (int i=1;i<=ps->len;i++) {
			add(ps->num(i).x,ps->num(i).y);
		}
	}
	float Correlator::totalweight() {
	  float t=0.0;
	  for (int i=1;i<=ws.len;i++)
	    t+=ws.num(i);
	  return t;
	}
	float Correlator::corrxy() {
	  if (cxy==NULL) {
	    cxy=new float[1];
	    *cxy=corrS(xs,ys);
	  }
	  return *cxy;
	}
	float Correlator::corrxx() {
	  if (cxx==NULL) {
	    cxx=new float[1];
	    *cxx=corrS(xs,xs);
	  }
	  return *cxx;
	}
	float Correlator::corryy() {
	  if (cyy==NULL) {
	    cyy=new float[1];
	    *cyy=corrS(ys,ys);
	  }
	  return *cyy;
	}
	float Correlator::correlation() {
		return corrxy()/sqrt(corrxx()*corryy());
	}
	float Correlator::A() { //  of y=Ax+b
		return corrxy()/corrxx();
  	float xa=corrave(xs)*(float)xs.len;
	  float ya=corrave(ys)*(float)ys.len;
	  float top=0;
	  for (int i=1;i<=xs.len;i++)
	  	top=top+xs.num(i)*ys.num(i);
	  top=top-xa*ya;
	  float bottom=0;
	  for (int i=1;i<=xs.len;i++)
	  	bottom=bottom+square(xs.num(i));
 		bottom=bottom-square(xa);
 		return top/bottom;
 }
	float Correlator::corrS(List<float> xs,List<float> ys) {
  	float xa=corrave(xs);
	  float ya=corrave(ys);
  	float s=0;
	  for (int i=1;i<=xs.len;i++)
  	  s+=(xs.num(i)-xa)*(ys.num(i)-ya);
	  return s/(float)xs.len;
	}
	void Correlator::freedom() {
		xs.freedom();
		ys.freedom();
	}

	float Correlator::sumx() {
	  float t=0;
	  for (int i=1;i<=xs.len;i++)
	    t+=xs.num(i);
	  return t;
	}
	
	float Correlator::sumy() {
	  float t=0;
	  for (int i=1;i<=ys.len;i++)
	    t+=ys.num(i);
	  return t;
	}
	
  /* Skipping declared method V2d Correlator::centroid() */

	/* Skipping declared method float Correlator::angerror(float) */

	/* Skipping declared method float Correlator::error() */

  /* Skipping declared method V2d Correlator::ori() */

  /* Skipping declared method Line2d Correlator::line() */

  /* Skipping declared method float Correlator::length() */

  /* Skipping declared method float Correlator::fatness() */

	/* Skipping declared method float Correlator::angle() */
 // In clock form I believe
	/* Skipping declared method float Correlator::crossesy() */

	/* Skipping declared method float Correlator::crossesyxoutliers() */

	/* Skipping declared method float Correlator::yforx(float x) */

	
	float Correlator::grad() {
	  float a=angle();
	  float g=cos(a)/sin(a);
	  printf("++++++++++ Got grad %f, but A %f\n",g,A());
	  return g;
	}
	
	float Correlator::bestgradxoutliers() {
	  float a=bestanglexoutliers();
	  float g=cos(a)/sin(a);
	  return g;
	}
	
	// This method is not actually appropriate because
	// it works out ordered combinations
	// making 1,2,3 and 3,2,1 different!
	List<int> Correlator::decompose(int n,int excl) {
	  printf("De %i (%i,%i): ",n,excl,xs.len);
	  List<int> ls;
	  for (int i=0;i<excl;i++) {
	    int a=(n % (xs.len-i))+1;
	    for (int j=1;j<=ls.len;j++)
	      if (a>=ls.num(j))
	        a++;
	    ls.add(a);
	    printf("%i ",a);
	    n=n/(xs.len-i);
	  }
	  printf("\n");
	  return ls;
	}
	
	float Correlator::bestanglexoutliers() {
	  return bestanglexoutliers(xs.len/4);
	}
	
	/* Skipping declared method float Correlator::bestanglexoutliers(int k) */


// End class 


		

String floattoString(float f) {
  return Sformat("%f",f);
}

int max(int a,int b) {
  if (a>b)
    return a;
  else
    return b;
}

// a^b=c
// b=log c / log a
float depower(float c,float a) {
  float b=log(c)/log(a);
  return b;
}

float invert(float f) {
  return -f;
}

/*float invert(bool b) {
  return !b;
}*/

bool angleless(float a,float b) {
  float d=b-a;
  d=mymod2(d,-pi,pi);
//  printf("Angles a=%.2f + %.2f = %.2f=b, where firstless=%i\n",a,d,b,(int)(d>0));
  return (d>0);
}

template<class Object>
Object *p2newtemplate(Object o) {
  Object* n=new Object[1];
  n[0]=o;
  return n;
}

float *p2newfloat(float f) {
  return p2newtemplate(f);
}

// ang and bang may not be more than 2pi apart
float anglebetween(float ang,float bang) {
    return mymod2(bang-ang,-pi,+pi);
}

int factorial(int n) {
  if (n<=1)
    return 1;
  else
    return n*factorial(n-1);
}

#endif
