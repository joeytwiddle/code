#ifndef writeable_H
  #define writeable_H

class Writeable {
public:
  int width,height; // Exists

  virtual void setpos(int i,int j,int c); // Method

  virtual void setpos(int i,int j,myRGB c); // Method

//

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

//


  virtual void arrow(Pixel a,Pixel b,myRGB c); // Method


  virtual void plotandfree(List<Pixel> ps,int c); // Method


  virtual void plotandfree(List<Pixel> ps,myRGB c); // Method


  template<class ColType>
  void cross(V2d cen,float s,ColType c); // Method


};

//


// Keep getting error setpos not cast before write!

template<class Object>
void plotandfree(Object *w,List<Pixel> ps,int c); // Method


template<class Object>
void plotandfree(Object *w,List<Pixel> ps,myRGB c); // Method


#endif
