float classifyrnd(List<Pixel> *ps) {
  return myrnd();
}

void writemeasures(Vnd *input) {
  String s="";
  for (int i=1;i<=nummeasures;i++) {
    float x=input->getaxis(i);
    if (x>1000000 || x<-1000000)
      x=0.0;
    s=Sformat("%s%f ",s,x);
  }
  traindata.add(s);
}

void writetraindata(Vnd *input,float output) {
  numtrainexs++;
  traindata.add(Sformat("# Input %i",numtrainexs));
  writemeasures(input); // here
  traindata.add(Sformat("# Target %i",numtrainexs));
  if (!twooutnodes)
    traindata.add(Sformat("%f",output));
  else
    traindata.add(Sformat("%f %f",output,1.0-output));
}

float trainerror=0;

bool positivedata(IOData d) {
  return (d.output>=1.0-trainerror);
}

bool negativedata(IOData d) {
  return (d.output<=trainerror);
}

void addtraindata(bool (*suitable)(IOData),int howmany) {
  int fails=10000;
  for (int i=1;i<=howmany && fails>0;i++) {
    int j=intrnd(1,data.len);
    IOData d=data.num(j);
    if (suitable(d))
      writetraindata(d.input,d.output);
    else {
      i--;
      fails--;
    }
  }
  if (fails==0)
    printf("Dropped out due to too few suitable patterns\n");
}

Vnd *summarise(List<Pixel> *ps) {
  Vnd *nv=new Vnd(nummeasures);
  nv->setaxis(1,measureedgeanglecancelling(ps));
  nv->setaxis(2,sqrt(measureglvariance(ps)));
  nv->setaxis(3,measurehiststability(ps));
  nv->setaxis(4,measureedgecount(ps));
  nv->setaxis(5,measureedgespread(ps));
//  nv->setaxis(5,measureoldedgeanglecancelling(ps));
  if (show)
    for (int i=1;i<=nummeasures;i++)
      if (ps->len>1)
        for (int j=1;j<=ps->len;j++)
          measmaps.num(i)->setpos(ps->num(j),nv->getaxis(i));
      else {
        for (int x=0;x<windres;x++)
        for (int y=0;y<windres;y++)
          measmaps.num(i)->setpos(ps->num(1)-Pixel(windres/2,windres/2)+Pixel(x,y),nv->getaxis(i));
      }
  return nv;
}

float classifytrain(List<Pixel> *ps) {
  bool good=true;
  int cnt=0;
  List<Pixel> *qs;
  bool local=false;
  if (ps->len>1)
    qs=ps;
  else {
    local=true;
    qs=p2pixelsincircle(ps->num(1),max(glhistrad,eacrad));
  }
  for (int i=1;i<=qs->len;i++) {
    Pixel p=qs->num(i);
    if (!expected->inmap(p)) {
      good=false;
      break;
    }
    if (expected->getpos(p.x,p.y))
      cnt++;
  }
  float proportiontext=(float)cnt/(float)qs->len; // Desired output of NN
  if (local)
    destroy(qs);
  if (good) {
    Vnd *summary=summarise(ps);
//    printf("Here\n");
//    printf("Input %s should have output %f\n",summary->toString(),proportiontext);
    data.add(IOData(summary,proportiontext,ps->num(1)));
//    printf("Hello\n");
//    destroy(ps);
//    printf("F\n");
//    writetraindata(summary,proportiontext);
    // destroy(summary);
    if (positivedata(data.num(data.len)))
      positivecnt++;
    else if (negativedata(data.num(data.len)))
      negativecnt++;
    else
      naffcnt++;
  } else
    badcnt++;
  return proportiontext; // +floatrnd(-.2,.2);
}

float getnextmeasure() {
  float answer,oanswer;
  if (currentline>traindata.len) {
//    printf("Trying to read non-existent result\n");
    return 0;
  }
  if (!twooutnodes)
    sscanf(traindata.num(currentline),"%f",&answer);
  else {
    sscanf(traindata.num(currentline),"%f %f",&answer,&oanswer);
    answer=answer-oanswer;
/*    if (answer>oanswer)
      answer=1;
    else
      answer=0;*/
  }
  currentline+=4;
//  printf("£Got answer %f from line %s\n",answer,traindata.num(currentline));
  return answer;
}

float classifynn(List<Pixel> *ps) {
  if (twopass) {
    if (pass==1) {
      Vnd *summary=summarise(ps);
//      writetraindata(summary,123.456);
      data.add(IOData(summary,123.456,ps->num(1)));
//      destroy(summary);
      return 123.456;
    } else {
      return getnextmeasure();   
    }
  } else {
  Vnd *summary=summarise(ps);
  traindata.clear();
  traindata.add("SNNS pattern definition file V3.2");
//  traindata.add(Sformat("generated at %s\n",timestring()));
  traindata.add("generated at unknown time\n");
  traindata.add("No. of patterns: 1");
  traindata.add(Sformat("No. of input units: %i",nummeasures));
  if (!twooutnodes)
    traindata.add("No. of output units: 1");
  else
    traindata.add("No. of output units: 2");
  traindata.add("");
  traindata.add("# Quick question for the NN");
  writemeasures(summary);
  destroy(summary);
  traindata.add("# Don't know output");
  if (!twooutnodes)
    traindata.add("123.456");
  else
    traindata.add("123.456 654.321");
  writelinestofile(traindata,"question.pat");
  system("asknn > tmp.txt");
  List<String> ls=readlinesfromfile("answer.res");
//  deletefile("answer.res");
  float answer;  // Haven't bothered with twooutnodes case.
  sscanf(ls.num(14),"%f",&answer);
  printf("Answer was %f\n",answer);
  return answer;
  }
}

void addheaderto(List<String> *ls,int numpats) {
  ls->insert(1,"SNNS pattern definition file V3.2\ngenerated at Tue Nov 23 16:25:13 1999\n");
  ls->insert(2,Sformat("No. of patterns: %i",numpats));
  ls->insert(3,Sformat("No. of input units: %i",nummeasures));
  if (!twooutnodes)
    ls->insert(4,"No. of output units: 1");
  else
    ls->insert(4,"No. of output units: 2");
  ls->insert(5,"");
}

void go() {

  String fname=Sconc(ImageDir,iname,".bmp");
  String ename=Sconc(ImageDir,"exp/",iname,".bmp");
printf("read %s\n",fname);
  orig=RGBmp::readfile(fname);
//  image=orig->scaledby(scale)->getv3ds();
  image=orig->scaletowidth(processwidth)->getv3ds();
  printf("Scanning image %i x %i\n",image->width,image->height);
printf("done\n");
  if (train || test)
    expected=Map2d<float>::readfile(ename)->threshold(0.5)->binscaleto(image->width,image->height);

  greyscale=image->applyfn(&V3dtofloat);

  starttimer();

  greyscale->edgedetection(Map2d<float>::sobel(),&edgemag,&edgeang);

  ghistscale=1.0/(float)windres;
  ghs=new Map2d<GHist *>(image->width*ghistscale,image->height*ghistscale,(GHist *)NULL);
  
  if (train)
    data=List<IOData>(2000);
  
  printf("Starting segmentation...\n");
  Seg seg;
  if (train)
    seg=Seg(&classifytrain);
  else
    seg=Seg(&classifynn);
  

  if (show)
    for (int i=1;i<=nummeasures;i++)
      measmaps.add(new Map2d<float>(image->width,image->height,(float)0));

  if (twopass) {
    pass=1;
    printf("First scan, reading measures...\n");
    Map2d<bool> *b=seg.classifywindows();
    destroy(b);
    normalisedata();
    pausetimer();
    printf("Asking neural network...\n");
printf("A\n");
    for (int i=1;i<=data.len;i++) {
      writetraindata(data.num(i).input,123.456);
      data.p2num(i)->freedom();
    }
    printf("Freeing data\n");
    data.freedom();
printf("B\n");
    addheaderto(&traindata,numtrainexs);
    writelinestofile(&traindata,"question.pat");
    printf("Not freeing traindata strings\n");
//    traindata.freeall();
    printf("Freeing traindata\n");
    traindata.freedom();
    system("asknn");
    traindata=readlinesfromfile("answer.res");
    unpausetimer();
    currentline=14;
    pass=2;
    printf("Second scan...\n");
  }
  Map2d<bool> *b=seg.classifywindows();

  printf("Time taken: %f seconds.\n",gettimer());

  if (show)
    for (int i=1;i<=nummeasures;i++)
      measmaps.num(i)->writefile(getnextfilename("meas","bmp"));

//  Seg seg=Seg(&measureglvariance);
//  Map2d<bool> *b=seg.classifytopdown();
//  Map2d<bool> *b=seg.classifyneighbours();
//  Map2d<bool> *b=seg.classifywindows();

  b->scaleto(image->width,image->height)->writefile("initseg.bmp");
  
  // Reject small regions
  List< Region * > *l=b->getrealregions();
  b=new Map2d<bool>(b->width,b->height,false);
  for (int i=1;i<=l->len;i++) {
    List<Pixel> *ps=l->num(i)->getlist();
    if (ps->len>=minarea)
      for (int j=1;j<=ps->len;j++)
        b->setpos(ps->num(j),true);
  }
  b->writefile("initb4join.bmp");

  // Perform morphological joining (dilation and erosion)
  b=b->binscaleto(b->width/windres,b->height/windres);
  b=b->expand(morphrad)->contract(2*morphrad)->expand(morphrad);
  // b=b->contract(morphrad)->expand(2*morphrad)->contract(morphrad);
  // b=b->expand(morphrad)->inverse()->expand(morphrad)->inverse();
  b=b->binscaleto(image->width,image->height);
  // b->writefile("initjoined.bmp");
  
  b->writefile("textseg.bmp");
  
  if (test) {
    int correct=0;
    int wrong=0;
    int ctextcorrect=0;
    int cnottextcorrect=0;
    int ctextincorrect=0;
    int cnottextincorrect=0;
    int total=0;
    for (int i=0;i<b->width;i++)
    for (int j=0;j<b->height;j++) {
      total++;
      bool exp=expected->getpos(i*expected->width/b->width,j*expected->height/b->height);
      bool cla=b->getpos(i,j);
      if (exp==cla)
        correct++;
      else
        wrong++;
      if (exp)
        if (cla)
          ctextcorrect++;
        else
          cnottextincorrect++;
      else
        if (cla)
          ctextincorrect++;
        else
          cnottextcorrect++;
    }
    if (fileexists("test.dat")) {
      List<String> ls=readlinesfromfile("test.dat");
      correct+=tofloat(ls.num(1));
      wrong+=tofloat(ls.num(2));
      ctextcorrect+=tofloat(ls.num(3));
      cnottextcorrect+=tofloat(ls.num(4));
      ctextincorrect+=tofloat(ls.num(5));
      cnottextincorrect+=tofloat(ls.num(6));
      total+=tofloat(ls.num(7));
    }
    List<String> ls;
    ls.add(Sformat("%i correctly classified",correct));
    ls.add(Sformat("%i incorrectly classified",wrong));
    ls.add(Sformat("%i correctly classified as text",ctextcorrect));
    ls.add(Sformat("%i correct classified as non-text",cnottextcorrect));
    ls.add(Sformat("%i classified as text when not",ctextincorrect));
    ls.add(Sformat("%i classified as non-text when was text",cnottextincorrect));
    ls.add(Sformat("%i total",total));
    ls.add("");
    int totaltext=ctextcorrect+cnottextincorrect;
    int totalnottext=ctextincorrect+cnottextcorrect;
    ls.add(Sformat("That's %i text in total",totaltext));
    ls.add(Sformat("and %i non-text in total",totalnottext));
    ls.add("");
    ls.add("And in percent:");
    ls.add(Sformat("%f percent correctly classified",100.0*(float)correct/(float)total));
    ls.add(Sformat("%f percent incorrectly classified",100.0*(float)wrong/(float)total));
    ls.add(Sformat("%f percent of text correctly classified as text",100.0*(float)ctextcorrect/(float)totaltext));
    ls.add(Sformat("%f percent of non-text correctly classified as non-text",100.0*(float)cnottextcorrect/(float)totalnottext));
    writelinestofile(ls,"test.dat");
  }
    
}
