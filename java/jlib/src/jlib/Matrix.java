package jlib;

public class Matrix {

  double[][] v=new double[3][3];

  public Matrix(Transform3d t) { // Dependent on another transform
    V3d i=t.apply(V3d.i);
    V3d j=t.apply(V3d.j);
    V3d k=t.apply(V3d.k);
    v[0][0]=i.x;
    v[1][0]=j.x;
    v[2][0]=k.x;
    v[0][1]=i.y;
    v[1][1]=j.y;
    v[2][1]=k.y;
    v[0][2]=i.z;
    v[1][2]=j.z;
    v[2][2]=k.z;
  }

  public Matrix(V3d axis,double ang) { // Rotation matrix
    v[0][0]=V3d.oldrotate(V3d.i,axis,ang).x;
    v[1][0]=V3d.oldrotate(V3d.j,axis,ang).x;
    v[2][0]=V3d.oldrotate(V3d.k,axis,ang).x;
    v[0][1]=V3d.oldrotate(V3d.i,axis,ang).y;
    v[1][1]=V3d.oldrotate(V3d.j,axis,ang).y;
    v[2][1]=V3d.oldrotate(V3d.k,axis,ang).y;
    v[0][2]=V3d.oldrotate(V3d.i,axis,ang).z;
    v[1][2]=V3d.oldrotate(V3d.j,axis,ang).z;
    v[2][2]=V3d.oldrotate(V3d.k,axis,ang).z;
  }

  public V3d mult(V3d u) {
    V3d n=new V3d(0,0,0);
    double c;
    for (int i=0;i<3;i++) {
      c=u.var(i);
      n.x=n.x+c*v[i][0];
      n.y=n.y+c*v[i][1];
      n.z=n.z+c*v[i][2];
    }
    return n;
  }

}
