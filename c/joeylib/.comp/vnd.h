#ifndef vnd_H
  #define vnd_H

  #include <strings.h>

// #define DEBUG

extern int Vcount; // Variable initialised in .c file

// Template class appears in original format:
template<class Object>
class Vector {
public:
  int dim;
  Object *ds;
  Vector<Object>() {
  }
  Vector<Object>(const Vector<Object> &v) {
    dim=v.dim;
//    printf(">");
    ds=new Object[dim];
    printf(">\n");
    for (int i=0;i<dim;i++)
      ds[i]=v.ds[i];
    #ifdef DEBUG
    Vcount++;
    printf("%i Vnds\n",Vcount);
    #endif
//    printf("%i.",Vcount);
  }
  Vector<Object>(int ss) {
    dim=ss;
//    printf("V+\n");
    ds=new Object[dim];
//    printf("V++\n");
    for (int i=0;i<dim;i++)
      ds[i]=0;
//    printf("V-\n");
  }
/*  Vector<Object>(int ss, ... ) {
    dim=ss;
    ds=new Object[dim];
    va_list ap;
    va_start(ap, count);
    for (int i=0;i<ss;i++)
      ds[i]=va_arg(ap,Object);
  }*/
  Vector<Object> operator+(Vector<Object> o) {
    // Note: I think the o here is a new copy of the original passed,
    // and should be destroyed
    #ifdef DEBUG
      printf("s");
    #endif
    Vector<Object> n=Vector<Object>(dim);
    for (int i=0;i<dim;i++)
      n.ds[i]=ds[i]+o.ds[i];
    o.freedom();
    return n;
  }
  Vector<Object> operator+(Vector<Object> *o) {
    Vector<Object> n=Vector<Object>(dim);
    for (int i=0;i<dim;i++)
      n.ds[i]=ds[i]+o->ds[i];
    return n;
  }
  Vector<Object> operator*(float f) {
    Vector<Object> n=Vector<Object>(dim);
    for (int i=0;i<dim;i++)
      n.ds[i]=ds[i]*f;
    return n;
  }
  Vector<Object> operator/(float f) {
    Vector<Object> n=Vector<Object>(dim);
    for (int i=0;i<dim;i++)
      n.ds[i]=ds[i]/f;
    return n;
  }
  void setaxis(int a,Object o) {
//    printf("Setting axis %i / %i\n",a,dim);
    if (a<1 || a>dim)
      error("Tried to set axis %i of vector of %i dimensions",a,dim);
    if (ds==NULL)
      error("Vnd: Not dimmed.");
    ds[a-1]=o;
//    printf("Done.\n");
  }
  Object getaxis(int a) {
    if (a<1 || a>dim)
      error("Tried to get axis %i from vector of %i dimenstions",a,dim);
    return ds[a-1];
  }
  String toString() {
    String s="(";
    for (int i=1;i<=dim;i++) {
      s=Sconc(s,",",floattoString(ds[i-1]));
    }
    s=Sconc(s,")");
    return Seat(s,2,1);
  }
  static float dist(Vector<Object> a,Vector<Object> b) {
    float x=0;
    for (int i=0;i<a.dim;i++)
      x+=mysquare(a.ds[i]-b.ds[i]);
    a.freedom();
    b.freedom();
    return sqrt(x);
  }
  static float dist(Vector<Object> *a,Vector<Object> *b) {
    float x=0;
    for (int i=0;i<a->dim;i++)
      x+=mysquare(a->ds[i]-b->ds[i]);
    return sqrt(x);
  }
  void freedom() {
    /* Breaks
    for (int i=dim-1;i>=0;i--)
      free(&ds[i]);
    */
    free(ds);
    #ifdef DEBUG
    printf("!");
    #endif
    Vcount--;
  }
};// End template class

         
template<class Object>
float tofloat(Vector<Object> v); // Method


#define Vnd Vector<float>

#endif
