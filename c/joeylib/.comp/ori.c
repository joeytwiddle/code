#ifndef ori_C
  #define ori_C

  #include <ori.h>

// Starts of class Ori
 // Orientate pushes into real frame
            // Disorientate brings into local frame

  // Variable declared in .h file
   Ori::Ori() {
    x=V3d::i;
    y=V3d::j;
  }
   Ori::Ori(V3d t1,V3d t2) {
    x=t1; y=t2;
  }
  String Ori::toString() {
		return Sformat("Ori u=%s r=%s",y.toString(),x.toString());
    // return "Ori:not written";
  }
  V3d Ori::z() {
    return V3d::cross(x,y);
  }
  void Ori::roll(float angle) {
    V3d qz=z();
    x=V3d::rotate(x,qz,-angle);
    y=V3d::rotate(y,qz,-angle);
  }
  void Ori::pitch(float angle) {
    y=V3d::rotate(y,x,angle);
  }
  void Ori::turn(float angle) {
    x=V3d::rotate(x,y,-angle);
  }
  void Ori::yaw(float angle) {
    turn(angle);
  }
  void Ori::ensureaxesnormalisedbecauseimgoingtoreadthem() {
    x.normalise();
    y.normalise();
  }
   Ori Ori::orientate(Ori a,Ori b) {
    Ori c=Ori();
    c.x=V3d::orientate(a.x,b);
    c.y=V3d::orientate(a.y,b);
    return c;
  }
  void Ori::quickorisetup() {
    ensureaxesnormalisedbecauseimgoingtoreadthem();
    qz=z();
  }
  void Ori::forcez(V3d nz) {
    nz=nz.normalised();
    y=V3d::normcross(x,nz).neg();
    x=V3d::normcross(y,nz);
    qz=nz;
  }
   Ori Ori::indir(V3d v) {
    V3d x=V3d::normcross(v,V3d::crazy);
    V3d y=V3d::normcross(x,v);
    return Ori(x,y);
  }
   Ori Ori::xz(V3d x,V3d z) {
    return Ori(x,V3d::normcross(z,x));
  }
// End class 


#endif
