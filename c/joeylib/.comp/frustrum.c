#ifndef frustrum_C
  #define frustrum_C

  #include <frustrum.h>

// Starts of class Frustrum


  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
   Frustrum::Frustrum(V3d a,Ori x,float c,float d) {
    cen=a;
    ori=x;
    nearclip=c;
    farclip=d;
    V3d l=PPgetleft();
    V3d lin=V3d::normcross(l,V3d::j).neg();
    left=Plane(cen+l.orientate(ori),lin.orientate(ori));
    V3d r=PPgetright();
    V3d rin=V3d::normcross(r,V3d::j);
    right=Plane(cen+r.orientate(ori),rin.orientate(ori));
    V3d t=PPgettop();
    V3d tin=V3d::normcross(t,V3d::i).neg();
    top=Plane(cen+t.orientate(ori),tin.orientate(ori));
    V3d b=PPgetbottom();
    V3d bin=V3d::normcross(b,V3d::i);
    bottom=Plane(cen+b.orientate(ori),bin.orientate(ori));
  }
  int Frustrum::include(V3d v,float size) {
    float zdist=V3d::dot(ori.z(),v-cen);
    float ld=left.distto(v);
    float rd=right.distto(v);
    float td=top.distto(v);
    float bd=bottom.distto(v);
    if (zdist<nearclip-size || zdist>farclip+size ||
      ld<-size || rd<-size || td<-size || bd<-size)
    return 0; // definitely out
    if (zdist>nearclip+size && zdist<farclip-size &&
      ld>size && rd>size && td>size && bd>size)
    return 2; // definitely in
    return 1; // could be in or out
  }
// End class 


#endif
