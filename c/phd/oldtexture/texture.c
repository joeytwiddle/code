#include <joeylib.c>

// Usually, classification and training operate on the same region for all measures.
// However, when one pixel is passed, each measure chooses its own local window size.

float scale;
Map2d<V3d> *image;
Map2d<bool> *expected;
Map2d<float> *greyscale;
Map2d<float> *edgemag,*edgeang;
float ghistscale;
Map2d<GHist *> *ghs;

// Measures
int nummeasures=3;
int eacq=16;
int eacrad=6;
int glq=32;
int glvarrad=4;
bool showmeas;
List<Map2d *> measmaps;

// Segmenter
int botres=2; // botres*2^n=topres must hold for some n (to make check for botres in neighbour classifier work)
int topres=32;
//float notext=0.1;
//float istext=0.8;
float notext=-4.5;
float istext=-2.0;

// Training
bool train;
int numtrainexs=0;
List<String> traindata;

float measureedgeanglecancelling(List<Pixel> *ps) {
  // Generate histogram of edges
//  float *hs=new float[eacq];
  if (ps->len==1) {
    Pixel p=ps->num(1);
    destroy(ps);
    ps=p2pixelsincircle(p,eacrad);
  }
  float *hs=(float *)malloc(eacq*sizeof(float));
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
    if (greyscale->inmap(ps->num(i)))
      gh->addocc(greyscale->getpos(ps->num(i)));
  return gh;
}

float measureglvariance(List<Pixel> *ps) {
  if (ps->len==1) {
    Pixel p=ps->num(1);
    destroy(ps);
    ps=p2pixelsincircle(p,glvarrad);
  }
  GHist *gh=gethist(ps);
  float v=gh->glvariance();
  destroy(gh);
  return v;
}

GHist *gethist(Pixel p) {
  Pixel q=p*ghistscale;
  if (ghs->getpos(q)==NULL) {
  List<Pixel> *ps=p2pixelsincircle(p,6);
  ghs->setpos(q,gethist(ps));
  destroy(ps);
  return ghs->getpos(q);
}

float measurehiststability(Pixel p) {
  Pixel q=p*ghistscale;
  if (q.x>=1 && q.y>=1 && q.x<ghs->width-1 && q.y<ghs->height-1) {
    float d=0;
    for (int i=-1;i<=1;i++)
    for (int j=-1;j<=1;j++)
      if (i!=0 || j!=0)
        d+=ghs->getpos(q)->difference(ghs->getpos(q+Pixel(i,j)));
    return d/8.0;
  } else
    return 0;
}

float measurehiststability(List<Pixel> *ps) {
  if (ps->len==1)
    return measurehiststability(ps->num(1));
  error("I do not know how to measure histogram stability of a randomly shaped region");
}

float measurehiststability(int l,int t,int r,int bot) {
  int mx=(l+r)/2;
  int my=(t+bot)/2;
  List<Pixel> *ps=p2pixelsinrectangle(l,t,mx,my);
  GHist *a=gethist(ps);
  destroy(ps);
  ps=p2pixelsinrectangle(mx,t,r,my);
  GHist *b=gethist(ps);
  destroy(ps);
  ps=p2pixelsinrectangle(l,my,mx,bot);
  GHist *c=gethist(ps);
  destroy(ps);
  ps=p2pixelsinrectangle(mx,my,r,bot);
  GHist *d=gethist(ps);
  destroy(ps);
  // Could be combined differently, or each squared
  float diff=sqrt(a->difference(b)+b->difference(d)+d->difference(c)+c->difference(a)+a->difference(c)+b->difference(d));
  destroy(a);
  destroy(b);
  destroy(c);
  destroy(d);
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
  Seg() { }
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
//          float c=classify(&vs);
          float c=measurehiststability(i*res,j*res,(i+1)*res-1,(j+1)*res-1);
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
//      float c=measureedgeanglecancelling(ps);
      float c=classify(ps);
      destroy(ps);
      b->setpixel(i,j,c);
    }
    }
//    return b;
    b->writefile("windowmeas.bmp");
    return NULL;
  }
      
};

float classifyrnd(List<Pixel> *ps) {
  return myrnd();
}

void writemeasures(Vnd *input) {
  String s="";
  for (int i=1;i<=nummeasures;i++)
    s=Sformat("%s%f ",s,input->getaxis(i));
  traindata.add(s);
}

void writetraindata(Vnd *input,float output) {
  numtrainexs++;
  traindata.add(Sformat("# Training example number %f",numtrainexs));
  writemeasures(input);
  traindata.add(Sformat("# Output number %f",numtrainexs));
  traindata.add(Sformat("%f",output));
}

Vnd *summarise(List<Pixel> *ps) {
  Vnd *nv=new Vnd(nummeasures);
  nv->setaxis(1,measureedgeanglecancelling(ps));
  nv->setaxis(2,measureglvariance(ps));
  nv->setaxis(3,measurehiststability(ps));
  if (showmeas)
    for (int i=1;i<=nummeasures;i++)
      measmaps.num(i)->setpos(ps->centroid(),nv->getaxis(i));
  return nv;
}

float classifytrain(List<Pixel> *ps) {
  int cnt=0;
  for (int i=1;i<=ps->len;i++) {
    Pixel p=ps->num(i);
    if (expected->getpos(p.x,p.y))
      cnt++;
  }
  float proportiontext=(float)cnt/(float)ps->len; // Desired output of NN
  Vnd *summary=summarise(ps);
  writetraindata(summary,proportiontext);
  destroy(summary);
  return proportiontext+floatrnd(-.2,.2);
}

float classifynn(List<Pixel> *ps) {
  Vnd *summary=summarise(ps);
  traindata.clear();
  traindata.add("# Quick question for the NN");
  writemeasures(summary);
  destroy(summary);
  writelinestofile("question.pat");
  command("asknn question.pat answer.pat");
  List<String> ls=getlinesfromfile("answer.pat");
  deletefile("answer.pat");
  float answer;
  sscanf(ls.num(1),"%f",&answer);
  return answer;
}

void main(int argc,String *argv) {
  
  randomise();

  ArgParser a=ArgParser(argc,argv);

  String task=a.arg("task");
  a.opts.add("task = new | train | test");

  if (Seq("task","new")) {
    // Generate a new neural network

  } else {

  String iname=a.getarg("image file");
  scale=a.floatafter("-s","scale",0.5);
  ghistscale=a.floatafter("-ghs","scale for hist stability",0.5);
  showmeas=!argexists("-noshow","don't show measures");
  a.done();
  
  image=RGBmp::readfile(fname)->scaledby(scale)->getv3ds();

  train=Seq(task,"train");
  if (!train && !Seq(task,"test"))
    error("Need task = new | train | test");
  String fname=Sconc("home/joey/images/
  String ename=Sconc("home/joey/images/

  if (train)
    expected=Map2d<float>::readfile(ename)->scaleto(image->width,image->height)->threshold(0.5);

  greyscale=image->applyfn(&V3dtofloat);
  greyscale->edgedetection(Map2d<float>::sobel(),&edgemag,&edgeang);

  ghs=new Map2d<GHist *>(image->width*ghistscale,image->height*ghistscale,NULL);
  
  if (showmeas)
    for (int i=1;i<=nummeasures;i++)
      measmaps.add(new Map2d<float>(image->width,image->height));

  printf("Starting segmentation...\n");
  Seg seg;
  if (train)
    seg=Seg(&classifytrain);
  else
    seg=Seg(&classifynn);
  Map2d<bool> *b=seg.classifywindows();

//  Seg seg=Seg(&measureglvariance);
//  Map2d<bool> *b=seg.classifytopdown();
//  Map2d<bool> *b=seg.classifyneighbours();

  if (train)
    writelinestofile(&traindata,Sformat("%s.td",iname));

  if (showmeas)
    for (int i=1;i<=nummeasures;i++)
      measmaps.num(i)->writetofile(getnextfilename("meas","bmp"));

//  if (!train)
    b->scaleto(expected->width,expected->height)->writefile("textseg.bmp");

  } 
 
}
