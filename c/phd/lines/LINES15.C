// For efficiency:
// Calculate closest dist using sub-blobs

#include <joeylib.c>

#define Extension 2.0
#define maxblobs 1500
#define Undefined -1
#define Base 0
#define Character 1
#define Word 2
#define Sentence 3
#define Paragraph 4

bool approx,usexv;

class Blob;

Map2d<int> rm;
List<Blob> bs;
Map2d<float> distmap=Map2d<float>(maxblobs,maxblobs,-1.0);

class Blob  {
public:
  int col,dad,type,area;
  bool blobbed;
  V2d centroid;
  List<Pixel> bpixs;
//  List<Polar> bpols;
  List<int> subs;
  List<int> bases;
//  List<int> allsubs;
  int basecnt;
  int sentcnt;
  V2d *memori;

  Blob(int c,Region *r) { // A base blob from an image region
    col=c;
    centroid=r->centroid();
    bpixs=*r->getboundary();
    area=r->getlist()->len;
//    for (int i=1;i<=bpixs.len;i++)
//      bpols.add(Polar(bpixs.num(i)));
    type=Base;
    sentcnt=0;
    basecnt=1;
    blobbed=false;
    bases.add(c);
    memori=NULL;
  }

  Blob(int c) { // A new blob for adding to
    col=c;
    blobbed=false;
    type=Undefined; //type=b->type; // This is biased towards this blob over others in hit!
    area=0;
    centroid=V2d(0,0);
    bpixs=List<Pixel>();
    sentcnt=0;
    basecnt=0;
    memori=NULL;
  }

  void add(int bi) {
    Blob *b=bs.p2num(bi);
    subs.add(bi);
//    allsubs.add(bi);
//    allsubs.add(b->allsubs);
    if (b->gettype()==Sentence)
      sentcnt++;
    sentcnt+=b->sentcnt;
    basecnt+=b->basecnt;
    bases.add(b->bases);
    centroid=(centroid*area+b->centroid*b->area)/(float)(area+b->area);
    area+=b->area;
    bpixs.add(b->bpixs);
    b->blobbed=true;
    b->dad=col;
  }

  List<int> getsubblobs() {
    List<int> ls;
    ls.add(subs);
//    bool done=false;
//    while (!done) {
//      done=true;
      for (int i=1;i<=ls.len;i++) {
//        printf("%s\n",ls.toString());
        Blob *b=bs.p2num(ls.num(i));
        if (b->subs.len>0) {
        printf("%i owns %s\n",i,b->subs.toString());
//            done=false;
//          printf("Removing %i\n",ls.num(i));
//          ls.removenum(i); // Don't remove any more or we just end up with Bases!
          ls.add(b->subs);
//          printf("Adding %s\n",b->subs.toString());
//          i--; // Ensures blob after this one removed is not skipped
        }
//        printf("%i/%i\n",i,ls.len);
      }
//    }
    return ls;
  }

  List<int> getfirsts(int t) {
    // Finds all first blobs in the tree of type t
    List<int> ls;
    ls.add(subs);
    bool done=false;
    while (!done) {
      done=true;
      for (int i=1;i<=ls.len;i++) {
        Blob *b=bs.p2num(ls.num(i));
        if (b->subs.len>0) {
        int k=b->gettype();
        if (b->gettype()!=t) {
          done=false;
          ls.removenum(i);
          ls.add(b->subs);
          i--; // Ensures blob after this one removed is not skipped
        }
        }
      }
    }
    // Remove all blobs above that type
    for (int i=1;i<=ls.len;i++)
      if (bs.p2num(ls.num(i))->gettype()!=t) {
        ls.removenum(i);
        i--;
//        printf("Did have to remove one after all\n");
      }
    return ls;
  }

  int gettype() { // *** If contains 2+ sentences then Paragraph
    if (type==Undefined) {
/*      int basecnt=0;
      int sentcnt=0;
      printf("%i subblobs\n",sbs.len);
      for (int i=1;i<=allsubs.len;i++) {
        int t=bs.p2num(allsubs.num(i))->gettype();
        if (t==Base)
          basecnt++;
        if (t==Sentence) {
          sentcnt++;
          printf("Found a sentence!\n");
        }
      }
      sbs.freedom();*/
//      printf("Base: %i Sentences: %i\n",basecnt,sentcnt);
      if (sentcnt>=2)
        type=Paragraph;
      else if (basecnt>=2 && width()>3.0*height())
        type=Word;
      else
        type=Character;
    }
    return type;
  }

  void plot(RGBmp *m) {
    int t=gettype();
    myRGB r=( t==Base      ? myRGB::white   :
              t==Character ? myRGB::yellow  :
              t==Word      ? myRGB::red     :
              t==Sentence  ? myRGB::green   :
              t==Paragraph ? myRGB::magenta :
                             myRGB::cyan    );
//    r=myRGB::hue(myrnd());
    if (t==Word || t==Sentence) {
      V2d x=ori()*width();
      V2d y=ori().perp()*height();
      m->line(centroid-x+y,centroid+x+y,r);
      m->line(centroid+x-y,centroid-x-y,r);
      if (t==Word) {
        m->line(centroid-x-y,centroid-x+y,r);
        m->line(centroid+x+y,centroid+x-y,r);
      }
      m->filledcircle(centroid,4,r);
// setpos error      plotandfree(m,Pixel::line(centroid-o,centroid+o),r);
    } else {
      for (int j=1;j<=bpixs.len;j++)
        m->setpixel(bpixs.num(j),r);
    }
    for (int i=1;i<=bases.len;i++) {
      Blob *b=bs.p2num(bases.num(i));
      m->line(Pixel(b->centroid)+Pixel(-5,0),Pixel(b->centroid)+Pixel(+5,0),r); //myRGB::blue);
      m->line(Pixel(b->centroid)+Pixel(0,-5),Pixel(b->centroid)+Pixel(0,+5),r); //myRGB::blue);
    }
  }

  V2d ori() {
//    List<int> sbs=getfirsts(Base);
if (memori==NULL) {
    if (bases.len<2)
      error("Trying to get ori but not enough points!");//return V2d(0,0.1);
    Correlator c=Correlator();
    for (int i=1;i<=bases.len;i++) {
      int cnt=bs.p2num(bases.num(i))->area;
      V2d ctr=bs.p2num(bases.num(i))->centroid;
      for (int j=1;j<=cnt;j++)
        c.add(ctr);
    }
    memori=new V2d[1];
    *memori=c.ori();
    c.freedom();
    }
    return *memori;
  }

  float compare(V2d lori) {
    V2d ctr=centroid;
    float len=0;
    for (int i=1;i<=bpixs.len;i++) {
      float d=V2d::dot(lori,V2d(bpixs.num(i))-ctr);
      if (myabs(d)>len)
        len=myabs(d);
    }
    return len;
  }

  float width() {
    return compare(ori());
  }

  float height() {
    return compare(ori().perp());
  }

};

int finddad(int b) {
  if (!bs.p2num(b)->blobbed)
    return b;
  else
    return finddad(bs.p2num(b)->dad);
}

float closestdist(int ia,int ib) {
  if (distmap.getpos(ia,ib)<0) {
    float closest;

    // Optimisation: If A or B are subbed, find closest from subs!
    if (bs.p2num(ia)->subs.len>0) {

      List<int> subs=bs.p2num(ia)->subs;
      closest=closestdist(ib,subs.num(1));
      for (int i=2;i<=subs.len;i++) { // closest=min{asubs,b}
        float c=closestdist(ib,subs.num(i));
        if (c<closest)
          closest=c;
      }

    } else if (bs.p2num(ib)->subs.len>0) {
      closest=closestdist(ib,ia); // Switch to do above

    } else if (approx) {
      closest=V2d::dist(bs.p2num(ia)->centroid,bs.p2num(ib)->centroid);

    } else {

    List<Pixel> as=bs.p2num(ia)->bpixs;
    List<Pixel> cs=bs.p2num(ib)->bpixs;

    // Search both boundaries
    closest=dist(as.num(1),cs.num(1));
    for (int i=1;i<=as.len;i++)
    for (int j=1;j<=cs.len;j++) {
      float d=dist(as.num(i),cs.num(j));
      if (d<closest) {
        closest=d;
      }
    }

    }

    distmap.setpos(ia,ib,closest);
    distmap.setpos(ib,ia,closest);
  }

  return distmap.getpos(ia,ib);
}

List<int> findfriends(int b,int tolerance) {
  List<int> ls;
  if (bs.p2num(b)->blobbed)
    error("Should not be finding friends of a blobbed blob!");
  for (int i=1;i<=bs.len;i++) {
    if (i!=b)
      if (!bs.p2num(i)->blobbed)
        if (closestdist(i,b)<=tolerance) {
          int a=finddad(i);
          if (a!=b)
            ls.add(a);
        }
  }
  return ls;
}

void addhits(int ib,List<int> hits) {
  Blob n=Blob(bs.len+1);
  n.add(ib);
  for (int i=1;i<=hits.len;i++) {
    int a=hits.num(i);
    if (a==ib)
      error("One of the hits is the destination blob");
    Blob *c=bs.p2num(a);
    if (c->blobbed)
      error("One of the hits was blobbed");
    n.add(a);
  }
  bs.add(n);
}

void plot() {
    printf("Plotting %i...\n",bs.len);
    RGBmp out=RGBmp(rm.width,rm.height,myRGB::black);
    starttimer();
    for (int i=1;i<=bs.len;i++) {
      Blob *b=bs.p2num(i);
      if (!b->blobbed) {
        b->plot(&out);
      }
    }
    printf("%f seconds\n",gettimer());
    String fname=getnextfilename("out","bmp");
    out.writefile(fname);
    out.freedom();
    if (usexv)
      system(Sconc("xv ",fname," &"));
}

void findlines() {
  int tolerance=1;
  while (true) {

    printf("Extending words...\n");
    bool change=true;
    while (change) {
      change=false;
      starttimer();
      for (int i=1;i<=bs.len;i++) {
        Blob *b=bs.p2num(i);
        if (!b->blobbed)
        if (b->gettype()==Word) {
//          printf("%i/%i\n",i,bs.len);

          V2d cen=b->centroid;
          V2d o=b->ori();
          float len=b->width();
          V2d u=o*len;
          float hei=b->height();
          V2d v=o.perp()*hei;

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
            int g=rm.getpos(search.num(j));
            if (g>0) {
              g=finddad(g);
              if (g!=i)
                hits.addifnot(g);
            }
          }
          search.freedom();
          if (hits.len==0)
            b->type=Sentence;
          else {
//            printf("%i hits\n",hits.len);
            addhits(i,hits);
            change=true;
          }
          hits.freedom();

        }
      }
      printf("%f seconds\n",gettimer());

      plot();

    }

    printf("Joining neighbours...\n");
    starttimer();
    for (int i=1;i<=bs.len;i++) {
      if (!bs.p2num(i)->blobbed) {
//        printf("%i/%i\n",i,bs.len);
        List<int> hits=findfriends(i,tolerance);
        if (hits.len>0)
          addhits(i,hits);
      }
    }
    printf("%f seconds\n",gettimer());
    tolerance++;

    /*Map2d<int> out=Map2d<int>(rm.width,rm.height,0);
    for (int i=1;i<=bs.len;i++) {
      Blob b=bs.num(i);
      if (!b.blobbed)
        for (int j=1;j<=b.bpixs.len;j++)
          out.setpos(b.bpixs.num(j),i);
    }
    out.hueify().writefile(getnextfilename("out","bmp"));*/

    plot();

  }
}

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  bool useold=a.argexists("-o","use a previous calculation of the map");
  usexv=a.argexists("-xv","display progress using xv");
  String iname=a.getarg();
  String bname=a.getarg();
  approx=false;
  a.done();

  Map2d<float> *image=Map2d<float>::readfile(iname);
  Map2d<bool> total=*Map2d<bool>::readbinfile(bname)->binscaleto(image->width,image->height);
	
  if (!useold) {
	  rm=total.getregionmap(true);
	  rm.writefileandscale("regmap.bmp");
	} else {
  	rm=*Map2d<int>::readfileandscale("regmap.bmp");
  	distmap=*Map2d<float>::readfileandscale("distmap.bmp");
 }
	
	List<Region> cs=rm.collectregions();
	
	for (int i=1;i<=cs.len;i++) {
	  bs.add(Blob(i,cs.p2num(i)));
  }

  printf("Calculating initial distances...\n");
  for (int i=1;i<=bs.len;i++)
  for (int j=1;j<=bs.len;j++)
    if (i!=j)
      float dummy=closestdist(i,j);
  distmap.writefileandscale("distmap.bmp");
	
  findlines();

}

// Get first types:
// Find all subblobs of type where higher-level type is different

// Get final:
// Find all subblobs of type where this type does not appear in subs
//                        or where not all subs are this type (still parse subs!)
// It's difficult to define.  We want characters when we know they are
// characters.