#ifndef maths2_C
  #define maths2_C

  #include <maths2.h>

//RGBmp *OUTLIERIMAGE;
List<V2d> Excled;

myRGB pull(myRGB a,float t,myRGB b) {
  int dr=(int)(t*(b.r-a.r));
  int dg=(int)(t*(b.g-a.g));
  int db=(int)(t*(b.b-a.b));
  return myRGB(a.r+dr,a.g+dg,a.b+db);
}

V2d Correlator::centroid() {
  	V2d centroid=V2d(0,0);
  	for (int i=1;i<=xs.len;i++) {
  		centroid=centroid+V2d(xs.num(i),ys.num(i))*ws.num(i);
  	}
  	centroid=centroid/totalweight();
  	return centroid;
}

float Correlator::angerror(float a) {
    V2d ctr=centroid();
    float error=0;
    for (int i=1;i<=xs.len;i++) {
      V2d v=V2d(xs.num(i),ys.num(i))-ctr;
      error+=sqrt(square(v.x*sin(a)+v.y*cos(a)))*ws.num(i);
    }
    return error/totalweight();
}

V2d Correlator::ori() {
  float a=angle();
  return V2d(sin(a),cos(a)).norm();
}

Line2d Correlator::line() {
  V2d c=centroid();
  V2d o=ori();
  return Line2d(c,c+o);
}

float Correlator::length() {
  V2d ctr=centroid();
  V2d lori=ori();
  float len=0;
  for (int i=1;i<=xs.len;i++) {
    float d=V2d::dot(lori,V2d(xs.num(i),ys.num(i))-ctr);
    if (myabs(d)>len)
      len=myabs(d);
  }
  return len;
}

// Not compensated for weighting
float Correlator::fatness() {
  V2d ctr=centroid();
  V2d lori=ori().perp().norm();
  float fat=0;
  for (int i=1;i<=xs.len;i++) {
    float d=V2d::dot(lori,V2d(xs.num(i),ys.num(i))-ctr);
    if (myabs(d)>fat)
      fat=myabs(d);
  }
  return fat;
}

  void Correlator::add(V2d p) {
    add(p.x,p.y,1.0);
  }

  void Correlator::add(V2d p,float w) {
    add(p.x,p.y,w);
  }

  void Correlator::add(List<V2d> ls) {
    for (int i=1;i<=ls.len;i++)
      add(ls.num(i));
  }
	
	float Correlator::angle() {
	
//	  return atan(sumx()/sumy());
	
/*  	double t=0;
  	V2d c=centroid();
  	for (int i=1;i<=xs.len;i++)
  	  t=t+myabs(xs.num(i)-c.x)/myabs(ys.num(i)-c.y);
//  	  t=t+(xs.num(i)-c.x)/(ys.num(i)-c.y);
  	return atan(t);*/
	
	  // Search!
    if (ang==NULL) {
      float best=angerror(0);
      float bestt=0;
      for (float t=0;t<pi;t=t+pi/500.0) {
        float e=angerror(t);
        if (e<best) {
          best=e;
          bestt=t;
        }
      }
      ang=new float[1];
      *ang=bestt+pi/2.0; // Should make it clock-like
    }
    return *ang;
  }

  float Correlator::error() {
    if (ang==NULL)
      float dummy=angle();
    return angerror((*ang)-pi/2.0);
  }

  // None of the rest has been compensated for weighting
	float Correlator::crossesy() {
	  float g=grad();
	  // y=Ax+b b=y-Ax x=-b/A
	  V2d cen=centroid();
	  float b=cen.y-g*cen.x;
	  float x=-b/g;
	  return x;
	}
	
	float Correlator::crossesyxoutliers() {
	  float g=bestgradxoutliers();
	  // y=Ax+b b=y-Ax x=-b/A
	  V2d cen=centroid();
	  float b=cen.y-g*cen.x;
	  float x=-b/g;
	  return x;
	}
	
	float Correlator::yforx(float x) {
//	  float a=angle();
	  float g=grad();
	  V2d cen=centroid();
	  float b=cen.y-g*cen.x;
	  return g*x+b;
	}

float Correlator::bestanglexoutliers(int k) {
	  // Could make a line then exclude the furthest
	  float a=angle();
	  if (k<=0)
	    return a;
	  float worst=0;
	  int worsti=0;
	  V2d cen=centroid();
	  for (int i=1;i<=xs.len;i++) {
	    float dist=square((xs.num(i)-cen.x)*sin(a-pi/2.0)+(ys.num(i)-cen.y)*cos(a-pi/2.0));
	    if (dist>worst) {
	      worst=dist;
	      worsti=i;
	    }
	  }
	  if (worsti==0)
	    return a;
	  else {
	    V2d v=V2d(xs.num(worsti),ys.num(worsti));
	    printf("Excluded num %i dist %f\n",worsti,worst);
	    Excled.add(v);
	    //OUTLIERIMAGE->line(v-V2d(5,5),v+V2d(5,5),myRGB::red);
	    //OUTLIERIMAGE->line(v+V2d(-5,5),v+V2d(5,-5),myRGB::red);
	  }
	  List<Pixel> ps;
	  for (int i=1;i<=xs.len;i++)
	    if (i!=worsti)
	      ps.add(Pixel(xs.num(i),ys.num(i)));
	  Correlator c=Correlator(&ps);
	  float ca=c.bestanglexoutliers(k-1); // Do not use () !
	  c.freedom();
	  ps.freedom();
	  return ca;
	
/*	  // Current method: try all combinations without 1, without 2, without 3
    float besterr=1000000000000.0;
    float bestang=0;
    for (int excl=1;excl<=3;excl++) {
      int combs=1;
      for (int i=0;i<excl;i++)
        combs=combs*(xs.len-i);
      for (int i=0;i<combs;i++) {
        List<int> ls=decompose(i,excl);
        Correlator c=Correlator();
        for (int j=1;j<=xs.len;j++)
          if (!ls.contains(j))
            c.add(xs.num(j),ys.num(j));
        float a=c.angle();
        float e=c.angerror(a-pi/2.0);
        if (e<besterr) {
          besterr=e;
          bestang=a;
          printf("%f is best with error %f\n",a,e);
          printf("Excluded: %s\n",ls.toString());
        }
        ls.freedom();
        c.freedom();
      }
    }
    return bestang;*/

  }

  // See drawCorrelator(c) in map2d.c


float gabor(V2d v,V2d d,float size,float freq) {
  return gaussian(v.mag()/size)*sin(v.dot(d)*freq);
}

float gaborabs(V2d v,V2d d,float size,float freq) {
//  return gaussian(v.mag()/size)*sin(v.dot(d)*freq);
}





// A circle that maps to the whole R^2 plane.
// Points at radius 1 on the circle map to infinity on the plane.

// method 1 still favours infinity!
// boundedinfcirrad = 1.0-1.0/(planerad/Z+1.0)
// planerad = Z(1.0/(1.0-boundedinfcirrad)-1.0)

// method 2
// planerad = exp( original )
// boundedinfcirrad = 1.0-1.0/(log(planerad)/Z+1.0)

// method 3
// planerad = exp( max * infcirrad/res ) ( where infcirrad/res \elem {0-1} )
// infcirrad = res * log(planerad) / max
				
// Methods 1,2,3:
float maxinfcir=1.0e3;
// float maxinfcir=5.0; // takes us to 4.0e208 at res 50
// float maxinfcir=20.0; // takes us to 4.0e208 at res 50

  V2d infcircletoplane(V2d v) {
    float r=v.mod();
    float a=v.angle();
    if (r>=1.0)
      printf("infcircletoplane: rad = %f > 1.0 !\n",r);
		// Methods 1,2,3:
    float d=maxinfcir*(1.0/(1.0-r)-1.0);
    // float d=exp(maxinfcir*(1.0/(1.0-r)-1.0));
    // float d=exp(maxinfcir*r);
    V2d pol=V2d::polar(a,d);
    return pol;
  }

  V2d planetoinfcircle(V2d v) {
    float d=v.mod();
    float a=v.angle();
		// Methods 1,2,3:
    float r=1.0-1.0/(d/maxinfcir+1.0); // I think
    // float r=1.0-1.0/(log(d)/maxinfcir+1.0);
    // float r=log(d)/maxinfcir;
    return V2d::polar(a,r);
  }


#endif
