class Viewpoint {
public:
  V3d pos;
  Ori ori;
  Viewpoint() { }
  Viewpoint(V3d p,Ori o) {
    pos=p;
    ori=o;
  }
  Viewpoint(V3d from,V3d to,V3d up) {
    pos=from;
    ori=Ori::xz(V3d::normcross(up,to-from),to-from);
  }
  V3d orient(V3d x) {
    return (x-pos).orientate(ori);
  }
  V3d disorient(V3d v) {
    return (v-pos).disorientate(ori);
  }
  V3d bring(V3d v) {
    return disorient(v);
  }
  static Viewpoint fromto(V3d from,V3d to,V3d up) {
    return Viewpoint(from,Ori::xz(V3d::normcross(up,to-from),to-from));
  }
};

V3d V3d::orient(Viewpoint v) {
  return v.orient(*this);
}

V3d V3d::disorient(Viewpoint v) {
  return v.disorient(*this);
}
