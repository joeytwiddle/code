// Clip picture by boolean image, find regions

// Extract from original large image

// #define NOGRAPHICS

#include <joeylib.c>

#include <viseng.c>

int rsmall,rlarge;

bool notsmall(Region *r) {
  return (r->list->len>rsmall);
}

int main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String imagefile=a.getarg("Image file");
  String boolfile=a.getarg("Binary file");
  a.done();
  
  RGBmp *p=RGBmp::readfile(imagefile);
  Map2d<float> *b=Map2d<float>::readfile(boolfile);
//  rad=0;
//  Map2d<float> *gs=p->getgreyscale();
//  gs=gs->smoothed(rad);

  printf("Picture %i %i clipped mask %i %i\n",p->width,p->height,b->width,b->height);
  
  // Find regions, keep largest region, and split
  List< Region * > *l=b->getrealregions();
  printf("Got %i regions.\n",l->len);

/*  int larea=-1;
  int largest=-1;
  for (int i=1;i<=l->len;i++) {
    printf("*");
    if (l->num(i)->area()>larea) {
      larea=l->num(i)->area();
      largest=i;
    }
  }*/

  OrderedList< Region * > ol;
  for (int i=1;i<=l->len;i++)
    ol.add(l->p2num(i),l->num(i)->area());
  
  pos=V3d(1,1,0);
  ori=Ori::indir(V3d(0,0,1)-V3d(1,1,0));

  for (int i=ol.len;i>=1;i--) {

  Region *vs=ol.num(i);
  printf("Recovered largest.\n");

  printf("Generating points...\n");
  octree->clear();
    List<V2d> ps=*vs->getlist();
    for (int i=1;i<=ps.len;i++) {
      int x=ps.num(i).x;
      int y=ps.num(i).y;
      myRGB r=p->getpos(x,y);
      octree->add(V3d((float)r.r/128.0,(float)r.g/128.0,(float)r.b/128.0));
    }

  for (int i=0;i<=100;i++) {
    octree->add(V3d((float)i*255.0/100.0/128.0,0,0));
    octree->add(V3d(0,(float)i*255.0/100.0/128.0,0));
    octree->add(V3d(0,0,(float)i*255.0/100.0/128.0));
  }

  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  PPsetup(scrwid,scrhei,2.0);

  visengfly();

  if (key[KEY_ESC])
    break;

  }

  allegroexit();
  
}
