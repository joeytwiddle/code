// For efficiency:
// Calculate boundary of big blob as sum of smaller blob's boundaries
// Calculate closest dist using sub-blobs

#include <joeylib.c>

#define Extension 2.0

class Blob : public Region, public List<Region *> {
//#define
public:

  bool blobbed; // Shouldn't be set true without specifying a dad
  int dad;

  List<Pixel> centres; // Not really used

  Blob(Region *r) {
    Blob::add(r);
    Region::list=NULL; // Don't know where it gets set non-NULL, but it does!
    blobbed=false;
//    centres.add(r->centroid());   set in isword instead
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
    myerror("Can't blob without dad!");
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
//    printf("Made proper list size %i\n",Region::list->len);
    }
  }

  List<Pixel> *getlist() {
    remakelist();
    return Region::list;
  }

  Line2d line() {
//    Region::list=NULL;
    Blob::remakelist();
    // float a=angle();
//    printf("%f\n",a);
    // V2d v=V2d(sin(a+pi/2.0),cos(a+pi/2.0))*(float)sqrt(Region::list->len);
    V2d v=ori().norm()*(float)sqrt(Region::list->len);
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

  bool isword() { // Should be 3+ characters, arranged in a line!
    bool t=(len>=4);
    if (t && centres.len==0)
      centres.add(centroid());
    return t;
  }

  void paint(Map2d<int> *mmap,int c) {
    paint(mmap,c,0);
  }

  void paint(Map2d<int> *mmap,int c,int special) {
    if (blobbed || special>0) {
      makeboundary();
      for (int i=1;i<=boundary->len;i+=special)
        mmap->setpos(boundary->num(i),c);
    } else {
      remakelist();
      for (int i=1;i<=Region::list->len;i++) {
        mmap->setpos(Region::list->num(i),c);
      }
    }
  }

  V2d ori() {
    // return V2d(0.01,0.01);
    if (centres.len>1) {
    Correlator c=Correlator();
    for (int i=1;i<=centres.len;i++) {
      c.add(centres.num(i));
    }
/*    for (int i=1;i<=len;i++) {
      Region *r=num(i);
      V2d cen=r->centroid();
      int cnt=r->getlist()->len;
      for (int j=1;j<=cnt;j++)
        c.add(cen);
    }*/
    V2d ori=c.ori();
    c.freedom();
    return ori.norm();
    } else
      return V2d(0.01,0.01);
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

Blob *finddad(List<Blob> *bs,int b) {
  Blob *t=bs->p2num(b);
  if (!t->blobbed)
    return t;
  else
    return finddad(bs,t->dad);
}

Map2d<float> distmap=Map2d<float>(500,500,-1.0);

bool addhits(List<Blob> *bs,int o,List<int> hits) {
  Blob *b=bs->p2num(o);
  // What if b's blobbed, or any of those being added?
  // b shouldn't be blobbed, due to algorithm
  if (b->blobbed)
    error("addhits: got blobbed blob!");
  bool hit=false;
  for (int i=1;i<=hits.len;i++) {
    int j=hits.num(i);
    if (j!=o) {
      // Hits shouldn't be blobbed either
      // Actually, we find their dad
//      if (bs->p2num(j)->blobbed)
//        error("addhits: got blobbed hit!");
      Blob *a=finddad(bs,j);
      if (a==b)
        printf("Found dad of hit same as receiving blob\n");
      else {
//        b->remakeboundary();
//        a->remakeboundary();
//        b->getboundary()->add(a->getboundary());
        for (int k=1;k<=a->len;k++) {
          b->add(a->num(k));
//          b->centres.addifnot(a->num(k)->centroid()); // probably equivalent to |
        }                                        //                        V
        b->centres.addifnot(a->centres);
        a->blobbed=true;
        a->dad=o;
        hit=true;
      }
//      b->add(bs->p2num(j));
    }
  }
  // Destroy all knowledge about b (shouldn't need to do a since should be dadded in future)
  if (hit) {
    for (int i=0;i<500;i++) {
      distmap.setpos(i,o,-1.0);
      distmap.setpos(o,i,-1.0);
    }
  }
  return hit;
}

float closestdist(List<Blob> *bs,int a,int b) {
  if (distmap.getpos(a,b)<0) {
//  printf("*\n");
  Blob *ra=bs->p2num(a);
  Blob *rb=bs->p2num(b);
  ra->remakelist();
  rb->remakelist();
//  List<Pixel> as=*(ra->getlist());
//  List<Pixel> bs=*(ra->getlist());
  List<Pixel> as=*ra->getboundary();
  List<Pixel> bs=*rb->getboundary();
  destroy(ra->map);
  destroy(rb->map);
  float closest=dist(as.num(1),bs.num(1));
  for (int i=1;i<=as.len;i++) {
  for (int j=1;j<=bs.len;j++) {
    float d=dist(as.num(i),bs.num(j));
    if (d<closest) {
      closest=d;
//      printf("New winner %f\n",d);
    }
  }
  }
    distmap.setpos(a,b,closest);
    distmap.setpos(b,a,closest);
  }
  return distmap.getpos(a,b);
}

List<int> findfriends(List<Blob> *bs,int o,int tolerance) {
  List<int> ls;
  for (int i=1;i<=bs->len;i++) {
    if (i!=o)
      if (!bs->p2num(i)->blobbed && !bs->p2num(o)->blobbed)
        if (closestdist(bs,i,o)<=tolerance)
          ls.add(i);
  }
  return ls;
}

void findlines(List<Blob> bs,Map2d<int> rm) {
  int tolerance=1;
  while (true) {
    //

    Map2d<int> mymap=Map2d<int>(rm.width,rm.height,0);

    printf("Extending words...\n");
    bool change=true;
    while (change) {
      Map2d<int> *mymap2=new Map2d<int>(rm.width,rm.height,0);
      change=false;
      for (int i=1;i<=bs.len;i++) {
        Blob *b=bs.p2num(i);
/*          List<Pixel> ps;
          ps.add(Pixel(30,30));
          plotandfree(mymap2->ps,10);*/
        if (!b->blobbed) {
          if (b->isword() && b->blength()>2.0*b->bheight()) {
            // Extend either end of b
            V2d cen=b->centroid();
            //+b->bheight()*2.0;
            V2d o=b->ori();
            float len=b->blength();
            V2d u=o*len;
            float hei=b->bheight();
            V2d v=o.perp()*hei;

/*            mymap.line(cen-u+v,cen+u+v,i);
            mymap.line(cen+u+v,cen+u-v,i);
            mymap.line(cen+u-v,cen-u-v,i);
            mymap.line(cen-u-v,cen-u+v,i);
            mymap.line(cen+u,cen+u+o*hei*Extension,i);
            mymap.line(cen-u,cen-u-o*hei*Extension,i);*/
            b->remakelist();
//            b->paint(&mymap,i); // Don't paint blobbed ones!

            // Search cen, ori
            List<int> hits;
            List<Pixel> search;
            V2d out=o*hei*Extension;
            V2d up=o.perp()*hei/2.0;
            search.addandfree(Pixel::line(cen+u+up,cen+u+up+out));
            search.addandfree(Pixel::line(cen+u+up+out,cen+u-up+out));
            search.addandfree(Pixel::line(cen+u-up+out,cen+u-up));
            search.addandfree(Pixel::line(cen-u+up,cen-u+up-out));
            search.addandfree(Pixel::line(cen-u+up-out,cen-u-up-out));
            search.addandfree(Pixel::line(cen-u-up-out,cen-u-up));
            for (int j=1;j<=search.len;j++) {
//              mymap.setpos(search.num(j),i);
              int g=rm.getpos(search.num(j));
              if (g>0 && g!=i)
                hits.addifnot(g);
            }
            search.freedom();
            if (hits.len>0)
              printf("%i hits\n",hits.len);
            if (addhits(&bs,i,hits))
              change=true;
            hits.freedom();

            if (!b->blobbed) {
            b->remakelist();
//            b->paint(mymap2,i); // Don't paint blobbed ones!
            V2d cennow=b->centroid();

//            mymap2->plotandfree(Pixel::line(cennow+Pixel(-5,-5),cennow+Pixel(5,5)),0);
//            mymap2->plotandfree(Pixel::line(cennow+Pixel(5,-5),cennow+Pixel(-5,5)),0);

            }

          } else {
            b->remakelist();
//            b->paint(&mymap,i,2); // Don't paint blobbed ones!
          }
        }
      }
/*      RGBmp r=mymap.hueify();
      RGBmp r3=mymap2->hueify();
      r.writefile(getnextfilename("owner","bmp"));
      r3.writefile(getnextfilename("owner","bmp"));
      r.freedom();
      r3.freedom();*/
      mymap.freedom();
      destroy(mymap2);
      mymap=Map2d<int>(rm.width,rm.height,0);

    }

      printf("Joining neighbours...\n");
      change=false;
      for (int i=1;i<=bs.len && !change;i++) {
        if (!bs.p2num(i)->blobbed) {
//          printf("Finding friends...\n");
          List<int> hits=findfriends(&bs,i,tolerance);
//          printf("Got %i hits for %i/%i\n",hits.len,i,bs.len);
          // Need to combine with hits then restart!
//          printf("Adding hits\n");
          bool worked=addhits(&bs,i,hits);
//          printf("Done adding hits\n");
          hits.freedom();
//          printf("Done freeing hits\n");
          Blob *b=bs.p2num(i);
          b->paint(&mymap,i); // Don't paint blobbed ones!

          V2d cen=b->centroid();
          V2d o=b->ori();
          float len=b->blength();
          V2d u=o*len;
          mymap.line(cen-o*len*2.0,cen+o*len*2.0,i);

//          printf("Done painting\n");
        }
//        } else
//          printf("Blobbed: skipping\n");

      }
      tolerance++;

      for (int i=1;i<=bs.len;i++) {
        Blob *b=bs.p2num(i);
        if (finddad(&bs,i)==b) {
//        if (!b->blobbed) {
          for (int j=1;j<=b->centres.len;j++) {
            V2d cennow=b->centres.num(j);
            plotandfree(&mymap,Pixel::line(cennow+Pixel(-5,0),cennow+Pixel(+5,0)),255);
            plotandfree(&mymap,Pixel::line(cennow+Pixel(0,-5),cennow+Pixel(0,+5)),255);
          }
        }
      }


      RGBmp r2=mymap.hueify();
      r2.writefile(getnextfilename("owner","bmp"));
      r2.freedom();
      mymap.freedom();
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
	  //rm.writefile("map.bmp");
  	//writeinttofile("map.bmp.dat",rm.largest());
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