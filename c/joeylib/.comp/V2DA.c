#ifndef V2DA_C
  #define V2DA_C

  #include <V2DA.h>

#include <stdio.h>
#include <math.h>

/* class V2d;
   
   V2d operator*(float d,V2d v);
   V2d operator*(V2d v,float d);
   V2d operator/(V2d v,float d);*/
				/*: public Displayable*/

// Starts of class V2d


  // Variable declared in .h file
  
   V2d::V2d() {
  }
  
   V2d::V2d(float u,float v) {
    x=u;
    y=v;
  }

  bool V2d::operator==(V2d o) {
    return (x==o.x && y==o.y);
  }
  
  V2d V2d::operator+(V2d v) {
    return V2d(x+v.x,y+v.y);
  }
  
  V2d V2d::operator-(V2d v) {
    return V2d(x-v.x,y-v.y);
  }
  
  void V2d::print() {
    printf("(%f,%f)",x,y); 
  }
  
  String V2d::str() {
    return myformat("(%f,%f)",x,y);
  }
  
   void V2d::print(V2d v) {
    v.print(); 
  }
  
  
   V2d V2d::add(V2d u, V2d v) {
    return V2d(u.x+v.x,u.y+v.y); 
  }
  
  
   V2d V2d::mult(float a,V2d v) {
    return V2d(a*v.x,a*v.y); 
  }
  
  
  V2d V2d::neg() {
    return V2d::mult(-1,*this); 
  }
  
   V2d V2d::neg(V2d v) {
    return v.neg(); 
  }
  
  
   V2d V2d::sub(V2d u, V2d v) {
    return V2d::add(u,v.neg()); 
  }
  
  
   float V2d::mod(V2d v) {
    return v.mod(); 
  }
  
  float V2d::mod() {
    return sqrt(x*x+y*y); 
  }
  
  
   float V2d::radius(V2d v) {
    return V2d::mod(v); 
  }
  
  
   V2d V2d::norm(V2d v) {
    return V2d::mult(1/mod(v),v); 
  }
  
  V2d V2d::norm() {
    return V2d::norm(*this);
  }
  
  void V2d::normalise() {
    changeto(norm());
  }
  
   float V2d::dist(V2d u,V2d v) {
    return V2d::mod(V2d::sub(u,v));
  }
  
   int V2d::equal(V2d u,V2d v) {
    
    if (V2d::mod(V2d::sub(u,v))<0.001)
      return 1;
    else
    return 0;
  }
  
   float V2d::dot(V2d u,V2d v) {
    return u.x*v.x+u.y*v.y;
  }
  
   float V2d::normdot(V2d u,V2d v) {
    return V2d::dot(u.norm(),v.norm());
  }
  
   V2d V2d::rotate(V2d v,float ang) {
    return V2d(v.x*cos(ang)+v.y*sin(ang),-v.x*sin(ang)+v.y*cos(ang));
  }
  
  V2d V2d::rotate(float ang) {
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
  
   V2d V2d::random() {
    float a=myrnd()*2*pi;
    return rotate(V2d(0,1),a);
  }
  
  void V2d::changeto(V2d v) {
    x=v.x; y=v.y;
  }
  
  float V2d::var(int i) {
    switch (i) {
      case 0 : return x;
      case 1 : return y;
      default: return 0;
    }
  }
  
  float V2d::getlongestside() {
    if (x>y)
      return x;
    else
    return y;
  }
  
  void V2d::chop(float low,float high) {
    if (x<low)
      x=low;
    if (x>high)
      x=high;
    if (y<low)
      y=low;
    if (y>high)
      y=high;
  }

// End class 


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





                                                                                                 
                                                                                                 

#endif
