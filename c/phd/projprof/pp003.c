/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

// #define USINGJFONT

#include <joeylib.c>

class ProjectionProfiler {
public:
  int as;
  bool showpp;
  bool showpps;
  bool showppp;
  bool showppps;
  Map2d<bool> binimg;

  ProjectionProfiler(int a,Map2d<bool> b) {
    as=a;
    binimg=b;
    showpp=false;
    showpps=false;
    showppp=false;
    showppps=true;
  }
 
 List<Map1d<int> > getprojprofiles(List<float> angs) {
  List<Map1d<int> > pps;
  int ppwidth=sqrt(mysquare(binimg.width)+mysquare(binimg.height));
  for (int i=1;i<=angs.len;i++)
    pps.add(Map1d<int>(ppwidth));
  for (int i=0;i<binimg.width;i++) {
    if (i%17==0)
      printf("%i/%i\n",i,binimg.width);
  for (int j=0;j<binimg.height;j++) {
    if (binimg.getpos(i,j))
    for (int k=1;k<=angs.len;k++) {
      V2d tmp=V2d(i,j)-V2d(binimg.width/2,binimg.height/2);
      float x=tmp.dot(V2d::angle(angs.num(k)));
      int l=ppwidth/2+x;
      pps.p2num(k)->incpos(l);
    }
  }
  }
  return pps;
 }

  /* Perspective projection profiles */
  List<Map1d<int> > getppps(float ang,int ds) {
  List<Map1d<int> > pps;
  int ppwidth=sqrt(mysquare(binimg.width)+mysquare(binimg.height));
  for (int i=1;i<=ds;i++)
    pps.add(Map1d<int>(ppwidth));
  for (int i=0;i<binimg.width;i++) {
    if (i%17==0)
      printf("%i/%i\n",i,binimg.width);
  for (int j=0;j<binimg.height;j++) {
    if (binimg.getpos(i,j))
    for (int d=1;d<=ds;d++) {
      float side=((d%2)==0 ? -1 : +1);
      V2d tmp=V2d(i,j)-V2d(binimg.width/2,binimg.height/2);
      float dist=(float)ppwidth/2.0/(1.0-(float)(d-1)/(float)ds)-1.0;
      V2d far=side*V2d::angle(ang+pi/2.0)*dist;
      tmp=tmp-far;
      V2d back=side*V2d::angle(ang-pi/2.0);
      float x=V2d::angbetween(tmp,back);
      float left=V2d::angbetween(side*V2d::angle(ang)*-(float)ppwidth/2.0-far,back);
      float right=V2d::angbetween(side*V2d::angle(ang)*(float)ppwidth/2.0-far,back);
      // Don't ne
      if (left>right) {
        float tmp=left;
        left=right;
        right=tmp;
      }
      int l=(x-left)/(right-left)*(float)ppwidth;
      if (l<0)
        l=-l;
      // ed this
      if (l>0 && l<ppwidth)
        pps.p2num(d)->incpos(l);
    }
  }
  }
  if (showppps)
  for (int i=1;i<=ds;i++) {
      Map2d<bool> d=pps.p2num(i)->draw();
      d.writefile(getnextfilename("ppp","bmp"));
      d.freedom();
  }
  return pps;
  }

 float getlineangle() {
  List<float> angs;
  for (float a=0;a<=pi;a+=pi/(float)as)
    angs.add(a);
  List<Map1d<int> > pps=getprojprofiles(angs);
  float lowestent=0.0;
  int lowestind=-1;
  getlowestent(pps,&lowestent,&lowestind);
  printf("Lowest was %i with %.5f\n",lowestind,lowestent);
  float ang=lowestind*pi/(float)as;
  if (showpp) {
    Map2d<bool> d=pps.p2num(lowestind)->draw();
    d.writefile("bestpp.bmp");
    d.freedom();
  }
  return ang;
  }

  void getlowestent(List<Map1d<int> > pps,float *lowestent,int *lowestind) {
  *lowestent=0.0;
  *lowestind=-1;
  for (int i=1;i<=pps.len;i++) {
    float ent=pps.p2num(i)->entropy();
    if (ent<*lowestent) {
      *lowestent=ent;
      *lowestind=i;
    }
    if (showpps || showppps) {
      Map2d<bool> d=pps.p2num(i)->draw();
  //    jfont.writeString(Sformat("%.5f",ent),&d,0,12,10.0,true);
      d.writefile(getnextfilename("ppq","bmp"));
      d.freedom();
    }
  }
  }

};
 
void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  int as=a.intafter("-as","angles examined",128);
  int ds=a.intafter("-ds","distances examined",30);
  bool showpp=a.argexists("-spp","show winning projection profile");
  bool showpps=a.argexists("-spps","show all projection profiles");
  String bname=a.getarg("binary image");
  a.done();

  Map2d<bool> binimg=*Map2d<bool>::readfile(bname)->threshold(0.5);

  ProjectionProfiler pp=ProjectionProfiler(as,binimg);
  pp.showpps=showpps;
  pp.showpp=showpp;
  float ang=pp.getlineangle();

  List<Map1d<int> > ppps=pp.getppps(ang,ds);
  float lowestent;
  int lowestind;
  pp.getlowestent(ppps,&lowestent,&lowestind);
  printf("Distances lowest was %i with %f\n",lowestind,lowestent);
    
  Pixel cen=Pixel(binimg.width/2,binimg.height/2);
  Pixel diff=Pixel(V2d::angle(ang)*binimg.width);
  binimg.line(cen-diff,cen+diff,true);
  #ifdef USINGJFONT
    jfont.writeString(Sformat("entropy=%.5f",lowestent),&binimg,0,10,8.0,true);
  #endif
  binimg.writefile("result.bmp");

}
