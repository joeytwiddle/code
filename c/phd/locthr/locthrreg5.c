// Perform local thresholds on image regions
// Region by region (but overlap happens)

#include <joeylib.c>

Map2d<int> regmap;
List<Region> regs;

float virtualwidth(int x,int y) {
  int c=regmap.getpos(x,y);
  if (c>0)
    return sqrt(regs.p2num(c)->getlist()->len);
  else
    return -1;
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
//  float p=a.floatafter("-p","proportion",0.5);
//  int r=a.intafter("-r","radius",20);
  float rp=a.floatafter("-rp","window size proportional to width=sqrt(area of region)",0.5);
  String iname=a.getarg();
  String bname=a.argor("*NONE*");
  a.done();

  Map2d<float> *map=Map2d<float>::readfile(iname);
  Map2d<bool> *bin=( Seq(bname,"*NONE*") ?
                     new Map2d<bool>(map->width,map->height,true) :
                     Map2d<bool>::readbinfile(bname)->binscaleto(map->width,map->height) );
	
  regmap=bin->getregionmap(true);
	regs=regmap.collectregions();

  // Create partial sums map
  printf("Creating partial sums\n");
  Map2d<float> *ps=new Map2d<float>(map->width,map->height);
  Map2d<int> *psa=new Map2d<int>(map->width,map->height);
  for (int y=0;y<ps->height;y++) {
  float psrow=0;
  int psrowa=0;
  for (int x=0;x<ps->width;x++) {
      if (bin->getpos(Pixel(x,y))) {
        psrow+=map->pos[x][y];
        psrowa++;
      }
    if (y==0) {
      ps->setpos(x,y,psrow);
      psa->setpos(x,y,psrowa);
    } else {
      ps->setpos(x,y,psrow+ps->getpos(x,y-1));
      psa->setpos(x,y,psrowa+psa->getpos(x,y-1));
    }
  }
  }

  Map2d<bool> *n=new Map2d<bool>(map->width,map->height,false);

  // Threshold original
  printf("Thresholding\n");
  for (int i=0;i<n->width;i++) {
  for (int j=0;j<n->height;j++) {
    if (bin->getpos(i,j)>0) {
      float r=rp*virtualwidth(i,j);
      float c=ps->getposclip(i+r/2,j+r/2)-ps->getposclip(i+r/2,j-r/2)-ps->getposclip(i-r/2,j+r/2)+ps->getposclip(i-r/2,j-r/2);
      int b=psa->getposclip(i+r/2,j+r/2)-psa->getposclip(i+r/2,j-r/2)-psa->getposclip(i-r/2,j+r/2)+psa->getposclip(i-r/2,j-r/2);
      c=c/(float)b; // c=Average
//      c=c/(float)((r+1)*(r+1)); // c=Average
      if (map->getpos(i,j)<c)
        n->setpos(i,j,true);
      else
        n->setpos(i,j,false);
    }
  }
  }

  n->writefile("total.bmp");

}
