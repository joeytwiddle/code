#include <stdio.h>
#include <math.h>

/* class V2d;
   
   V2d operator*(float d,V2d v);
   V2d operator*(V2d v,float d);
   V2d operator/(V2d v,float d);*/
				/*: public Displayable*/

class V2d {
public:
  float x,y; // Exists
  
   V2d(); // Method

  
   V2d(float u,float v); // Method


  bool operator==(V2d o); // Method

  
  V2d operator+(V2d v); // Method

  
  V2d operator-(V2d v); // Method

  
  void print(); // Method

  
  String str(); // Method

  
  static void print(V2d v); // Method

  
  
  static V2d add(V2d u, V2d v); // Method

  
  
  static V2d mult(float a,V2d v); // Method

  
  
  V2d neg(); // Method

  
  static V2d neg(V2d v); // Method

  
  
  static V2d sub(V2d u, V2d v); // Method

  
  
  static float mod(V2d v); // Method

  
  float mod(); // Method

  
  
  static float radius(V2d v); // Method

  
  
  static V2d norm(V2d v); // Method

  
  V2d norm(); // Method

  
  void normalise(); // Method

  
  static float dist(V2d u,V2d v); // Method

  
  static int equal(V2d u,V2d v); // Method

  
  static float dot(V2d u,V2d v); // Method

  
  static float normdot(V2d u,V2d v); // Method

  
  static V2d rotate(V2d v,float ang); // Method

  
  V2d rotate(float ang); // Method

  
  /* static V2d rotatelista(List<V2d> l,V2d axis,float ang) {
       MATRIX_f m;
       get_vector_rotation_matrix_f(&m,(fix)axis.x,(fix)axis.y,(fix)axis.z,(fix)(256*mymod(ang/2/pi)));
       // fix x,y,z;
       for (int i=1;i<=l.length();i++) {
         V2d* v=l.p2num(i);
         // apply_matrix(&m,(fix)v->x,(fix)v->y,(fix)v->z,&(x),&(y),&(z));
         // v->x=x; v->y=y; v->z=z;
         // v->changeto(V2d(x,y,z));
         apply_matrix_f(&m,v->x,v->y,v->z,&v->x,&v->y,&v->z);
       }
     }
     
     static V2d rotatelist(List<V2d> l,V2d axis,float ang) {
       Matrix m=Matrix();
       m.makerotation(axis,ang);
       for (int i=1;i<=l.length();i++) {
         V2d* v=l.p2num(i);
         v->changeto(m*(*v));
       }
     }*/
  
  static V2d random(); // Method

  
  void changeto(V2d v); // Method

  
  float var(int i); // Method

  
  float getlongestside(); // Method

  
  void chop(float low,float high); // Method


};

V2d operator*(float d,V2d v); // Method


V2d operator*(V2d v,float d); // Method


V2d operator/(V2d v,float d); // Method


List<V2d> v2dsincircle(int cx,int cy,int rad); // Method






                                                                                                 
                                                                                                 
