#include <joeylib.c>

void main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String imname=a.getarg("image file");
  String bname=a.getarg("binary file");
  String oname=a.getarg("output file");
  myRGB shade=myRGB::hue(a.floatafter("-h","Shade hue",1.0/6.0));
  a.done();
  
  RGBmp *r=RGBmp::readfile(imname)->scaledby(0.5);
  Map2d<bool> *b=Map2d<float>::readfile(bname)->threshold(0.5);
  
  for (int i=0;i<r->width;i++)
  for (int j=0;j<r->height;j++) {
    Pixel p=Pixel(i*b->width/r->width,j*b->height/r->height);
    if (b->getpos(p) && r->inimage(i,j))
      r->bmp[i][j]=shade;
  }
  
  r->writefile(oname);
  
}