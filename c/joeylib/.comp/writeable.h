#ifndef writeable_H
  #define writeable_H

  #include <myrgb.h>
  #include <line2d.h>

class Writeable {
public:
  int width,height; // Exists

  virtual void setpos(int i,int j,int c); // Method

  virtual void setpos(int i,int j,myRGB c); // Method

/*  template <class CT>
  void setpos(int x,int y,CT c) {
    error("Writeable::setpos(x,y,c) Must be overwritten!");
  }*/
  template <class CT>
  void setpos(Pixel p,CT c); // Method

  template <class CT>
  void setpixel(int x,int y,CT c); // Method

  template <class CT>
  void setpixel(Pixel p,CT c); // Method


  template <class CT>
  void line(int x1, int y1, int x2, int y2,CT r); // Method

  template <class CT>
  void line(Pixel a,Pixel b,CT c); // Method

  template <class CT>
  void line(V2d a,V2d b,CT c); // Method


  template <class CT>
  void cross(V2d cen,int s,CT c); // Method


  template <class CT>
  void thickcross(V2d cen,int s,float wid,CT c); // Method


  template <class CT>
  void vline(int x,int ya,int yb,CT c); // Method


  template <class CT> // Silly! Should keep w with a and b!
  void thickline(V2d a,V2d b,CT r,float w); // Method


  template <class CT>
  void thicklineh(V2d a, V2d b, float w, CT c); // Method

  template <class CT>
  void thicklinev(V2d a, V2d b, float w, CT c); // Method

  template <class CT>
  void rthickline(V2d a, V2d b, float w, CT c); // Method

  template <class CT>
  void sthickline(V2d a, V2d b, float w, CT c); // Method

  template <class CT>
  void filltri(V2d a, V2d b, V2d c, CT col); // Method

  template <class CT>
  void filltri(int xa,int ya,int xb,int yb,int xc,int yc,CT c); // Method


  template <class CT>
  void filledcircle(V2d cen,float r,CT c); // Method


  template <class CT>
  void filledcircle(int cx,int cy,float r,CT c); // Method


template <class CT>
void opencircle(int x,int y,float r,CT c); // Method




//  virtual void opencircle(Pixel p,int r,

/*  template <class CT>
  virtual void filledcircle(int cx,int cy,float r,CT c) {
    printf("Filling circle %i %i %f (%f) %i %i\n",cx,cy,r,c,width,height);
    int x,h,y,px,py;
    for (x=-r;x<=r;x++) {
      h=(int)sqrt(mysquare(r)-mysquare(x));
      for (y=-h;y<=h;y++) {
        px=cx+x; py=cy+y;
        if (px>=0 && px<width && py>=0 && py<height)
          setpos(Pixel(px,py),c);
      }
    }
  }
  template <class CT>
  virtual void filledcircle (V2d v, float f, CT c) {
    this->filledcircle((int)v.x,(int)v.y,f,r);;
  }*/

  virtual void arrow(Pixel a,Pixel b,myRGB c); // Method


  virtual void plotandfree(List<Pixel> ps,int c); // Method


  virtual void plotandfree(List<Pixel> ps,myRGB c); // Method


  template<class ColType>
  void cross(V2d cen,float s,ColType c); // Method


};

/*
void plotandfree(Writeable *w,List<Pixel> ps,int c) {
    for (int i=1;i<=ps.len;i++)
      w->setpos(ps.num(i),c);
    ps.freedom();
  }

void plotandfree(Writeable *w,List<Pixel> ps,myRGB c) {
    for (int i=1;i<=ps.len;i++)
      w->setpos(ps.num(i),c);
    ps.freedom();
  }
*/

// Keep getting error setpos not cast before write!

template<class Object>
void plotandfree(Object *w,List<Pixel> ps,int c); // Method


template<class Object>
void plotandfree(Object *w,List<Pixel> ps,myRGB c); // Method


#endif
