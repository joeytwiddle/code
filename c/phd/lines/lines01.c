#include <joeylib.c>

#include "group.c"

Map2d<float> *recovertext(Map2d<float> *image,Region *r) {
  // Threshold region

  float f=0; int c=0;
  List<Pixel> *ps=r->getlist();
  for (int i=1;i<=ps->len;i++) {
    Pixel p=ps->num(i);
    f=f+image->getpos(p);
    c++;
  }

  float ave=f/(float)c;
  Map2d<bool> *bin=image->threshold(ave);
  bin->clipby(r,true);

  group(bin);

  return bin->smoothed(0);
}

List<Map2d<float> *> recovertext(Map2d<float> *image,List<Region *> regs) {
  List<Map2d<float> *> recs;
	for (int i=1;i<=regs.len;i++) {
	  recs.add(recovertext(image,regs.num(i)));
	}
  return recs;
}

List<Map2d<float> *> recovertext(Map2d<float> *image,Map2d<bool> *bin) {
  bin=bin->binscaleto(image->width,image->height);
//  bin->writefile(getnextfilename("bin","bmp"));
  List<Region *> regs=*bin->getrealregions();
  List<Map2d<float> *> recl=recovertext(image,regs);
  regs.destroyall();
  regs.freedom();
  return recl;
}

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  String iname=a.getarg();
  String bname=a.getarg();
  a.done();

  Map2d<float> *image=Map2d<float>::readfile(iname);
  Map2d<bool> *bin=Map2d<bool>::readbinfile(bname);

  List<Map2d<float> *> ls=recovertext(image,bin);
  for (int i=1;i<=ls.len;i++)
    ls.num(i)->writefile(getnextfilename("rec","bmp"));

}
