class Plane {
public:
  V3d pos,nor; // nor must be normalised or distances will not work properly
  Plane() {
  }
  Plane(V3d a,V3d b) {
    pos=a; nor=b.normal();
  }
  float distto(V3d v) { // same as distAbove
    return V3d::dot(v-pos,nor);
  }
  Viewpoint orientor() {
    // Returns an viewpoint to frame where plane is x,y plane
    return Viewpoint(pos,Ori::indir(nor));
  }
  Plane disorient(Viewpoint v) {
    return Plane(pos.disorient(v),nor.disorientate(v.ori));
  }
  Plane bring(Viewpoint v) {
    return disorient(v);
  }
  Plane orient(Viewpoint v) {
    return Plane(pos.orient(v),nor.orientate(v.ori));
  }
  Line2d intersection(Plane p) {
    Viewpoint v=orientor();
    Plane pp=bring(v);
    // Where does pp meet this plane?
    // (X-PPP).PPN=0
    // X=(x,y,0)
    // (x-pppx)*pppnx+(y-pppy)*pppny+pppz*pppnz=0
    // y=(-pppz*pppnz+(pppx-x)*pppnx)/pppny+pppy
    #define yfrom(i) (-pp.pos.z*pp.nor.z+(pp.pos.x-(i))*pp.nor.x)/pp.nor.y+pp.pos.y
    Line2d l=Line2d(V2d(-100.0,yfrom(-100.0)),V2d(100.0,yfrom(100.0)));
//    Line3d t=Line3d(l);
//    t=t.orient(v);
    return l;
  }

  V3d intersection(Line3d l) {
    return l.intersection(*this);
  }

  V3d intersect(Line3d l) {
    return l.intersection(*this);
  }

  float distAbove(V3d v) { // should be part of V3d!
    return v.distAbove(*this);
  }

  static Plane fromLines(Line3d a,Line3d b) {
    if (!a.a.equals(b.a))
      error("Can only create plane from lines if they both start at some point!");
    return Plane(a.a,V3d::normcross(a.b-a.a,b.b-b.a));
  }

  String toString() {
    return Sformat(".%s^%s",pos.toString(),nor.toString());
  }

};

float V3d::distAbove(Plane p) { // -ve if below in terms of plane's normal
  return p.distto(*this);
}

V3d Line3d::intersection(Plane p) {
    float ad=a.distAbove(p);
    float bd=b.distAbove(p);
    // 0 = ad + thru * (bd - ad)
    // thru = -ad / (bd-ad)
    float thru=-ad/(bd-ad);
    return a+thru*(b-a);
}
