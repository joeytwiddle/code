// Clip picture by boolean image, find regions

// Extract from original large image

#include <joeylib.c>

int rsmall,rlarge;

bool notsmall(Region *r) {
  return (r->list->len>rsmall);
}

int main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String infile=a.argafter("-p","Picture","..\\stats\\image.bmp");
  String clipfile=a.argafter("-b","Boolean image","..\\stats\\final.bmp");
  String ofile=a.argafter("-o","Output clipped image","threshed.bmp");
  float thresh=a.floatafter("-t","threshold",0.4);
  float ambient=a.floatafter("-a","Ambient shade intesity",0.2);
  myRGB shade=myRGB::hue(a.floatafter("-h","Shade hue",2.0/3.0));
  int rad=a.intafter("-r","Smoothing radius for adaptive threshold",0);
  rsmall=a.intafter("-sr","Remove small regions below this size",400);
  rlarge=a.intafter("-lr","Split large regions above this size",800);
  float realtol=a.floatafter("-ht","tolerance level for Hough",0.8);
  // myRGB shade=myRGB((float)0.0,(float)0.0,(float)1.0);
  
  RGBmp *p=RGBmp::readfile(infile);
  Map2d<float> *b=Map2d<float>::readfile(clipfile);
  rad=0;
  Map2d<float> *gs=p->getgreyscale()->smoothed(rad);

  printf("Picture %i %i clipped mask %i %i\n",p->width,p->height,b->width,b->height);
  
  //p->display();
  //gs->display();
  
  // Find regions, keep largest region, and split
  List< Region * > *l=b->getrealregions();
  int larea=-1;
  int largest=-1;
  for (int i=1;i<=l->len;i++) {
    if (l->num(i)->area()>larea) {
      larea=l->num(i)->area();
      largest=i;
    }
  }

  Region *vs=l->num(largest);

    vs->scaleto(gs->width,gs->height);
    Map2d<bool> *filter=vs->getmap();
    float threshval=gs->adaptivethresholdnum(thresh,vs);

    for (float i=threshval-0.1;i<=threshval+0.1;i+=0.01) {
      Map2d<bool> *text=gs->realthreshold(i,vs)->applyfn(&boolneg);
      text->writefile(getnextfilename("th","bmp"));
    }
  
}
