#include <joeylib.c>

void displaypic(RGBmp *pic);

int main(int argc, char **argv) {

  // Parse command line parameters
  if (argc==1) {
    printf("jshow : rgbmp reader and displayer (Joey Feb 99)\n");
    exit(0);
  }

  String infile=argv[1];

  RGBmp *in=new RGBmp(infile);

  in->applybrightness(.63);
//  pic->applycontrast(500);
  in->applycontrast(10);
  in->applygreyscale();

  RGBmp *pic=in->smoothed(0);

//  pic=pic->smoothed(7);
 
//  Map2d<float> *m=pic->getedges(1);
//  m->display();
//  exit(0);

//  in->display();
//  pic->display();
//  exit(0);

  List<List<Pixel> > *l=pic->classifyallscattered(.2,1);
//  List<List<Pixel> > *l=pic->oldclassifyscattered(*new myRGB(0,0,0),.4,1);
//  List<List<Pixel> > *l=pic->getfilledregions(0.8);
  printf("%i regions\n",l->len);

//exit(0);

  for (int i=1;i<=l->len;i++) {
    float r=(1+myrnd())/2.0;
    myRGB col=myRGB::hue(myrnd())*r;
    List<Pixel> *ps=l->p2num(i);
    for (int j=1;j<=ps->len;j++) {
      Pixel *p=ps->p2num(j);
      pic->bmp[p->x][p->y]=col;
    }
  }

//  pic->display();
//  exit(0);
  // Generate Hough transform
  // and centroid display
  Map2d<int> *hs=new Map2d<int>(500,500,&gimmezeroint);
  RGBmp *cen=new RGBmp(pic->width,pic->height);

  for (int i=1;i<=l->len;i++) {
    printf(":");

    List<Pixel> *ps=l->p2num(i);

    if (ps->len>100)
      printf("%i\n",ps->len);
    else {

    Pixel centroid=newPixel(0,0);
    for (int j=1;j<=ps->len;j++) {
      Pixel *p=ps->p2num(j);
//      hs->houghlinethrough(p->x,p->y,1);
      centroid=centroid+*p;
    }
    centroid=centroid/ps->len;

    hs->houghlinethrough(centroid.x,centroid.y,1);
    cen->bmp[centroid.x][centroid.y]=*new myRGB(255,255,255);

    }
    
  }
  printf("\n");

  in->display();
  pic->display();
  cen->display();
  hs->display();

  RGBmp *n=new RGBmp(infile);

  // Overlay lines found in Hough transform
  int tolerance=hs->largest()*.65;
  for (int r=hs->width/16;r<hs->width;r++) {
    for (int aa=0;aa<hs->height;aa++) {
      if (hs->pos[r][aa]>tolerance) {
        float a=2*pi*aa/(float)hs->height;
        float c=r/sin(a);
        float m=-cos(a)/sin(a);
        int xa=0;
        int ya=m*xa+c;
        int xb=pic->width;
        int yb=m*xb+c;
        n->line(xa,ya,xb,yb,new myRGB(255,255,255));
      }
    }
  }

  n->display();

  pic->writefile("o1pic.bmp");
  cen->writefile("o2cen.bmp");
  hs->writefile("o3hough.bmp");
  n->writefile("o4out.bmp");

  allegro_exit();

}
