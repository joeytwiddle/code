#include <joeylib.c>

void main() {

  randomise();

  for(int i=0;i<20;i++) {
    Correlator2d c;
    int num=intrnd(3,15);
    for (int j=0;j<num;j++)
      c.add(V2d(floatrnd(-1.0,1.0),floatrnd(-1.0,1.0)));
    c.make();
    drawCorrelator2d(c).writefile(getnextfilename("cortest","bmp"));
  }

}