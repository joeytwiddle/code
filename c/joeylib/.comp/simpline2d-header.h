class Line2d {
public:
  V2d a,b; // Exists
  float length; // Exists
   Line2d(); // Method

   Line2d(V2d aa,V2d bb); // Method

  void swapends(); // Method

  V2d *findintersection(Line2d o); // Method

};

class Polygon2d {
public:
  List<V2d> vs; // Exists
   Polygon2d(List<V2d> v); // Method

   Polygon2d(Polygon2d *o,int numps, ... ); // Method

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
   Polygon2d(Polygon2d *o,int a,int b); // Method

  Polygon2d maketri(int a,int b,int c); // Method

  float area(); // Method

};
