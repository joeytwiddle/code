#include <joeylib.c>

float affinity(Region *r,Line2d l) {
  Line2d rl=r->line();
  float c=mysquare(r->correlation()); // 0 bad - 1 good I think
  float a=myabs(V2d::normdot(l.b-l.a,rl.b-rl.a)); // 0 bad - 1 good
  return (1.0-a)*c; // 1 bad - 0 good
}

float affinity(Region *a,Region *b) {
  Line2d l=Line2d(a->centroid(),b->centroid());
  return affinity(a,l)*affinity(b,l);
}

List<int> proximity(Map2d<int> map,Region r,int rad) {
  Region search=r.expanded(rad);
  search.getlist()->remove(*r.getlist());
  List<Pixel> *ps=search.getlist();
  List<int> found;
  for (int i=1;i<=ps->len;i++) {
    int j=map.getpos(ps->num(i));
    if (j>0 && !found.contains(j))
      found.add(j);
  }
  search.freedom();
  return found;
}

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  bool useold=a.argexists("-o","use a previous calculation of the map");
  String iname=a.getarg();
  String bname=a.getarg();
  a.done();

  Map2d<float> *image=Map2d<float>::readfile(iname);
  Map2d<bool> total=*Map2d<bool>::readbinfile(bname)->binscaleto(image->width,image->height);
	
  Map2d<int> rm;
  if (!useold) {
	  rm=total.getregionmap(true);
	  rm.writefile("map.bmp");
  	writeinttofile("map.bmp.dat",rm.largest());
	} else {
	  int i=readintfromfile("map.bmp.dat");
	  if (i>255)
	    error("Cannot re-read map: greyscale resolution too low!");
  	rm=*Map2d<int>::readfile("map.bmp",i);
 }
	
	List<Region> cs=rm.collectregions();
	
	RGBmp *out=new RGBmp(image->width,image->height);
	
	for (int i=1;i<=cs.len;i++) {
	  out->line(cs.num(i).line(),myRGB::red);
    printf("%i/%i: ",i,cs.len);
	  List<int> rs=proximity(rm,cs.num(i),3);
	  for (int j=1;j<=rs.len;j++) {
	    printf("%i ",rs.num(j)-i);
      out->line(cs.num(i).centroid(),cs.num(rs.num(j)).centroid(),myRGB::blue);
      cs.p2num(i)->destroyallbutlist();
      cs.p2num(rs.num(j))->destroyallbutlist();
	  }
	  rs.freedom();
	  printf("\n");
	}
	
	out->writefile("out.bmp");
		
}