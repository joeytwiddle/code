#ifndef renderable_C
  #define renderable_C

  #include <renderable.h>

// General interpolation interpolates on screen and in space and uses
// texture call to evaluate colour at each pixel

// class Texture declared at top of RGBmp

// ImageType and ColourType define in jfont.c

#ifdef USINGJFONT

// Starts of class Renderable


  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
   V3d Renderable::pos() {
    return cen;
  }
   Renderable Renderable::translated(V3d t) {
  #ifdef DEBUG
    error("Renderable object has not been cast");
  #endif
  }
   Renderable Renderable::rotated(V3d ax,float ang) {
  #ifdef DEBUG
    error("Renderable object has not been cast");
  #endif
  }
   Renderable Renderable::operator*(Matrix m) {
  #ifdef DEBUG
    error("Renderable object has not been cast");
  #endif
  }
   void Renderable::render(JBmp *j,Viewpoint *v) {
  #ifdef DEBUG
    error("Renderable object has not been cast");
  #endif
  }
   void Renderable::render(RGBmp *j,Viewpoint *v) {
  #ifdef DEBUG
    error("Renderable object has not been cast");
  #endif
  }
   void Renderable::renderwireframe(ImageType *p,Viewpoint *v) {
  helloworld();
  #ifdef DEBUG
    error("Renderable object has not been cast");
  #endif
  }
// End class 


Renderable operator*(Matrix m,Renderable r) {
  return r*m;
}

// Starts of class Sphere


  // Variable declared in .h file
   Sphere::Sphere(V3d c,float r) {
    cen=c;
    rad=r;
    size=rad;
  }
  Renderable Sphere::translated(V3d t) {
    return Sphere(cen+t,rad);
  }
  Renderable Sphere::rotated(V3d ax,float ang) {
    return Sphere(cen.rotated(ax,ang),rad);
  }
  Renderable Sphere::operator*(Matrix m) {
    return Sphere(m*cen,rad);
  }
  void Sphere::render(JBmp *j,Viewpoint *v) {
    int x,y;
    float p=PPgetunitnoadd(v->bring(cen));
    if (p) {
      float r=rad*p;
      PPgetscrposnoadd(v->bring(cen),&x,&y);
      j->filledcircle(x,y,r,255);
    }
  }
  void Sphere::renderwireframe(ImageType *ima,Viewpoint *v) {
//    printf("Drawing sphere\n");
    int x,y;
    float p=PPgetunitnoadd(v->bring(cen));
    if (p) {
      float r=rad*p;
      PPgetscrposnoadd(v->bring(cen),&x,&y);
      ima->opencircle(x,y,r,312.0);
    }
  }
// End class 


// Starts of class Disc


  // Variable declared in .h file
  // Variable declared in .h file
   Disc::Disc() { }
   Disc::Disc(V3d pos,V3d nor,float f) {
    cen=pos;
    norm=nor.norm();
    rad=f;
  }
  void Disc::renderwireframe(ImageType *p,Viewpoint *v) {
  }
// End class 


// Starts of class LineRend


  // Variable declared in .h file
   LineRend::LineRend(V3d aa,V3d bb) {
    a=aa;
    b=bb;
    cen=(a+b)/2.0;
    size=V3d::dist(cen,a);
  }
  void LineRend::renderwireframe(ImageType *p,Viewpoint *v) {
    int xa,ya,xb,yb;
    PPgetscrposnoadd(v->bring(a),&xa,&ya);
    PPgetscrposnoadd(v->bring(b),&xb,&yb);
//    printf("Drawing line %i %i %i %i (%f)\n",xa,ya,xb,yb,DefaultColour);
    p->line(xa,ya,xb,yb,DefaultColour);
  }
// End class 


// Starts of class Cylinder


  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
   Cylinder::Cylinder(V3d aa,V3d bb,float r) {
    rad=r;
    a=aa;
    b=bb;
    cen=(a+b)/2.0;
    size=V3d::dist(a,b)/2.0;
    da=Disc(a,b-a,rad);
    db=Disc(b,a-b,rad);
  }
  void Cylinder::renderwireframe(ImageType *p,Viewpoint *v) {
    da.renderwireframe(p,v);
    db.renderwireframe(p,v);
    V3d per=V3d::normcross(b-a,v->ori.z());
    V3d eal=a+per*rad;
    V3d ear=a-per*rad;
    V3d ebl=b+per*rad;
    V3d ebr=b-per*rad;
    LineRend(eal,ebl).renderwireframe(p,v);
    LineRend(ear,ebr).renderwireframe(p,v);
  }
    
// End class 


// Starts of class TextRend


  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
   TextRend::TextRend(String t,int s,V3d pos,float d) {
    text=Snew(t);
    dist=d;
    fontsize=s;
    cen=pos;
  }
  void TextRend::renderwireframe(ImageType *p,Viewpoint *v) {
    int x,y;
    PPgetscrposnoadd(v->bring(cen),&x,&y);
    float s=PPgetunitnoadd(v->bring(cen));
    jfont.centerString(text,p,x,y,s,DefaultColour);
  }
// End class 


// Starts of class Triangle3d


  // Variable declared in .h file
   Triangle3d::Triangle3d(V3d aa,V3d bb,V3d cc) {
    a=aa;
    b=bb;
    c=cc;
    vs.add(a);
    vs.add(b);
    vs.add(c);
    cen=(aa+bb+cc)/3.0;
    size=max(max((a-cen).mod(),(b-cen).mod()),(c-cen).mod());
  }
  V3d Triangle3d::normal() {
    return V3d::normcross(b-a,c-a);
  }
  Renderable Triangle3d::translated(V3d t) {
    return Triangle3d(a+t,b+t,c+t);
  }
  Renderable Triangle3d::rotated(V3d ax,float ang) {
    return Triangle3d(a.rotated(ax,ang),b.rotated(ax,ang),c.rotated(ax,ang));
  }
  Renderable Triangle3d::operator*(Matrix m) {
    return Triangle3d(m*a,m*b,m*c);
  }
  void Triangle3d::render(JBmp *j,Viewpoint *v) {
    int xa,xb,xc,ya,yb,yc;
    if (PPgetscrposnoadd(v->bring(a),&xa,&ya) &&
        PPgetscrposnoadd(v->bring(b),&xb,&yb) &&
        PPgetscrposnoadd(v->bring(c),&xc,&yc)) {
      V3d n=normal().disorientate(v->ori);
      int c=ucharchop(255*myabs(V3d::normdot(n,V3d(-1,2,-1))));
      j->filltri(xa,ya,xb,yb,xc,yc,c);
//      j->filltri(xa,ya,xb,yb,xc,yc,ucharchop(255-255*v->bring(cen).z/8.0));
    }
  }
// End class 


// Class declaration of existence: Gouraud


// Starts of class Corner


  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
   Corner::Corner() { }
   Corner::Corner(V3d x) {
    pos=x;
  }
   Corner::Corner(V3d x,V3d n) {
    pos=x;
    normal=n;
  }
  /* Skipping declared method void Corner::findnormal() */

// End class 


float tofloat(Corner c) {
  return tofloat(c.pos);
}

// Starts of class Gouraud


  // Variable declared in .h file
  // Variable declared in .h file
   Gouraud::Gouraud(Texture *t,Corner *aa,Corner *bb,Corner *cc) {
    text=NULL;
    a=aa;
    b=bb;
    c=cc;
//    printf("Adding gouraud %s %s %s\n",aa->pos.toString(),b->pos.toString(),b->pos.toString());
    a->gs.add(this);
    b->gs.add(this);
    c->gs.add(this);
    cen=(a->pos+b->pos+c->pos)/3.0;
    size=max(max((a->pos-cen).mod(),(b->pos-cen).mod()),(c->pos-cen).mod());
    text=t;
  }
   Gouraud::Gouraud(Corner *aa,Corner *bb,Corner *cc) {
    text=NULL;
    a=aa;
    b=bb;
    c=cc;
//    printf("Adding gouraud %s %s %s\n",aa->pos.toString(),b->pos.toString(),b->pos.toString());
    a->gs.add(this);
    b->gs.add(this);
    c->gs.add(this);
    cen=(a->pos+b->pos+c->pos)/3.0;
    size=max(max((a->pos-cen).mod(),(b->pos-cen).mod()),(c->pos-cen).mod());
  }
  void Gouraud::render(JBmp *j,Viewpoint *v) {
    int xa,xb,xc,ya,yb,yc;
    if (PPgetscrposnoadd(v->bring(a->pos),&xa,&ya) &&
        PPgetscrposnoadd(v->bring(b->pos),&xb,&yb) &&
        PPgetscrposnoadd(v->bring(c->pos),&xc,&yc)) {
      V3d n=normal().disorientate(v->ori);
      int ca=a->col;
      int cb=b->col;
      int cc=c->col;
//      j->filltri(xa,ya,xb,yb,xc,yc,ca);
      j->intertri(xa,ya,xb,yb,xc,yc,ca,cb,cc);
//      j->line(xa,ya,xb,yb,ca); j->line(xb,yb,xc,yc,cb); j->line(xc,yc,xa,ya,cc);
    }
  }
  void Gouraud::render(RGBmp *r,Viewpoint *v) {
    int xa,xb,xc,ya,yb,yc;
    if (PPgetscrposnoadd(v->bring(a->pos),&xa,&ya) &&
        PPgetscrposnoadd(v->bring(b->pos),&xb,&yb) &&
        PPgetscrposnoadd(v->bring(c->pos),&xc,&yc)) {
      V3d n=normal().disorientate(v->ori);
      if (text==NULL)
        r->intertri(xa,ya,xb,yb,xc,yc,a->rgb,b->rgb,c->rgb);
      else
        r->intertri(xa,ya,xb,yb,xc,yc,a->pos,b->pos,c->pos,text);
    }
  }
  V3d Gouraud::normal() {
    return V3d::normcross(b->pos-a->pos,c->pos-b->pos);
  }
// End class 


  void Corner::findnormal() {
    normal=V3d::o;
    for (int i=1;i<=gs.len;i++)
      normal=normal+gs.num(i)->normal();
    if (gs.len>0)
      normal.normalise();
    col=ucharchop(255*myabs(V3d::normdot(normal,V3d(-1,2,-1))));
    rgb=rgb*((float)col/255.0);
//    col=col*(float)gs.len/6.0;
  }

V3d getpos(Renderable *r) {
  return r->cen;
}

void render(Octree<Renderable*> *o,Viewpoint *v) {
  PPscr.clear();
  Frustrum f=Frustrum(v->pos,v->ori,0.1,20);
//   List<Renderable*> *ol=o->getclippedlist(f);
  OrderedList<Renderable*> *ol=o->getorderedlist(f);
  for (int i=1;i<=ol->len;i++) {
//    printf(".");
    Renderable *r=ol->num(ol->len+1-i);
    r->render(&PPscr,v);
//    free(r); Assumed to be done by destroy(octree)
  }
  destroystruct(ol); // don't destroy elements, as above
  PPshow();
}

void render(RGBmp *rgbmp,Octree<Renderable*> *o,Viewpoint *v,myRGB rgb) {
  rgbmp->clear(rgb);
  Frustrum f=Frustrum(v->pos,v->ori,0.1,20);
//   List<Renderable*> *ol=o->getclippedlist(f);
  OrderedList<Renderable*> *ol=o->getorderedlist(f);
  for (register int i=1;i<=ol->len;i++) {
//    printf(".");
    Renderable *r=ol->num(ol->len+1-i);
    r->render(rgbmp,v);
//    free(r); Assumed to be done by destroy(octree)
  }
  destroystruct(ol); // don't destroy elements, as above
//  rgbmp->display();
  rgbmp->writetoscreen();
}

void render(RGBmp *rgbmp,Octree<Renderable*> *o,Viewpoint *v) {
  render(rgbmp,o,v,myRGB::black);
}
  

V3d posforrend(Renderable *r) {
  return r->cen;
}

#endif

#endif
