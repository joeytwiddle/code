package jlib;

import java.lang.Math;

import jlib.Ori;

public class V3d {

  public double x,y,z;
  
  public static final V3d origin=new V3d(0.0,0.0,0.0);
  public static final V3d o=origin;
  public static final V3d i=new V3d(1.0,0.0,0.0);
  public static final V3d j=new V3d(0.0,1.0,0.0);
  public static final V3d k=new V3d(0.0,0.0,1.0);
  public static final V3d crazy=new V3d(13.0,17.0,19.0);

  public V3d() {
    x=(double)0.0; y=(double)0.0; z=(double)0.0;
  }
  public V3d(double u,double v,double w) {
    x=u;
    y=v;
    z=w;
  }
  public V3d(float u,float v,float w) {
    x=(double)u;
    y=(double)v;
    z=(double)w;
  }

  public boolean equals(V3d b) {
    return (dist(this,b)<0.0001);
  }
  
  public String toString() {
    return "("+(float)x+","+(float)y+","+(float)z+")";
  }
  
  public static V3d add(V3d u, V3d v) {
    return new V3d(u.x+v.x,u.y+v.y,u.z+v.z);
  }

  public V3d add(V3d u) {
    return add(this,u);
  }

  public static V3d mult(double a,V3d v) {
    return new V3d(a*v.x,a*v.y,a*v.z);
  }
  public V3d mult(double a) {
    return mult(a,this);
  }

  public static V3d div(V3d v,double a) {
    return new V3d(v.x/a,v.y/a,v.z/a);
  }
  public V3d div(double a) {
    return div(this,a);
  }

  public V3d neg() {
    return V3d.mult(-1,this);
  }
  
  public static V3d neg(V3d v) {
    return v.neg(); 
  }
  
  
  public static V3d sub(V3d u, V3d v) {
    return V3d.add(u,v.neg());
  }
  public V3d sub(V3d v) {
    return V3d.add(this,v.neg());
  }
  public V3d subtract(V3d v) {
    return V3d.add(this,v.neg());
  }
  public V3d minus(V3d v) {
    return V3d.add(this,v.neg());
  }

  public static double mod(V3d v) {
    return v.mod(); 
  }
  
  static double smod(V3d v) {
    return v.mod(); 
  }
  
  public double mod() {
    return Math.sqrt(x*x+y*y+z*z);
  }
  
  public double mag() {
    return mod(); 
  }
  
  
  static double radius(V3d v) {
    return V3d.mod(v);
  }
  
  
  static V3d norm(V3d v) {
    return V3d.mult(1/mod(v),v);
  }
  
  public V3d norm() {
    return V3d.norm(this);
  }
  
  public V3d normal() {
    return V3d.norm(this);
  }
  
  public V3d normalised() {
    return V3d.norm(this);
  }
  
  public void normalise() {
    V3d n=normal();
    x=n.x; y=n.y; z=n.z;
  }
  
  public static double dist(V3d u,V3d v) {
    return V3d.mod(V3d.sub(u,v));
  }
  
  public static int equal(V3d u,V3d v) {
    
    if (V3d.mod(V3d.sub(u,v))<0.001)
      return 1;
    else
    return 0;
  }
  
  public static double dot(V3d u,V3d v) {
    return u.x*v.x+u.y*v.y+u.z*v.z;
  }
  public double dot(V3d v) {
    return dot(this,v);
  }

  public static V3d dimscale(V3d u,V3d v) {
    return new V3d(u.x*v.x,u.y*v.y,u.z*v.z);
  }
  
  public static double normdot(V3d u,V3d v) {
    return V3d.dot(u.norm(),v.norm());
  }
  
  public static V3d cross(V3d u,V3d v) { // i x j = k (left handed)
    return new V3d(u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x);
  }
  
  public static V3d normcross(V3d u,V3d v) {
    return V3d.norm(V3d.cross(u,v));
  }
  
  public static V3d oldrotate1(V3d v,V3d axis,double ang) {
    double height,rad;
    V3d y=V3d.normcross(v,axis);
    V3d x=V3d.normcross(axis,y);
    // printf("y"); y.print();
    // printf("x"); x.print();
    rad=V3d.dot(v,x);
    height=V3d.dot(V3d.norm(axis),v);
    // printf("h%f r%f\n",height,rad);
    return (V3d.mult(height,V3d.norm(axis))).add((V3d.add(V3d.mult(rad*Math.cos(ang),x),V3d.mult(rad*Math.sin(ang),y))));
  }
  
  public static V3d oldrotate(V3d v,V3d axis,double ang) {
    double height,rad;
    if (Math.abs(V3d.normdot(v,axis))>.999)
      return v;
    V3d y=V3d.normcross(v,axis);
    V3d x=V3d.normcross(axis,y);
    // printf("y"); y.print();
    // printf("x"); x.print();
    rad=V3d.dot(v,x);
    if (Math.abs(rad)<0.00001)
      return v;
    height=V3d.dot(V3d.norm(axis),v);
    // printf("h%f r%f\n",height,rad);
    V3d q=(V3d.mult(height,V3d.norm(axis))).add((V3d.add(V3d.mult(rad*Math.cos(ang),x),V3d.mult(rad*Math.sin(ang),y))));
    // printf("Rotated %s about %s to get %s .  ",v.str(),axis.str(),q.str());
    // printf("%f\n",rad);
    return q;
  }
  
  public static V3d rotate(V3d v,V3d axis,double ang) {
    Matrix m=new Matrix(axis,ang);
    return m.mult(v);
  }
  
  public V3d rotate(V3d axis,double ang) {
    return V3d.rotate(this,axis,ang);
  }
  
  public double var(int i) {
    switch (i) {
      case 0 : return x;
      case 1 : return y;
      case 2 : return z;
      default: return 0;
    }
  }

  public void setvar(int i,double d) {
    switch (i) {
      case 0 : x=d; break;
      case 1 : y=d; break;
      case 2 : z=d; break;
      default: break;
    }
  }

  public double getlongestside() {
    if (x>y && x>z)
      return x;
    else if (y>z)
    return y;
    else
    return z;
  }
  
  public void chop(double low,double high) {
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
  
/*  static V3d orientate(V3d v,Ori ori);
  static V3d qorientate(V3d v,Ori ori);
  static V3d disorientate(V3d v,Ori ori);
  V3d orientate(Ori ori);
  V3d disorientate(Ori ori); */
  
  public V3d mult(V3d o) {
    return new V3d(x*o.x,y*o.y,z*o.z);
  }

//  V3d orient(Viewpoint);
//  V3d disorient(Viewpoint);

  public static V3d orientate(V3d a,Ori o) {
    return o.orientate(a);
//    return o.x.mult(a.x).add(o.y.mult(a.y).add(o.z().mult(a.z)));
  }

  public V3d orientate(Ori o) {
    return orientate(this,o);
  }

  public static V3d checksum=o;
  public static int checkcnt=0;

  public static V3d randomcube() {
    return randomcube(V3d.o,1.0f);
  }

  public static V3d randomcube(V3d cen,float rad) {
    return new V3d(JMaths.rnd(-rad,rad),JMaths.rnd(-rad,rad),JMaths.rnd(-rad,rad));
  }

  public static V3d random() {
    V3d r=new V3d(-1.0+2.0*Math.random(),-1.0+2.0*Math.random(),-1.0+2.0*Math.random());
//    checkcnt++;
//    checksum=checksum.mult((double)(checkcnt-1)).add(r).div((double)checkcnt);
//    System.out.println(""+checksum);
    return r;
  }

/*  public static V3d random() {
    // angle a has circum c=2*pi*r where r=sin(a)
    // A(a) = int([0,a],2*pi*sin(a),da) = 2*pi*(1-cos(a))
    // A(pi) = 4*pi
    // cos(a)=1-A/(2*pi)
    // a=invcos(1-A/(2*pi))
    double t,a,b;
    t=4.0*Math.random()*Math.PI;
    a=Math.acos(1.0-t/2.0/Math.PI);
    b=Math.random()*2.0*Math.PI;
    V3d v=V3d.oldrotate(j,i,a);
    v=V3d.oldrotate(v,j,b);
    return v;
  }*/

  public static V3d randomvolume() {
    return random().mult(Math.sqrt(Math.random()));
  }

}

/*
V3d operator*(double d,V3d v) {
  return new V3d(d*v.x,d*v.y,d*v.z);
}

V3d operator*(V3d v,double d) {
  return new V3d(d*v.x,d*v.y,d*v.z);
}

V3d operator/(V3d v,double d) {
  return new V3d(v.x/d,v.y/d,v.z/d);
}

V3d myRGB.v3d() {
  return new V3d(r/255.0,g/255.0,b/255.0);
}
*/