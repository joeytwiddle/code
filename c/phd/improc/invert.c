// Invert

#include <joeylib.h>

int main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String infile=a.argafter("-i","Input file","histvar.bmp");
  String outfile=a.argafter("-o","Output file","s1hv.bmp");
  bool col=a.argexists("-col","Colour");
  bool border=a.argexists("-b","Border");
  a.done();
  if (col) {
    RGBmp *in=RGBmp::readfile(infile);
    RGBmp *out=new RGBmp(in->width,in->height);
    for (int i=0;i<in->width;i++)
    for (int j=0;j<in->height;j++)
      out->setpos(i,j,myRGB::white-in->getpos(i,j));
    if (border) {
    out->line(0,0,out->width-1,0,&myRGB::black);
    out->line(out->width-1,0,out->width-1,out->height-1,&myRGB::black);
    out->line(0,out->height-1,out->width-1,out->height-1,&myRGB::black);
    out->line(0,0,0,out->height-1,&myRGB::black);
    }
    out->writefile(outfile);
  } else {
  Map2d<float> *in=Map2d<float>::readfile(infile);
  Map2d<float> *out=in->applyfn(&floatneg);
  if (border) {
    float c=out->smallest();
    out->line(0,0,out->width-1,0,c);
    out->line(out->width-1,0,out->width-1,out->height-1,c);
    out->line(0,out->height-1,out->width-1,out->height-1,c);
    out->line(0,0,0,out->height-1,c);
  }
  out->writefile(outfile);
  }
}
