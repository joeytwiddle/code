#ifndef rgbmp_H
  #define rgbmp_H

// See quad function at the bottom

#include <stdio.h>
#define uchar unsigned char

// too slow!
//

extern float PRdist; // Variable initialised in .c file
class PerpRecv {
public:
  V2d a,b,c,d; // Exists
  V2d vv,vh; // Exists
   PerpRecv(V2d aa,V2d bb,V2d cc,V2d dd); // Method

  V2d find(float e,float s,float l,float r,float t,float b,V2d aa,V2d bb,V2d cc,V2d dd); // Method

  V2d sourceof(float e,float s); // Method

};

class Texture {
public:
  virtual myRGB colat(V3d w); // Method

};

int gimmezero(); // Method


unsigned int getshort(FILE *fp); // Method

unsigned int getint(FILE *fp); // Method

void putshort(FILE *fp,int i); // Method

void putint(FILE *fp, int i); // Method


 class RGBmp;

RGBmp *angusreadbmp8(char *fn_image); // Method

RGBmp *angusreadbmp24(char *fn_image); // Method

void anguswritebmp24(char *fn_image,RGBmp *r); // Method

void writeppm(char *fn_image,RGBmp *r); // Method


//
  
class RGBmp : public Writeable {
public:
  int width,height; // Exists
  myRGB **bmp; // Exists

   RGBmp(); // Method

   RGBmp(int w,int h); // Method

   RGBmp(int w,int h,myRGB r); // Method


  myRGB getpos(int x,int y); // Method


  myRGB getpos(Pixel p); // Method


  void make(int w,int h); // Method


  void applygreyscale(); // Method

  
  void applycontrast(float f); // Method

  
  void applybrightness(float f); // Method

  
  Map3d<int> *houghtransform(int angquant,int xquant,int yquant,int spacerad); // Method

  
  Map2d<float> *getedges(int maskrad); // Method

  
  void setrectangle(int left,int top,int wid,int hei,myRGB r); // Method

  
  RGBmp *smoothed(int rad); // Method

  
  int inimage(int x, int y); // Method

  
  
  void thickline(Line2d l,myRGB r,float w); // Method

  void thickline(V2d a,V2d b,myRGB r,float w); // Method


  void thicklineinf(V2d a,V2d b,float w,myRGB r); // Method


  void filltri(V2d a,V2d b,V2d c,myRGB r); // Method


  void line(Line2d l,myRGB r); // Method

  void line(V2d a,V2d b,V3d v); // Method

  void line(V2d a,V2d b,myRGB r); // Method

  void line(Pixel a,Pixel b,myRGB r); // Method

  void line(int x1, int y1, int x2, int y2,myRGB *r); // Method

  
  
  //
  
  boolean valid(int x,int y,Map2d<int> *chosen,myRGB *r,float tol); // Method

  
  void explore(int x,int y,List<List<Pixel> > *l,Map2d<int> *chosen,int c,myRGB *r,float tol,int d); // Method

  
  List<List<Pixel> > *getfilledregions(float tolerance); // Method

  
  
  List<List<Pixel> > *oldclassifyscattered(myRGB preferred,float tolerance,int scatrad); // Method

  
  
  
  
  List<List<Pixel> > *classifyallscattered(float tolerance,int scatrad); // Method

  
  
  
  
  
  
  List<List<Pixel> > *classifyscattered(myRGB preferred,float tolerance,int scatrad); // Method

  
  
  
  
  
  
  
  
  
  
  
  void display(); // Method


  void display(int winwid,int winhei); // Method

  void writetoscreen(); // Method

  
  //
  
  // attempt for DOS
   RGBmp(String fname); // Method


//    error("RGbmp(filename) deprecated");

//
  
  void writebmp24(char *fname); // Method

  
  static RGBmp *readfile(String s); // Method

  
  static RGBmp *readppm(String filename); // Method

  
  static RGBmp *readbmp(String fname); // Method

  
  void writefile(String fname); // Method

  
  Map2d<float> *getgreyscale(); // Method

  Map2d<V3d> *getv3ds(); // Method

  
  Map2d<RGBHistogram> *getlocalhistmap(int histrad,float scale); // Method

  

  static RGBmp *toRGBmp(Map2d<float> *m); // Method


  template <class Obj>
  static RGBmp *toRGBmp(Map2d<Obj> *m); // Method



  void setupgrid(Map2d<V2d> *grid,int left,int top,int size,V2d hvp,V2d vvp); // Method

  // This method is accurate excepting estimate of aspect ratio
  RGBmp *recoverquadrilateral(V2d a,V2d b,V2d c,V2d d,int imwidth); // Method




//

    myRGB getinter(float x,float y); // Method



RGBmp *recoverquad(V2d a,V2d b,V2d c,V2d d,int imwidth); // Method


//

  RGBmp *linearrecoverquad(List<V2d> *ls,float scale,int imwid); // Method


  // The 3d quad recovery bodge
  // I think the maths works, but it's not confirmed.
  // The results are certainly comparable to those from recoverquadrilateral
  RGBmp *recoverquad(List<V2d> *ls,float scale,int imwid); // Method


  void setpos(int x,int y,V3d c); // Method


  void setpos(int x,int y,myRGB c); // Method


  void setpixel(int x,int y,V3d c); // Method


  void setpixel(int x,int y,myRGB c); // Method


  void setpixel(Pixel p,myRGB c); // Method


  void setpos(Pixel p,myRGB c); // Method


  void intvline(int x,int ya,int yb,V3d ca,V3d cb); // Method


  void intvline(int x,int ya,int yb,V3d ca,V3d cb,Texture *t); // Method


  void intertri(int xa,int ya,int xb,int yb,int xc,int yc,V3d ca,V3d cb,V3d cc,Texture *t); // Method



  void intertri(int xa,int ya,int xb,int yb,int xc,int yc,myRGB ca,myRGB cb,myRGB cc); // Method


  void intertri(V2d a,V2d b,V2d c,myRGB ca,myRGB cb,myRGB cc); // Method


  void intertri(int xa,int ya,int xb,int yb,int xc,int yc,V3d ca,V3d cb,V3d cc); // Method


  void clear(); // Method


  void clear(myRGB c); // Method


  void clear(V3d c); // Method


  bool inbmp(int x,int y); // Method


  RGBmp *scaletowidth(int w); // Method


  RGBmp *scaledby(float s); // Method


  myRGB getposinterpolate(float x,float y); // Method



  static RGBmp *scale(RGBmp *in,float scale); // Method


  RGBmp *inverted(); // Method


  static RGBmp *fromv3ds(Map2d<V3d> *m); // Method


  void line(Line2d *l,myRGB c); // Method

  void sthickline(V2d a,V2d b,float w,myRGB c); // Method

  void thickline(V2d a,V2d b,float w,myRGB c); // Method

  void thickline(int xa,int ya,int xb,int yb,float w,myRGB c); // Method

  void thickline(Line2d l,int w,myRGB c); // Method

  void thicklinev(V2d a,V2d b,float w,myRGB c); // Method

  void thicklineh(V2d a,V2d b,float w,myRGB c); // Method

  void rthickline(V2d a,V2d b,float w,myRGB c); // Method

void opencircle(Pixel p,float r,myRGB c); // Method

void opencircle(V2d v,float r,myRGB c); // Method

void opencircle(int x,int y,float r,myRGB c); // Method

  void filledcircle(int cx,int cy,float r,myRGB c); // Method

  void cross(V2d cen,float s,myRGB c); // Method

  void cross(float x,float y,float s,myRGB c); // Method

  void filledcircle(V2d pos,float r,myRGB c); // Method

  RGBmp *applyfn(myRGB fn(myRGB)); // Method

  
  void freedom(); // Method

  
  myRGB greyify(myRGB r); // Method

  
  void greyify(); // Method
    

};

//

// #include <writeppm.c>

//





V2d rectangleproject(V3d w,V3d eye); // Method


V2d vvpfromhvpandbaseline(V2d hvp,Line2d baseline,V3d eye,RGBmp origimage); // Method


//

List<V3d> rectanglefromquadrilateral(List<V2d> v2ds,V3d eye); // Method



List<V3d> rectanglefromquadrilateralgroovy(List<V2d> v2ds,V3d eye); // Method


#endif
