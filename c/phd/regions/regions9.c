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
  Map2d<float> *gs=p->getgreyscale()->smoothed(rad);

  printf("Picture %i %i clipped mask %i %i\n",p->width,p->height,b->width,b->height);
  
  //p->display();
  //gs->display();
  
  // Find regions, remove small regions, and split large regions
  List< Region * > *l=b->getrealregions();
  printf("%i regions, splitting:\n",l->len);
  l->filterby(&notsmall);
  for (int i=1;i<=l->len;i++) {
    Region *r=l->num(i);
    if (r->getlist()->len>rlarge) {
      printf("*");
      List<Region * > *nl=r->split();
      printf(".");
//      printf("Got list with %i %i\n",nl->num(1)->list->len,nl->num(2)->list->len);
      l->add(nl);
      destroy(nl);
      l->removenumkeeporder(i);
      destroy(r);
      i--;
    }
  }
  printf("\n");
  printf("%i regions\n",l->len);
  
  // Output pictures
  Map2d<bool> *o=new Map2d<bool>(p->width,p->height,false);
  Map2d<bool> *oo=new Map2d<bool>(p->width,p->height,false);
  
  List<Map2d<bool> > his;

//  gs=gs->scaleto(b->width,b->height);

//  gs->display();

  Map2d<float> *threshfound=new Map2d<float>(p->width,p->height,0.0);

  for (int i=1;i<=l->len && !key[KEY_ESC];i++) {

    // Extract text from region using adaptive threshold
    Region *vs=l->num(i);
    printf("%i (%i) : ",i,vs->getlist()->len);
    vs->scaleto(gs->width,gs->height);
    Map2d<bool> *filter=vs->map;
    float threshval=gs->adaptivethresholdnum(thresh,vs);
    Map2d<bool> *text=gs->realthreshold(threshval,vs)->applyfn(&boolneg);
//    Map2d<bool> *text=gs->adaptivethreshold(thresh,vs)->applyfn(&boolneg);
    Map2d<bool> *bit=Map2d<bool>::applyop(text,&booland,filter);
    Map2d<bool> *no=Map2d<bool>::applyop(o,&boolor,bit);
    destroy(o);
    o=no;

    threshfound->fill(vs,threshval);

    destroy(vs);
    destroy(text);
    destroy(bit);
    
  }
  
  o->display();
  o->writefile(ofile);
  threshfound->writefile("threshes.bmp");
  
}
