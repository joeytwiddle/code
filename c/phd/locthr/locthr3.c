// Perform local thresholds on image regions
// Region by region (but overlap happens)

#include <joeylib.c>

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
//  float p=a.floatafter("-p","proportion",0.5);
  int r=a.intafter("-r","radius",100);
  String iname=a.getarg();
  String bname=a.getarg();
  a.done();

  Map2d<float> *map=Map2d<float>::readfile(iname);
  Map2d<float> *image=map;

  Map2d<bool> *bin=Map2d<bool>::readbinfile(bname)->binscaleto(image->width,image->height);
	
  List<Region *> rs=*bin->getrealregions();
	
  Map2d<bool> *n=new Map2d<bool>(map->width,map->height,false);

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
    psa->writefile("psa.bmp");

  for (int i=1;i<=rs.len;i++) {
  	printf("Region %i/%i\n",i,rs.len);
  	Region *reg=rs.num(i);
    List<Pixel> *rps=rs.num(i)->getlist();
    Map2d<bool> *rmap=rs.num(i)->getmap();

    // Threshold original
    printf("Thresholding\n");
    for (int i=reg->leftmost();i<reg->rightmost();i++) {
    for (int j=reg->topmost();j<reg->bottommost();j++) {
      if (reg->map->getpos(i,j)>0) {
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

	}

  n->writefile("total.bmp");

}
