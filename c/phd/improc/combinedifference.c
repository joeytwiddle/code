#include <joeylib.c>

void main(int argc,String* argv) {

  List<RGBmp> ims;
  for (int i=1;i<argc;i++) {
    String fname=argv[i];
    ims.add(*RGBmp::readfile(fname));
  }

  RGBmp out=RGBmp(ims.num(1).width,ims.num(2).height);

  for (int i=0;i<out.width;i++)
  for (int j=0;j<out.height;j++) {
    myRGB col=ims.p2num(1)->getpos(i,j);
    myRGB other=myRGB::magenta;
    int cntcol=1;
    int cntother=0;
    for (int k=2;k<=ims.len;k++) {
      myRGB c=ims.p2num(k)->getpos(i,j);
      if ((c.v3d()-col.v3d()).mag()<0.1) {
        cntcol++;
      } else {
        other=c;
        cntother++;
      }
    }
    myRGB inout=( cntother>0 && cntother<=cntcol ? other : col );
    out.setpos(i,j,inout);
  }

  out.writefile("out.bmp");

}