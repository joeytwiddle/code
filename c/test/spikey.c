/* This source code is freely distributable under the LGPL
   (lesser GNU public licence).  Please keep this tag with
   this code, and add your own if you contribute.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark | www.changetheworld.org.uk */

#include <joeylib.c>

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  int w=a.intafter("-w","width",275);
  int h=a.intafter("-h","height",116);
  int numpts=a.intafter("-n","number of points",100);
  a.done();


  FILE *file=fopen("region.txt","wa");
  fprintf(file,"[Normal]\n");
  fprintf(file,"Numpoints=%i\n",numpts);

  V2d cen=V2d((float)w/2.0,(float)h/2.0);

  for (int i=0;i<numpts;i++) {

    V2d a=10.0*(w+h)*V2d::angle(-pi/4.0+2.0*pi*(float)i/(float)numpts);

    a=a+cen;

    if (a.x<0)
      a=Line2d(cen,a).intersect(Line2d(V2d(0,0),V2d(0,1)));
    if (a.y<0)
      a=Line2d(cen,a).intersect(Line2d(V2d(0,0),V2d(1,0)));
    if (a.x>w)
      a=Line2d(cen,a).intersect(Line2d(V2d(w,0),V2d(w,1)));
    if (a.y>h)
      a=Line2d(cen,a).intersect(Line2d(V2d(0,h),V2d(1,h)));

    a=a-cen;
    float rnd=myrnd();
    a=a*rnd;

    a=a+cen;

    fprintf(file,"%i,%i",(int)a.x,(int)a.y);
    if (i<numpts-1)
      fprintf(file,", ");

  }

  fprintf(file,"\n");

  fclose(file);

}
