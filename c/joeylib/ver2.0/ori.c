class Ori { // Orientate pushes into real frame
            // Disorientate brings into local frame
public:
  V3d x,y,qz;
  Ori() {
    x=V3d::i;
    y=V3d::j;
  }
  Ori(V3d t1,V3d t2) {
    x=t1; y=t2;
  }
  String toString() {
    return "Ori:not written";
  }
  V3d z() {
    return V3d::cross(x,y);
  }
  void roll(float angle) {
    V3d qz=z();
    x=V3d::rotate(x,qz,-angle);
    y=V3d::rotate(y,qz,-angle);
  }
  void pitch(float angle) {
    y=V3d::rotate(y,x,angle);
  }
  void turn(float angle) {
    x=V3d::rotate(x,y,-angle);
  }
  void yaw(float angle) {
    turn(angle);
  }
  void ensureaxesnormalisedbecauseimgoingtoreadthem() {
    x.normalise();
    y.normalise();
  }
  static Ori orientate(Ori a,Ori b) {
    Ori c=Ori();
    c.x=V3d::orientate(a.x,b);
    c.y=V3d::orientate(a.y,b);
    return c;
  }
  void quickorisetup() {
    ensureaxesnormalisedbecauseimgoingtoreadthem();
    qz=z();
  }
  void forcez(V3d nz) {
    nz=nz.normalised();
    y=V3d::normcross(x,nz).neg();
    x=V3d::normcross(y,nz);
    qz=nz;
  }
  static Ori indir(V3d v) {
    V3d x=V3d::normcross(v,V3d::crazy);
    V3d y=V3d::normcross(x,v);
    return Ori(x,y);
  }
  static Ori xz(V3d x,V3d z) {
    return Ori(x,V3d::normcross(z,x));
  }
};
