// Projection profiler, with vanishing points

/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

// #define USINGJFONT
#define QUIET

#include <joeylib.c>

int outcount=0;

float (*measure)(Map1d<int>);

float measureentropy(Map1d<int> m) {
  return m.entropy();
}
float measurederiv(Map1d<int> m) {
  return -m.totalderiv();
}
float measuresquare(Map1d<int> m) {
  return -m.totalsquare();
}
float measurevariance(Map1d<int> m) {
  return -m.variance();
}

// A vanishing point with its projection profile
// which will be accumulated using the add() function
class VP {
public:
  V2d pos;
  Map1d<int> pp;
  float left,right; // relative to vector to centre
  V2d tocentre;
  float realleft; // absolute
  int q;

  // Set up the window of angles over which the VP can
  // see the text region.
  // Set up optimization variables to make the add()
  // function efficient.
  VP(V2d p,int res,int width) {
    pos=p;
    tocentre=p.neg().norm();
    pp=Map1d<int>(res); // => pp.width==res
    left=V2d::angbetween(width*tocentre.perp()-pos,tocentre);
    right=V2d::angbetween(-width*tocentre.perp()-pos,tocentre);
    if (right<left)
      right=right+2.0*pi;
    q=res/(right-left);
    realleft=modtwopi(tocentre.angle()+left);
//    printf("VP %s l=%.2f r=%.2f\n",pos.toString(),left,right);
  }

  // Add to the projection profile a pixel at point v
  void add(V2d v) { // accepts a +- vector relative to centre of image, with mag<=width above
//    printf("%s %s\n",v.toString(),pos.toString());
    // Fastest method
    float ang=modtwopi((v-pos).angle()-realleft);
    int l=ang*q;
    // Slower method
    /* float ang=V2d::angbetween(v-pos,tocentre);
    if (ang<left)
      ang=ang+2.0*pi;
    int l=(ang-left)*q; */
    // Even slower method
    /* float ang=V2d::angbetween(v-pos,tocentre);
    int l=modtwopi(ang-left)*(float)pp.width/(right-left); */
    if (l>=0 && l<pp.width)
      pp.incpos(l);
    else {
      outcount++;
//      printf("%s: %s (i%i) out of range %i (l%f,%f,r%f),%i\n",pos.toString(),v.toString(),l,pp.width,left,ang,right,q);
    }
  }

};

float tofloat(VP *v) {
  return 0.0;
}

class ProjectionProfiler {
public:
  int res,ppres;
  Map2d<VP *> vps;
  Map2d<bool> binimg;
  Pixel best;

  ProjectionProfiler(int c,int p,Map2d<bool> b) {
    res=c;
    ppres=p;
    binimg=b;
  }

  // From vps map to infinity circle
  V2d maptocircle(int i,int j) {
    return (V2d(i,j)-V2d((float)res/2.0,(float)res/2.0))/((float)res/2.0);
  }

  // From infinity circle to vps map
  Pixel mapfromcircle(V2d v) {
    return (float)res/2.0*(v+V2d(1.0,1.0))+V2d(.5,.5);
  }

  // Initialises a set of vanishing points for which
  // projection profiles will be calculated
  // VPs are distributed "evenly" across R^2
  void setup() {
    int width=sqrt(mysquare(binimg.width)+mysquare(binimg.height))/2;
    vps=Map2d<VP *>(res,res);
    for (int i=0;i<res;i++)
    for (int j=0;j<res;j++) {
      V2d pos=maptocircle(i,j);
//      printf("Circle=%s\n",pos.toString());
      vps.setpos(i,j,NULL);
      if (pos.mag()<1.0) {
        // We offset the position of the VP slightly
        // so that the one at (0,0) is resolvable!
        V2d realpos=infcircletoplane(pos+V2d(0.0000001,0.0000001));
        if (realpos.mag()>width*1.5) {
//          printf("Plane=%s\n",realpos.toString());
          vps.setpos(i,j,new VP(realpos,ppres,width));
        }
      }
    }
  }

  // Accumulates projection profiles of the points
  // in the input image
  void calculate() {
    int w=binimg.width;
    int h=binimg.height;
    int hw=w/2;
    int hh=h/2;
    int vw=vps.width;
    int vh=vps.height;
    for (int i=0;i<w;i++) {
      if (i%17==0) // Learn your 17 times table as the
        printf("%i/%i\n",i,w); // program runs!
      for (int j=0;j<h;j++) {
        if (binimg.getpos(i,j)) {
          V2d pos=V2d(i-hw,j-hh);
          for (int a=0;a<vw;a++)
          for (int b=0;b<vh;b++) {
            VP *vp=vps.getpos(a,b);
            if (vp!=NULL)
              vp->add(pos);
          }
        }
      }
    }
  }

  // Finds the entropy of all the projection profiles
  // and puts them in a map
  Map2d<float> getppmap() {
    Map2d<float> map=Map2d<float>(vps.width,vps.height,-1234567.8e9);
    for (int i=0;i<vps.width;i++)
    for (int j=0;j<vps.height;j++)
      if (vps.getpos(i,j)!=NULL) {
        map.setpos(i,j,measure(vps.getpos(i,j)->pp));
//        map.setpos(i,j,-vps.getpos(i,j)->pp.totalsquare());
//        map.setpos(i,j,-vps.getpos(i,j)->pp.totalderiv());
//        map.setpos(i,j,vps.getpos(i,j)->pp.entropy());
      }
    map.searchreplace(-1234567.8e9,map.largest());
    return map;
  }

  // Finds the best vanishing point by finding the
  // PP with lowest entropy
  V2d getvp() {
    Map2d<float> map=getppmap();
    float lowest=0.0;
    V2d vp;
    for (int i=0;i<vps.width;i++)
    for (int j=0;j<vps.height;j++)
      if (vps.getpos(i,j)!=NULL)
        if (map.getpos(i,j)<lowest) {
          lowest=map.getpos(i,j);
          printf("Lowest so far %i,%i with %f\n",i,j,lowest);
          best=Pixel(i,j);
          vp=vps.getpos(i,j)->pos+V2d(binimg.width/2,binimg.height/2);
          // Draw this "good" pp
          Map2d<bool> acc=vps.getpos(i,j)->pp.draw();
          acc.writefile(Sformat("pp%02ix%02i.bmp",i,j));
        }
    #define scale 5.0
    Map2d<float> map2=*map.scaledby(scale);
    Pixel p=best;
    printf("%s\n",p.toString());
    V2d c=(V2d(p)+V2d(0.5,0.5))*scale;
    float col=map.largest();
    map2.line(c-V2d(scale,scale),c+V2d(scale,scale),col);
    map2.line(c-V2d(scale,-scale),c+V2d(scale,-scale),col);
    map2.writefile("ppmap.bmp");
    return vp;
  }

  // Draw a Real^2 picture of the distribution of
  // the vanishing points
  Map2d<float> drawvps() {
    Map2d<float> b=Map2d<float>(300,300,0.0);
//    float farthest=maxinfcir*1.1e2;
    float farthest=0.0;
    for (int i=0;i<vps.width;i++)
    for (int j=0;j<vps.height;j++)
      if (vps.getpos(i,j)!=NULL) {
        V2d v=vps.getpos(i,j)->pos;
        if (v.mod()>farthest)
          farthest=v.mod();
      }
    for (int i=0;i<vps.width;i++)
    for (int j=0;j<vps.height;j++)
      if (vps.getpos(i,j)!=NULL) {
        V2d v=vps.getpos(i,j)->pos;
        Pixel p=Pixel(b.width/2.0*(v.x/farthest+1.0),
                      b.height/2.0*(v.y/farthest+1.0));
        float rad=5.0*v.mod()/farthest;
        b.circle(p,rad,vps.getpos(i,j)->pp.entropy());
        if (best==Pixel(i,j))
          b.circle(p,7,-1.0);
      }
    return b;
  }

};
 
void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  int cirres=a.intafter("-res","resolution of infinity circle",30);
  int ppres=a.intafter("-ppres","resolution of each projection profile",200);
  String bname=a.getarg("binary image");
  measure=&measuresquare;
  if (a.argexists("-deriv","use derivative measure"))
    measure=&measurederiv;
  if (a.argexists("-square","use square measure"))
    measure=&measuresquare;
  if (a.argexists("-entropy","use entropy measure"))
    measure=&measureentropy;
  if (a.argexists("-variance","use variance measure"))
    measure=&measurevariance;
  a.done();

  Map2d<bool> binimg=*Map2d<bool>::readfile(bname)->threshold(0.5);

  ProjectionProfiler pp=ProjectionProfiler(cirres,ppres,binimg);
  pp.setup();
  pp.calculate();
  V2d vp=pp.getvp();

  V2d cen=V2d(binimg.width/2,binimg.height/2);
  V2d diff=(cen-vp).perp().norm()*binimg.width/2;
  binimg.line(cen-diff,cen+diff,true);
  binimg.line(vp,V2d(cen+diff),true);
  binimg.line(vp,V2d(cen-diff),true);
  #ifdef USINGJFONT
//    jfont.writeString(Sformat("entropy=%.5f",lowestent),&binimg,0,10,8.0,true);
  #endif
  binimg.writefile("result.bmp");
  pp.drawvps().writefile("pps.bmp");
  if (outcount>0)
    printf("%i pixels fell outside their window\n");

}
