#include <joeylib.c>

#ifdef LINUX
#define ImageDir "/home/joey/j/images/"
#else
#define ImageDir "/home/pgrad/pclark/images/"
#endif

// #define ImageDir "/home/joey/j/images/"

void donothing() { }

// #define destroy(f) donothing()
// #define free(f) donothing()

ArgParser a;

String nnname;
String tsname;
String iname;

bool train,test,show;

float scale;
int processwidth;
RGBmp *orig;
Map2d<V3d> *image;
Map2d<bool> *expected;
Map2d<float> *greyscale;
Map2d<float> *edgemag,*edgeang;
float ghistscale;
Map2d<GHist *> *ghs;

// Measures
// Usually, classification and training operate on the same region for all measures.
// However, when one pixel is passed, each measure chooses its own local window size.
int nummeasures=5;
int eacq;
int eacrad;
int glq;
int glhistrad; // for variance and difference
List<Map2d<float> *> measmaps;

// Segmenter
bool twopass=false;
int currentline;
int minarea;
int pass;
int windres=5; // Process a window every n pixels
int botres=2; // botres*2^n=topres must hold for some n (to make check for botres in neighbour classifier work)
int topres=32;
float notext=0.1;
float istext=0.8;
//float notext=-4.5;
//float istext=-2.0;
int morphrad;

// Network
bool twooutnodes;
class IOData {
public:
  Vnd *input;
  float output;
  Pixel pos;
  IOData(Vnd *vv,float pp,Pixel pps) {
    input=vv;
    output=pp;
    pos=pps;
  }
  void freedom() {
    input->freedom();
  }
};
int numtrainexs=0;
int usenumpostrainexs;
int usenumnegtrainexs;
List<IOData> data;
List<String> traindata;

void normalisedata() {
  // Get average for each measure
  Vnd ave=Vnd(nummeasures);
  for (int i=1;i<=data.len;i++)
  for (int j=1;j<=nummeasures;j++)
    ave.setaxis(j,ave.getaxis(j)+data.num(i).input->getaxis(j));
  ave=ave/(float)data.len;
  // Get variance for each measure
  Vnd var=Vnd(nummeasures);
  for (int i=1;i<=data.len;i++)
  for (int j=1;j<=nummeasures;j++)
    var.setaxis(j,var.getaxis(j)+mysquare(data.num(i).input->getaxis(j)-ave.getaxis(j)));
  // Get standard deviation
  Vnd std=Vnd(nummeasures);
  for (int j=1;j<=nummeasures;j++)
    std.setaxis(j,sqrt(var.getaxis(j))/(float)data.len);
  // Normalise (ave=0,stdev=1.0)
  for (int i=1;i<=data.len;i++) {
    Vnd *tmp=data.num(i).input;
    for (int j=1;j<=nummeasures;j++)
      tmp->setaxis(j,(tmp->getaxis(j)-ave.getaxis(j))/std.getaxis(j));
  }
//  for (int j=1;j<=nummeasures;j++)
//    printf("Measure %i has ave=%f, var=%f, std=%f\n",j,ave.getaxis(j),var.getaxis(j),std.getaxis(j));
}  

int positivecnt=0;
int negativecnt=0;
int naffcnt=0;
int badcnt=0;

#include "measures.c"
#include "segmenter.c"
#include "other.c"
#include "tasks.c"

void main(int argc,String *argv) {
  
  randomise();

  a=ArgParser(argc,argv);
 
  nnname=a.argafter("-nn","nn name","current.net");
  tsname=a.argafter("-ts","trainset name","current.pat");
  eacq=a.intafter("-eqcq","Edge Angle Cancelling histogram quantisation",16);
  eacrad=a.intafter("-eacrad","Default radius for eac",6);
  glhistrad=a.intafter("-glvr","Default radius for gl variance",3);
  glq=a.intafter("-glq","greylevel quantisation",32);
  windres=a.intafter("-wskip","pixel skip for window segmentation",5);
  botres=a.intafter("-br","bottom res for neighbour segmenter (botres*2^n=topres)",2);
  topres=a.intafter("-tr","top res",32);
  notext=a.floatafter("-nt","not text if < than",-4.5);
  istext=a.floatafter("-it","is text if > than",-2.0);
  show=a.argexists("-show","show results of measures");

//  show=true; // ***

  usenumpostrainexs=a.intafter("-nptes","number of positive training examples to output",50);
  usenumnegtrainexs=a.intafter("-nntes","number of positive training examples to output",50);
  scale=a.floatafter("-s","scale (deprecated)",0.5);
  processwidth=a.intafter("-pw","process width",400);
  morphrad=a.intafter("-mr","radius for morphology",3);
  twooutnodes=true; // a.argexists("-to","two output nodes");
  minarea=a.intafter("-ms","minimum size of kept region",200);
//  ghistscale=a.floatafter("-ghs","scale for hist stability",0.5);

  a.opts.add("task = trainset | newnn | trainnn | scan | test");
  String task=a.arg("task");
 
  if (Seq(task,"trainset")) {
      a.opts.add("trainset task = new | image | finish");
      String task2=a.arg("trainset task");

      if (Seq(task2,"new")) {
        tsname=a.argor(tsname);
        a.done();
        makenewtrainingset();

      } else if (Seq(task2,"image")) {
        iname=a.arg("image file");
        a.done();
        trainimage();

      } else if(Seq(task2,"finish")) {
        a.done();
        finishtrainingset();

      } else {
        a.done();
        error("Please choose a task for the training set");

      }

  } else if (Seq(task,"newnn")) {
      a.opts.add("nn type = hopfield | banana");
      String type=a.arg("nn type");
      a.done();
      makenewnn(type);

  } else if (Seq(task,"trainnn")) {
      a.done();
      trainnetwork();
    
  } else if (Seq(task,"scan")) {
      iname=a.arg("image file");
      a.done();
      scanimage();
    
  } else if (Seq(task,"test")) {
      iname=a.arg("image file");
      a.done();
      testimage();

  } else {
      a.done();
      error("Please choose a task.");

  }
  
  printf("JNN done.\n");

}
