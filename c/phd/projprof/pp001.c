/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

#define USINGJFONT

#include <joeylib.c>

List<Map1d<int> > getprojprofiles(Map2d<bool> b,List<float> angs) {
  List<Map1d<int> > pps;
  int ppwidth=sqrt(mysquare(b.width)+mysquare(b.height));
  for (int i=1;i<=angs.len;i++)
    pps.add(Map1d<int>(ppwidth));
  for (int i=0;i<b.width;i++) {
    if (i%17==0)
      printf("%i/%i\n",i,b.width);
  for (int j=0;j<b.height;j++) {
    if (b.getpos(i,j))
    for (int k=1;k<=angs.len;k++) {
      V2d tmp=V2d(i,j)-V2d(b.width/2,b.height/2);
      float x=tmp.dot(V2d::angle(angs.num(k)));
      int l=ppwidth/2+x;
      pps.p2num(k)->incpos(l);
    }
  }
  }
  return pps;
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  int as=a.intafter("-as","angles examined",16);
  String bname=a.getarg("binary image");
  a.done();

  Map2d<bool> binimg=*Map2d<bool>::readfile(bname)->threshold(0.5);

  List<float> angs;
  for (float a=0;a<=pi;a+=pi/(float)as)
    angs.add(a);
  List<Map1d<int> > pps=getprojprofiles(binimg,angs);
  float lowestent=0.0;
  int lowestind=-1;
  for (int i=1;i<=pps.len;i++) {
//    Map2d<bool> d=pps.p2num(i)->draw();
    float ent=pps.p2num(i)->entropy();
    if (ent<lowestent) {
      lowestent=ent;
      lowestind=i;
    }
//    jfont.writeString(Sformat("%.5f",ent),&d,0,12,10.0,true);
//    d.writefile(getnextfilename("pp","bmp"));
//    d.freedom();
  }
  float ang=lowestind*pi/(float)as;
  Pixel cen=Pixel(binimg.width/2,binimg.height/2);
  Pixel diff=Pixel(V2d::angle(ang)*binimg.width);
  binimg.line(cen-diff,cen+diff,true);
  jfont.writeString(Sformat("entropy=%.5f",lowestent),&binimg,0,10,8.0,true);
  binimg.writefile("result.bmp");
  printf("Lowest was %i with %.5f\n",lowestind,lowestent);

}
