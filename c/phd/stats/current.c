// Decidedly dodgy!

#include <joeylib.c>

void displaypic(RGBmp *pic);

float mod(V3d v) {
return v.mod();
}

float combinemaganddifferential(float m,float d) {
return m-d;
}

int main(int argc, char **argv) {

ArgParser a=ArgParser(argc,argv);

String infile=a.argafter("-i","input filename","grey.bmp");
String outfile=a.argafter("-o","edges output filename","edgemag.bmp");
String aoutfile=a.argafter("-oa","edge angles output filename","edgeangs.bmp");
String oeutfile=a.argafter("-oe","edge angle distribution variance output filename","eadv.bmp");
int anghistq=a.intafter("-q","edge angle distribution quantisation",8);
int angrad=a.intafter("-r","radius of distribution sample mask",3);
String moutfile=a.argafter("-oe","text edges cancel out output filename","myeadv.bmp");
float myscale=a.floatafter("-ms","scale for my method",0.5);
int myangrad=a.intafter("-mr","radius for my method",6);
float t=a.floatafter("-t","Threshold",0.3);
String gfoutfile=a.argafter("-ogf","GF output file","es.gf");

Map2d<float> *in=Map2d<float>::readfile(infile);
Map2d<float> *gs=in->scaleby(400.0/in->width);
printf("Read./n");

// Edge detection
Map2d<float> *change,*angs,*d2change,*d2angs,*end;
gs->edgedetection(Map2d<float>::sobel(),&change,&angs);
// vs->vedgedetection(Map2d<float>::thinsobel(),&change,&angs);
change->writefile(outfile);
angs->writefile(aoutfile);

Map2d<float> *dx=change->filterby(Map2d<float>::simple());
Map2d<float> *dy=change->filterby(Map2d<float>::simple()->transpose());

dx->display();
dy->display();

Map2d<bool> *dxb=new Map2d<bool>(gs->width,gs->height,false);
for (int y=0;y<gs->height;y++) {
float last=dx->pos[0][y];
for (int x=1;x<gs->width;x++) {
float next=dx->pos[x][y];
if (((last==0 || next==0) && last!=next) || mysgn(next)!=mysgn(last))
//     if (mysgn(next)!=mysgn(last))
dxb->pos[x][y]=true;
last=next;
}
}

Map2d<bool> *dyb=new Map2d<bool>(gs->width,gs->height,false);
for (int x=0;x<gs->width;x++) {
float last=dx->pos[x][0];
for (int y=1;y<gs->height;y++) {
float next=dy->pos[x][y];
//      if (mysgn(next)!=mysgn(last))
if (((last==0 || next==0) && last!=next) || mysgn(next)!=mysgn(last))
dyb->pos[x][y]=true;
last=next;
}
}

Map2d<float> *dxf=new Map2d<float>(gs->width,gs->height,0.0);
for (int y=0;y<gs->height;y++) {
float last=dx->pos[0][y];
for (int x=1;x<gs->width;x++) {
float next=dx->pos[x][y];
if (dxb->pos[x][y])
dxf->pos[x][y]=change->pos[x][y];
//      dxf->pos[x][y]=diff(last,next);
last=next;
}
}

Map2d<float> *dyf=new Map2d<float>(gs->width,gs->height,0.0);
for (int x=0;x<gs->width;x++) {
float last=dy->pos[x][0];
for (int y=1;y<gs->height;y++) {
float next=dy->pos[x][y];
if (dyb->pos[x][y])
dxf->pos[x][y]=change->pos[x][y];
//        dyf->pos[x][y]=diff(last,next);
last=next;
}
}

dxb->display();
dyb->display();
dxf->display();
//  dyf->display();


change->edgedetection(Map2d<float>::thinsobel(),&d2change,&d2angs);
end=change->applyop(&combinemaganddifferential,d2change);
change->display();
d2change->display();
end->display();
exit(0);

Map2d<bool> *out=change->threshold(t);
out->writefile(outfile);
List<String> ls;
ls.add("@Set Description/nFormat Text/ninput file: img04.pgm/nimage size:  720 x  576/nsubimage offsets:    0    0/nsubimage size:  720 x  576/n/nprogram: es/nversion 2.1/nfilter type (-f) = OPT2D/noptimal filter parameter (-s) = 1.00/nautoscaled threshold (-m) = 0.6/nmask width (-w) =   7/nlower hysteresis threshold (-hl) = 0.20/nupper hysteresis threshold (-hu) = 0.40/nmin length of edge string (-n) =   3/n@/n");
ls.add("@SET EdgeStrings/nGeomStruct pointset rep x y angle/nFormat Index PointSet/n");
ls.add("should be replaced");
int rem=ls.len;
int n=0;
String s="";
float tp=angs->largest();
float bt=angs->smallest();
for (int x=0;x<out->width;x++)
for (int y=0;y<out->height;y++) {
if (out->pos[x][y]) {
n++;
ls.add(Sformat(" $i $i $f/mnt/filfirin//",x,y,2.0*pi*(angs->pos[x][y]-bt)/(tp-bt)-pi/2.0));
}
}
ls.list[rem-1]=Sformat("1 $i",n);
ls.add("/n");
/*
int n=0;
for (int x=0;x<out->width;x++)
for (int y=0;y<out->height;y++) {
if (out->pos[x][y]) {
ls.add(Sformat("$i 1 $i $i $i",n,x,y,0));
n++;
}
}
*/
ls.add("@");
writelinestofile(ls,gfoutfile);

}
