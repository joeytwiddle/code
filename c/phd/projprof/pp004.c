// Projection profiler, with vanishing points

/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

// #define USINGJFONT

#include <joeylib.c>

// A vanishing point with its projection profile
// which will be accumulated using the add() function
class VP {
public:
  V2d pos;
  Map1d<int> pp;
  float left,right; // relative to ...
  V2d tocentre;
  float realleft;
  int q;

  // Set up the window of angle over which the VP can
  // see the text region.
  // Set up optimization variables to make the add()
  // function efficient.
  VP(V2d p,int res,int width) {
    pos=p;
    tocentre=p.neg().norm();
    pp=Map1d<int>(res);
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
//    float ang=modtwopi(v.angle()-realleft);
//    int l=ang*q;
    // Slower method
    float ang=V2d::angbetween(v-pos,tocentre);
    if (ang<left)
      ang=ang+2.0*pi;
    int l=(ang-left)*q;
    // Even slower method (may not compensate for 0,2pi boundaries)
//    float ang=V2d::angbetween(v-pos,tocentre);
//    int l=(ang-left)*(float)pp.width/(right-left);
    if (l>=0 && l<pp.width)
      pp.incpos(l);
//    else
//      printf("%i out of range %i (%f,%f,%f),%i\n",l,pp.width,left,ang,right,q);
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
      if (pos.mag()>=1.0)
        vps.setpos(i,j,NULL);
      else {
//        printf("Plane=%s\n",infcircletoplane(pos).toString());
        vps.setpos(i,j,new VP(infcircletoplane(pos),ppres,width));
      }
    }
  }

  // Accumulates projection profiles of the points
  // in the input image
  void calculate() {
    for (int i=0;i<binimg.width;i++) {
      if (i%17==0) // Learn your 17 times table as the
        printf("%i/%i\n",i,binimg.width); // program runs!
      for (int j=0;j<binimg.height;j++) {
        if (binimg.getpos(i,j)) {
          V2d pos=V2d(i,j)-V2d(binimg.width/2,binimg.height/2);
          for (int a=0;a<vps.width;a++)
          for (int b=0;b<vps.height;b++)
            if (vps.getpos(a,b)!=NULL)
              vps.getpos(a,b)->add(pos);
        }
      }
    }
  }

  // Finds the entropy of all the projection profiles
  // and puts them in a map
  Map2d<float> getppmap() {
    Map2d<float> map=Map2d<float>(vps.width,vps.height,-0.4);
    for (int i=0;i<vps.width;i++)
    for (int j=0;j<vps.height;j++)
      if (vps.getpos(i,j)!=NULL) {
//        map.setpos(i,j,-vps.getpos(i,j)->pp.totalsquare());
        map.setpos(i,j,-vps.getpos(i,j)->pp.totalderiv());
//        map.setpos(i,j,vps.getpos(i,j)->pp.entropy());
      }
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
          printf("Lowest so far %i %i\n",i,j);
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
    V2d c=V2d(p)*scale;
    map2.line(c-V2d(scale,scale),c+V2d(scale,scale),0);
    map2.line(c-V2d(scale,-scale),c+V2d(scale,-scale),0);
    map2.writefile("ppmap.bmp");
    return vp;
  }

  // Draw a Real^2 picture of the distribution of
  // the vanishing points
  Map2d<float> drawvps() {
    Map2d<float> b=Map2d<float>(300,300,0.0);
    float size=maxinfcir*1.1e2;
    for (int i=0;i<vps.width;i++)
    for (int j=0;j<vps.height;j++)
      if (vps.getpos(i,j)!=NULL) {
        V2d v=vps.getpos(i,j)->pos;
        Pixel p=Pixel(b.width/2.0*(v.x/size+1.0),
                      b.height/2.0*(v.y/size+1.0));
        float rad=5.0*v.mod()/size;
        b.circle(p,rad,vps.getpos(i,j)->pp.entropy());
        if (best==Pixel(i,j))
          b.circle(p,6,-1.0);
      }
    return b;
  }

};
 
void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  int cirres=a.intafter("-res","resolution of infinity circle",30);
  int ppres=a.intafter("-ppres","resolution of each projection profile",200);
  String bname=a.getarg("binary image");
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

}
