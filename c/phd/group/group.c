#include <joeylib.c>

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  String outname=a.argafter("-o","output filename","group.bmp");
  String imname=a.getarg();
  int smrad=a.intafter("-r","radius",2);
  float scale=a.floatafter("-s","scale",0.5);
  a.done();
  
  randomise();

  RGBmp *image=RGBmp::readfile(imname)->scaledby(scale);
  Map2d<float> *gs=image->getgreyscale();
  
  Map2d<float> *change,*angs;
  gs->edgedetection(Map2d<float>::sobel(),&change,&angs);
  change=change->smoothed(smrad);
  Map2d<bool> *b=change->realthreshold(change->getaverage());
  
  change->writefile("change.bmp");
  b->writefile(outname);
  change->threshold(0.3)->writefile("simp.bmp");
  change->adaptivethreshold(0.7)->writefile("adap.bmp");  

}
