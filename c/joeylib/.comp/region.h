#ifndef region_H
  #define region_H

// make() functions use memoing
// remake() will re-memo

class Region {
public:
  // Size of image, coordinates of window
  int width,height,left,right,top,bottom; // Exists
  // Can be represented in different forms:
  Map2d<bool> *map; // Exists
  List<Pixel> *list; // Exists
  Correlator *cor; // Exists
  List<Pixel> *boundary; // Exists
  void destroyallbut(void *n); // Method

  void destroyall(); // Method

  void destroyallbutmap(); // Method

  void destroyallbutlist(); // Method

   Region(); // Method

   Region(int w,int h); // Method

   Region(Polygon2d p,int w,int h); // Method

   Region(Map2d<bool> *m); // Method

   Region(List<Pixel> *l); // Method

   Region(List<V2d> l); // Method

  void clear(); // Method

  float listleftmost(); // Method

  float listrightmost(); // Method

  float listtopmost(); // Method

  float listbottommost(); // Method

  float leftmost(); // Method

  float rightmost(); // Method

  float topmost(); // Method

  float bottommost(); // Method

  static Region *listready(); // Method

  List<Region *> *oldsplit(); // Method

  List<Region *> *split(); // Method

  void add(Pixel v); // Method

  void makelist(); // Method

  void remakelist(); // Method

  void remakemap(int width,int height); // Method

  Map2d<bool> *getmap(int width,int height); // Method

  Map2d<bool> *getmap(); // Method

  void makemap(int width,int height); // Method

  void makemap(); // Method

  List<Pixel> *getlist(); // Method

  void freedom(); // Method

  void getsize(); // Method

        
  void scaleto(int w,int h); // Method


  Pixel centreofmass(); // Method


  void contract(int r); // Method


  Region contracted(int r); // Method


  Region expanded(int r); // Method


  void newlist(); // Method


  void singleexpansion(); // Method


  List<int> checkexpansion(Map2d<int> *regmap); // Method


  List<int> checkexpansion(Map2d<int> *regmap,int col); // Method


  int area(); // Method


  V2d centroid(); // Method


  float correlation(); // Method

  void makecorrelator(); // Method

  void remakecorrelator(); // Method

  Correlator *getcor(); // Method

  //

  float error(float a); // Method


  float angle(); // Method


//

  V2d ori(); // Method


  float blength(); // Method


  float bheight(); // Method


  Line2d endtoend(); // Method


  Line2d endtoend(V2d ori); // Method


  Line2d endtoendvp(V2d vp); // Method


  Line2d line(); // Method


  void makeboundary(); // Method

  Pixel dir(int i); // Method

  void remakeboundary(); // Method

  List<Pixel> *getboundary(); // Method


};

  /* Skipping later :: bool Polygon2d::overlaps(Polygon2d *p); // Method
*/







#endif
