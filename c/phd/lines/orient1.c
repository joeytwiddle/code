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
	
  Map2d<float> *edgemag,*edgeang;
  image->edgedetection(Map2d<float>::sobel(),&edgemag,&edgeang);
  edgemag->writefile("edgemag.bmp");

  List<Region *> bs;
  for (int i=1;i<=rs.len;i++) {
  	printf("Region %i/%i\n",i,rs.len);
    List<Pixel> *ps=rs.num(i)->getlist();
    float ave=0;
    float edgeave=0;
    float maxedge=0;
		for (int j=1;j<=ps->len;j++) {
			ave=ave+image->getpos(ps->num(j));
			edgeave+=edgemag->getpos(ps->num(j));
			if (edgemag->getpos(ps->num(j))>maxedge)
			  maxedge=edgemag->getpos(ps->num(j));
		}
		ave=ave/(float)ps->len;
		edgeave=edgeave/(float)ps->len;
		Map2d<bool> *tmpb=image->threshold(ave);
		tmpb->clipby(rs.num(i),true);
		
/*		// High gradient edges become part of page background
		String s=Sformat("%i",getnextfilenum());
		tmpb->writefile(Sconc(s,"a",".bmp"));
		for (int j=1;j<=ps->len;j++) {
			if (edgemag->getpos(ps->num(j))>(maxedge*0.7))
			  tmpb->setpos(ps->num(j),true);
		}
		tmpb->writefile(Sconc(s,"b",".bmp"));
*/
//		tmpb->writefile(getnextfilename("tmp","bmp"));
		bs.add(tmpb->getrealregions(false));
		rs.destroynum(i);	i--;
		destroy(tmpb);
		printf("%i\n",bs.len);
	}
	//bs.freedom();
	
	printf("%i blobs found\n",bs.len);
	
	Map2d<float> mytest=Map2d<float>(image->width,image->height,0.0);
	RGBmp *out=new RGBmp(image->width,image->height,myRGB(0.0,0.0,0.0));
	Map2d<bool> *binned=new Map2d<bool>(image->width,image->height,false);
	for (int i=1;i<=bs.len;i++) {
		Region *b=bs.num(i);
//		b->getmap()->writefile(getnextfilename("blob","bmp"));
		List<Pixel> *ps=b->getlist();
		if (ps->len>10) {
//		fprintf(stdout,"Blob %i, pixels %i\n",i,ps->len);
		for (int j=1;j<=ps->len;j++) {
			out->setpixel(ps->num(j),myRGB(0.3,0.0,0.0));
			mytest.setpixel(ps->num(j),b->correlation());
		}
		
/*		List<Pixel> *bnd=b->getboundary();
		for (int k=1;k<=bnd->len;k++) {
		  out->setpixel(bnd->num(k),myRGB::red);
		}*/

//		  binned->setpixel(ps->num(j),true);
/*		Correlator cor=Correlator(ps);
		float c=cor.A();
//		printf("%f\n",c);
		V2d v=V2d(1.0,c).norm()*(float)sqrt(ps->len);
		V2d centroid=b->centroid();*/
/*		float xv=0;
		float yv=0;
		for (int k=1;k<=ps->len;k++) {
			xv=xv+square((float)ps->num(k).x-centroid.x);
			yv=yv+square((float)ps->num(k).y-centroid.y);
		}
		V2d v=V2d(xv,-yv).norm()*sqrt((float)ps->len)/2.0;*/
//		out->line(centroid-v,centroid+v,myRGB::blue);
    out->line(b->line(),myRGB::green);
// to clear list;		bs.destroynum(i); i--;
//		cor.freedom();
    }
	}
	out->writefile("out.bmp");
	mytest.writefile("mytest.bmp");

exit(0);

	// Find affinity between blobs
	printf("Finding affinities...\n");
	Map2d<float> *aff=new Map2d<float>(bs.len,bs.len);
	for (int i=1;i<=bs.len;i++) {
//  	for (int j=0;j<i;j++) // Fill triangular side with -1's
//  	  aff->setpos(i-1,j-1,0);
	  for (int j=i;j<=bs.len;j++) {
	    Region *a=bs.num(i);
	    Region *b=bs.num(j);
	    float af=affinity(a,b);
	    if (af>=-1.0 && af<=2.0) { }
	    else
	      af=0.0;
	    aff->setpos(i-1,j-1,af);
	    aff->setpos(j-1,i-1,af);
//	    printf("%f, ",affinity(a,b));
	  }    	
	}
	aff->writefile("aff.bmp");
	
	// Show which ones love each other
	Pixel bestp;
	float bestaff=10.0;
	for (int i=1;i<=bs.len;i++)
	for (int j=1;j<=bs.len;j++) {
	  if (i!=j) {
	    float a=aff->getpos(i-1,j-1)+aff->getpos(j-1,i-1);
	    if (a<bestaff) {
	      bestaff=a;
	      bestp=Pixel(i,j);
	    }
	  }
	}
	  Region *aa=bs.num(bestp.x);
	  Region *b=bs.num(bestp.y);
//	  out->arrow(Pixel(a->centroid()),Pixel(b->centroid()),myRGB::white);
		List<Pixel> *ps=b->getlist();
		for (int j=1;j<=ps->len;j++)
			out->setpixel(ps->num(j),myRGB::white);
		out->line(b->line(),myRGB::green);
		ps=aa->getlist();
		for (int j=1;j<=ps->len;j++)
			out->setpixel(ps->num(j),myRGB::white);
		out->line(b->line(),myRGB::green);
  out->writefile("out.bmp");
	
/*	for (int i=1;i<=bs.len;i++) {
	  float best=10.0;
	  int bestj=-1;
	  for (int j=1;j<=bs.len;j++) {
	    if (i!=j) {
	      if (aff->getpos(i-1,j-1)<best) {
	        best=aff->getpos(i-1,j-1);
	        bestj=j;
	      }
	    }
	  }
	  Region *a=bs.num(i);
	  Region *b=bs.num(bestj);
//	  out->arrow(Pixel(a->centroid()),Pixel(b->centroid()),myRGB::white);
		List<Pixel> *ps=b->getlist();
		for (int j=1;j<=ps->len;j++)
			out->setpixel(ps->num(j),myRGB::white);
		out->line(b->line(),myRGB::green);
	}*/
	out->writefile("out.bmp");
	
}