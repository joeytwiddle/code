#ifndef v2d_C
  #define v2d_C

  #include <v2d.h>

#include <stdio.h>
#include <math.h>

 // Class declaration of existence: V2d

   
   /* Skipping declared method V2d operator*(float d,V2d v) */

   /* Skipping declared method V2d operator*(V2d v,float d) */

   /* Skipping declared method V2d operator/(V2d v,float d) */


// Starts of class V2d


  // Variable declared in .h file

  // static V2d o,i,j;
 V2d V2d::o=V2d(0,0);
 V2d V2d::i=V2d(1,0);
 V2d V2d::j=V2d(0,1);
  
   V2d::V2d() {
  }

   V2d::V2d(const V2d &o) {
    x=o.x;
    y=o.y;
  }
  
   V2d::V2d(float u,float v) {
    x=u;
    y=v;
  }

   V2d::V2d(V2d *o) {
    x=o->x;
    y=o->y;
  }

   V2d::V2d(Pixel p) {
        x=p.x;
        y=p.y;
  }

   V2d V2d::polar(float ang,float rad) {
    V2d v=V2d::angle(ang);
//    printf("%f x %s\n",ang,v.toString());
    return rad*v;
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
  
  V2d V2d::normalised() {
    return norm();
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

  float V2d::dist(V2d o) {
    return V2d::mod(*this-o);
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

  float V2d::dot(V2d o) {
    return V2d::dot(*this,o);
  }
  
   float V2d::normdot(V2d u,V2d v) {
    return V2d::dot(u.norm(),v.norm());
  }
  
   V2d V2d::rotate(V2d v,float ang) {
    return V2d(v.x*cos(ang)+v.y*sin(ang),-v.x*sin(ang)+v.y*cos(ang));
  }
  
/*  V2d rotate(float ang) { // Changes me _and_ rotates
    changeto(V2d::rotate(*this,ang));
    return V2d::rotate(*this,ang);
  }*/

  void V2d::rotate(float ang) {
    changeto(V2d::rotate(*this,ang));
  }

  V2d V2d::rotated(float ang) {
    return V2d::rotate(*this,ang);
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

   V2d V2d::randomcircle() { // within an area - evenly?
    float a=myrnd()*2*pi;
    // circum=2.0*pi*r
    // area=pi*r*r
    // r=sqrt(area/pi);
    float area=myrnd();
    float r=sqrt(area);
    return rotate(V2d(0,r),a);
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
  
  V2d V2d::perp() { // rotated 90deg clockwise
    return V2d(y,-x);
  }
  
  String V2d::toString() {
    return Sformat("(%.2f,%.2f)",x,y);
  }

  float V2d::getx() {
    return x;
  }

  float V2d::gety() {
    return y;
  }

   float V2d::getx(V2d v) {
    return v.x;
  }

   float V2d::gety(V2d v) {
    return v.y;
  }

   V2d V2d::angle(float f) {
    return V2d(mysin(f),mycos(f));
  }

  float V2d::mag() {
    return mod();
  }

  float V2d::angle() { // 0-2pi clockwise from (0,1)=up
//    return modtwopi(atan2(x,y));

    if (x==0.0 && y==0.0)
      return 0.0;
    if (myabs(y)<0.00000001*myabs(x))
      return ( mysgn(x)==+1 ? pi/2.0 : - pi/2.0 );
    float ang=atan(x/y);
    if (y<0) // Which side?
      ang=ang+pi;
    if (ang<0) // Keep it 0-2pi
      ang=ang+2.0*pi;
    return ang;
  }

  // Between -pi and +pi
   float V2d::angbetween(V2d a,V2d b) {
    float ang=a.angle();
    float bang=b.angle();
    return anglebetween(ang,bang);
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


 Pixel::Pixel(V2d v) {
  x=v.x;
  y=v.y;
}

// Starts of class Polar


  // Variable declared in .h file
   Polar::Polar(float a,float m) {
    ang=a;
    mag=m;
  }
   Polar::Polar(V2d v) {
    ang=v.angle();
    mag=v.mag();
  }
  V2d Polar::v2d() {
    return V2d::angle(ang)*mag;
  }
// End class 


V2d operator-(V2d v) {
  return V2d(-v.x,-v.y);
}

float tofloat(V2d v) {
  return v.mag();
}

#endif
