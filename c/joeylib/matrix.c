V3d Matrix::operator*(V3d u) {
  V3d n=V3d(0,0,0);
  float c;
  for (int i=0;i<=2;i++) {
    c=u.var(i);
    n.x=n.x+c*v[i][0];
    n.y=n.y+c*v[i][1];
    n.z=n.z+c*v[i][2];
  }
  return n;
}

Matrix::Matrix() {
  v=new (float*)[2];
  for (int i=0;i<=2;i++)
  v[i]=new float[2];
}

fillup Matrix Matrix::rotation(V3d axis,float ang) {
  Matrix m;
  m.makerotation(axis,ang);
  return m;
}

void Matrix::makerotation(V3d axis,float ang) {
  
  // v'=Mv
  // x'=M00x+M10y+M20z
  // y'=M01x+M11y+M21z
  // z'=M02x+M12y+M22z
  
  // M00=xof(100)afterrot
  
  v[0][0]=V3d::oldrotate(V3d::i,axis,ang).x;
  v[1][0]=V3d::oldrotate(V3d::j,axis,ang).x;
  v[2][0]=V3d::oldrotate(V3d::k,axis,ang).x;
  v[0][1]=V3d::oldrotate(V3d::i,axis,ang).y;
  v[1][1]=V3d::oldrotate(V3d::j,axis,ang).y;
  v[2][1]=V3d::oldrotate(V3d::k,axis,ang).y;
  v[0][2]=V3d::oldrotate(V3d::i,axis,ang).z;
  v[1][2]=V3d::oldrotate(V3d::j,axis,ang).z;
  v[2][2]=V3d::oldrotate(V3d::k,axis,ang).z;
  
  /* float c=cos(ang);
     float s=sin(ang);
     float cc=1-c;
     axis.normalise();
     v[0][0]=(cc*axis.x*axis.x)+c;
     v[0][1]=(cc*axis.x*axis.y)+(axis.z*s);
     v[0][2]=(cc*axis.x*axis.z)-(axis.y*s);
     v[1][0]=(cc*axis.x*axis.y)-(axis.z*s);
     v[1][1]=(cc*axis.y*axis.y)+c;
     v[1][2]=(cc*axis.z*axis.y)+(axis.x*s);
     v[2][0]=(cc*axis.x*axis.z)+(axis.y*s);
     v[2][1]=(cc*axis.y*axis.z)-(axis.x*s);
     v[2][2]=(cc*axis.z*axis.z)+c;*/
  
}

void Matrix::makeorientation(Ori o) {
  V3d x=V3d::i.orientate(o);
  V3d y=V3d::j.orientate(o);
  V3d z=V3d::k.orientate(o);
  v[0][0]=x.x;
  v[1][0]=y.x;
  v[2][0]=z.x;
  v[0][1]=x.y;
  v[1][1]=y.y;
  v[2][1]=z.y;
  v[0][2]=x.z;
  v[1][2]=y.z;
  v[2][2]=z.z;
}

Ori operator*(Matrix m,Ori o) {
  return Ori(m*o.x,m*o.y);
}
