#include <joeylib.c>

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  String fname=a.getarg(1);
  String oname=a.getarg(1);
  
  Map2d<float> *in=Map2d<float>::readfile(fname);
  Map2d<float> *edges,*angles;
  in->edgedetection(Map2d<float>::sobel(),&edges,&angles);
  edges->writefile(oname);

}
