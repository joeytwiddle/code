#include <joeylib.c>

class Blob : public Region, public List<Region *> {
//#define
public:

  bool blobbed;

  Blob(Region *r) {
    Blob::add(r);
    Region::list=NULL; // Don't know where it gets set non-NULL, but it does!
    blobbed=false;
  }
  void add(Region *r) {
    clear();
    List<Region *>::add(r);
    width=r->width;
    height=r->height;
//    printf("One region added\n");
  }
  void add(Blob *b) {
    printf("*\n");
    clear();
    List<Region *>::add((Region *)b);
    b->blobbed=true;
//    printf("bi\n");
  }

  void clear() {
    left=right=top=bottom=-1;
/*    Region::list=NULL; // should free these!
    map=NULL;
    cor=NULL;*/
//    printf("A\n");
    Region::destroyall();
//    printf("B\n");
  }
  void remakemap() {
    myerror("Can't make new map yet!");
  }
  void remakelist() {
//    printf("Trying!\n");
    if (Region::list==NULL) {
    Region::list=new List<Pixel>();
    for (int i=1;i<=len;i++)
      Region::list->add(num(i)->getlist());
    printf("Made proper list size %i\n",Region::list->len);
    }
  }

  Line2d line() {
//    Region::list=NULL;
    Blob::remakelist();
    float a=angle();
//    printf("%f\n",a);
    V2d v=V2d(sin(a+pi/2.0),cos(a+pi/2.0))*(float)sqrt(Region::list->len);
    V2d c=centroid();
//    printf("Made proper line length %f List %i\n",v.mod(),Region::list->len);
    return Line2d(c-v,c+v);
  }

  List<int> expand(Map2d<int> *rm) {
    List<int> l;
    for (int i=1;i<=len;i++) {
      Region *r=num(i);
      List<int> ls=r->checkexpansion(rm); //,myrnd()*256);
      l.add(ls);
      ls.freedom();
    }
    return l;
  }

  bool isword() {
    return (len>=3);
  }

  void paint(Map2d<int> *mmap,int c) {
    // getlist() doesn't work!
    remakelist();
    printf("Painting %i\n",Region::list->len);
    for (int i=1;i<=Region::list->len;i++) {
//      printf("%s\n",Region::list->num(i).toString());
      mmap->setpos(Region::list->num(i),c);
    }
  }

};

int numblobs(Blob b) {
  return b.len;
}

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

void addhits(List<Blob> *bs,int o,List<int> hits) {
  Blob *b=bs->p2num(o); // What if b's blobbed?
  for (int i=1;i<=hits.len;i++) {
    int j=hits.num(i);
    if (j!=o) {
      Blob *a=bs->p2num(j);
      for (int j=1;j<=a->len;j++)
        b->add(a->num(j));
      a->blobbed=true;
//      b->add(bs->p2num(j));
    }
  }
}

void findlines(List<Blob> bs,Map2d<int> rm) {
  while (true) {
    //
    bool change=true;
    while (change) {
      change=false;
      for (int i=1;i<=bs.len;i++) {
        Blob *b=bs.p2num(i);
        if (!b->blobbed && b->isword()) {
          // Extend either end of b
          V2d cen=b->centroid();
          float newlength=b->blength()+b->bheight()*2.0;
          V2d ori=b->ori()*newlength;

          // Search cen, ori
//          change=true;
        }
      }
    }

      change=false;
      Map2d<int> mymap=Map2d<int>(rm.width,rm.height,0);
      for (int i=1;i<=bs.len && !change;i++) {
          List<int> hits=bs.num(i).expand(&rm);
          printf("Got %i hits for %i/%i\n",hits.len,i,bs.len);
//          printf(hits.toString());
          // Need to combine with hits then restart!

          addhits(&bs,i,hits);
          hits.freedom();

          bs.p2num(i)->paint(&mymap,i);

      }

      mymap.hueify().writefile(getnextfilename("owner","bmp"));
  }
}

/*void findlines(List<Blob> bs,Map2d<int> rm) {
  while (true) {

    // Is there a three+ connected blob?
    int best=bs.findbest(&numblobs,1);
/*    if (best>-1) {
      // Expand
      Blob b=bs.num(best);
      printf("Blob %i has %i\n",best,numblobs(b));
    } else {*//*
      // Expand all single blobs
      printf("No blobs\n");
      bool done=false;
      for (int i=1;i<=bs.len && !done;i++) {
        //if (numblobs(bs.num(i))==1) {
          List<int> hits=bs.num(i).expand(&rm);
          printf("Got %i hits for %i/%i\n",hits.len,i,bs.len);
          printf(hits.toString());
          // Need to combine with hits then restart!
          hits.freedom();
          // Removal fucks up map!
/*          Blob n=Blob(bs.p2num(i));
          for (int i=1;i<=hits.len;i++)
            n.add(bs.p2num(hits.num(i)));
          bs.removenum(i);
          for (int i=1;i<=hits.len;i++) // Removal fucks up lines
            bs.removenum(hits.num(i));
          bs.add(n);
          n.paint(&rm,bs.len);*//*
//          done=true;
//        }
      }
//    }
    rm.writefile(getnextfilename("regmap","bmp"));
  }

}*/

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
	
	List<Blob> bs;
	for (int i=1;i<=cs.len;i++) {
	  bs.add(Blob(cs.p2num(i)));
//	  for (;i<=cs.len && myrnd()<0.9;i++)
//  	  bs.p2num(bs.len)->add(cs.p2num(i));
  }
	
/*	RGBmp *out=new RGBmp(image->width,image->height);
	
	for (int i=1;i<=bs.len;i++)
	  out->line(bs.p2num(i)->line(),myRGB::red);
	
	out->writefile("out.bmp");*/
		
  findlines(bs,rm);

}