void makenewtrainingset() {
  List<String> ls;
  ls.add("# New training set");
  writelinestofile(ls,tsname);
}

void trainimage() {
  train=true;
  test=false;
  go();
//  writelinestofile(&traindata,Sformat("%s.td",iname));
  normalisedata();
  normalisedata();
  normalisedata();
  traindata=readlinesfromfile(tsname);
  addtraindata(&positivedata,usenumpostrainexs);
  addtraindata(&negativedata,usenumnegtrainexs);
  writelinestofile(&traindata,tsname);
  printf("Positive data: %i\n",positivecnt);
  printf("Negative data: %i\n",negativecnt);
  printf("Naff data:     %i (partly text)\n",naffcnt);
  printf("Bad data:      %i (too close to edge of image)\n",badcnt);
}

void finishtrainingset() {
  List<String> ls=readlinesfromfile(tsname);
  // Calculate number of training examples:
  ls.removenumkeeporder(1);
  int pats=(ls.len)/4;
  addheaderto(&ls,pats);
  writelinestofile(ls,tsname);
}

void makenewnn(String type) {
  List<String> ls;

  if (Seq(type,"hopfield")) {
    ls.add("# Hopfield network");

  } else if (Seq(type,"banana")) {
    ls.add("# Banana network");

  } else
    error("Please give a correct type of nn.");

  writelinestofile(ls,nnname);
}

void trainnetwork() {
  system(Sformat("cp %s initialnet.net",nnname));
  system(Sformat("cp %s traindata.pat",tsname));
  system("batchman -f trainnet.bat");
}
 
void scanimage() {
  train=false;
  test=false;
  show=true;
  twopass=true;
  go();
}

void testimage() {
  train=false;
  test=true;
  twopass=true;
  go();
}

