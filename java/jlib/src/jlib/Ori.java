package jlib;

import jlib.V3d;
import jlib.*;

public class Ori implements Cloneable,Transform3d {
  // Orientate pushes into real frame
  // Disorientate brings into local frame

  public V3d x,y,qz;

  // For efficiency, the Ori keeps a Matrix to perform
  // it's transform.  The Matrix is updated whenever
  // the Ori changes.
  Matrix m;
  Ori memo=null; // A copy of the original, so it knows when it's changed.

  public Ori() {
    x=V3d.i;
    y=V3d.j;
  }
  public Ori(V3d t1,V3d t2) {
    x=t1; y=t2;
  }
  public String toString() {
    return "Ori:not written";
  }
  public V3d z() {
    return V3d.cross(x,y);
  }
  public void roll(double angle) {
    qz=z();
    x=V3d.rotate(x,qz,-angle);
    y=V3d.rotate(y,qz,-angle);
  }
  public void pitch(double angle) {
    y=V3d.rotate(y,x,angle);
  }
  public void turn(double angle) {
    x=V3d.rotate(x,y,-angle);
  }
  public void yaw(float angle) {
    turn(angle);
  }
  public void ensureaxesnormalisedbecauseimgoingtoreadthem() {
    x.normalise();
    y.normalise();
  }
  public static Ori orientate(Ori a,Ori b) {
    Ori c=new Ori();
    c.x=V3d.orientate(a.x,b);
    c.y=V3d.orientate(a.y,b);
    return c;
  }
  public V3d apply(V3d a) {
    return x.mult(a.x).add(y.mult(a.y).add(z().mult(a.z)));
  }
  public void recalcMatrix() {
//      System.out.println("new Matrix");
      m=new Matrix(this);
      try {
        memo=(Ori)this.clone();
      } catch (Exception e) {
        System.out.println("Unexpected Ori.orientate(): "+e);
      }
  }
  public V3d orientate(V3d a) {
    if (!this.equals(memo))
      recalcMatrix();
    return m.mult(a);
//    return x.mult(a.x).add(y.mult(a.y).add(z().mult(a.z)));
  }
  public boolean equals(Ori o) {
    return ( o==null ? false : x.equals(o.x) && y.equals(o.y) );
  }
  public void quickorisetup() {
    ensureaxesnormalisedbecauseimgoingtoreadthem();
    qz=z();
  }
  public void forcez(V3d nz) {
    nz=nz.normalised();
    y=V3d.normcross(x,nz).neg();
    x=V3d.normcross(y,nz);
    qz=nz;
  }
  public static Ori indir(V3d v) {
    V3d x=V3d.normcross(v,V3d.crazy);
    V3d y=V3d.normcross(x,v);
    return new Ori(x,y);
  }
  public static Ori xz(V3d x,V3d z) {
    return new Ori(x,V3d.normcross(z,x));
  }
};
