#ifndef tree_H
  #define tree_H

// Template class appears in original format:
template <class Object>
class Tree {
public:
  Object o;
  List<Tree> ts;
  Tree(Object oo) {
    o=oo;
  }
  Tree(List<Tree> tt) {
    ts=tt;
  }
  Tree(Object oo,List<Tree> tt) {
    o=oo;
    ts=tt;
  }
  Tree(List<Tree> tt,Object oo) {
    o=oo;
    ts=tt;
  }
};// End template class

#endif
