// Decidedly dodgy!

#include <joeylib.c>

void displaypic(RGBmp *pic);

float mod(V3d v) {
return v.mod();
}

int main(int argc, char **argv) {

ArgParser a=ArgParser(argc,argv);

String infile=a.argafter("-i","input filename","grey.bmp");
String outfile=a.argafter("-o","edges output filename","edgemag.bmp");
String aoutfile=a.argafter("-oa","edge angles output filename","edgeangs.bmp");
String oeutfile=a.argafter("-oe","edge angle distribution variance output filename","eadv.bmp");
int anghistq=a.intafter("-q","edge angle distribution quantisation",8);
int angrad=a.intafter("-r","radius of distribution sample mask",6);
String moutfile=a.argafter("-oe","text edges cancel out output filename","myeadv.bmp");
float myscale=a.floatafter("-ms","scale for my method",0.5);
int myangrad=a.intafter("-mr","radius for my method",3);

a.done(); //untested

Map2d<float> *gs=Map2d<float>::readfile(infile);

// Edge detection
Map2d<float> *change,*angs;
gs->edgedetection(Map2d<float>::sobel(),&change,&angs);
// vs->vedgedetection(Map2d<float>::thinsobel(),&change,&angs);
change->writefile(outfile);
change->writefiletop("emnorm.bmp",3.0);
angs->writefile(aoutfile);
Map2d<float> *d2change,*d2angs;
change->edgedetection(Map2d<float>::sobel(),&d2change,&d2angs);
d2change->writefile("d2edges.bmp");

// Edge angle distribution variance
Map2d<float> *angdist=angs->getdistributionvariancewithweighting(change,anghistq,angrad);
angdist->writefiletop(oeutfile,15);

// Edge angle cancelling outness
Map2d<float> *mine=angs->getmydistributionvariancewithweighting(change,myangrad,myscale);
mine->writefiletop(moutfile,0.5);

// Edge angle visualisation
RGBmp *r=new RGBmp(angs->width,angs->height);
for (int i=0;i<r->width;i++) {
printf(".");
for (int j=0;j<r->height;j++)
r->bmp[i][j]=change->pos[i][j]*myRGB::hue(0.5+angs->pos[i][j]/2.0/pi);
}
r->inverted()->writefile("edgeangv.bmp");

}
