#include <joeylib.h>

// Template class appears in original format:
template <class A,class B>
class MapElem {
public:

  A a;
  List<B> bs;

  MapElem(A aa,B bb) {
    a=aa;
    bs.add(bb);
  }

};// End template class



// Template class appears in original format:
template <class A,class B>
class Map {
public:

  List<MapElem<A,B> > els;

  Map() { }
  void put(A a,B b) {
    for (int i=0;i<els.len;i++)
      if (equal(els.get*(0)->a,a))
        els.get*(0)->add(b);

  }
  void get(A a) {
  }

};// End template class

