#include <joeylib.c>

void displaypic(RGBmp *pic);

int main(int argc, char **argv) {

ArgParser a=ArgParser(argc,argv);
float scale=a.floatafter("-s","scale",0.1);
String infile=a.getarg(1);
String outfile=a.argor("histcor.bmp");
a.done();

RGBmp *pic=new RGBmp(infile);
// pic->height=pic->width=100;

// Create local histogram map of image
Map2d<RGBHistogram> *lh=pic->getlocalhistmap(10,scale);

// Compute histogram correlation
Map2d<float> *cor=new Map2d<float>(pic->width*scale,pic->height*scale,&gimmezerofloat);
for (int i=0;i<lh->width;i+=1) {
printf("*");
for (int j=0;j<lh->height;j+=1) {
cor->pos[i][j]=lh->pos[i][j].correlation();
//printf("$f ",cor->pos[i][j]);
}
}
printf("\n");

//  cor->display();

cor->writefile(outfile);

}
