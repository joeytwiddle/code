#include <joeylib.c>

int main(int argc,String *argv) {
ArgParser a=ArgParser(argc,argv);
String infile=a.argafter("-i","Input file","photo.bmp");
float scale=a.floatafter("-s","Scale",0.2);
int histquant=a.intafter("-q","Quantisation of histograms",10);
int histrad=a.intafter("-r","Radius of local histograms",7);
float vartop=a.floatafter("-v","Variance top cutoff",100);
RGBmp *n=new RGBmp(infile);
Map2d<float> *p=n->getgreyscale();
//  p->display();
//  exit(0);
// Find local histograms
//  printf("$f",scale);
Map2d<GHist> *lh=new Map2d<GHist>(p->width*scale,p->height*scale);
printf("wibble/n");
printf("$i",lh->width);
float high=p->largest();
float low=p->smallest();
float tscale=1.0/(high-low);
for (int x=0;x<lh->width;x=x+1) {
printf("-");
for (int y=0;y<lh->height;y=y+1) {
lh->pos[x][y]=GHist(histquant);
for (int i=-histrad;i<=histrad;i++) {
int h=sqrt(histrad*histrad-i*i);
for (int j=-h;j<=h;j++) {
int dx=(float)x*p->width/lh->width+i;
int dy=(float)y*p->height/lh->height+j;
if (p->inmap(dx,dy)) {
lh->pos[x][y].addocc(((float)p->pos[dx][dy]-low)*tscale);
}
}
}
//      lh->pos[x][y].smooth(1);
}
}
// Find histogram difference and variance
Map2d<float> *hd=new Map2d<float>(lh->width,lh->height,0.0);
Map2d<float> *hv=new Map2d<float>(lh->width,lh->height,0.0);
Map2d<float> *glv=new Map2d<float>(lh->width,lh->height,0.0);
for (int i=0;i<lh->width-1;i++) {
for (int j=0;j<lh->height-1;j++) {
hd->pos[i][j]=pythag(lh->pos[i][j].difference(&lh->pos[i+1][j]),lh->pos[i][j].difference(&lh->pos[i][j+1]));
hv->pos[i][j]=lh->pos[i][j].variance();
glv->pos[i][j]=lh->pos[i][j].glvariance();
}
}
p->writefile("grey.bmp");
hd->writefile("histdiff.bmp");
//  hd->smoothed(5)->adaptivethreshold(0.5)->writefile("m1hd.bmp");
//  hv->display();
glv->writefiletop("histvar.bmp",vartop);
glv->writefile("hvnorm.bmp");
hv->writefile("realhvnotgl.bmp");
//  hv->display();
//  hv->smoothed(3)->adaptivethreshold(0.5)->writefile("m2shv.bmp");
}
