// Paragraph should stop expanding when there is no blob above or below
// it, within one line-space

//#define DEBUG

// For faster processing, reduced accuracy
// #define ClosestDistQuick
#define ClosestDistApproxAt 50

// For good accuracy with faraway vanishing points
#define float double

#include <joeylib.c>

// How far does a work extend?
#define Extension 2.0 // of its height
// How close are other lines that should be in a paragraph?
#define LineSpace 2.0 // paragraph will expand if element lies within this*linespacing

#define maxblobs 2000 // If it goes over, they just recalculate from source! (since OOB gives -1)

// Blob types:
#define Undefined -1
#define Base 0
#define Character 1
#define Word 2
#define Sentence 3
#define Paragraph 4
#define Block 5

// Have maybe-words, which do not change into sentences when they stop expanding
// This can be used for low-res words, and for 'i's

bool approx,usexv,showprogress,showlots;
int blockswritten=0;

class Blob;

RGBmp *image;
RGBmp *origimage;
Map2d<int> rm;
List<Blob> bs;
Map2d<float> distmap; // =Map2d<float>(maxblobs,maxblobs,-1.0);

V2d sortdir;
bool sortfn(int i,int j);

class Blob  {
public:
  int col,dad,type,area;
  bool blobbed;
  V2d centroid;
  List<Pixel> bpixs;
//  List<Polar> bpols;
  List<int> subs;
  List<int> bases;
  List<int> lines;
//  List<int> allsubs;
  int basecnt;
  int sentcnt;
  V2d *memori;
  float *memheight,*memwidth,*memparaheight,*memparawidth;

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
    memheight=memwidth=memparaheight=memparawidth=NULL;
    dad=-47;
  }

  Blob(int c) { // A new blob for adding to
    col=c;
    dad=c; // ? ***
    dad=-47;
    blobbed=false;
    type=Undefined; //type=b->type; // This is biased towards this blob over others in hit!
    area=0;
    centroid=V2d(0,0);
    bpixs=List<Pixel>();
    sentcnt=0;
    basecnt=0;
    memori=NULL;
    memheight=memwidth=memparaheight=memparawidth=NULL;
  }

  void add(int bi) {
    Blob *b=bs.p2num(bi);
    subs.add(bi);
//    allsubs.add(bi);
//    allsubs.add(b->allsubs);
    if (b->gettype()==Sentence) {
      sentcnt++;
      lines.add(bi);
    }
    lines.add(b->lines);
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

  V2d lineori() {
    if (type!=Paragraph && type!=Block)
      printf("Line ori requested but not a paragraph!\n");
    V2d v=V2d(0,0);
    for (int i=1;i<=lines.len;i++)
      v=v+bs.p2num(lines.num(i))->ori();
    return v/(float)lines.len;
  }

  float linedist(int i) {
    float f=V2d::dot(sortdir,bs.p2num(lines.num(i+1))->centroid)-V2d::dot(sortdir,bs.p2num(lines.num(i))->centroid);
    return f;
  }

  float linedist() {
    if (type!=Paragraph && type!=Block)
      printf("Line distance requested but not a paragraph!\n");
    sortdir=lineori().perp(); // Will sort them in order of this direction
    if (sortdir.y<0)
      sortdir=-sortdir; // Point downwards
    lines.sort(&sortfn);
    float best=0;
    for (int i=1;i<=lines.len-1;i++) {
      float f=linedist(i);
      if (myabs(f)>best)
        best=myabs(f);
    }
    return best;
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
      if (sentcnt>=2) { // Should check relationship of sentences
        type=Paragraph;
      } else if (basecnt>=3 && width()>3.0*height())
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
    for (int i=1;i<=bases.len;i++) {
      Blob *b=bs.p2num(bases.num(i));
      m->line(Pixel(b->centroid)+Pixel(-3,0),Pixel(b->centroid)+Pixel(+3,0),myRGB::blue);
      m->line(Pixel(b->centroid)+Pixel(0,-3),Pixel(b->centroid)+Pixel(0,+3),myRGB::blue);
    }
    if (t==Base || t==Character) {
      for (int j=1;j<=bpixs.len;j++)
        m->setpixel(bpixs.num(j),r);
    } else if (t==Word || t==Sentence) {
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
    } else if (t==Paragraph || t==Block) {
      for (int i=1;i<=lines.len;i++) {
        Blob *b=bs.p2num(lines.num(i));
        V2d o=b->ori()*b->width();
        m->line(b->centroid-o,b->centroid+o,myRGB::random());
      }
      V2d o=lineori().norm().perp()*linedist();
      m->line(centroid-o/2.0,centroid+o/2.0,myRGB::white);
    }
  }

  V2d ori() {
//    List<int> sbs=getfirsts(Base);
    if (type==Paragraph || type==Block)
      printf("Trying to get ori from paragraph!\n");
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

  Line2d line() {
    V2d o=ori()*width();
    return Line2d(centroid-o,centroid+o);
  }

  float furthest(V2d lori) {
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
    if (type==Paragraph || type==Block)
      printf("Requested width from Paragraph!\n");
    if (memwidth==NULL)
      memwidth=p2newfloat(furthest(ori()));
    return *memwidth;
  }

  float height() {
    if (type==Paragraph || type==Block)
      printf("Requested height from Paragraph!\n");
    if (memheight==NULL) {
      memheight=p2newfloat(furthest(ori().perp()));
    }
    return *memheight;
  }

  float parawidth() {
    if (type!=Paragraph && type!=Block)
      printf("Requested parawidth from non-Paragraph!\n");
    if (memparawidth==NULL)
      memparawidth=p2newfloat(furthest(lineori()));
    return *memparawidth;
  }

  float paraheight() {
    if (type!=Paragraph && type!=Block)
      printf("Requested paraheight from non-Paragraph!\n");
    if (memparaheight==NULL)
      memparaheight=p2newfloat(furthest(lineori().perp()));
    return *memparaheight;
  }

};

bool sortfn(int i,int j) {
  return V2d::dot(sortdir,bs.p2num(i)->centroid)<V2d::dot(sortdir,bs.p2num(j)->centroid);
}

int finddad(int b) {
  if (b<=0 || b>bs.len) {
    #ifdef DEBUG
      printf("Tried to find dad of blob number %i\n",b);
    #endif
    return -17;
  }
  if (!bs.p2num(b)->blobbed)
    return b;
  else
    return finddad(bs.p2num(b)->dad);
}

float closestdist(int ia,int ib) {
  if (distmap.getpos(ia,ib)<0) {
    float closest;

    if (V2d::dist(bs.p2num(ia)->centroid,bs.p2num(ib)->centroid)>ClosestDistApproxAt)
      closest=V2d::dist(bs.p2num(ia)->centroid,bs.p2num(ib)->centroid);
    else {
    
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

    #ifdef ClosestDistQuick
      closest=V2d::dist(bs.p2num(ia)->centroid,bs.p2num(ib)->centroid);
    #else

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
    
    }

    #endif

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
    printf("Plotting %i ...\n",bs.len);
    RGBmp out=RGBmp(rm.width,rm.height,myRGB::black);
    starttimer();
    for (int i=1;i<=bs.len;i++) {
      Blob *b=bs.p2num(i);
      if (!b->blobbed) {
        b->plot(&out);
      }
    }
    printf("%.1f seconds\n",gettimer());
    String fname=getnextfilename("out","bmp");
    out.writefile(fname);
    out.freedom();
    if (usexv)
      system(Sconc("xv ",fname," &"));
}

void lookforparagraphs() {
  for (int i=1;i<=bs.len;i++) {
    Blob *b=bs.p2num(i);
    if (!b->blobbed && b->gettype()==Paragraph && b->sentcnt>=3) {
      printf("Inspecting a paragraph...\n");
      float ld=LineSpace*b->linedist();

      // Is it expandable?  (Are there any other blobs within 2*line spacing?)
      // Is it expandable?  Are there any blobs above or below?
      // Get para box
      float pw=b->parawidth();
      float ph=b->paraheight();
      float phin=ph+ld;
      V2d x=b->lineori().norm();
      V2d y=x.perp().norm();

      bool found=false;
      for (int j=1;j<=bs.len && !found;j++)
        if (j!=i && !bs.p2num(j)->blobbed) {
//          if (closestdist(i,j)<ld) {
            V2d c=bs.p2num(j)->centroid;
            if (myabs((c-b->centroid).dot(y))<=phin && myabs((c-b->centroid).dot(x))<=pw)
              found=true;
//          }
        }

      if (!found) {
        // Recover paragraph
        b->type=Block;

        RGBmp out=RGBmp(rm.width,rm.height);
        b->plot(&out);

        // An early bit of the VVP function
        // Find vertical line through centres of text lines
/*        Correlator c=Correlator();
        for (int i=1;i<=b->lines.len;i++)
          c.add(bs.p2num(b->lines.num(i))->centroid);
        Line2d l=Line2d(c.centroid(),c.centroid()+c.ori().norm());
        c.freedom(); */
        // Find vertical line as perpendicular to lines
        Line2d l=Line2d(b->centroid,b->centroid+b->lineori().perp().norm());
        out.line(l.a,l.a+(l.b-l.a)*200,myRGB::red);

        printf("Finding HVP...\n");
        // Intersect all lines in paragraph with each other and cluster (by averaging!)
        V2d hvpl=V2d(0,0);
        V2d hvpr=V2d(0,0);
        int cnte=b->lines.len*(b->lines.len-1)/2;
        int cntl=0;
        int cntr=0;
        int cntt=0;
        for (int i=1;i<=b->lines.len;i++) {
          Line2d la=bs.p2num(b->lines.num(i))->line();
          out.line(la,myRGB::yellow);
          for (int j=1;j<=b->lines.len;j++) {
            if (i!=j) {
              Line2d lb=bs.p2num(b->lines.num(j))->line();
              V2d li=la.findintersection(lb);
              if ((li-b->centroid).mag()<10000000) {
                out.line(la.center(),li,myRGB::white*0.5);
                out.line(lb.center(),li,myRGB::white*0.5);
                if (l.whichsideis(li)<0) {
                  cntl++;
                  hvpl=hvpl+li/(float)cnte;
                } else {
                  cntr++;
                  hvpr=hvpr+li/(float)cnte;
                }
              }
              cntt++;
              printf("Intersect distance: %.2f\n",(li-b->centroid).mag());
            }
          }
        }
        V2d hvp=( cntl>cntr ? hvpl : hvpr );
        int cnt=( cntl>cntr ? cntl : cntr );
        printf("Intersections on left %i right %i\n",cntl,cntr);
        if (cntl>cntr)
          printf("HPP found on left with %i intersections out of %i\n",cntl,cntt);
        else
          printf("HPP found on right with %i intersections out of %i\n",cntl,cntt);
        if (cnt!=cnte) {
          printf("Got count %i for HVP wrong, actually used %i intersections!\n",cnte,cnt);
          hvp=hvp*(float)cnte;
          hvp=hvp/(float)cnt;
        }
        printf("HVP=%s\n",hvp.toString());
        float zh=(b->centroid-hvp).mag();
        printf("Distance to HVP: %f\n",zh);
        if (!(zh<bigfloat*10.0)) {
          zh=bigfloat*10.0;
          hvp=b->centroid+(hvp-b->centroid).norm()*zh;
          zh=(b->centroid-hvp).mag();
          if (!(zh<bigfloat*10.0)) {
            zh=bigfloat*10.0;
            hvp=b->centroid+V2d(-1,0)*zh;
          }
          printf("  too large: now %f %s\n",zh,hvp.toString());
        }
//        printf("Plotting lines...");
        for (int i=1;i<=b->lines.len;i+=b->lines.len-1)
          out.line(hvp,bs.p2num(b->lines.num(i))->centroid,myRGB::green);

        printf("Finding VVP...\n");

        // Estimate distance of vanishing point
        Correlator c2=Correlator();
        V2d lineori=b->lineori();

        // *** Try average base size in a line

        // Doing linear correlation with line width
        float scale=1.0;
        for (int i=1;i<=b->lines.len;i++) {
          V2d cen=bs.p2num(b->lines.num(i))->centroid;
          float x=(cen-l.a).dot((l.b-l.a).norm());
          float y=bs.p2num(b->lines.num(i))->width(); // Area of region! (used to be ->height()) // (used to be ->area())
          if (y>10) { // Bodge to ensure that erronous data not used
          printf("%f , %f\n",x,y);
          out.cross(l.a+(l.b-l.a).norm()*x+lineori*y*scale,5,myRGB::white);
          c2.add(x,y);
          }
        }

/*        float scale=5.0;
        for (int i=1;i<=b->lines.len-1;i++) { // not -1 for height
          V2d cen=bs.p2num(b->lines.num(i))->centroid;
          float x=(cen-l.a).dot((l.b-l.a).norm());
//          float y=bs.p2num(b->lines.num(i))->height(); // line (character) height
          float y=b->linedist(i); // line spacing
          if (y>0.5) { // Bodge to ensure two lines which should be the same are not used
          printf("%f , %f\n",x,y);
          out.cross(l.a+(l.b-l.a).norm()*x+lineori*y*scale,5,myRGB::white);
          c2.add(x,y);
          }
        }*/

        // Doing linear correlation with base sizes. Too noisy!
/*        float scale=3.0;
        for (int i=1;i<=b->bases.len;i++) {
          V2d cen=bs.p2num(b->bases.num(i))->centroid;
          float x=(cen-l.a).dot((l.b-l.a).norm());
          float y=bs.p2num(b->bases.num(i))->area; // Area of region! (used to be ->height())
          printf("%f , %f\n",x,y);
          if (y>10) {
            out.cross(l.a+(l.b-l.a).norm()*x+lineori*y*scale,5,myRGB::white);
            c2.add(x,y);
          } else {
            out.cross(l.a+(l.b-l.a).norm()*x+lineori*y*scale,5,myRGB::green*0.5);
          }
        }*/

        //OUTLIERIMAGE=&out;
        Excled.freedom();
        Excled=List<V2d>();
        float z=c2.crossesyxoutliers();
        for (int i=1;i<=Excled.len;i++) {
          V2d v=Excled.num(i);
          V2d ttt=l.a+(l.b-l.a).norm()*v.x+lineori*v.y*scale;
          out.cross(ttt,5,myRGB::red);
        }

        // Check the line we get is worth anything by plotting it!
        float tx=(bs.p2num(b->lines.num(1))->centroid-l.a).dot((l.b-l.a).norm());
        float ty=c2.yforx(tx);
        printf("First should be approx %f,%f\n",tx,ty);
        V2d lin1=l.a+(l.b-l.a).norm()*tx+lineori*ty*scale;
        tx=(bs.p2num(b->lines.num(b->lines.len))->centroid-l.a).dot((l.b-l.a).norm());
        ty=c2.yforx(tx);
        printf("Last should be approx %f,%f\n",tx,ty);
        V2d lin2=l.a+(l.b-l.a).norm()*tx+lineori*ty*scale;
        out.line(lin1,lin2,myRGB::white);

        if (!(z>=-bigfloat && z<=bigfloat)) {
          printf("VP too far away (areas suggested %f)!\n",z);
          z=bigfloat;
        }
        printf("Distance to VVP: %f (originally %f)\n",z,z);
        c2.freedom();
        V2d vvp=l.a+(l.b-l.a).norm()*z;
        printf("VVP=%s\n",vvp.toString());
        Line2d enil=bs.p2num(b->lines.num(1))->line();
        out.line(vvp,enil.a,myRGB::magenta);
        out.line(vvp,enil.b,myRGB::magenta);
        enil=bs.p2num(b->lines.num(b->lines.len))->line();
        out.line(vvp,enil.a,myRGB::magenta);
        out.line(vvp,enil.b,myRGB::magenta);

        float hva=(b->centroid-hvp).angle();
        float hvb=hva;
        float vva=(b->centroid-vvp).angle();
        float vvb=vva;
        printf("Centre angles: H=%.2f V=%.2f\n",hva*rad2deg,vva*rad2deg);
        for (int i=1;i<=b->bpixs.len;i++) {
          V2d v=V2d(b->bpixs.num(i));
          float ha=(v-hvp).angle();
          float va=(v-vvp).angle();
          if (angleless(ha,hva))
            hva=ha;
          if (angleless(hvb,ha))
            hvb=ha;
          if (angleless(va,vva))
            vva=va;
          if (angleless(vvb,va))
            vvb=va;
//          printf("Angles: H: %.2f< %.2f >%.2f V: %.2f< %.2f >%.2f\n",hva,ha,hvb,vva,va,vvb);
        }
        printf("Edge angles: H: %.2f-%.2f V: %.2f-%.2f\n",hva*rad2deg,hvb*rad2deg,vva*rad2deg,vvb*rad2deg);

        float vd=b->parawidth()*0.2/myabs(z);
        printf("Vertical margin angle %f degrees.\n",vd*360.0/2.0/pi);
        Line2d left=Line2d(vvp,vvp+V2d::angle(vvb+vd)*zh);
        Line2d right=Line2d(vvp,vvp+V2d::angle(vva-vd)*zh);
        float hd=b->paraheight()*0.2/zh;
        printf("Horizontal margin angle %f degrees.\n",hd*360.0/2.0/pi);
        Line2d top=Line2d(hvp,hvp+V2d::angle(hva-hd)*z);
        Line2d bottom=Line2d(hvp,hvp+V2d::angle(hvb+hd)*z);
        printf("Outer lines are\nleft=%s\nright=%s\ntop=%s\nbottom=%s\n",left.toString(),right.toString(),top.toString(),bottom.toString());
        V2d tl=top.intersect(left);
        V2d tr=top.intersect(right);
        V2d bl=bottom.intersect(left);
        V2d br=bottom.intersect(right);
        printf("Outer corners are\ntl=%s\ntr=%s\nbr=%s\nbl=%s\n",tl.toString(),tr.toString(),br.toString(),bl.toString());
        out.line(tl,tr,myRGB::yellow);
        out.line(tr,br,myRGB::yellow);
        out.line(br,bl,myRGB::yellow);
        out.line(bl,tl,myRGB::yellow);

        blockswritten++;
        out.writefile(Sformat("block%i.bmp",blockswritten));
//        rm.applyfn(&finddad)->hueify().writefile(Sformat("block%ilines.bmp",blockswritten));
        out.freedom();

/*        printf("Recovering quad...\n");
        List<V2d> qs;
        qs.add(tl); qs.add(tr); qs.add(br); qs.add(bl);
        RGBmp *n=image->recoverquad(&qs,1,600);
        n->writefile(Sformat("recover%i.bmp",blockswritten));
        printf("  done\n");  */

        printf("Recovering quad...\n");
        List<V2d> qs;
        qs.add(tl*origimage->width/image->width); qs.add(tr*origimage->width/image->width); qs.add(br*origimage->width/image->width); qs.add(bl*origimage->width/image->width);
        RGBmp *n=origimage->recoverquad(&qs,1,600);
        n->writefile(Sformat("recover%i.bmp",blockswritten));
        printf("  done\n");

        // Write quadrilateral point to text file (0,0)-(1,1)
        List<String> qd;
        qd.add("Quad points {topleft,topright,bottomright,bottomleft}");
        qd.add(Sformat("%f %f",tl.x/image->width,tl.y/image->height));
        qd.add(Sformat("%f %f",tr.x/image->width,tr.y/image->height));
        qd.add(Sformat("%f %f",br.x/image->width,br.y/image->height));
        qd.add(Sformat("%f %f",bl.x/image->width,bl.y/image->height));
        writelinestofile(qd,Sformat("quad%i.dat",blockswritten));

        // Remove paragraph from blobs
        b->blobbed=true;
      }
    }
  }
}

void findlines() {
  int tolerance=1;
  while (true && tolerance<rm.width+rm.height) {

    bool change=true;
    while (change) {
      printf("Extending words %i ...\n",bs.len);
      change=false;
      starttimer();
      for (int i=1;i<=bs.len;i++) {
        bool recentchange=false;
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
/*          search.addandfree(Pixel::line(cen+u+up,cen+u+up+out));
          search.addandfree(Pixel::line(cen+u+up+out,cen+u-up+out));
          search.addandfree(Pixel::line(cen+u-up+out,cen+u-up));
          search.addandfree(Pixel::line(cen-u+up,cen-u+up-out));
          search.addandfree(Pixel::line(cen-u+up-out,cen-u-up-out));
          search.addandfree(Pixel::line(cen-u-up-out,cen-u-up));*/
          search.addandfree(Pixel::line(cen+u,cen+u+out));
          search.addandfree(Pixel::line(cen-u,cen-u-out));
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
            change=recentchange=true;
          }
          hits.freedom();

        }
        if (recentchange && showlots && myrnd()>.98)
          plot();
      }
      printf("%.1f seconds\n",gettimer());

      if (change) {
        lookforparagraphs();
      }
      if (change && showlots)
        plot();

    }
    
    if (change && !showlots)
      plot();

    change=false;
    printf("Joining neighbours %i ...\n",bs.len);
    starttimer();
    while (!change) {
      int bsatstart=bs.len;
      for (int i=1;i<=bs.len;i++) {
        if (!bs.p2num(i)->blobbed) {
          if (showprogress && ((i % 10)==1))
            printf("%i/%i\n",i,bs.len);
          List<int> hits=findfriends(i,tolerance);
          if (hits.len>0) {
            addhits(i,hits);
            change=true;
          }
        }
      }
      if ((bsatstart!=bs.len)!=change)
        printf("Addhits says change %i, bs num change says %i\n",change,(bsatstart!=bs.len));
      if (change) {
        lookforparagraphs();
      }
      tolerance++;
    }
    if (change || showlots)
      plot();
    printf("%.1f seconds\n",gettimer());

  }
  
}

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  bool useold=a.argexists("-o","use a previous calculation of the map");
  bool writenew=a.argexists("-wn","write new");
  usexv=a.argexists("-xv","display progress using xv");
  String iname=a.getarg("image");
  String bname=a.getarg("binary image");
  approx=false;
  showprogress=a.argexists("-sp","show progress");
  showlots=a.argexists("-sl","show lots of output");
  bool doscale=a.argexists("-ds","do pre-scaling");
  int scalewid=a.intafter("-sw","scale to width",400);
  bool showscaled=a.argexists("-ss","show scaled images");
  a.done();

  origimage=RGBmp::readfile(iname);
  if (doscale)
    image=origimage->scaletowidth(scalewid);
  else
    image=origimage;
  Map2d<bool> total=*Map2d<bool>::readbinfile(bname)->binscaleto(image->width,image->height);
  if (showscaled) {
    image->writefile("scaledimg.bmp");
    total.writefile("scaledbin.bmp");
  }
	
  if (!useold) {
	  rm=total.getregionmap(true);
	  if (writenew) {
	  rm.writefileandscale("regmap.bmp");
	  rm.hueify().writefile("regmapvis.bmp");
	  }
	} else {
  	rm=*Map2d<int>::readfileandscale("regmap.bmp");
  	distmap=*Map2d<float>::readfileandscale("distmap.bmp");
  }
	
  printf("Extracting blobs\n");
  List<Region> cs=rm.collectregions();
  for (int i=1;i<=cs.len;i++) {
    bs.add(Blob(i,cs.p2num(i)));
  }

  printf("Calculating initial distances...\n");
  distmap=Map2d<float>(bs.len*2,bs.len*2,-1.0);
  for (int i=1;i<=bs.len;i++) {
    if (showprogress && ((i % 10)==1))
      printf("%i/%i\n",i,bs.len);
  for (int j=1;j<=bs.len;j++)
    if (i!=j)
      float dummy=closestdist(i,j);
  }
  if (writenew)
    distmap.writefileandscale("distmap.bmp");
	
  findlines();
  
  printf("Line finder stopped.\n");

}

// Get first types:
// Find all subblobs of type where higher-level type is different

// Get final:
// Find all subblobs of type where this type does not appear in subs
//                        or where not all subs are this type (still parse subs!)
// It's difficult to define.  We want characters when we know they are
// characters.