#ifndef line2d_H
  #define line2d_H

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
  V2d a,b; // Exists

  float length; // Exists

// Don't know what these were for!  Some PhD stuff no doubt.
//  List<List<Line2d *> > ls;//=List<List<Line2d *> >();//2,List<Line2d *>());
//  List<List<int> > es;//=List<List<int> >(2,List<int>());
//  bool allusedup;

   Line2d(); // Method

   Line2d(V2d aa,V2d bb); // Method

  void refresh(); // Method

  void swapends(); // Method

  float findintersectionnum(Line2d *o); // Method

  float findintersectionnum(Line2d o); // Method

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
  V2d *findintersectionornull(Line2d o); // Method

  V2d findintersection(Line2d o); // Method

  V2d intersection(Line2d o); // Method

  V2d intersect(Line2d o); // Method

  bool crosses(Line2d *o); // Method

  bool crosses(Line2d o); // Method

  V2d end(int i); // Method

  Line2d operator+(V2d v); // Method

  Line2d operator*(float f); // Method


  String toString(); // Method


  static float getlen(Line2d l); // Method


  // Set intersection point only if lines are not parallel
  void setintifnotpar(Line2d ol,V2d *in); // Method


  void clipbyrectangle(float l,float t,float r,float bo); // Method


  V2d center(); // Method


  int whichsideis(V2d v); // Method


  Line2d expandedBy(float dist); // Method

  Line2d expandedForever(); // Method

  Line2d expandedTo(float wid); // Method


};

Line2d operator*(float f,Line2d l); // Method


class QuadsLine2d : public Line2d {
public:
  // Don't know what these were for!  Some PhD stuff no doubt.
  List<List<QuadsLine2d *> > ls; // Exists//=List<List<Line2d *> >();//2,List<Line2d *>());
  List<List<int> > es; // Exists//=List<List<int> >(2,List<int>());
  bool allusedup; // Exists

   QuadsLine2d(); // Method

   QuadsLine2d(V2d aa,V2d bb); // Method

  QuadsLine2d operator*(float f); // Method


};

#endif
