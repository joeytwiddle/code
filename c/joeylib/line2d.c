/** Apparently intersection can be done like this:
 *  Line1: La-Lb Line2: Ma-Mb
 *  Intersection P = ( La X Lb ) X ( Ma X Mb )
 *  where X is 2D cross product
**/

//class Line2d;
//void flibble(Line2d,int);
//void fobble(V2d);

// a=one end, b=the other
// (used to be a=one end, b=direction, but not any more!)

class Line2d {
public:
  V2d a,b;

  float length;

// Don't know what these were for!  Some PhD stuff no doubt.
//  List<List<Line2d *> > ls;//=List<List<Line2d *> >();//2,List<Line2d *>());
//  List<List<int> > es;//=List<List<int> >(2,List<int>());
//  bool allusedup;

  Line2d() {
//    allusedup=false;
  }
  Line2d(V2d aa,V2d bb) {
//    allusedup=false;
    a=aa;
    b=bb;
    refresh();
/*    ls.add(List<Line2d *>());
    ls.add(List<Line2d *>());
    es.add(List<int>());
    es.add(List<int>());*/
  }
  void refresh() {
    length=V2d::dist(a,b);
  }
  void swapends() {
//    error("End swapping not allowed.");
    V2d tmp=a;
    a=b;
    b=tmp;
  }
  float findintersectionnum(Line2d *o) {
    // float u=((double)o->a.y-(double)a.y+((double)o->b.y-(double)o->a.y)*((double)a.x-(double)o->a.x)/((double)o->b.x-(double)o->a.x))/((double)b.y-(double)a.y-((double)o->b.y-(double)o->a.y)*((double)b.x-(double)a.x)/((double)o->b.x-(double)o->a.x));
    // return u;
    return findintersectionnum(*o);
  }
  float findintersectionnum(Line2d o) {

    float ang=(b-a).angle()-pi/2.0;
    V2d na=(o.a-a);
    na=na.rotated(-ang);
    V2d nb=(o.b-a);
    nb=nb.rotated(-ang);
//    printf("%s and %s rotated to %s and %s\n",(o.a-a).toString(),(o.b-a).toString(),na.toString(),nb.toString());
    // Two tests for failure of method:
    if (nb.y-na.y==0) {
//      printf("Intersection of %s and %s failed because rotated line %s-%s is horizontal.\n",toString(),o.toString(),na.toString(),nb.toString());
      if ((b-a).mag()==0)
        return 0;
      return bigfloat/(b-a).mag();
    }
    float hit;
    if (nb.x-na.x==0) {
//      printf("Intersection of %s and %s is perpendicular because rotated line %s-%s is vertical.\n",toString(),o.toString(),na.toString(),nb.toString());
      hit=nb.x;
    } else {
      float grad=(nb.y-na.y)/(nb.x-na.x);
  //    printf("ydiff: %f xdiff: %f\n",nb.y-na.y,nb.x-na.x);
      float start=na.y-grad*na.x;
      hit=-start/grad;
    }
/*    printf("grad=%f start=%f hit=%f\n",grad,start,hit);
    opencircle(a+(b-a).norm()*hit,4,20);
    line(a+na,a+nb,9);
    opencircle(a+na,4,7);
    opencircle(a+nb,5,7);
    line(o.a,a+na,4);
    line(o.b,a+nb,4);
    line(a,o.a,6);
    line(a,a+na,6);
    V2d pos2=a+(b-a).norm()*hit;*/
    float num=hit/(b-a).mag();
/*    V2d pos=a+(b-a)*num;
    printf("Num %f, Pos=%s = %s ?\n",num,pos.toString(),pos2.toString());*/
    return num;

    // I = A + u*(B-A) = OA + v*(OB-OA)
    // Ax + u(Bx-Ax) = OAx + v(OBx-OAx)
    // Ay + u(By-Ay) = OAy + v(OBy-OAy)
    // v = { Ax + u(Bx-Ax) - OAx } / (OBx-OAx)
    // Ay + u(By-Ay) = OAy + (OBy-OAy) * ( Ax + u(Bx-Ax) - OAx ) / (OBx-OAx)
    // u(By-Ay) = OAy - Ay + (OBy-OAy) * u(Bx-Ax)/(OBx-OAx) + (OBy-OAy) * ( Ax - OAx ) / (OBx-OAx)
    // u(By-Ay-(OBy-OAy)*(Bx-Ax)/(OBx-OAx)) = OAy - Ay + (OBy-OAy) * ( Ax - OAx ) / (OBx-OAx)
    // u = { OAy - Ay + (OBy-OAy) * ( Ax - OAx ) / (OBx-OAx) } / { By - Ay - (OBy-OAy)*(Bx-Ax)/(OBx-OAx) }
    //float u=(o.a.y-a.y+(o.b.y-o.a.y)*(a.x-o.a.x)/(o.b.x-o.a.x))/(b.y-a.y-(o.b.y-o.a.y)*(b.x-a.x)/(o.b.x-o.a.x));
//    float top=((double)o.a.y-(double)a.y+((double)o.b.y-(double)o.a.y)*((double)a.x-(double)o.a.x)/((double)o.b.x-(double)o.a.x));
//    float bottom=((double)b.y-(double)a.y-((double)o.b.y-(double)o.a.y)*((double)b.x-(double)a.x)*((double)o.b.x-(double)o.a.x));
//    printf("bl=%f br=%f\n",(double)b.y-(double)a.y,((double)o.b.y-(double)o.a.y)*((double)b.x-(double)a.x)*((double)o.b.x-(double)o.a.x));
    double ans=((double)o.a.y-(double)a.y+((double)o.b.y-(double)o.a.y)*((double)a.x-(double)o.a.x)/((double)o.b.x-(double)o.a.x))/((double)b.y-(double)a.y-((double)o.b.y-(double)o.a.y)*((double)b.x-(double)a.x)/((double)o.b.x-(double)o.a.x));
//    double ans=top/bottom;
    if ( /*bottom==0 || top==-inf || top==inf ||*/ !(ans>=-10000000 && ans<=10000000)) {
      printf("Intersection %s with %s failed!\n",toString(),o.toString());
      return 0.5;
    }
//    printf("top=%f bot=%f get=%f\n",top,bottom,ans);
    return ans;
  }
/*  float findintersectionnum(Line2d o) {
//    printf("Looking for intersection\n");
    // i=A+k(B-A)
    // i=OA+l(OB-OA)
    // Ax+k(Bx-Ax)=OAx+l(OBx-OAx)
    // Ay+k(By-Ay)=OAy+l(OBy-OAy)
    // k=(OAx+l(OBx-OAx)-Ax)/(Bx-Ax)
    // Ay+(OAx+l(OBx-OAx)-Ax)/(Bx-Ax)*(By-Ay)=OAy+l(OBy-OAy)
    // Ay+(By-Ay)/(Bx-Ax)*(OAx-Ax)+l*(By-Ay)/(Bx-Ax)*(OBx-OAx)=OAy+l(OBy-OAy)
    // l*{ OAy-OBy + (By-Ay)/(Bx-Ax)*(OBx-OAx) } = OAy-Ay-(By-Ay)/(Bx-Ax)*(OAx-Ax)
    float l=(o.a.y-a.y-(b.y-a.y)/(b.x-a.x)*(o.a.x-a.x))/(o.a.y-o.b.y+(b.y-a.y)/(b.x-a.x)*(o.b.x-o.a.x));
//    return l;
    float k=(o.a.x+l*(o.b.x-o.a.x)-a.x)/(b.x-a.x);
    return k;
  }*/
  V2d *findintersectionornull(Line2d o) {
    float l=findintersectionnum(o);
    // Is intersection beyond end of line?
    if (l<0 || l>1.0)
      return NULL;
    return new V2d(a+l*(b-a));
  }
  V2d findintersection(Line2d o) {
//    printf("Finding intersection of %s with %s\n",toString(),o.toString());
    float l=findintersectionnum(o);
//    printf("Num %f Pos %s\n",l,(a+l*(b-a)).toString());
    return a+l*(b-a);
  }
  V2d intersection(Line2d o) {
    return findintersection(o);
  }
  V2d intersect(Line2d o) {
    return findintersection(o);
  }
  bool crosses(Line2d *o) {
//    //flibble(*this,2);
//    //flibble(*o,3);
    float l=findintersectionnum(o);
    float k=o->findintersectionnum(this);
    if (l<0 || l>1.0 || k<0 || k>1.0)
      return false;
    else {
      V2d *x=findintersectionornull(*o);
      if (x==NULL)
        error("shouldn't be NULL!");
//      fobble(x);
      return true;
    }
  }
  bool crosses(Line2d o) {
    return crosses(&o);
  }
  V2d end(int i) {
    if (i==1)
      return a;
    if (i==2)
      return b;
  }
  Line2d operator+(V2d v) {
    return Line2d(a+v,b+v);
  }
  Line2d operator*(float f) {
    return Line2d(a*f,b*f);
  }

  String toString() {
    return Sformat("Line %s-%s",a.toString(),b.toString());
  }

  static float getlen(Line2d l) {
    return l.length;
  }

  // Set intersection point only if lines are not parallel
  void setintifnotpar(Line2d ol,V2d *in) {
    float d=V2d::normdot(b-a,ol.b-ol.a);
    if (myabs(d)<0.999) {
      *in=intersect(ol);
      refresh();
    }
  }

  void clipbyrectangle(float l,float t,float r,float bo) {
//    printf("Clipping line %s\n",toString());
    if (a.x<l)
      setintifnotpar(Line2d(V2d(l,t),V2d(l,bo)),&a);
    if (b.x<l)
      setintifnotpar(Line2d(V2d(l,t),V2d(l,bo)),&b);
    if (a.x>=r)
      setintifnotpar(Line2d(V2d(r,t),V2d(r,bo)),&a);
    if (b.x>=r)
      setintifnotpar(Line2d(V2d(r,t),V2d(r,bo)),&b);
    if (a.y<t)
      setintifnotpar(Line2d(V2d(l,t),V2d(r,t)),&a);
    if (b.y<t)
      setintifnotpar(Line2d(V2d(l,t),V2d(r,t)),&b);
    if (a.y>=bo)
      setintifnotpar(Line2d(V2d(l,bo),V2d(r,bo)),&a);
    if (b.y>=bo)
      setintifnotpar(Line2d(V2d(l,bo),V2d(r,bo)),&b);
    refresh();
//    printf("Clipped to %s\n",toString());
  }

  V2d center() {
    return (a+b)/2.0;
  }

  int whichsideis(V2d v) {
    return mysgn(v.dot((b-a).perp()));
  }

  Line2d expandedBy(float dist) {
    V2d dir=(b-a).normalised();
    return Line2d(a-dir*dist,b+dir*dist);
  }
  Line2d expandedForever() {
    V2d len=(b-a);
    float bignum=100000000.0/len.mag();
    return expandedBy(bignum);
  }
  Line2d expandedTo(float wid) {
    float len=(b-a).mag();
    float diff=(wid-len)/2.0;
    return expandedBy(diff);
  }

};

Line2d operator*(float f,Line2d l) {
  return l*f;
}

class QuadsLine2d : public Line2d {
public:
  // Don't know what these were for!  Some PhD stuff no doubt.
  List<List<QuadsLine2d *> > ls;//=List<List<Line2d *> >();//2,List<Line2d *>());
  List<List<int> > es;//=List<List<int> >(2,List<int>());
  bool allusedup;

  QuadsLine2d() {
    allusedup=false;
  }
  QuadsLine2d(V2d aa,V2d bb) {
    allusedup=false;
    a=aa;
    b=bb;
    refresh();
    ls.add(List<QuadsLine2d *>());
    ls.add(List<QuadsLine2d *>());
    es.add(List<int>());
    es.add(List<int>());
  }
  QuadsLine2d operator*(float f) {
    return QuadsLine2d(a*f,b*f);
  }

};
