#include <stdio.h>
#include <math.h>

/* class V2d;
   
   V2d operator*(float d,V2d v);
   V2d operator*(V2d v,float d);
   V2d operator/(V2d v,float d);*/

class V2d /*: public Displayable*/ {
public:
  float x,y;
  
  V2d() {
  }
  
  V2d(float u,float v) {
    x=u;
    y=v;
  }

  bool operator==(V2d o) {
    return (x==o.x && y==o.y);
  }
  
  V2d operator+(V2d v) {
    return V2d(x+v.x,y+v.y);
  }
  
  V2d operator-(V2d v) {
    return V2d(x-v.x,y-v.y);
  }
  
  void print() {
    printf("(%f,%f)",x,y); 
  }
  
  String str() {
    return myformat("(%f,%f)",x,y);
  }
  
  static void print(V2d v) {
    v.print(); 
  }
  
  
  static V2d add(V2d u, V2d v) {
    return V2d(u.x+v.x,u.y+v.y); 
  }
  
  
  static V2d mult(float a,V2d v) {
    return V2d(a*v.x,a*v.y); 
  }
  
  
  V2d neg() {
    return V2d::mult(-1,*this); 
  }
  
  static V2d neg(V2d v) {
    return v.neg(); 
  }
  
  
  static V2d sub(V2d u, V2d v) {
    return V2d::add(u,v.neg()); 
  }
  
  
  static float mod(V2d v) {
    return v.mod(); 
  }
  
  float mod() {
    return sqrt(x*x+y*y); 
  }
  
  
  static float radius(V2d v) {
    return V2d::mod(v); 
  }
  
  
  static V2d norm(V2d v) {
    return V2d::mult(1/mod(v),v); 
  }
  
  V2d norm() {
    return V2d::norm(*this);
  }
  
  void normalise() {
    changeto(norm());
  }
  
  static float dist(V2d u,V2d v) {
    return V2d::mod(V2d::sub(u,v));
  }
  
  static int equal(V2d u,V2d v) {
    
    if (V2d::mod(V2d::sub(u,v))<0.001)
      return 1;
    else
    return 0;
  }
  
  static float dot(V2d u,V2d v) {
    return u.x*v.x+u.y*v.y;
  }
  
  static float normdot(V2d u,V2d v) {
    return V2d::dot(u.norm(),v.norm());
  }
  
  static V2d rotate(V2d v,float ang) {
    return V2d(v.x*cos(ang)+v.y*sin(ang),-v.x*sin(ang)+v.y*cos(ang));
  }
  
  V2d rotate(float ang) {
    changeto(V2d::rotate(*this,ang));
  }
  
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
  
  static V2d random() {
    float a=myrnd()*2*pi;
    return rotate(V2d(0,1),a);
  }
  
  void changeto(V2d v) {
    x=v.x; y=v.y;
  }
  
  float var(int i) {
    switch (i) {
      case 0 : return x;
      case 1 : return y;
      default: return 0;
    }
  }
  
  float getlongestside() {
    if (x>y)
      return x;
    else
    return y;
  }
  
  void chop(float low,float high) {
    if (x<low)
      x=low;
    if (x>high)
      x=high;
    if (y<low)
      y=low;
    if (y>high)
      y=high;
  }

};

V2d operator*(float d,V2d v) {
  return V2d(d*v.x,d*v.y);
}

V2d operator*(V2d v,float d) {
  return V2d(d*v.x,d*v.y);
}

V2d operator/(V2d v,float d) {
  return V2d(v.x/d,v.y/d);
}

List<V2d> v2dsincircle(int cx,int cy,int rad) {
  List<V2d> l;
  for (int x=-rad;x<=rad;x++) {
    int h=(int)sqrt(rad*rad-x*x);
    for (int y=-h;y<=h;y++) {
      V2d p=V2d(cx+x,cy+y);
      l+p;
    }
  }
  return l;
}





                                                                                                 
                                                                                                 
