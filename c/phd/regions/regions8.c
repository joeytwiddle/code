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
    if (r->list->len>rlarge) {
      printf("*");
      List<Region * > *nl=r->split();
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

  for (int i=1;i<=l->len && !key[KEY_ESC];i++) {

    // Extract text from region using adaptive threshold
    Region *vs=l->num(i);
    printf("%i (%i) : ",i,vs->getlist()->len);
    vs->scaleto(gs->width,gs->height);
    Map2d<bool> *filter=vs->map;
    Map2d<bool> *text=gs->adaptivethreshold(thresh,vs)->applyfn(&boolneg);
    Map2d<bool> *bit=Map2d<bool>::applyop(text,&booland,filter);
    Map2d<bool> *no=Map2d<bool>::applyop(o,&boolor,bit);
    destroy(o);
    o=no;
    // Map2d<bool> *bit=Map2d<bool>::applyop(text,&booland,region);
    // Map2d<bool>::applyop(text,&booland,bit)->display();
    
    /* // Make Hough transform
       printf("Performing Hough transform");
       Map2d<bool> hi=*bit;
       float l=bit->leftmost();
       float r=bit->rightmost();
       float t=bit->topmost();
       float b=bit->bottommost();
       bit=bit->crop();
       printf("...\n");
       Map2d<int> *hs=new Map2d<int>(100,100,&gimmezeroint);
       // Map2d<bool> hi=Map2d<bool>(p->width,p->height,0);
       List<V2d> *rs=bit->getlist();
       float scale=1.0/pythag(bit->width,bit->height);
       float oscale=hs->width;
       for (int i=1;i<=rs->len;i++) {
         hs->houghlinethrough(rs->num(i).x,rs->num(i).y,1,scale,oscale);
       }
       // hs->display();
       // Overlay lines found in Hough transform
       int tolerance=hs->largest()*realtol;
       for (int r=hs->width/16;r<hs->width;r++) {
         for (int aa=0;aa<hs->height;aa++) {
           if (hs->pos[r][aa]>tolerance) {
             float a=2*pi*aa/(float)hs->height;
             float m=-cos(a)/sin(a);
             V2d start=r*V2d(cos(a),sin(a));
             start=start+V2d(l,t);
             hi.line(start,start+30.0*V2d(cos(a),-sin(a)),true);
             float c=start.y-m*start.x;
             // float c=r/scale/hs->width/sin(a);
             // c=t+(b-t)*c/(float)bit->height-m*l;
             int xa=0;
             int ya=m*xa+c;
             int xb=hi.width;
             int yb=m*xb+c;
             hi.line(xa,ya,xb,yb,true);
           }
         }
       }
       his.add(hi);
       hi.display();
       // hs->display();*/
    
    destroy(vs);
    destroy(text);
    destroy(bit);
    
  }
  
  o->display();
  o->writefile(ofile);
  
/*  for (int i=1;i<=l->len;i++) {
    Region *r=l->num(i);
    printf("Getting map.\n");
    Map2d<bool> *tm=r->getmap(p->width,p->height);
    printf("Contracting.\n");
    free(tm);
    Map2d<bool> *noo=Map2d<bool>::applyop(oo,&boolor,tm->contract(1));
    free(oo);
    oo=noo;
  }
  
  oo->display();*/
  
  // for (int i=1;i<=his.len && !key[KEY_ESC];i++)
  // his.num(i).display();

}
