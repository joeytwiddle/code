#ifndef polygon2d_H
  #define polygon2d_H

 class Region;

class Polygon2d {
public:

  fillin bool container; // Exists
  fillin bool contained; // Exists
  // static bool container;
  // static bool contained;

  List<V2d> vs; // Exists

  Region *regionised; // Exists
  Region *smallreg; // Exists
  float score; // Exists

  String name; // Exists

   Polygon2d(); // Method


   Polygon2d(List<V2d> v); // Method


/*  static Polygon2d (Polygon2d *o,int numps, ... ) {
    va_list ap;
    va_start(ap, numps);
    vs=List<V2d>();
    for (int i = 0; i < numps; i++)
      vs.add(o->vs.num(va_arg(ap, int)));
    va_end(ap);
  }*/

/*  Polygon2d(int numps, ... ) { // Doesn't work!
    va_list ap;
    va_start(ap, numps);
    vs=List<V2d>();
    for (int i = 0; i < numps; i++) {
      V2d tmp=va_arg(ap,V2d);
//      tmp.print();
      vs.add(tmp);
    }
    va_end(ap);
  }*/

/*  Polygon2d(Polygon2d *o,int a,int b) {
    vs=List<V2d>();
    bool finished=false;
    for (int i=a;!finished;i=wrap(i+1,1,o->vs.len)) {
//      printf("%i ",i);
      vs.add(o->vs.num(i));
      if (i==b)
        finished=true;
    }
  }*/

  Polygon2d maketri(int a,int b,int c); // Method


  Polygon2d subpath(int a,int b); // Method


  static float area(Polygon2d p); // Method


  float area(); // Method

  List<Polygon2d> splittotris(); // Method

    
  String toString(); // Method

  bool contains(V2d v); // Method


  static Polygon2d fromlist(List<V2d> l); // Method


  void scale(float f); // Method


  Line2d linefrom(int i); // Method


  V2d num(int i); // Method


  bool crosses(Line2d l); // Method


  bool overlaps(Polygon2d p); // Method


  virtual bool overlaps(Polygon2d *p); // Method


  bool overlaps(Polygon2d *p,float lenience); // Method


  Polygon2d scaleaboutcentroid(float scale); // Method


  V2d centroid(); // Method


  void add(V2d v); // Method


  void freedom(); // Method


  Polygon2d operator*(float f); // Method


};                  

// fillup extern bool Polygon2d::container; // Variable initialised in .c file
// fillup extern bool Polygon2d::contained; // Variable initialised in .c file

#endif
