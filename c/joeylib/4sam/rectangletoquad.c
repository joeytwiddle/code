List<V3d> rectanglefromquadrilateral(List<V2d> v2ds,V3d eye) {
  V2d a=v2ds.num(1); // tl
  V2d b=v2ds.num(2); // tr
  V2d c=v2ds.num(3); // br
  V2d d=v2ds.num(4); // bl

  // Lines through each corner (on which the true 3d corners must lie)
  Line3d al=Line3d(eye,V3d(a.x,a.y,0.0));
  Line3d bl=Line3d(eye,V3d(b.x,b.y,0.0));
  Line3d cl=Line3d(eye,V3d(c.x,c.y,0.0));
  Line3d dl=Line3d(eye,V3d(d.x,d.y,0.0));
  
  // Plane of left edge
  Plane pl=Plane::fromLines(al,dl);
  printf("Left plane %s\n",pl.toString());

  // Define distance of world corner c
  V3d cw=V3d(c.x,c.y,0.0);
  printf("Defined corner %s\n",cw.toString());

  // Find world corner b using left hand plane on c
  Plane plonr=Plane(cw,pl.nor);
  V3d bw=plonr.intersect(bl);
  printf("Used plane %s to get B %s\n",plonr.toString(),bw.toString());

  // Plane of top edge
  Plane pt=Plane::fromLines(al,bl);
  Plane ptonb=Plane(cw,pt.nor);
  V3d dw=ptonb.intersect(dl);

  // Plane of right edge
  Plane pr=Plane::fromLines(bl,cl);
  Plane pronl=Plane(dw,pr.nor);
  V3d aw=pronl.intersect(al);

  // the other way round (a check)
  Plane pb=Plane::fromLines(cl,dl);
  Plane pbont=Plane(bw,pb.nor);
  V3d aaw=pbont.intersect(al);

  if (!aaw.equals(aw))
    printf(">>>>>>> rgbmp.c: rectanglefromquadrilateral() Should have got %s %s equal!",aw.toString(),aaw.toString());

  printf("Got points with distances %f %f %f %f\n",(aw-eye).mag(),(bw-eye).mag(),(cw-eye).mag(),(dw-eye).mag());

  float score=1.0;
  score=score*(1.0-V3d::normdot(bw-aw,cw-bw));
  score=score*(1.0-V3d::normdot(cw-bw,dw-cw));
  score=score*(1.0-V3d::normdot(dw-cw,aw-dw));
  score=score*(1.0-V3d::normdot(aw-dw,bw-aw));
  printf("Score of rectangularity: %f\n",score);

  List<V3d> ls;
  ls.add(aw);
  ls.add(bw);
  ls.add(cw);
  ls.add(dw);

  return ls;
}


List<V3d> rectanglefromquadrilateralgroovy(List<V2d> v2ds,V3d eye) {
  V2d a=v2ds.num(1); // tl
  V2d b=v2ds.num(2); // tr
  V2d c=v2ds.num(3); // br
  V2d d=v2ds.num(4); // bl

  V2d hvp=Line2d(a,b).intersection(Line2d(c,d));
  V2d vvp=Line2d(b,c).intersection(Line2d(d,a));

  Line3d la=Line3d(eye,V3d(a.x,a.y,0.0));
  Line3d lb=Line3d(eye,V3d(b.x,b.y,0.0));
  Line3d lc=Line3d(eye,V3d(c.x,c.y,0.0));
  Line3d ld=Line3d(eye,V3d(d.x,d.y,0.0));
  Line3d lhvp=Line3d(eye,V3d(hvp.x,hvp.y,0.0));
  Line3d lvvp=Line3d(eye,V3d(hvp.x,hvp.y,0.0));

  Plane pleft=Plane::fromLines(la,ld);
  Plane pright=Plane::fromLines(lb,lc);
  Plane ptop=Plane::fromLines(la,lb);
  Plane pbot=Plane::fromLines(lc,ld);

  V3d hd=V3d::cross(ptop.nor,pbot.nor).normalised();
  V3d vd=V3d::cross(pleft.nor,pright.nor).normalised();

  Plane ptext=Plane(V3d(a.x,a.y,0.0),V3d::normcross(hd,vd));

  V3d aw=ptext.intersect(la);
  V3d bw=ptext.intersect(lb);
  V3d cw=ptext.intersect(lc);
  V3d dw=ptext.intersect(ld);

  printf("Got points with distances %f %f %f %f\n",(aw-eye).mag(),(bw-eye).mag(),(cw-eye).mag(),(dw-eye).mag());

  float score=1.0;
  score=score*(1.0-V3d::normdot(bw-aw,cw-bw));
  score=score*(1.0-V3d::normdot(cw-bw,dw-cw));
  score=score*(1.0-V3d::normdot(dw-cw,aw-dw));
  score=score*(1.0-V3d::normdot(aw-dw,bw-aw));
  printf("Score of rectangularity: %f\n",score);

  // Check against HVP,VVP (not done yet)

  List<V3d> ls;
  ls.add(aw);
  ls.add(bw);
  ls.add(cw);
  ls.add(dw);

  return ls;
}
