#include <joeylib.c>

void displaypic(RGBmp *pic);

float mod(V3d v) {
return v.mod();
}

int main(int argc, char **argv) {

// Parse command line parameters
if (argc==1) {
printf("jshow : rgbmp reader and displayer (Joey Feb 99)/n");
exit(0);
}

String infile=argv[1];
RGBmp *pic=new RGBmp(infile);

Map2d<float> *gs=pic->getgreyscale();
Map2d<V3d> *vs=pic->getv3ds();

Map2d<float> *change=new Map2d<float>(pic->width,pic->height);
Map2d<float> *angs=new Map2d<float>(pic->width,pic->height);
//  gs->edgedetection(Map2d<float>::sobel(),&change,&angs);
vs->vedgedetection(Map2d<float>::thinsobel(),&change,&angs);

//  Map2d<float> *angdist=angs->getdistributionvariance(8,4);
Map2d<float> *angdist=angs->getdistributionvariancewithweighting(change,8,4);

//  angdist->display();

//  angdist=angdist->smoothed(6);

angdist->display();
angdist->adaptivethreshold(0.6)->display();

exit(0);

//  allegro_exit();

}
