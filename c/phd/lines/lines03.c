#include <joeylib.c>

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
	out->writefile("out.bmp");
//	binned->writefile("binned.bmp");
		
/*  List<Map2d<float> *> ls=recovertext(image,bin);
  for (int i=1;i<=ls.len;i++)
    ls.num(i)->writefile(getnextfilename("rec","bmp"));*/

}
