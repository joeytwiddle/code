// Clip picture by boolean image, find regions

#include <joeylib.c>

int main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String infile=a.argafter("-p","Picture","image.bmp");
  String clipfile=a.argafter("-b","Boolean image","final.bmp");
  String ofile=a.argafter("-o","Output clipped image","clipped.bmp");
  float ambient=a.floatafter("-a","Ambient shade intesity",0.2);
  myRGB shade=myRGB::hue(a.floatafter("-h","Shade hue",2.0/3.0));
//  myRGB shade=myRGB((float)0.0,(float)0.0,(float)1.0);
  RGBmp *p=new RGBmp(infile);
  Map2d<float> *b=Map2d<float>::readfile(clipfile);

  // Find regions
  List< List<V2d> > *l=b->getregions();

  // Output picture
  RGBmp *o=new RGBmp(p->width,p->height);

  printf("%i regions\n",l->len);

  for (int i=1;i<=l->len;i++) {
    List<V2d> *vs=l->p2num(i);
    printf("%i points\n",vs->len);

    for (int j=1;j<=vs->len;j++) {
      V2d v=vs->num(j);
      int x=v.x;
      int y=v.y;
      o->bmp[x][y]=p->bmp[x][y];
    }
  }


  o->display();
  exit(0);

  for (int i=0;i<p->width;i++) {
    for (int j=0;j<p->height;j++) {
      if (b->pos[i*b->width/p->width][j*b->height/p->height])
        o->bmp[i][j]=p->bmp[i][j];
      else
        o->bmp[i][j]=shade*p->bmp[i][j];
        
    }
  }
  o->writefile(ofile);
}
