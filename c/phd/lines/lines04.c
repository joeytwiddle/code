#include <joeylib.c>

float affinity(Region *r,Line2d l) {
  Line2d rl=r->line();
  float c=r->correlation(); // -1 bad - 1 good
  float a=myabs(V2d::normdot(l.b-l.a,rl.b-rl.a)); // 0 bad - 1 good
  return (1-a)*(-c+1); // 1 bad - 0 good
}

float affinity(Region *a,Region *b) {
  Line2d l=Line2d(a->centroid(),b->centroid());
  return affinity(a,l)*affinity(b,l);
}

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  String iname=a.getarg();
  String bname=a.getarg();
  a.done();

  Map2d<float> *image=Map2d<float>::readfile(iname);
  Map2d<bool> *bin=Map2d<bool>::readbinfile(bname)->binscaleto(image->width,image->height);
	
  printf("Got here\n");
  List<Region *> rs=*bin->getrealregions();
  printf("Got %i regions\n",rs.len);
	
  List<Region *> bs;
  for (int i=1;i<=rs.len;i++) {
  	printf("Region %i/%i\n",i,rs.len);
    List<Pixel> *ps=rs.num(i)->getlist();
    float ave=0;
		for (int j=1;j<=ps->len;j++)
			ave=ave+image->getpos(ps->num(j));
		ave=ave/(float)ps->len;
		Map2d<bool> *tmpb=image->threshold(ave);
		tmpb->clipby(rs.num(i),true);
//		tmpb->writefile(getnextfilename("tmp","bmp"));
		bs.add(tmpb->getrealregions(false));
		rs.destroynum(i);	i--;
		destroy(tmpb);
		printf("%i\n",bs.len);
	}
	//bs.freedom();
	
	printf("%i blobs found\n",bs.len);
	
	RGBmp *out=new RGBmp(image->width,image->height,myRGB(0.0,0.0,0.0));
	Map2d<bool> *binned=new Map2d<bool>(image->width,image->height,false);
	for (int i=1;i<=bs.len;i++) {
		Region *b=bs.num(i);
//		b->getmap()->writefile(getnextfilename("blob","bmp"));
		List<Pixel> *ps=b->getlist();
//		fprintf(stdout,"Blob %i, pixels %i\n",i,ps->len);
		for (int j=1;j<=ps->len;j++)
			out->setpixel(ps->num(j),myRGB(0.3,0.0,0.0));
//		  binned->setpixel(ps->num(j),true);
		Correlator cor=Correlator(ps);
		float c=cor.A();
		cor.freedom();
		printf("%f\n",c);
		V2d v=V2d(1.0,c).norm()*cor.correlation()*(float)sqrt(ps->len);
		V2d centroid=b->centroid();
/*		float xv=0;
		float yv=0;
		for (int k=1;k<=ps->len;k++) {
			xv=xv+square((float)ps->num(k).x-centroid.x);
			yv=yv+square((float)ps->num(k).y-centroid.y);
		}
		V2d v=V2d(xv,-yv).norm()*sqrt((float)ps->len)/2.0;*/
		out->line(centroid-v,centroid+v,myRGB::blue);
		bs.destroynum(i); i--;
	}
	// out->writefile("out.bmp");

	// Find affinity between blobs
	Map2d<float> aff=Map2d<float>(bs.len,bs.len);
	for (int i=1;i<=bs.len;i++) {
  	for (int j=0;j<i;j++) // Fill triangular side with -1's
  	  aff.setpos(i,j,-1);
	  for (int j=i;j<=bs.len;j++) {
	    Region *a=bs.num(i);
	    Region *b=bs.num(j);
	    aff.setpos(i,j,affinity(a,b));
	  }
	}
	aff.writefile("aff.bmp");
	
}
