#ifndef jbmp_H
  #define jbmp_H

// #define pi 3.1415926535897932385

int smileymouthpolarfn(float t,float r); // Method


int heartpolarfn(float t,float r); // Method


int flowerpolarfn(float t,float r); // Method


//


int starpolarfn(float th,float r); // Method


class JBmp : public Writeable {
public:
  unsigned char **bmp; // Exists
  unsigned char **spare; // Exists
  int width,height; // Exists
   JBmp(); // Method

  void setupgrid(int w,int h); // Method

   JBmp(int w,int h); // Method

  void clear(int c); // Method

   JBmp(int w,int h,int c); // Method

  void clear(); // Method

  void blit(); // Method

  void writetoscreen(); // Method

  void display(); // Method

  //

  void filledcircle(int cx,int cy,float r,int c); // Method

  void filledcircle(V2d cen,float r,int c); // Method

  void filledcirclenodarker(int cx,int cy,int r,int c); // Method

  void shadedcircle(int cx,int cy,int r,int inc,int outc); // Method

  void shadedcirclenodarker(int cx,int cy,int r,int inc,int outc); // Method

  void addcircle(int cx,int cy,int r,int c); // Method

  // r = .5 + 2 * t / pi + .5 * SIN(1.5 * t)
  void filledpolar(int (*fn)(float,float),int cx,int cy,int r,float rot,int c); // Method

  void flower(int cx,int cy,int r,float rot,int cp,int cc); // Method

  void smiley(int cx,int cy,int r,float rot,int c); // Method

  void incpixel(int x,int y,int n); // Method

  bool inbmp(int x,int y); // Method

  bool inimage(int x,int y); // Method

  void incpixel(int x,int y); // Method

  void setpixel(int x,int y,int c); // Method

  void setpixelnc(int x,int y,int c); // Method

  void setpixelud(int x,int y,int c); // Method

  void setpixelnodarker(int x,int y,int c); // Method

  void addtopixel(int x,int y,int c); // Method

  int pointat(int x,int y); // Method

  void setrectangle(int l,int t,int w,int h,int c); // Method

  void hliner(int x,int y,int dx,int c); // Method

  void hline(int x,int y,int xb,int c); // Method

  void vliner(int x,int y,int dy,int c); // Method

  void vline(int x,int y,int yb,int c); // Method

  void line(int x1,int y1,int x2,int y2,int c); // Method

  void line(V2d a,V2d b,int c); // Method

  void line(Line2d l,int c); // Method

  void line(Line2d *l,int c); // Method

  void sthickline(V2d a,V2d b,float w,int c); // Method

  void thickline(V2d a,V2d b,float w,int c); // Method

  void thickline(int xa,int ya,int xb,int yb,float w,int c); // Method

  void thickline(Line2d l,int w,int c); // Method

  void thicklinev(V2d a,V2d b,float w,int c); // Method

  void thicklineh(V2d a,V2d b,float w,int c); // Method

  void rthickline(V2d a,V2d b,float w,int c); // Method

  void liner(V2d a,V2d b,int c); // Method

  int point(int x,int y); // Method

  void interpolate(int l,int t,int r,int b,int nwcol,int necol,int swcol,int secol); // Method


  Map2d<uchar> tomap(); // Method


  void writefile(String fname); // Method

  void intvline(int x,int ya,int yb,int ca,int cb); // Method

  void intertri(V2d a,V2d b,V2d c,int ca,int cb,int cc); // Method

  void intertri(int xa,int ya,int xb,int yb,int xc,int yc,int ca,int cb,int cc); // Method

  void filltri(int xa,int ya,int xb,int yb,int xc,int yc,int c); // Method

//

  void filltri(V2d a,V2d b,V2d c,int d); // Method

  void fillpoly(Polygon2d p,int c); // Method

  
  //

void fillquad(int xa,int ya,int xb,int yb,int xc,int yc,int xd,int yd,int c); // Method

void fillquad(V2d a,V2d b,V2d c,V2d d,int z); // Method

void writeframe(); // Method

void saveframe(); // Method

void fadeby(int c); // Method

void fillstar(int x,int y,int r,float dth,int c); // Method

void opencircle(int x,int y,float r,int c); // Method

void opencircle(V2d cen,float r,int c); // Method

uchar chopbmp(int x,int y); // Method

//

void smooth(int r); // Method

int fsrad; // Exists
int *hs; // Exists
int cnt; // Exists
void setupfastsmooth(int r); // Method

void fastsmooth(); // Method

void randomfastsmooth(); // Method

bool inbmp(V2d p); // Method

};      

//


#endif
