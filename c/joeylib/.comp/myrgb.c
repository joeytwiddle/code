#ifndef myrgb_C
  #define myrgb_C

  #include <myrgb.h>

// Class declaration of existence: V3d


// Starts of class myRGB


  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
   myRGB::myRGB() {
    myRGB(0,0,0);
  }
  bool myRGB::operator==(myRGB o) {
    return (r==o.r && g==o.g && b==o.b);
  }
  /*bool similar(myRGB o) {
    return ( (v3d()-o.v3d()).mag()<0.1 );
  }*/
   myRGB::myRGB(int x,int y,int z) {
    // printf("*%i %i %i    ",x,y,z);
    r=ucharchop(x); g=ucharchop(y); b=ucharchop(z);
    // if (r==255 && g==255 && b==255)
    // printf("+%i %i %i    ",r,g,b);
  }
   myRGB::myRGB(uchar x,uchar y,uchar z) {
    r=x; g=y; b=z;
    // printf("-%i %i %i    ",r,g,b);
  }
/*  myRGB(float x,float y,float z) {
    r=(uchar)(255*chop(x)); g=(uchar)(255*chop(y)); b=(uchar)(255*chop(z));
  }*/
   myRGB::myRGB(double x,double y,double z) {
    r=(uchar)(255*chop(x)); g=(uchar)(255*chop(y)); b=(uchar)(255*chop(z));
  }
  void myRGB::print() {
    printf("RGB(%f,%f,%f)\n",r/255,g/255,b/255);
  }
  /* Skipping declared method V3d myRGB::v3d() */

  void myRGB::perturb(int amount) {
    r=(uchar)chop(r+intrnd(-amount,amount),0,255);
    g=(uchar)chop(g+intrnd(-amount,amount),0,255);
    b=(uchar)chop(b+intrnd(-amount,amount),0,255);
  }
  myRGB myRGB::operator+(myRGB o) {
    return myRGB(r+o.r,g+o.g,b+o.b);
  }
  myRGB myRGB::operator/(float f) {
    return myRGB(r/f/255,g/f/255,b/f/255);
  }
  myRGB myRGB::operator-(myRGB o) {
    return myRGB(r-o.r,g-o.g,b-o.b);
  }
  float myRGB::brightness() {
    return sqrt(r*r+g*g+b*b)/255/3;
  }
   float myRGB::distance(myRGB *a,myRGB *b) {
    return sqrt(mysquare(a->r-b->r)+mysquare(a->g-b->g)+mysquare(a->b-b->b))/255/sqrt(3);
  }
   float myRGB::difference(myRGB *a,myRGB *b) {
    return sqrt(mysquare(a->r-b->r)+mysquare(a->g-b->g)+mysquare(a->b-b->b))/255/sqrt(3);
  }
   float myRGB::huefor(float t,float o) {
    float ctmp;
    ctmp=myabs(mymod(t-o));
    if (ctmp>0.5)
      ctmp=1.0-ctmp;
    // return 255*((float)0.5+0.5*cos(2*pi*ctmp));
    ctmp=ctmp*6.0;
    if (ctmp<1.0)
      return 1.0;
    if (ctmp>2)
      return 0.0;
    return 2.0-ctmp;
  }
   float myRGB::darkhuefor(float t,float o) {
    float ctmp;
    ctmp=myabs(t-o);
    if (ctmp>0.5)
      ctmp=(float)1-ctmp;
    // return 255*((float)0.5+0.5*cos(2*pi*ctmp));
    ctmp=ctmp*6.0;
    if (ctmp>1)
      return 0;
    return (float)1-ctmp;
  }
   float myRGB::darkhuefor2(float t,float o) {
    float ctmp;
    ctmp=myabs(t-o);
    if (ctmp>0.5)
      ctmp=(float)1-ctmp;
    // return 255*((float)0.5+0.5*cos(2*pi*ctmp));
    ctmp=ctmp*6;
    if (ctmp>2)
      return 0;
    return (float)2-ctmp;
  }
   myRGB myRGB::hue(float a) {
    return myRGB(huefor(a,0),huefor(a,(float)1/3),huefor(a,(float)2/3));
  }
   myRGB myRGB::darkhue(float a) {
    return myRGB(darkhuefor(a,0),darkhuefor(a,(float)1/3),darkhuefor(a,(float)2/3));
  }
   myRGB myRGB::darkhue2(float a) {
    return myRGB(darkhuefor2(a,0),darkhuefor2(a,(float)1/3),darkhuefor2(a,(float)2/3));
  }
   myRGB myRGB::random() { // Random hue, coloured dark->pure->pastel
      myRGB r=myRGB::hue(myrnd());
      if (myrnd()>0.5)
        r=r+myRGB::white*myrnd()*0.6;
      else
        r=r*(0.4+myrnd()*0.6);
    return r;
  }
  myRGB myRGB::dark() {
    return ave(myRGB::black);
  }
  myRGB myRGB::pastel() {
    return ave(myRGB::white);
  }
   myRGB myRGB::randompastel() {
    myRGB r=myRGB::hue(myrnd());
    return r.ave(myRGB::white);
  }
   myRGB myRGB::randomdark() {
    myRGB r=myRGB::hue(myrnd());
    return r.ave(myRGB::black);
  }
   myRGB myRGB::randomdarkpastel() {
    myRGB r=myRGB::hue(myrnd());
    return r.ave(myRGB::grey);
  }
  /* Skipping declared method myRGB myRGB::ave(myRGB o) */

  myRGB myRGB::operator*(float a) {
    return myRGB((int)(r*a),(int)(g*a),(int)(b*a));
  }
  myRGB myRGB::operator*(myRGB o) {
    return myRGB(r*o.r/255,g*o.g/255,b*o.b/255);
  }
  myRGB myRGB::darklight(float f) {
    if (f<0.5)
      return *this*(f*2.0);
    else {
      float g=(f-0.5)*2.0;
      return myRGB::white*g+*this*(1.0-g);
    }
  }
  String myRGB::toString() {
    return Sformat("r%i/g%i/b%i",r,g,b);
  }
// End class 


myRGB operator*(float a,myRGB r) {
  return r*a;
}

fillup myRGB myRGB::white=myRGB(1.0,1.0,1.0);
fillup myRGB myRGB::red  =myRGB(1.0,0.0,0.0);
fillup myRGB myRGB::green=myRGB(0.0,1.0,0.0);
fillup myRGB myRGB::blue =myRGB(0.0,0.0,1.0);
fillup myRGB myRGB::yellow=myRGB(1.0,1.0,0.0);
fillup myRGB myRGB::magenta=myRGB(1.0,0.0,1.0);
fillup myRGB myRGB::cyan=myRGB(0.0,1.0,1.0);
fillup myRGB myRGB::black=myRGB(0.0,0.0,0.0);
fillup myRGB myRGB::grey=myRGB(0.5,0.5,0.5);

fillup myRGB myRGB::darkred=red.dark();
fillup myRGB myRGB::darkgreen=green.dark();
fillup myRGB myRGB::darkblue=blue.dark();
fillup myRGB myRGB::darkyellow=yellow.dark();
fillup myRGB myRGB::darkmagenta=magenta.dark();
fillup myRGB myRGB::darkcyan=cyan.dark();

#endif
