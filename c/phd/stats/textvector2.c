#include <joeylib.c>

void displaypic(RGBmp *pic);

int main(int argc, char **argv) {

// Parse command line parameters
if (argc==1) {
printf("jshow : rgbmp reader and displayer (Joey Feb 99)/n");
exit(0);
}

String infile=argv[1];

RGBmp *pic=new RGBmp(infile);
// pic->height=pic->width=100;

// Create local histogram map of image
float scale=0.2;
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
printf("/n");

//  cor->display();

//  exit(0);

// Compute histogram variance
Map2d<float> *var=new Map2d<float>(pic->width*scale,pic->height*scale,&gimmezerofloat);
for (int i=0;i<lh->width;i+=1) {
printf("@");
for (int j=0;j<lh->height;j+=1) {
var->pos[i][j]=lh->pos[i][j].variance();
}
}
printf("/n");

//  var->display();

//  exit(0);

Map2d<float> *svar=var->smoothed(3);
Map2d<bool> *vthr=svar->threshold(0.3);

//  vthr->display();

// Compute histogram change
Map2d<float> *hcha=new Map2d<float>(pic->width*scale,pic->height*scale,&gimmezerofloat);
for (int i=1;i<lh->width-1;i+=1) {
printf("~");
for (int j=1;j<lh->height-1;j+=1) {
float hor=RGBHistogram::difference(lh->pos[i][j],lh->pos[i+1][j]);
float ver=RGBHistogram::difference(lh->pos[i][j],lh->pos[i][j+1]);
hcha->pos[i][j]=sqrt(hor*hor+ver*ver);
}
}
printf("/n");
//  hcha=hcha->smoothed(5);

//  hcha->display();

//  exit(0);

// Compute derivative (edges)
int maskrad=1;
Map2d<float> *hc=new Map2d<float>(pic->width,pic->height,&gimmezerofloat);
Map2d<float> *vc=new Map2d<float>(pic->width,pic->height,&gimmezerofloat);
Map2d<float> *change=new Map2d<float>(pic->width,pic->height,&gimmezerofloat);
Map2d<float> *angs=new Map2d<float>(pic->width,pic->height,&gimmezerofloat);
for (int i=maskrad;i<pic->width-maskrad;i++) {
printf("}");
for (int j=maskrad;j<pic->height-maskrad;j++) {
float horchange=myRGB::difference(&pic->bmp[i-1][j],&pic->bmp[i+1][j]);
float verchange=myRGB::difference(&pic->bmp[i][j-1],&pic->bmp[i][j+1]);
float totalchange=sqrt(mysquare(horchange)+mysquare(verchange));
float angchange=myangle(horchange,verchange);
hc->pos[i][j]=horchange;
vc->pos[i][j]=verchange;
change->pos[i][j]=totalchange;
if (totalchange>0.15)
angs->pos[i][j]=angchange;
}
}
printf("/n");

Map2d<float> *shcha=hcha->smoothed(3);

Map2d<bool> *hchathr=shcha->threshold(0.2);

Map2d<float> *schange=change->smoothed(5);
Map2d<bool> *sthr=schange->threshold(0.2);
Map2d<bool> *edge=change->threshold(0.2);
Map2d<float> *sedge=edge->smoothed(5);
Map2d<bool> *sedgethr=sedge->threshold(0.2); // This seems to be better (or have I just chosen better thresholds?!)

hchathr=hchathr->filterby(new Map2d<float>(1,1,-1.0));

Map2d<bool> *reg=new Map2d<bool>(pic->width,pic->height,&gimmezeroint);
/*  for (int i=0;i<reg->width;i++) {
for (int j=0;j<reg->height;j++) {
int hx=i*scale;
int hy=j*scale;
if (vthr->inmap(hx,hy))
if (vthr->pos[hx][hy] && !hchathr->pos[hx][hy] && sthr->pos[i][j] && sedgethr->pos[i][j])
reg->pos[i][j]=true;
else
reg->pos[i][j]=false;
}
}*/

//  reg=vthr->applyop(&booland,sthr)->applyop(&booland,sedgethr)->applyop(&booland,hchathr);
reg=sthr->applyop(&booland,sedgethr)->applyop(&booland,hchathr);

/*
// cor->display();
//  var->display();
//  svar->display();
vthr->display();
//  hcha->display();
//  shcha->display();
hchathr->display();

*/

//  change->display();
//  schange->display();
//  sthr->display();
//  edge->display();
//  sedge->display();
//  sedgethr->display();

reg->display();

/*  var->writefile("h1var.bmp");
svar->writefile("h2svar.bmp");
vthr->writefile("h3vthr.bmp");
hcha->writefile("h4hcha.bmp");
shcha->writefile("h5shcha.bmp");
hchathr->writefile("h6hchathr.bmp");*/

change->writefile("e1change.bmp");
schange->writefile("e2schange.bmp");
sthr->writefile("e3sthr.bmp");
edge->writefile("e4edge.bmp");
sedge->writefile("e5sedge.bmp");
sedgethr->writefile("e6sedgethr.bmp");

reg->writefile("reg.bmp");

allegro_exit();

}
