#include <joeylib.c>

int main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  float realtol=a.floatafter("-ht","tolerance level for Hough",0.8);
  String binfile=a.getarg("binary file");
  String origfile=a.argor("image file",binfile);
  a.done();

  Map2d<bool> *bin=Map2d<bool>::readbinfile(binfile);

    // Perform Hough transform
    int max=pythag(bin->width,bin->height);
    Map2d<int> *hs=new Map2d<int>(max/4,200,&gimmezeroint);
//    List<Pixel> *rs=bin->getlist();
//    printf("Doing %i...\n",rs->len);
//    for (int i=1;i<=rs->len;i++) {
    for (int i=0;i<bin->width;i++)
    for (int j=0;j<bin->height;j++)
      if (bin->getpos(i,j))
        hs->houghlinethrough(Pixel(i,j),bin);
    hs->writefile("hough.bmp");
    Map2d<float> *shs=hs->smoothed(0);
    shs->writefile("houghsmoothed.bmp");

    // Overlay lines found in Hough transform
    RGBmp hi=*RGBmp::readfile(origfile)->scaletowidth(bin->width);
    int tolerance=hs->largest()*realtol;
    for (int aa=0;aa<hs->height;aa++) {
      for (int r=0;r<hs->width;r++) {
        if (hs->pos[r][aa]>tolerance) {
          Line2d l=hs->houghlinefor(aa,r,bin);
          //printf("Plotting line %s\n",l.toString());
          hi.line(l,myRGB(255,0,0));
/*          float a=2.0*pi*aa/(float)hs->height;
          float m=-cos(a)/sin(a);
          V2d start=(r-hs->width/2)*V2d(cos(a),sin(a));
          // start=start+V2d(l,t);
          hi.line(start,start+30.0*V2d(cos(a),-sin(a)),myRGB(255,255,0));
          float c=start.y-m*start.x;
//          float c=r/scale/hs->width/sin(a);
//          c=t+(b-t)*c/(float)bit->height-m*l;
          int xa=0;
          int ya=m*xa+c;
          int xb=hi.width;
          int yb=m*xb+c;
          hi.line(xa,ya,xb,yb,&myRGB(0,255,255));*/
        }
      }
    }
    hi.writefile("lines.bmp");

}
