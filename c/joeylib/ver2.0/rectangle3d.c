class Rectangle3d : public Plane {
public:
  V3d right,down;

  Rectangle3d() {
    right=V3d::o;
    down=V3d::o;
    Plane(V3d::o,V3d::i);
  }

  Rectangle3d(V3d topleft,V3d r,V3d d) {
    setupRectangle3d(topleft,r,d);
  }

  void setupRectangle3d(V3d topleft,V3d r,V3d d) {
    right=r;
    down=d;
    setupPlane(topleft,V3d::cross(right,down));
  }

  bool inrectangle(V3d v) {
    if (!onplane(v)) {
      printf("V3d %s is not even on plane %s\n",v.toString(),toString());
      return false;
    } else {
      V2d p=projectDown(v);
      return (p.x>=0 && p.x<=1 && p.y>=0 && p.y<=1);
    }
  }

  V2d projectDown(V3d v) {
    return V2d( V3d::dot(v - pos,right.normalised()) / right.mag() ,
        V3d::dot(v - pos,down.normalised()) / down.mag() );
  }
  
};
