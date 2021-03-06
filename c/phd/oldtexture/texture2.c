#include <joeylib.c>

// Segmenter
int botres=2; // botres*2^n=topres must hold for some n (to make check for botres in neighbour classifier work)
int topres=64;
//float notext=0.1;
//float istext=0.8;
float notext=-4.5;
float istext=-2.0;

// Measures
int eacq=16;
int glq=32;

Map2d<bool> *expected;
Map2d<float> *greyscale;
Map2d<V3d> *image;
Map2d<float> *edgemag,*edgeang;

float measureedgeanglecancelling(List<Pixel> *ps) {
  // Generate histogram of edges
//  float *hs=new float[eacq];
  float *hs=malloc(eacq*sizeof(float));
  for (int i=0;i<eacq;i++)
    hs[i]=0;
  float total=0;
  V2d v=V2d(0,0);
  for (int i=1;i<=ps->len;i++) {
    Pixel p=ps->num(i);
    if (edgemag->inmap(p)) {
    int j=eacq/2*(1+edgeang->getpos(p)/pi);
    hs[j]+=edgemag->getpos(p);
    total+=edgemag->getpos(p);
    v=v+edgemag->getpos(p)*V2d::angle(edgeang->getpos(p));
    }
  }
  float offness=0;
  for (int i=0;i<eacq/2;i++)
    offness+=mysquare(hs[i]-hs[i+eacq/2]);
  free(hs);
  // Good start, disintegrates to rubbish
  return -sqrt(offness*(float)eacq)/total;
  // Some definite rejection
//  return -sqrt(offness*(float)eacq)/(float)ps->len;
}

GHist *gethist(List<Pixel> *ps) {
  GHist *gh=new GHist(glq);
  for (int i=1;i<=ps->len;i++)
    gh->addocc(greyscale->getpos(ps->num(i)));
  return gh;
}

float measureglvariance(List<Pixel> *ps) {
  GHist *gh=gethist(ps);
  float v=gh->glvariance();
  destroy(gh);
  return v;
}

float measurehiststability(int l,int t,int r,int b) {
  int mx=(l+r)/2;
  int my=(t+b)/2;
  List<Pixel> *ps=p2pixelsinrectangle(l,t,mx,my);
  GHist a=gethist(ps);
  destroy(ps);
  ps=p2pixelsinrectangle(mx,t,r,my);
  GHist b=gethist(ps);
  destroy(ps);
  ps=p2pixelsinrectangle(l,my,mx,b);
  GHist c=gethist(ps);
  destroy(ps);
  ps=p2pixelsinrectangle(mx,my,r,b);
  GHist d=gethist(ps);
  destroy(ps);
  // Could be combined differently, or each squared
  float diff=sqrt(a.difference(&b)+b.difference(&d)+d.difference(&c)+c.difference(&a));
  a.freedom();
  b.freedom();
  c.freedom();
  d.freedom();
  return -diff; // Could be reciprocal
}

class Seg {
public:
  float (*classifyfn)(List<Pixel> *);
  Map2d<bool> *bin;

  float classify(Region *r) {
    return classifyfn(r->getlist());
  }
  float classify(List<Pixel> *l) {
    return classifyfn(l);
  }
  Seg(float (*c)(List<Pixel> *)) {
    classifyfn=c;
  }
  Map2d<bool> *classifytopdown() { // Big blocks sometimes wrongly classified
    bin=new Map2d<bool>(image->width,image->height,false);
    topdown(0,0,image->width-1,image->height-1);
    return bin;
  }
  void topdown(int l,int t,int r,int b) {
//printf("a %i %i %i %i\n",l,t,r-l,b-t);
    List<Pixel> vs;
    for (int i=l;i<r;i++)
    for (int j=t;j<b;j++)
      vs.add(newPixel(i,j));
    float c=classify(&vs);
    vs.freedom();
    if (r-l>botres || b-t>botres) {
      if (c>istext) {
        bin->setrectangle(l,t,r-l,b-t,depower(r-l,2));
      }
      else if (c>notext) {
        int mx=(l+r)/2;
        int my=(t+b)/2;
        topdown(l,t,mx,my);
        topdown(mx,t,r,my);
        topdown(l,my,mx,b);
        topdown(mx,my,r,b);
      }
    } else if(c>0.5)
      bin->setrectangle(l,t,r-l,b-t,true);
  }
  Map2d<bool> *classifyneighbours() {
    #define toavoid 0
    #define tocheck 1
    #define totext 2
    Map2d<uchar> *todo=new Map2d<uchar>(image->width/topres,image->height/topres,tocheck);
    for (int res=topres;res>=botres;res=res/2) {
      Map2d<uchar> *todonext=todo->scaledby(1);
      Map2d<float> *score=new Map2d<float>(todo->width,todo->height);
      for (int i=0;i<todo->width;i++)
      for (int j=0;j<todo->height;j++) {
          List<Pixel> vs;
          for (int x=i*res;x<(i+1)*res;x++)
          for (int y=j*res;y<(j+1)*res;y++)
            vs.add(newPixel(x,y));
          float c=classify(&vs);
          score->setpos(i,j,c);
          vs.freedom();
        if (todo->pos[i][j]==tocheck) {
          if (res>botres) {
          if (c>istext) {
            todonext->pos[i][j]=totext;
            for (int x=-1;x<=1;x++)
            for (int y=-1;y<=1;y++)
              if (x!=0 || y!=0)
                if (todonext->getpixel(i+x,j+y)!=totext)
                  todonext->setpixel(i+x,j+y,tocheck);
          } else if (c>notext)
            todonext->setpixel(i,j,tocheck);
          else
            todonext->setpixel(i,j,toavoid);
          } else {
            if (c>0.5)
              todonext->setpixel(i,j,totext);
          }
        }
        if (todonext->pos[i][j]==totext) {
          for (int x=-1;x<=1;x++)
            for (int y=-1;y<=1;y++)
              if (x!=0 || y!=0)
                if (todonext->getpixel(i+x,j+y)!=totext)
                  todonext->setpixel(i+x,j+y,tocheck);
        }
      }
      destroy(todo);
      int f=getnextfilenum();
      todonext->scaledto(image->width/botres,image->height/botres)->writefiletop(Sformat("level%i.bmp",f),2);
      score->scaledto(image->width/botres,image->height/botres)->writefile(Sformat("measure%i.bmp",f));
      destroy(score);
      todo=todonext->scaledby(2);
      destroy(todonext);
    }
    return todo->threshold(0.3);
  }
  Map2d<bool> *classifywindows() {
    Map2d<float> *b=new Map2d<float>(image->width,image->height);
    for (int i=0;i<=image->width;i++) {
      if (intmod(i,10)==9)
      printf("%i / %i\n",i+1,image->width);
    for (int j=0;j<=image->height;j++) {
      List<Pixel> *ps=p2pixelsincircle(i,j,5);
      float c=measureedgeanglecancelling(ps);
      destroy(ps);
      b->setpixel(i,j,c);
    }
    }
//    return b;
    b->writefile("windowmeas.bmp");
    return NULL;
  }
      
};

float classifyrnd(List<Pixel> *ps,Map2d<V3d> *m) {
  return myrnd();
}

float classifytrain(List<Pixel> *ps) {
  int cnt=0;
  for (int i=1;i<=ps->len;i++) {
    Pixel p=ps->num(i);
//    printf("%i %i in %i %i\n",p.x,p.y,expected->width,expected->height);
    if (expected->getpos(p.x,p.y))
      cnt++;
  }
  return (float)cnt/(float)ps->len+floatrnd(-.2,.2);
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  String iname=a.getarg();
  String fname=Sconc("home/joey/images/ ",iname,".bmp");
  String ename=Sconc("home/joey/images/ exp",iname,".bmp");
  a.done();

  randomise();
  
  image=RGBmp::readfile(fname)->getv3ds();
  greyscale=image->applyfn(&V3dtofloat);
  
  greyscale->edgedetection(Map2d<float>::sobel(),&edgemag,&edgeang);
  expected=Map2d<float>::readfile(ename)->scaleto(image->width,image->height)->threshold(0.5);
  
  printf("Starting segmentation...\n");
  Seg seg=Seg(&measureglvariance);
//  Seg seg=Seg(&classifytrain);
//  Map2d<bool> *b=seg.classifytopdown();
  Map2d<bool> *b=seg.classifyneighbours();
//  Map2d<bool> *b=seg.classifywindows();
  // b->scaleto(expected->width,expected->height)->writefile("textseg.bmp");
  
}
