#include <joeylib.c>

void main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  float darklight=a.floatafter("-dl","Dark/light 0-1",0.5);
  myRGB shade=myRGB::hue(a.floatafter("-hue","Shade hue (R=0 G=1/3 B=2/3)",1.0/6.0)).darklight(darklight);
  bool inv=a.argexists("-inv","invert (shade unwanted)");
  float gamma=a.floatafter("-g","gamma",0.5);
  String imname=a.getarg("image file");
  String bname=a.getarg("binary file");
  String oname=a.getarg("output file");
  a.done();
  
  RGBmp *r=RGBmp::readfile(imname)->scaledby(0.5);
  Map2d<bool> *b=Map2d<float>::readfile(bname)->threshold(0.5);
  
  for (int i=0;i<r->width;i++)
  for (int j=0;j<r->height;j++) {
    Pixel p=Pixel(i*b->width/r->width,j*b->height/r->height);
    if (b->getpos(p)!=inv && r->inimage(i,j)) {
      myRGB a=r->bmp[i][j];
      myRGB b=shade;
      r->bmp[i][j]=a*(1.0-gamma)+b*gamma;
    }
  }
  
  r->writefile(oname);
  
}