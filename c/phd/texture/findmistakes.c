#include <joeylib.c>

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  float difft=a.floatafter("-dt","difference threshold => wrong",0.5);
  String fname=a.argor("results file","testresults.res");
  a.done();

  List<String> ls=readlinesfromfile(fname);
  List<String> newtr;
  String ps=Safter(ls.num(4),":");
//  printf("Parsing %s\n",ps);
  int numpats=Stoint(ps);
  int i=11;
  int j=0;
  int t=0;
  for (int p=1;p<=numpats;p++) {
    i++;
    String measures=ls.num(i);
    i++;
    bool istext=Seq(ls.num(i),"1 0");
    float pt,pnt;
    i++;
    sscanf(ls.num(i),"%f %f",&pt,&pnt);
    i++;
//    printf("%f %f\n",diff(pt-pnt,istext),difft);
    if (diff(pt-pnt,istext)>difft) {
      j++;
      newtr.add(Sformat("# Input %i (%i)\n%s\n#Target %i (%f)\n%f %f",j,p,measures,j,pt-pnt,(float)istext,(float)(1-istext)));
      if (istext)
        t++;
    }
  }
  printf("Made %i mistakes out of %i (That's %i percent)\n",j,numpats,100*j/numpats);
  printf("%i percent of mistakes were text (%i/%i)\n",100*t/j,t,j);
  newtr.insert(1,"SNNS pattern definition file V3.2\ngenerated at Tue Nov 23 16:25:13 1999\n");
  newtr.insert(2,Sformat("No. of patterns: %i",j));
  newtr.insert(3,Sformat("No. of input units: %i",5));
  newtr.insert(4,"No. of output units: 2");
  newtr.insert(5,"");
  writelinestofile(newtr,"correctdata.pat");
}