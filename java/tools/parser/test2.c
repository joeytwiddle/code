
class V2d;

class Correlator {
public:
	List<float> xs;
	List<float> ys;
	float *cxy;
	float *cxx;
	float *cyy;
	float *ang;
	Correlator() {
	  cxy=cxx=cyy=ang=NULL;
	}
	void add(V2d p);
	void add(float x,float y) {
	  xs.add(x);
	  ys.add(y);
  }
	Correlator(List<float> x,List<float> y) {
	  cxy=cxx=cyy=ang=NULL;
		xs=x;
		ys=y;
	}
	Correlator(List<Pixel> *ps) {
	  cxy=cxx=cyy=ang=NULL;
		for (int i=1;i<=ps->len;i++) {
			xs.add(ps->num(i).x);
			ys.add(ps->num(i).y);
		}
	}
	float corrxy() {
	  if (cxy==NULL) {
	    cxy=new float[1];
	    *cxy=corrS(xs,ys);
	  }
	  return *cxy;
	}
	float corrxx() {
	  if (cxx==NULL) {
	    cxx=new float[1];
	    *cxx=corrS(xs,xs);
	  }
	  return *cxx;
	}
	float corryy() {
	  if (cyy==NULL) {
	    cyy=new float[1];
	    *cyy=corrS(ys,ys);
	  }
	  return *cyy;
	}
	
  V2d centroid();
	float angerror(float);
  V2d ori();
  float length();
  float fatness();
	float angle(); // In clock form I believe
	float crossesy();
	float crossesyxoutliers();
	float yforx(float x);
	
	float grad() {
	  float a=angle();
	  float g=cos(a)/sin(a);
	  printf("++++++++++ Got grad %f, but A %f\n",g,A());
	  return g;
	}
	
};
