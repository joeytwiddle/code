// For efficiency:
// Calculate closest dist using sub-blobs

#include <joeylib.c>

#define Extension 2.0
#define maxblobs 500
#define Undefined -1
#define Base 0
#define Character 1
#define Word 2
#define Sentence 3
#define Paragraph 4

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

  Blob(int c,Region *r) {
    col=c;
    centroid=r->centroid();
    bpixs=*r->getboundary();
    area=r->getlist()->len;
//    for (int i=1;i<=bpixs.len;i++)
//      bpols.add(Polar(bpixs.num(i)));
    type=Base;
    blobbed=false;
  }

  Blob(int c,int bi) {
    Blob *b=bs.p2num(bi);
    col=c;
    blobbed=false;
    type=b->type;
    area=b->area;
    centroid=b->centroid;
    bpixs=List<Pixel>(b->bpixs);
    b->blobbed=true;
    b->dad=col;
    subs.add(bi);
    type=Undefined;
  }

  void add(int bi) {
    Blob *b=bs.p2num(bi);
    subs.add(bi);
    centroid=(centroid*area+b->centroid*b->area)/(float)(area+b->area);
    area+=b->area;
    bpixs.add(b->bpixs);
    b->blobbed=true;
    b->dad=col;
  }

  List<int> getsubblobs() {
    List<int> ls;
    ls.add(subs);
    bool done=false;
    while (!done) {
      done=true;
      for (int i=1;i<=ls.len;i++) {
        Blob *b=bs.p2num(ls.num(i));
        if (b->subs.len>0) {
          done=false;
//          printf("Removing %i\n",ls.num(i));
          ls.removenum(i);
          ls.add(b->subs);
//          printf("Adding %s\n",b->subs.toString());
          i--; // Ensures blob after this one removed is not skipped
        }
//        printf("%i/%i\n",i,ls.len);
      }
    }
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
        printf("Did have to remove one after all\n");
      }
    return ls;
  }

  int gettype() {
    if (type==Undefined) {
      List<int> sbs=getsubblobs();
      for (int i=1;i<=sbs.len;i++)
        if (bs.p2num(sbs.num(i))->gettype()!=Base) {
          sbs.removenum(i);
          i--;
          printf("Did have to remove one after all\n");
        }
      if (sbs.len>=2 && width()>3.0*height())
        type=Word;
      else
        type=Character;
      sbs.freedom();
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
    r=myRGB::hue(myrnd());
    if (t==Word || t==Sentence) {
      V2d x=ori()*width();
      V2d y=ori().perp()*height();
      m->line(centroid-x+y,centroid+x+y,r);
      m->line(centroid+x+y,centroid+x-y,r);
      m->line(centroid+x-y,centroid-x-y,r);
      m->line(centroid-x-y,centroid-x+y,r);
/*      V2d o=ori()*sqrt(area);
      m->line(centroid-o,centroid+o,r);
      m->filledcircle(centroid-o,2,r);
      m->filledcircle(centroid+o,2,r);*/
      m->filledcircle(centroid,4,r);
// setpos error      plotandfree(m,Pixel::line(centroid-o,centroid+o),r);
//      for (int j=1;j<=bpixs.len;j++)
//        m->setpixel(bpixs.num(j),r);
    } else {
      for (int j=1;j<=bpixs.len;j++)
        m->setpixel(bpixs.num(j),r);
    }
    List<int> sbs=getfirsts(Character);
    for (int i=1;i<=sbs.len;i++) {
      Blob *b=bs.p2num(sbs.num(i));
      m->line(Pixel(b->centroid)+Pixel(-5,0),Pixel(b->centroid)+Pixel(+5,0),r); //myRGB::blue);
      m->line(Pixel(b->centroid)+Pixel(0,-5),Pixel(b->centroid)+Pixel(0,+5),r); //myRGB::blue);
    }
  }

  V2d ori() {
    List<int> sbs=getfirsts(Character);
    if (sbs.len<2)
      return V2d(0,0.1);
    Correlator c=Correlator();
    for (int i=1;i<=sbs.len;i++)
      c.add(bs.p2num(sbs.num(i))->centroid);
    V2d ori=c.ori();
    c.freedom();
    return ori;
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

    List<Pixel> as=bs.num(ia).bpixs;
    List<Pixel> cs=bs.num(ib).bpixs;

    // Search both boundaries
    float closest=dist(as.num(1),cs.num(1));
    for (int i=1;i<=as.len;i++) {
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
  Blob n=Blob(bs.len+1,ib);
  for (int i=1;i<=hits.len;i++) {
    int a=hits.num(i);
    if (a==ib)
      error("One of the hits is the destination blob");
    Blob *c=bs.p2num(a);
    if (c->blobbed)
      error("One of the hits was blobbed");
    n.add(a);
  }
  // If newtype=oldtype then overwrite rather than add
  // That kinda means only join N if all N types are the same
  bs.add(n);
}

void findlines() {
  int tolerance=1;
  while (true) {

    printf("Extending words and sentences...\n");
    for (int i=1;i<=bs.len;i++) {
      Blob *b=bs.p2num(i);
      if (!b->blobbed)
      if (b->gettype()==Word || b->gettype()==Sentence) {

      }
    }

    printf("Joining neighbours...\n");
    for (int i=1;i<=bs.len;i++) {
      if (!bs.p2num(i)->blobbed) {
        List<int> hits=findfriends(i,tolerance);
        if (hits.len>0)
          addhits(i,hits);
      }
    }
    tolerance++;

    /*Map2d<int> out=Map2d<int>(rm.width,rm.height,0);
    for (int i=1;i<=bs.len;i++) {
      Blob b=bs.num(i);
      if (!b.blobbed)
        for (int j=1;j<=b.bpixs.len;j++)
          out.setpos(b.bpixs.num(j),i);
    }
    out.hueify().writefile(getnextfilename("out","bmp"));*/

    printf("Plotting...\n");
    RGBmp out=RGBmp(rm.width,rm.height,myRGB::black);
    for (int i=1;i<=bs.len;i++) {
      Blob *b=bs.p2num(i);
      if (!b->blobbed) {
        b->plot(&out);
      }
    }
    out.writefile(getnextfilename("out","bmp"));
    out.freedom();

  }
}

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  bool useold=a.argexists("-o","use a previous calculation of the map");
  String iname=a.getarg();
  String bname=a.getarg();
  a.done();

  Map2d<float> *image=Map2d<float>::readfile(iname);
  Map2d<bool> total=*Map2d<bool>::readbinfile(bname)->binscaleto(image->width,image->height);
	
  if (!useold) {
	  rm=total.getregionmap(true);
	} else {
	  int i=readintfromfile("map.bmp.dat");
	  if (i>255)
	    error("Cannot re-read map: greyscale resolution too low!");
  	rm=*Map2d<int>::readfile("map.bmp",i);
 }
	
	List<Region> cs=rm.collectregions();
	
	for (int i=1;i<=cs.len;i++) {
	  bs.add(Blob(i,cs.p2num(i)));
  }
	
  findlines();

}

// Get first types:
// Find all subblobs of type where higher-level type is different

// Get final:
// Find all subblobs of type where this type does not appear in subs
//                        or where not all subs are this type (still parse subs!)
// It's difficult to define.  We want characters when we know they are
// characters.