#include <joeylib.c>

void expect(String a,String b) {
  if (!Seq(a,b))
    error("Got %s expected %s",b,a);
}

void main(int argc,String *argv) {
  
  if (argc<3) {
    printf("%s","Syntax: forall <files> do <command>\n");
    printf("%s","The command may use %f as the filename,\n");
    printf("%s","%s as the filename stripped of extensions,\n");
    printf("%s","%w as the whole path of the filename,\n");
    printf("%s","%n as the number of the file being processed,\n");
    printf("%s","%a as '&', %p as '>',\n");
    printf("%s","and : to seperate commands.\n");
    exit(0);
  }
  ArgParser a=ArgParser(argc,argv);
  bool showprogress=a.argexists("-sp","show progress");
  List<String> fs;
  String s="";
  while (!Seq(s,"do") && a.argsleft()) {
    s=a.getarg(1);
    if (!Seq(s,"do")) {
      if (showprogress)
        printf("%s ",s);
      fs.add(getfiles(s));
    }
  }
  if (showprogress)
    printf("\n");
  List<String> ls;
  while (a.argsleft()) {
    ls.add(a.getarg(1));
//    printf("Got %s\n",ls.num(ls.len));
  }
  
  for (int i=1;i<=fs.len;i++) {
    String whole=fs.num(i);
    String fname;
    int n=max(Sinstrlast(whole,"/"),Sinstrlast(whole,"\\"));
    if (n==0)
      fname=whole;
    else
      fname=Sfrom(whole,n+1);
    String str=fname;
    int k;
    while ((k=Sinstr(str,"."))>0) {
      str=Sleft(str,k-1);
    }
    String com="";
    for (int j=1;j<=ls.len;j++) {
      String arg=ls.num(j);
      arg=Sreplaceall(arg,"\%n",Sformat("%i",i));
      arg=Sreplaceall(arg,"\%a","&");
      arg=Sreplaceall(arg,"\%f",fname);
      arg=Sreplaceall(arg,"\%w",whole);
      arg=Sreplaceall(arg,"\%s",str);
      arg=Sreplaceall(arg,"\%p",">");
      if (Seq(arg,":")) {
        com=Srls(com);
        printf("-%i/%i- %s\n",i,fs.len,com);
        system(com);
        com="";
      } else
        com=Sconc(com," ",arg);
    }
    com=Srls(com);
    printf("-%i/%i- %s\n",i,fs.len,com);
    system(com);
  }
  
}
