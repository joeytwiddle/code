#ifndef viewpnt_C
  #define viewpnt_C

  #include <viewpnt.h>

// Starts of class Viewpoint


  // Variable declared in .h file
  // Variable declared in .h file
   Viewpoint::Viewpoint() { }
   Viewpoint::Viewpoint(V3d p,Ori o) {
    pos=p;
    ori=o;
  }
   Viewpoint::Viewpoint(V3d from,V3d to,V3d up) {
    pos=from;
    ori=Ori::xz(V3d::normcross(up,to-from),to-from);
  }
  V3d Viewpoint::orient(V3d x) {
    return (x-pos).orientate(ori);
  }
  V3d Viewpoint::disorient(V3d v) {
    return (v-pos).disorientate(ori);
  }
  V3d Viewpoint::bring(V3d v) {
    return disorient(v);
  }
   Viewpoint Viewpoint::fromto(V3d from,V3d to,V3d up) {
    return Viewpoint(from,Ori::xz(V3d::normcross(up,to-from),to-from));
  }
// End class 


V3d V3d::orient(Viewpoint v) {
  return v.orient(*this);
}

V3d V3d::disorient(Viewpoint v) {
  return v.disorient(*this);
}

#endif
