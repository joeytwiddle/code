#include <joeylib.c>

int main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  float focallength=a.floatafter("-focal","focal length (prop to img width)",1.0);
  a.comment("Orientation of plane in degrees:");
  float roll=deg2rad(a.floatafter("-roll","roll",3.0));
  float yaw=deg2rad(a.floatafter("-yaw","yaw",20.0));
  float pitch=deg2rad(a.floatafter("-pitch","pitch",-40.0));
  float depth=a.floatafter("-depth","depth",100.0);
  float scale=a.floatafter("-scale","scale",0.8);
  int imgwidth=a.intafter("-width",640);
  int imgheight=imgwidth*3/4;
  // String inname=a.argor("document image file","in.bmp");
  // String outname=a.argor("output filename","out.bmp");
  int numlines=a.intafter("-lines","number of lines in paragraph",10);
  float noise=a.floatafter("-noise","noise",5.0);
  a.done();

  V3d cen=V3d(imgwidth/2,imgheight/2,depth);
  V3d right=V3d(1,0,0);
  V3d down=V3d(0,-1,0);
  right=right.rotated(V3d::i,pitch);
  right=right.rotated(V3d::j,yaw);
  right=right.rotated(V3d::k,roll);
  down=down.rotated(V3d::i,pitch);
  down=down.rotated(V3d::j,yaw);
  down=down.rotated(V3d::k,roll);
  printf("%s %s\n",right.toString(),down.toString());
  // Rectangle3d rec=Rectangle3d(cen,right,down,scale);
  Plane imageplane=Plane(V3d(0,0,0),V3d(0,0,1));

  V3d eye=V3d(imgwidth/2,imgheight/2,-focallength*imgwidth);
  List<Line2d> lines;
  for (int i=1;i<=numlines;i++) {
    float thru=(float)(i-1)/(float)(numlines-1);
    Line3d line=Line3d(cen-right/2-down/2+down*thru,
                    cen+right/2-down/2+down*thru);
    Line3d la=Line3d(eye,line.a);
    V3d inta=imageplane.intersect(la);
    Line3d lb=Line3d(eye,line.a);
    V3d intb=imageplane.intersect(lb);
    lines.add(Line2d(inta.dropz()+V2d::randomcircle()*noise,intb.dropz()+V2d::randomcircle()*noise));
  }

  V2d hvp=lines.num(1).intersect(lines.num(2));
  Line2d baseline=Line2d(lines.num(1).a,lines.num(numlines).a);
  V2d vvpdir=(baseline.b-baseline.a).norm();

  // From pp.c

  List<float> cds;
  for (int i=1;i<=numlines;i++) {
    V2d hit=baseline.intersect(Line2d(hvp,lines.num(i).b));
    printf("%s ~ %s ?\n",lines.num(i).a.toString(),hit.toString());
    cds.add((hit-baseline.a).dot(vvpdir));
  }
  
    // We now do simultaneous calculation of
    // various correlation methods for comparison

    // Accumulate correlator
    Correlator2d csspacing;
    Correlator2d cslength;
    // Why not do line-spacing with gaps?
    // (seems equivalent so no worry)
    for (int i=1;i<numlines;i++) {
      float a=cds.num(i);
      // if (correlator==LineSpacing) {
        // x = average of two cds, y = dist between
        float b=cds.num(i+1);
        // float pos=(a+b)/2.0;
        float pos=a;
        csspacing.add(V2d(pos,myabs(b-a)),1.0);
        // printf("Correlating using lines =)\n");
      // } else if (correlator==LineWidth) {
        // y = width of line
        // float pos=a;
        cslength.add(V2d(pos,lines.num(i).length),1.0);
      // } else {
        // error("Non existent correlation method");
      // }
    }

    // One of the methods is chosen for final VVP.
    float vpd=csspacing.crossesy();
    printf("VP is %f along baseline\n",vpd);
    V2d vvp=baseline.a+vvpdir*vpd;

    V2d vvpfromdata=baseline.intersect(Line2d(lines.num(1).b,lines.num(numlines).b));

    printf("Found %s ~ Source %s ?\n",vvp.toString(),vvpfromdata.toString());
  
    drawCorrelator2dInColour(csspacing).writefile("spacings.bmp");
    drawCorrelator2dInColour(cslength).writefile("lengths.bmp");

}
