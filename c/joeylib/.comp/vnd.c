#ifndef vnd_C
  #define vnd_C

  #include <vnd.h>

// #define DEBUG

int Vcount=0;

// Template class must appear in .h file

         
template<class Object>
float tofloat(Vector<Object> v) { // Euclidean distance
  float f=0;
  for (int i=0;i<v.dim;i++) {
    f=f+mysquare(tofloat(v.ds[i]));
  }
//  printf("%f -> ",sqrt(f));
  v.freedom();
  return sqrt(f);
}

#define Vnd Vector<float>

#endif
