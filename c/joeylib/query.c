template <class Object>
class Query {
public:
  Object o;
  float (*preop)(Object);
  float op;
  Object def(Object x) {
    return x;
  }
  Query(float oo) {
    preop=&def;
//    o=NULL;
    op=oo;
  }
  Query(Object oo) {
    preop=&def;
    o=oo;
    op=preop(o);
  }
  Query(Object oo,float (*p)(Object)) {
    preop=p;
    o=oo;
    op=preop(o);
  }
  Query(float oo,float (*p)(Object)) {
    preop=&def;
//    o=NULL;
    op=oo;
  }
  bool greaterthan(Object x) {
    return (preop(x)>op);
  }
  bool lessthan(Object x) {
    return (preop(x)<op);
  }
};
