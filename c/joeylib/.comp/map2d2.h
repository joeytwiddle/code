#ifndef map2d2_H
  #define map2d2_H

/* Skipping later :: template <class Object>
void Map2d<Object>::realsearch(Map2d<bool> *togo,Region *r,List<Pixel> *s); // Method
*/


/* Skipping later :: template <class Object>
List<Region *> *Map2d<Object>::getrealregions(bool oftype); // Method
*/


/* Skipping later :: template <class Object>
List<Region *> *Map2d<Object>::getrealregions(); // Method
*/


//



// Efficient
class RErec {
public:
  int col; // Exists
  List<int> equiv; // Exists
   RErec(int c); // Method

  bool inlist(int x); // Method

  int findequiv(List<List<int> > *e,int f); // Method

  void collect(List<List<int> > *e,List<int> *todo); // Method

  void freedom(); // Method

};

/* Skipping later :: template <class Object>
Map2d<int> Map2d<Object>::getregionmap(Object oftype); // Method
*/


/* Skipping later :: template <class Object>
List<Region> Map2d<Object>::collectregions(); // Method
*/

#endif
