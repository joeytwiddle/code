// Smooth

#include <joeylib.c>

int main(int argc,String *argv) {
ArgParser a=ArgParser(argc,argv);
String infile=a.argafter("-i","Input file","histvar.bmp");
String outfile=a.argafter("-o","Output file","s1hv.bmp");
int r=a.intafter("-r","Smoothing radius",0);
Map2d<float> *in=Map2d<float>::readfile(infile);
Map2d<float> *out=in->smoothed(r);
out->writefile(outfile);
}
