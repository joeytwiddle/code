#include <joeylib.c>

int main(int argc,String *argv) {
ArgParser a=ArgParser(argc,argv);
String infile=a.argafter("-i","Input file","photo.bmp");
float scale=a.floatafter("-s","Scale down for measure",0.1);
int histquant=a.intafter("-q","Quantisation of histograms",10);
int histrad=a.intafter("-r","Radius of local histograms",7);
float vartop=a.floatafter("-v","Variance top cutoff",100);
float initscale=a.floatafter("-is","initial scale (for neighbourhoods)",0.5);
a.done();
RGBmp *n=RGBmp::readfile(infile)->scaledby(initscale);
Map2d<float> *p=n->getgreyscale();
//  p->display();
//  exit(0);
// Find local histograms
//  printf("$f",scale);
Map2d<GHist> *lh=new Map2d<GHist>(p->width*scale,p->height*scale);
Map2d<RGBHistogram> *lrgbh=new Map2d<RGBHistogram>(n->width*scale,n->height*scale);
printf("wibble/n");
printf("$i",lh->width);
float high=p->largest();
float low=p->smallest();
float tscale=1.0/(high-low);
Map2d<float> *hc=new Map2d<float>(lh->width,lh->height,0.0);
for (int x=0;x<lh->width;x=x+1) {
printf("-");
for (int y=0;y<lh->height;y=y+1) {
lh->pos[x][y]=GHist(histquant);
lrgbh->pos[x][y]=RGBHistogram(5);
for (int i=-histrad;i<=histrad;i++) {
int h=sqrt(histrad*histrad-i*i);
for (int j=-h;j<=h;j++) {
int dx=(float)x*p->width/lh->width+i;
int dy=(float)y*p->height/lh->height+j;
if (p->inmap(dx,dy))
lh->pos[x][y].addocc(((float)p->pos[dx][dy]-low)*tscale);
if (n->inbmp(dx,dy))
lrgbh->pos[x][y].addocc(&n->bmp[dx][dy]);
}
}
if (x>histrad && x<p->width-histrad && y>histrad && y<p->height-histrad)
hc->pos[x][y]=lrgbh->pos[x][y].correlation();
else
hc->pos[x][y]=0;
//      free(&lrgbh->pos[x][y]);
//      lh->pos[x][y].smooth(1);
}
}
// Find histogram difference, variance and correlation
Map2d<float> *hd=new Map2d<float>(lh->width,lh->height,0.0);
Map2d<float> *hv=new Map2d<float>(lh->width,lh->height,0.0);
Map2d<float> *glhd=new Map2d<float>(lh->width,lh->height,0.0);
Map2d<float> *glhv=new Map2d<float>(lh->width,lh->height,0.0);
for (int i=0;i<lh->width-1;i++) {
for (int j=0;j<lh->height-1;j++) {
glhd->pos[i][j]=pythag(lh->pos[i][j].difference(&lh->pos[i+1][j]),lh->pos[i][j].difference(&lh->pos[i][j+1]));
glhv->pos[i][j]=lh->pos[i][j].variance();
hd->pos[i][j]=pythag(RGBHistogram::difference(lrgbh->pos[i][j],lrgbh->pos[i+1][j]),RGBHistogram::difference(lrgbh->pos[i][j],lrgbh->pos[i][j+1]));
hv->pos[i][j]=lrgbh->pos[i][j].variance();
}
}
p->writefile("grey.bmp");
//  hd->display();
hd->writefiletop("hdcol.bmp",100);
glhd->writefile("hdgl.bmp");
//  hd->smoothed(5)->adaptivethreshold(0.5)->writefile("m1hd.bmp");
//  hv->display();
hv->writefiletop("hvcol.bmp",vartop);
glhv->writefiletop("hvgl.bmp",700);
//  hv->display();
//  hv->smoothed(3)->adaptivethreshold(0.5)->writefile("m2shv.bmp");
hc->writefiletop("histcor.bmp",1.0);
}
