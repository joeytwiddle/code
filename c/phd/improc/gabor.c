/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

#include <joeylib.c>

float size;
int fsize;
float tmpgaus(int i,int j) {
  return gaussian(V2d(i-fsize/2,j-fsize/2).mag()/size);
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  float angle=a.floatafter("-a","angle",0);
  size=a.floatafter("-sz","size",20);
  float freq=a.floatafter("-f","frequency",1);
  float scale=a.floatafter("-sc","scale",0.5);
  String iname=a.getarg("filename");
  String oname=a.argor("output filename","gaborout.bmp");
  a.done();

  fsize=size*3;
  V2d dir=V2d::angle(angle);
  float thesin=sin(angle);
  float thecos=cos(angle);
  Map2d<float> *gfil=new Map2d<float>(fsize,fsize,&tmpgaus);
  Map2d<float> *image=Map2d<float>::readfile(iname);
  Map2d<float> *out=new Map2d<float>(image->width*scale,image->height*scale,0.0);
  for (int x=0;x<out->width;x++) {
    if ((x % (out->width/10))==0)
      printf("%i/%i\n",x,out->width);
  for (int y=0;y<out->height;y++) {
    if (x/scale>fsize/2 && x/scale<image->width-fsize/2 && y/scale>fsize/2 && y/scale<image->height-fsize/2) {
    float sum=0;
    for (int i=-fsize;i<=fsize;i++)
    for (int j=-fsize;j<=fsize;j++) {
      int p=x/scale+i; int q=y/scale+j;
//      sum=sum+image->getpos(p,q)*gaussian(V2d(i,j).mag()/size)*sin(V2d(p,q).dot(dir)*freq);
      sum=sum+image->getpos(p,q)*gfil->getpos(fsize/2+i,fsize/2+j)*sin((p*thesin+q*thecos)*freq);
    }
    out->setpos(x,y,sum);
    }
  }
  }
  out->writefile(oname);
  
}
