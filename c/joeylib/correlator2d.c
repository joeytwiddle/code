int Correlator2dMethod=1; // 1 = over-exclusive, 2 = over-inclusive

#define linearMode 0
#define joeysMode 1
#define perpMode 2

class Correlator2d {
public:
	int mode;
  List<V2d> points; // Points from which to find a line of best fit
  List<float> weights; // Weightings of the points

  int min;
  int maxposs;

  boolean made;
  // Redundant information (only exists after processing above)
  float bestang;
  List<int> pointskept;
  V2d centroid;
  float total;
  float besterr;

  Correlator2d() {
		// mode=perpMode;
		mode=joeysMode;
    made=false;
  }
  Correlator2d(List<V2d> pts) {
    points=pts;
    made=false;
  }

  void add(V2d v,float w) {
    points.add(v);
    weights.add(w);
    made=false;
  }
  void add(V2d v) {
    add(v,1.0);
  }
  void add(Pixel p) {
    add(V2d(p));
  }

  /* float total() {
    float total=0.0;
    for (int i=0;i<weights.len;i++)
      total+=weights.get(i);
    return total;
  }
  V2d centroid() {
    V2d cen=V2d(0,0);
    for (int i=0;i<points.len;i++)
      cen=cen+points.get(i);
    return cen/total();
  }
  void normalise() { // Normalises all points around centroid
    V2d cen=centroid();
    for (int i=0;i<points.len;i++)
      points.replace(i+1,points.get(i)-cen); // non-Java compensation
  } */

  void make() {
    if (!made)
      remake();
  }

  void addposs(List<int> n,List<List<int> > *poss,List<V2d> *cens,List<float> *totals) {
    poss->add(n);
        float total=0;
        V2d cen=V2d(0,0);
        for (int j=0;j<n.len;j++) {
          int k=n.get(j);
          cen=cen*total;
          cen=cen+points.get(k)*weights.get(k);
          total+=weights.get(k);
          cen=cen/total;
        }
    totals->add(total);
    cens->add(cen);
  }

  void remake() {

    if (points.len<=2)
      myerror("Correlator2d: I do not correlate only 2 points!");

    // What is the minimum number of points?
    min=( points.len<4 ? points.len : 4 );

    maxposs=-1; // not in use

    // Find best angle

    // Setup
    bestang=-1.0;
    besterr=inf;
    int bestposs=-1;

    // Try some random subsets
    printf("Trying random sets...\n");
    List<int> bestps;
    List<int> ps; // Declared outside because it may be efficient
    for (int i=1;i<200;i++) {
      ps.clear();
      int j=intrnd(min,points.len);
//   printf("Trying random set size %i/%i: ",j,points.len);
      for (int k=0;k<j;k++) {
        int l=intrnd(0,points.len-1);
        while (ps.contains(l)) {
          l=intrnd(0,points.len-1);
        }
        ps.add(l);
//    printf("%i ",l);
      }
//   printf("\n");
      if (tryset(ps)) {
        bestps.clear();
        bestps.add(ps);
//    printf("!!! Found a new winner %i : %s!\n",ps.len,ps.toString());
//    bestps.add(ps);
      }
    }

    printf("Best so far %s\n",bestps.toString());

    // So we have a fairly good match
    // Now we wish to try adding and subtracting points to get a better match
    printf("Trying to improve best...\n");
    boolean changed=true;
    while (changed) {
      changed=false;
      // For all points in whole set
      for (int i=0;i<points.len;i++) {
        ps.clear();
        ps.add(bestps);
        boolean valid=true;
        // Add or remove point from best set
        if (bestps.contains(i)) {
          if (bestps.len>min) {
           ps.remove(i);
//      printf("Trying %s without %i\n",ps.toString(),i);
          } else // cannot remove i since list too small
           valid=false; // do not try anything this time round
        } else {
          ps.add(i);
//     printf("Trying %s with %i\n",ps.toString(),i);
        }
        if (valid) {
          if (tryset(ps)) {
           changed=true;
           printf("*** Found a better set %s\n",ps.toString());
           bestps.clear(); // Replace immediately!
           bestps.add(ps);
          }
        }
      }
    } // If nothing has changed, exit

    printf("Best angle %f with %s (%i/%i)\n",bestang,bestps.toString(),bestps.len,points.len);
    pointskept.clear(); // Can't set pk = bps because bps is local!!!
    pointskept.add(bestps);
    centroid=centroidfor(bestps);
    total=totalfor(bestps);
    // return bestang;
    made=true;
  }

  float totalfor(List<int> ps) {
    float total=0;
    for (int i=0;i<ps.len;i++)
      total+=weights.get(ps.get(i));
    return total;
  }

  V2d centroidfor(List<int> ps) {
    V2d cen=V2d(0,0);
    for (int i=0;i<ps.len;i++) {
      int j=ps.get(i);
      cen=cen+weights.get(j)*points.get(j);
    }
    return cen/totalfor(ps);
  }

  boolean tryset(List<int> ps) {
    return tryset(ps,centroidfor(ps),totalfor(ps));
  }

  boolean trysetJoeysMode(List<int> ps,V2d cen,float total) {
    float changed=false;
    // Try all angles
    float angstep=pi/500.0;
    for (float ang=-pi;ang<=pi;ang+=angstep) {
      V2d perp=V2d::angle(ang).perp();
        if (ps.len>=min) {
          // Find error for each point
          float es=0.0;
          for (int j=0;j<ps.len;j++) {
           V2d v=points.get(ps.get(j))-cen;
           float e=mysquare(v.dot(perp))*weights.get(ps.get(j));
//      err.replace(i+1,e); // non-Java compensation
           es+=e; // err.get(ps.get(j));
          }
          if (Correlator2dMethod==1)
           es=es/total/total/mysquare(ps.len)/mysquare(log(ps.len));
          else
           es=es/total/total/mysquare(ps.len)/mysquare(ps.len)/mysquare(ps.len);
          if (es<besterr) {
           bestang=ang;
           besterr=es;
//      bestposs=i;
           changed=true;
          }
        }
    }
    return changed;
	}

  boolean trysetPerpMode(List<int> ps,V2d cen,float total) {
		float sumx=0;
		float sumxsqd=0;
		float sumy=0;
		float sumysqd=0;
		float sumxy=0;
		for (int i=0;i<ps.len;i++) {
			V2d *v=points.p2num(i+1);
			sumx+=v->x;
			sumxsqd+=v->x*v->x;
			sumy+=v->y;
			sumysqd+=v->y*v->y;
			sumxy+=v->x*v->y;
		}
		float oneovrn=1.0/(float)ps.len;
		float topleft=sumysqd-oneovrn*sumy*sumy;
		float topright=sumxsqd-oneovrn*sumx*sumx;
		float bottom=oneovrn*sumx*sumy-sumxy;
		double B;
		if (bottom*bottom<0.000001) {
			B=1.0e100;
		} else {
			B=0.5*(topleft-topright)/bottom;
		}
		double b1=-B+sqrt(B*B+1.0);
		double b2=-B-sqrt(B*B+1.0);
		float a1=(sumy-b1*sumx)*oneovrn;
		float a2=(sumy-b2*sumx)*oneovrn;
		float err1=errPerpMode(ps,a1,b1);
		float err2=errPerpMode(ps,a2,b2);
		if (err2<err1) {
			err1=err2;
			a1=a2;
			b1=b2;
		}
		if (err1<besterr) {
			besterr=err1;
			bestang=atan(1.0/b1);
			return true;
		}
		return false;
	}

	float errPerpMode(List<int> ps,float a,float b) {
		float err=0;
		for (int i=0;i<ps.len;i++) {
			V2d *v=points.p2num(i+1);
			err+=mysquare(v->y-(a+b*v->x));
		}
		err=err/(1+b*b);
    err=err/mysquare(mysquare(ps.len));
		return err;
	}

  boolean tryset(List<int> ps,V2d cen,float total) {
		if (mode==joeysMode)
			return trysetJoeysMode(ps,cen,total);
		if (mode==perpMode)
			return trysetPerpMode(ps,cen,total);
  }

  float angle() {
    make();
    return bestang;
  }
  float error() {
    make();
    return besterr;
  }
  Line2d line() {
    make();
    V2d ori=V2d::angle(bestang);
    return Line2d(centroid,centroid+ori);
  }

  float crossesy() {
    make();
    Line2d l=line();
    Line2d x=Line2d(V2d(0,0),V2d(1,0));
    // return l.intersect(x).x;
    printf("Old method got %f\n",l.intersect(x).x);
    // y=Ax+B
    double A=(l.a.y-l.b.y)/(l.a.x-l.b.x);
    double B=l.a.y-A*l.a.x;
    double xwhenyzero=-B/A;
    printf("New method got %f\n",xwhenyzero);
    return xwhenyzero;
  }

  void remakebruteforce() { // Pretty useless because exponential
    // I have tried to do as many subsets as possible but naff.

    if (points.len<=2)
      myerror("Correlator2d: I do not correlate only 2 points!");

    // What is the minimum number of points?
    min=(points.len+1)/2;
    if (min<3)
      min=3;

    maxposs=400;

    // Estimate number of combinations
    int howmany=pow(2.0,points.len)*(1.0-(float)(min)/(float)points.len);
    if (howmany>maxposs) {
      printf("%i points means %i possibilities!\n",points.len,howmany);
      printf("I cannot do that many, so I won't be able to check all possibilities.\n");
      for (;min<points.len-1;min++) {
        howmany=pow(2.0,points.len)*(1.0-(float)(min)/(float)points.len);
        if (howmany<=maxposs)
          break;
      }
      printf("Therefore ensuring min %i points, meaning %i possibilities.\n",min,howmany);
    }

//  normalise();

    // Set up possible subsets of points and each ones centroid and total
        int cnt=0;
        List<List<int> > poss;
        List<V2d> cens;
        List<float> totals;

        List<int> ntmp;
        for (int i=0;i<points.len;i++)
           ntmp.add(i);
        addposs(ntmp,&poss,&cens,&totals);

        for (int i=0;i<points.len;i++) {
           int sofar=poss.len;
           for (int j=0;j<sofar;j++) {
             if (poss.get(j).len>min) {
               List<int> n;
               n.add(poss.get(j));
               int k=n.find(i);
               if (k<=0)
                 myerror(Sformat("Didn't find %i in %s\n",i,n.toString()));
               n.removenum(k);
               addposs(n,&poss,&cens,&totals);
             }
           }
        }

/* Old way of building - overfills memory if points.len high
      regardless of min!
        for (int i=0;i<points.len;i++) {
           int sofar=poss.len;
           for (int j=0;j<sofar;j++) {
             // Leaving the original there,
             // create a new list which includes i
             List<int> n; // =List<int>(&poss.get(j));
             n.add(poss.get(j));
             n.add(i);
             poss.add(n);
if (n.len>=min)
    cnt++;
             // And a total for the new list
             float t=totals.get(j);
             t+=weights.get(i);
             totals.add(t);
             // And a centroid for the new list
             V2d c=cens.get(j);
             c = ( c*totals.get(j) + points.get(i)*weights.get(i) ) / totals.get(totals.len-1);
             cens.add(c);
           }
        } */

        /* Crap binary way - still takes too long!
        int totalposs=pow(2.0,points.len);
        for (int p=pow(2.0,min)-1;p<totalposs;p++) {
           int incnt=0;
           for (int i=0;i<points.len;i++)
             if (bingetbit(i,p)==1)
               incnt++;
           if (incnt>=min) {
             List<int> n;
             for (int i=0;i<points.len;i++)
               if (bingetbit(i,p)==1)
                 n.add(i);
             poss.add(n);
             float total=0;
             V2d cen=V2d(0,0);
             for (int j=0;j<n.len;j++) {
               int k=n.num(j);
               cen=cen*total;
               cen=cen+points.get(k)*weights.get(k);
               total+=weights.get(k);
               cen=cen/total;
             }
             totals.add(total);
             cens.add(cen);
           }
        } */

        printf("%i points: checking %i=%i possible combinations\n",points.len,howmany,poss.len);
        // Find best angle
        // Setup
        bestang=-1.0;
        besterr=inf;
        int bestposs=-1;
//    List<float> err;
//    for (int i=0;i<points.len;i++)
//      err.add(-1);
/*      for (int i=0;i<points.len;i++) {
             V2d v=points.get(i);
             float e=mysquare(v.dot(perp))*weights.get(i);
             err.replace(i+1,e); // non-Java compensation
           } */
           // Find error-sum for each subset
           List<int> ps; // Declared outside because it may be efficient
           for (int i=0;i<poss.len;i++) {
             ps=poss.get(i);
             if (tryset(ps,cens.get(i),totals.get(i)))
               bestposs=i;
           }
        printf("Best angle %f with %s\n",bestang,poss.get(bestposs).toString());
        pointskept=poss.get(bestposs);
        centroid=cens.get(bestposs);
        total=totals.get(bestposs);
        // return bestang;
        made=true;
    }

    bool used(int i) { /* i from 1..size */
        return (pointskept.find(i-1)>0);
    }

    // See drawCorrelator2d(c) in region.c

};





/* RGBmp drawCorrelator2d(Correlator2d cc) {
    Region r=Region(cc.points);
    int left=r.listleftmost()-5;
    int right=r.listrightmost()+5;
    int top=r.listtopmost()-5;
    int bottom=r.listbottommost()+5;
    if (bottom<0 && top<0)
        bottom=0;
    if (top>0 && bottom>0)
        top=0;
    int wid=right-left;
    int hei=bottom-top;
    int mw = ( wid>hei ? 400 : 400*wid/hei );
    int mh = ( wid>hei ? 400*hei/wid : 400 );
  RGBmp m=RGBmp(mw,mh,myRGB::black);
  float symbolsize=5.0*(float)cc.points.len/(float)cc.total;
  printf("Plotting set %s\n",cc.pointskept.toString());
  for (int i=1;i<=cc.points.len;i++) {
    Pixel p=Pixel(m.width*(cc.points.num(i).x-left)/wid,
              m.height*(cc.points.num(i).y-top)/hei);
    p.y=m.height-1-p.y; // invert height so y goes up!
    myRGB colour=myRGB::white;
    if (!cc.pointskept.contains(i-1)) {
      colour=myRGB::red;
//   m.circle(p,symbolsize*cc.weights.num(i),myRGB::red);
    }
    m.cross(p,symbolsize*cc.weights.num(i),colour);
  }
  V2d c=cc.centroid;
  V2d mc=V2d(m.width*(c.x-left)/wid,m.height*(c.y-top)/hei);
  mc.y=m.height-1-mc.y;
  V2d dir=V2d::angle(cc.bestang)*200;
  dir.y=-dir.y;
  m.line(mc-dir,mc+dir,myRGB::yellow);
  return m;
} */


Map2d<bool> drawCorrelator2d(Correlator2d cc) {
  Region r=Region(cc.points);
  int left=r.listleftmost()-5;
  int right=r.listrightmost()+5;
  int top=r.listtopmost()-5;
  int bottom=r.listbottommost()+5;
  if (bottom<0 && top<0)
    bottom=0;
  if (top>0 && bottom>0)
    top=0;
  int wid=right-left;
  int hei=bottom-top;
  int mw = ( wid>hei ? 400 : 400*wid/hei );
  int mh = ( wid>hei ? 400*hei/wid : 400 );
  Map2d<bool> m=Map2d<bool>(mw,mh,false);
  float symbolsize=5.0*(float)cc.points.len/(float)cc.total;
  for (int i=1;i<=cc.points.len;i++) {
    Pixel p=Pixel(m.width*(cc.points.num(i).x-left)/wid,
              m.height*(cc.points.num(i).y-top)/hei);
    p.y=m.height-1-p.y; // invert height so y goes up!
    m.cross(p,symbolsize*cc.weights.num(i),true);
    //if (cc.pointskept.find(i-1)==0)
    if (!cc.used(i))
      m.circle(p,symbolsize*cc.weights.num(i),true);
  }
  V2d c=cc.centroid;
  V2d mc=V2d(m.width*(c.x-left)/wid,m.height*(c.y-top)/hei);
  mc.y=m.height-1-mc.y;
  V2d dir=V2d::angle(cc.bestang)*200;
  dir.y=-dir.y;
  m.line(mc-dir,mc+dir,true);
  return m;
}

RGBmp drawCorrelator2dInColour(Correlator2d cc) {
  Region r=Region(cc.points);
  int left=r.listleftmost()-20;
  int right=r.listrightmost()+20;
  int top=r.listtopmost()-20;
  int bottom=r.listbottommost()+20;
  if (bottom<0 && top<0)
    bottom=0;
  if (top>0 && bottom>0)
    top=0;
  int wid=right-left;
  int hei=bottom-top;
  int mw = ( wid>hei ? 600 : 600*wid/hei );
  int mh = ( wid>hei ? 600*hei/wid : 600 );
  RGBmp m=RGBmp(mw,mh,myRGB::white);
  float symbolsize=5.0*(float)cc.points.len/(float)cc.total;
  for (int i=1;i<=cc.points.len;i++) {
    Pixel p=Pixel(m.width*(cc.points.num(i).x-left)/wid,
              		m.height*(cc.points.num(i).y-top)/hei);
    p.y=m.height-1-p.y; // invert height so y goes up!
    if (cc.used(i))
      m.cross(p,symbolsize*cc.weights.num(i),myRGB::darkgreen);
    else
      m.opencircle(p,symbolsize*cc.weights.num(i),myRGB::darkred);
  }
  V2d c=cc.centroid;
  V2d mc=V2d(m.width*(c.x-left)/wid,m.height*(c.y-top)/hei);
  mc.y=m.height-1-mc.y;
  V2d dir=V2d::angle(cc.bestang)*200;
  dir.y=-dir.y;
  m.line(mc-dir,mc+dir,myRGB::black);
  return m;
}
