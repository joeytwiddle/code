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


/* Inefficient
class RErec {
public:
  int col;
  List<int> equiv;
  RErec(int c) {
    col=c;
  }
  bool inlist(int x) {
    for (int i=1;i<=equiv.len;i++)
      if (equiv.num(i)==x)
        return true;
    return false;
  }
  int findequiv(List<Pixel> *e,int f) {
    for (int i=1;i<=e->len;i++) {
      Pixel p=e->num(i);
      if (p.x==f && !inlist(p.y)) {
        e->removenum(i);
        return p.y;
      }
      if (p.y==f && !inlist(p.x)) {
        e->removenum(i);
        return p.x;
      }
    }
    return -1;
  }
  void collect(List<Pixel> *e,List<int> *todo) {
    bool doneall=false;
    while (!doneall) {
      doneall=true;
      for (int i=1;(i<=equiv.len) && doneall;i++) {
        int c=equiv.num(i);
        // Could this class be equivalent?
        int t=findequiv(e,c);
        if (t>0) {
          doneall=false;
          equiv.add(t);
          todo->remove(t);
        }
      }
    }
  }
};

template <class Object>
Map2d<int> Map2d<Object>::getregionmap(Object oftype) {
  Map2d<int> n=Map2d<int>(width,height,0);
  List<Pixel> equiv; // Spread out equivalence
  int made=0;
//printf("A\n");
  printf("First scan\n");
  // Find all regions and make connection between joining regions
  for (int i=0;i<width;i++)
  for (int j=0;j<height;j++) {
    if (getpos(i,j)==oftype) {
      int l=n.getpos(i-1,j);
      int t=n.getpos(i,j-1);
      if (l>0 || t>0) {
        if (l>0 && t>0) {
          n.setpos(i,j,l);
          equiv.add(Pixel(t,l));
        } else if (l>0) {
          n.setpos(i,j,l);
        } else if (t>0) {
          n.setpos(i,j,t);
        }
      } else {
        made++;
        n.setpos(i,j,made);
      }
    }
  }
//printf("B\n");
  // Combine joined regions into one
  printf("Find equivalence\n");
  List<int> todo;
  for (int i=1;i<=made;i++)
    todo.add(i);
  List<RErec> realequiv;
  while (todo.len>0) {
    RErec r=RErec(realequiv.len+1);
    r.equiv.add(todo.pop(todo.len));
    r.collect(&equiv,&todo);
//    if (myrnd()<0.5)
    realequiv.add(r);
  }
  printf("Second scan\n");
  // Redraw map
  for (int i=0;i<width;i++)
  for (int j=0;j<height;j++) {
    int c=n.getpos(i,j);
    if (c>0) {
      // Which class is it?
      int found=-1;
      for (int k=1;(k<=realequiv.len) && found==-1;k++) {
        if (realequiv.num(k).inlist(c)) {
          found=k;
        }
      }
//      if (found>0)
      n.setpos(i,j,realequiv.num(found).col);
      if (found!=realequiv.num(found).col)
        error(Sformat("%i /= %i !\n",found,realequiv.num(found).col));
    }
  }
  return n;
}
*/



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
