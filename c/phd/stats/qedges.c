// Normal threshold

#include <joeylib.c>

int main(int argc,String *argv) {
ArgParser a=ArgParser(argc,argv);
String infile=a.argafter("-i","Input file","edgemag.bmp");
String outfile=a.argafter("-o","Output file","edges.bmp");
String gfoutfile=a.argafter("-ogf","GF output file","es.gf");
int r=a.intafter("-r","Smoothing radius",0);
float t=a.floatafter("-t","Threshold",0.3);
Map2d<float> *in=Map2d<float>::readfile(infile);
Map2d<float> *angs=Map2d<float>::readfile("edgeangs.bmp");
Map2d<bool> *out=in->smoothed(r)->threshold(t);
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
ls.add(Sformat(" $i $i $f",x,y,2.0*pi*(angs->pos[x][y]-bt)/(tp-bt)-pi/2.0));
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
writelinestofile(ls,gfoutfile,"");
}
