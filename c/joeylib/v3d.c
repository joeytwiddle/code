#include <stdio.h>
#include <math.h>

class Ori;
class Viewpoint;
class Plane;

/* class Displayable {
   public:
     virtual void print()=0;
   };*/

class V3d;

V3d operator*(float d,V3d v);
V3d operator*(V3d v,float d);
V3d operator/(V3d v,float d);

class V3d /*: public Displayable*/ {
public:
  float x,y,z;
  
  fillin V3d origin,o,i,j,k,crazy;
  
  V3d() {
    x=0; y=0; z=0;
  }
  
  V3d(float u,float v,float w) {
    
    x=u;
    y=v;
    z=w;
  }
  
  myRGB myrgb() {
    return myRGB(x,y,z);
  }
  
  myRGB rgb() {
    return myRGB(x,y,z);
  }
  
  bool operator==(V3d b) {
    return (dist(*this,b)<0.0001);
  }

  bool equals(V3d b) {
    return ( (*this) == b );
  }
  
  V3d operator+(myRGB r) {
    return V3d(x+r.r/255.0,y+r.g/255.0,z+r.b/255.0);
  }
  
  V3d operator+(V3d v) {
    return V3d(x+v.x,y+v.y,z+v.z);
  }
  
  V3d operator-(V3d v) {
    return V3d(x-v.x,y-v.y,z-v.z);
  }
  
  V3d operator-() {
    return V3d(-x,-y,-z);
  }
  
  void print() {
    float a=x;
    float b=y;
    float c=z;
    printf("(%f,%f,%f)",a,b,c); 
  }
  
  String str() {
    return Sformat("(%f,%f,%f)",x,y,z);
  }
  
  String toString() {
    return Sformat("(%f,%f,%f)",x,y,z);
  }
  
  static void print(V3d v) {
    v.print(); 
  }
  
  
  static V3d add(V3d u, V3d v) {
    return V3d(u.x+v.x,u.y+v.y,u.z+v.z); 
  }
  
  
  static V3d mult(float a,V3d v) {
    return V3d(a*v.x,a*v.y,a*v.z); 
  }
  
  
  V3d neg() {
    return V3d::mult(-1.0,*this);
  }
  
  static V3d neg(V3d v) {
    return v.neg(); 
  }
  
  
  static V3d sub(V3d u, V3d v) {
    return V3d::add(u,v.neg()); 
  }
  
  
  static float mod(V3d v) {
    return v.mod(); 
  }
  
  static float smod(V3d v) {
    return v.mod(); 
  }
  
  float mod() {
    return sqrt(x*x+y*y+z*z); 
  }
  
  float mag() {
    return mod(); 
  }
  
  
  static float radius(V3d v) {
    return V3d::mod(v); 
  }
  
  
  static V3d norm(V3d v) {
    return V3d::mult(1/mod(v),v); 
  }
  
  V3d norm() {
    return V3d::norm(*this);
  }
  
  V3d normal() {
    return V3d::norm(*this);
  }
  
  V3d normalised() {
    return V3d::norm(*this);
  }
  
  void normalise() {
    changeto(norm());
  }
  
  static float dist(V3d u,V3d v) {
    return V3d::mod(V3d::sub(u,v));
  }
  
  static int equal(V3d u,V3d v) {
    
    if (V3d::mod(V3d::sub(u,v))<0.001)
      return 1;
    else
    return 0;
  }
  
  static float dot(V3d u,V3d v) {
    return u.x*v.x+u.y*v.y+u.z*v.z;
  }
  
  static V3d dimscale(V3d u,V3d v) {
    return V3d(u.x*v.x,u.y*v.y,u.z*v.z);
  }
  
  static float normdot(V3d u,V3d v) {
    return V3d::dot(u.norm(),v.norm());
  }
  
  static float normdot(V3d *u,V3d *v) {
    return V3d::dot(u->norm(),v->norm());
  }
  
  static V3d cross(V3d u,V3d v) { // i x j = k (left handed)
    return V3d(u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x);
  }
  
  static V3d normcross(V3d u,V3d v) {
    return V3d::norm(V3d::cross(u,v));
  }
  
  static V3d oldrotate1(V3d v,V3d axis,float ang) {
    float height,rad;
    V3d y=V3d::normcross(v,axis);
    V3d x=V3d::normcross(axis,y);
    // printf("y"); y.print();
    // printf("x"); x.print();
    rad=V3d::dot(v,x);
    height=V3d::dot(V3d::norm(axis),v);
    // printf("h%f r%f\n",height,rad);
    return (V3d::mult(height,V3d::norm(axis)))+(V3d::add(V3d::mult(rad*cos(ang),x),V3d::mult(rad*sin(ang),y)));
  }
  
  static V3d oldrotate(V3d v,V3d axis,float ang) {
    float height,rad;
    if (myabs(V3d::normdot(v,axis))>.999)
      return v;
    V3d y=V3d::normcross(v,axis);
    V3d x=V3d::normcross(axis,y);
    // printf("y"); y.print();
    // printf("x"); x.print();
    rad=V3d::dot(v,x);
    if (myabs(rad)<0.00001)
      return v;
    height=V3d::dot(V3d::norm(axis),v);
    // printf("h%f r%f\n",height,rad);
    V3d q=(V3d::mult(height,V3d::norm(axis)))+(V3d::add(V3d::mult(rad*cos(ang),x),V3d::mult(rad*sin(ang),y)));
    // printf("Rotated %s about %s to get %s .  ",v.str(),axis.str(),q.str());
    // printf("%f\n",rad);
    return q;
  }
  
  #ifdef ALLEGRO
  static V3d rotatea(V3d v,V3d axis,float ang) {
    MATRIX m;
    get_vector_rotation_matrix(&m,(fix)axis.x,(fix)axis.y,(fix)axis.z,(fix)(256*mymod(ang/2/pi)));
    fix x,y,z;
    apply_matrix(&m,(fix)v.x,(fix)v.y,(fix)v.z,&(x),&(y),&(z));
    return V3d(x,y,z);
  }
  #endif
  
  static V3d rotate(V3d v,V3d axis,float ang) {
    Matrix m=Matrix();
    m.makerotation(axis,ang);
    return m*v;
  }
  
  V3d rotate(V3d axis,float ang) {
    changeto(V3d::rotate(*this,axis,ang));
    return *this;
  }
  
  V3d rotated(V3d axis,float ang) {
    return V3d::rotate(*this,axis,ang);
  }
  
  #ifdef ALLEGRO
  static V3d rotatelista(List<V3d> l,V3d axis,float ang) {
    MATRIX_f m;
    get_vector_rotation_matrix_f(&m,(fix)axis.x,(fix)axis.y,(fix)axis.z,(fix)(256*mymod(ang/2/pi)));
    // fix x,y,z;
    for (int i=1;i<=l.length();i++) {
      V3d* v=l.p2num(i);
      // apply_matrix(&m,(fix)v->x,(fix)v->y,(fix)v->z,&(x),&(y),&(z));
      // v->x=x; v->y=y; v->z=z;
      // v->changeto(V3d(x,y,z));
      apply_matrix_f(&m,v->x,v->y,v->z,&v->x,&v->y,&v->z);
    }
  }
  #endif
  
  static V3d rotatelist(List<V3d> l,V3d axis,float ang) {
    Matrix m=Matrix();
    m.makerotation(axis,ang);
    for (int i=1;i<=l.length();i++) {
      V3d* v=l.p2num(i);
      v->changeto(m*(*v));
    }
  }
  
  static V3d random() {
    /* angle a has circum c=2*pi*r where r=sin(a)
       A(a) = int([0,a],2*pi*sin(a),da) = 2*pi*(1-cos(a))
       A(pi) = 4*pi
       cos(a)=1-A/(2*pi)
       a=invcos(1-A/(2*pi)) */
    float t,a,b;
    // V3d v;
    t=4.0*myrnd()*pi;
    a=acos(1.0-t/2.0/pi);
    b=myrnd()*2.0*pi;
    // printf("\nAngles %f %f :",a,b);
    V3d v=V3d::oldrotate(V3d(0,1,0),V3d(1,0,0),a);
    // printf("First rotation"); v.print();
    v=V3d::oldrotate(v,V3d(0,1,0),b);
    // printf("Second rotation"); v.print();
    return v;
  }
  
  static V3d random(float f) {
    return f*random();
  }
  
  static V3d randomvolume() {
    return random()*sqrt(myrnd());
  }
  
  static V3d randomvolume(float f) {
    return f*random()*sqrt(myrnd());
  }
  
  void changeto(V3d v) {
    x=v.x; y=v.y; z=v.z;
  }
  
  void setto(V3d v) {
    x=v.x; y=v.y; z=v.z;
  }
  
  void operator=(V3d v) {
    changeto(v);
  }
  
  float var(int i) {
    switch (i) {
      case 0 : return x;
      case 1 : return y;
      case 2 : return z;
      default: return 0;
    }
  }
  
  float getlongestside() {
    if (x>y && x>z)
      return x;
    else if (y>z)
    return y;
    else
    return z;
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
    if (z<low)
      z=low;
    if (z>high)
      z=high;
  }
  
  static V3d orientate(V3d v,Ori ori);
  static V3d qorientate(V3d v,Ori ori);
  static V3d disorientate(V3d v,Ori ori);
  V3d orientate(Ori ori);
  V3d disorientate(Ori ori);
  
  /* V3d breakdown(V3d a,V3d b,V3d c) { // breaks down vector into axes
       // p*a+q*b+r*c=this
       // p=(x-q*b.x-r*c.x)/a.x
       // (x-q*b.x-r*c.x)/a.x*a.y+q*b.y+r*c.y=y
       // q(-b.x/a.x*a.y+b.y)=y-r*c.y-(r*c.x+x)/a.x*a.y
       // q=(y-r*c.y-(r*c.x+x)/a.x*a.y)/(-b.x/a.x*a.y+b.y)
       // r*c.z=z-(x-q*b.x-r*c.x)/a.x*a.z-(y-r*c.y-(r*c.x+x)/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z
       // r*(c.z-c.x/a.x*a.z-(c.y-c.x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z=z-(x-q*b.x)/a.x*a.z-(y-x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z
       // r=z-(x-q*b.x)/a.x*a.z-(y-x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z/((c.z-c.x/a.x*a.z-(c.y-c.x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z)
       float r=z-(x-q*b.x)/a.x*a.z-(y-x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z/((c.z-c.x/a.x*a.z-(c.y-c.x/a.x*a.y)/(-b.x/a.x*a.y+b.y)*b.z);
       float q=(y-r*c.y-(r*c.x+x)/a.x*a.y)/(-b.x/a.x*a.y+b.y);
       float p=(x-q*b.x-r*c.x)/a.x;
       return V3d(p,q,r);
     } */

  V3d operator*(V3d o) {
    return V3d(x*o.x,y*o.y,z*o.z);
  }

  V3d orient(Viewpoint);
  V3d disorient(Viewpoint);

  V2d dropz() {
    return V2d(x,y);
  }

  V2d xz() {
    return V2d(x,z);
  }

  V2d perpproj() {
    return V2d(x/z,y/z);
  }

	static float angBetween(V3d a,V3d b) {
		// a . b = |a| |b| cos(t)
		// a x b = |a| |b| sin(t) p
		return acos(V3d::normdot(a,b));
	}

  float distAbove(Plane p); // -ve if below in terms of plane's normal

};

V3d operator*(float d,V3d v) {
  return V3d(d*v.x,d*v.y,d*v.z);
}

V3d operator*(V3d v,float d) {
  return V3d(d*v.x,d*v.y,d*v.z);
}

V3d operator/(V3d v,float d) {
  return V3d(v.x/d,v.y/d,v.z/d);
}

V3d myRGB::v3d() {
  return V3d(r/255.0,g/255.0,b/255.0);
}


// myRGB myRGB::operator*(myRGB o) {
  // return V3d::dimscale(o.v3d(),this->v3d()).rgb();
// }

float tofloat(V3d v) {
  return v.mod();
}

float V3dtofloat(V3d v) {
  return v.mod();
}

float tofloat(myRGB r) {
  return r.v3d().mod();
}

V3d hang(V3d x,V3d cen,float damp,float rnd) {
  return cen+(x-cen)*damp+V3d::randomvolume(rnd);
}

V2d hang(V2d x,V2d cen,float damp,float rnd) {
  return cen+(x-cen)*damp+rnd*V2d::randomcircle();
}

V2d hang(V2d *x,V2d cen,float damp,float rnd) {
  *x=cen+(*x-cen)*damp+rnd*V2d::randomcircle();
}

V3d pull(V3d a,float t,V3d b) {
  return a*(1.0-t)+b*t;
}

myRGB myRGB::ave(myRGB o) {
  return ((v3d()+o.v3d())/2.0).myrgb();
}

fillup V3d V3d::origin=V3d(0,0,0);
fillup V3d V3d::o=V3d(0,0,0);
fillup V3d V3d::i=V3d(1,0,0);
fillup V3d V3d::j=V3d(0,1,0);
fillup V3d V3d::k=V3d(0,0,1);
fillup V3d V3d::crazy=V3d(123.456,135.527,54.67);

