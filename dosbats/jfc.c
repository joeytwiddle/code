#include <joeylib.c>

bool simple,yesno,difference=false;

void compare(List<String> als,List<String> bls,String aname,String bname) {
  if (!yesno) {
  printf("\n");
  printf("Lines in %s not present in %s:\n\n",aname,bname);
  }
  for (int i=1;i<=als.len;i++) {
    String al=als.num(i);
    bool found=false;
    for (int j=1;j<=bls.len && !found;j++) {
      String bl=bls.num(j);
      found=Ssimilar(al,bl);
    }
    if (!found) {
      difference=true;
      if (!yesno) {
      if (!simple)
        printf("%i : ",i);
      printf("%s\n",al);
      }
    }
  }
}

int main(int argc,String *argv) {
  if (argc<3) {
    printf("jfc <file> <file> : compare two files\n");
    exit(0);
  }
  ArgParser a=ArgParser(argc,argv);
  String aname=argv[1];
  String bname=argv[2];
  simple=a.argexists("simple","Simple output for pasting into files");
  yesno=a.argexists("yesno","Are they the same (0) or different (1) ?");
  bool oneway=a.argexists("oneway","Only compare in one direction");
  String seperators=a.argafter("-s","Seperators","\\n");
  seperators=Sreplaceall(seperators,"\\n","\n");
  List<String> als=readlinesfromfile(aname,seperators);
  List<String> bls=readlinesfromfile(bname,seperators);
  compare(als,bls,aname,bname);
  if (!oneway)
    compare(bls,als,bname,aname);
  if (yesno)
    if (difference) {
      printf("Different.\n");
      return 1;
    } else {
      printf("The same.\n");
      return 0;
    }
  return 0;
}
