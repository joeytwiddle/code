#include <joeylib.c>

void displaypic(RGBmp *pic);

float mod(V3d v) {
return v.mod();
}

int main(int argc, char **argv) {

ArgParser a=ArgParser(argc,argv);

String infile=a.argafter("-i","input filename","image.bmp");
String outfile=a.argafter("-o","output filename","grey.bmp");
RGBmp *pic=RGBmp::readfile(infile);

Map2d<float> *gs=pic->getgreyscale();

gs->writefile(outfile);

}
