#include <joeylib.h>

void main() {

  // randomise();

  for(int i=0;i<20;i++) {
    Correlator2d c;
    int num=intrnd(3,30);
		V2d ang=3.0*V2d::random();
    for (int j=0;j<num;j++)
      c.add(ang*floatrnd(-1.0,1.0)+(exp(floatrnd(0,2))-1.0)/2.0*V2d::random());
    c.make();
    drawCorrelator2d(c).writefile(getnextfilename("cortest","bmp"));
  }

}
