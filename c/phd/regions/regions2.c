// Clip picture by boolean image, find regions

#include <joeylib.c>

int main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String infile=a.argafter("-p","Picture","..\\stats\\image.bmp");
  String clipfile=a.argafter("-b","Boolean image","..\\stats\\final.bmp");
  String ofile=a.argafter("-o","Output clipped image","clipped.bmp");
  float thresh=a.floatafter("-t","threshold",0.5);
  float ambient=a.floatafter("-a","Ambient shade intesity",0.2);
  myRGB shade=myRGB::hue(a.floatafter("-h","Shade hue",2.0/3.0));
//  myRGB shade=myRGB((float)0.0,(float)0.0,(float)1.0);

  RGBmp *p=new RGBmp(infile);
  Map2d<float> *b=Map2d<float>::readfile(clipfile);
  Map2d<float> *gs=p->getgreyscale();

  // Find regions
  List< List<V2d> > *l=b->getregions();

//  printf("%i regions\n",l->len);

  // Output picture
  Map2d<bool> *o=new Map2d<bool>(p->width,p->height,false);

  for (int i=1;i<=l->len && !key[KEY_ESC];i++) {
    List<V2d> *vs=l->p2num(i);
    printf("%i points: ",vs->len);
    Map2d<bool> *text=gs->adaptivethreshold(thresh,vs)->applyfn(&boolneg);
//    text->display();
    Map2d<bool> *region=b->makeregion(vs);
    Map2d<bool> *bit=Map2d<bool>::applyop(text,&booland,region);
//    Map2d<bool> *bit=Map2d<bool>::applyop(text,&booland,region);
//    Map2d<bool>::applyop(text,&booland,region)->display();
    o=Map2d<bool>::applyop(o,&boolor,bit);
  }

//  gs->display();
  o->display();
  o->writefile(ofile);
  exit(0);

}
