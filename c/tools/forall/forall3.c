#define QUIET

#include <joeylib.c>

bool quiet;

void expect(String a,String b) {
  if (!Seq(a,b))
    error("Got %s expected %s",b,a);
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  bool dirsonly=a.argexists("-dirsonly","execute command on directories only");
  bool filesonly=a.argexists("-filesonly","execute command on non-directories only");
  bool showprogress=a.argexists("-showprog","show progress");
  bool shell=a.argexists("-shell","run command through a shell file");
  bool ccomments =!a.argexists("-//","[in]: ignore // commenting");
  bool shcomments=!a.argexists("-#"," [in]: ignore # commenting");
  quiet=a.argexists("-stealth","don't print stuff");
  bool checkforfilename=false;
  if (argc<3 || a.helpon) {
    printf("%s","Syntax: forall <files> do <command>\n");
    printf("%s","    or: forall in <filename> do <command>\n");
    printf("%s","The command may use %f as the filename,\n");
    printf("%s","%s as the filename stripped of extensions,\n");
    printf("%s","%e as the filename's extension,\n");
    printf("%s","%l as %f in lower case,\n");
    printf("%s","%w as the whole path of the filename,\n");
    printf("%s","%n as the number of the file being processed,\n");
    printf("%s","%a as & , %p as > , %q as \"\n");
    printf("%s","and : to seperate commands.\n");
    #ifdef DOS
      printf("%s","Note: for DOS, use $ instead of %\n");
    #endif
    a.done();
    exit(0);
  }

  // Read files until do
  if (showprogress)
    printf("Files: ");
  List<String> fs;
  if (!Seq(a.arg(1),"in")) { // Files provided as arguments
    String s="";
    while (!Seq(s,"do") && a.argsleft()) {
      s=a.getarg(1);
      if (!Seq(s,"do")) {
        if (showprogress)
          printf("%s ",s);
        boolean okay=true;
//        if (checkforfilename)
//          fs.add(getfiles(s));
//        else
/*        if (!fileexists(s)) {
          if (!quiet)
            printf("Didn't find file %s\n",s);
          okay=false;
        }*/
        if (dirsonly && !isdir(s))
          okay=false;
        if (filesonly && isdir(s))
          okay=false;
        if ((Sinstr(s,"*")>0) || (Sinstr(s,"?")>0)) { // Unresolved wildcard
          okay=false;
          if (!quiet)
            printf("forall: Did not find any \"%s\"\n",s);
        }
        if (Slen(s)==0)
          okay=false;
        if (okay)
          fs.add(s);
      }
    }
  } else { // Points to file containing list of files for forall
    String tmp=a.getarg(1);
    String fname=a.getarg(1);
    tmp=a.getarg(1);
    if (!Seq(tmp,"do"))
      error("Expected do in forall in <fname> do ...");
    fs.add(readlinesfromfile(fname));
    for (int i=1;i<=fs.len;i++) {
      String s=fs.num(i);
      if ( Slen(s)==0
        || (dirsonly && !isdir(s))
        || (filesonly && isdir(s))
        || (ccomments && Sstarts(s,"//"))
        || (shcomments && Sstarts(s,"#")) ) {
        fs.removenumkeeporder(i);
        i--;
      }
    }
    if (showprogress)
      printf("%s ",fs.num(fs.len));
  }
  if (showprogress)
    printf("\n");

  // Read command after do
  if (showprogress)
    printf("Command: ");
  a.com=Sconc(a.com,"do <command> ");
  List<String> ls;
  while (a.argsleft()) {
    ls.add(Ssplitaround(a.getarg(1)," "));
    if (showprogress)
      printf("\"%s\" ",ls.num(ls.len));
  }
  if (showprogress)
    printf("\n");
  if (ls.len==0)
    a.error("You must provide <command>");
  a.done();
  
  FILE *progress=NULL;
  progress=fopen("progress.fa","w");
  for (int i=1;i<=fs.len;i++) {
    String whole=fs.num(i);
    if (Slen(whole)>0) {
      String fname;
      int n=max(Sinstrlast(whole,"/"),Sinstrlast(whole,"\\"));
      if (n==0)
        fname=whole;
      else
        fname=Sfrom(whole,n+1);
      String strlow=Stolower(fname);
      /* Remove all '.'s
      int k;
      while ((k=Sinstr(str,"."))>0) {
        str=Sleft(str,k-1);
      } */
      int k=Sinstrlast(fname,".");
      String str=( k<=0 ? fname : Sleft(fname,k-1) );
      String ext=( k<=0 ? "" : Sfrom(fname,k+1) );
      List<String> coms;
      String com="";
      for (int j=1;j<=ls.len;j++) {
        String arg=ls.num(j);
  //      if (showprogress)
  //        printf("%s -> ",arg);
        #ifdef DOS
        arg=Sreplaceall(arg,"$n",Sformat("%03i",i));
        arg=Sreplaceall(arg,"$a","&");
        arg=Sreplaceall(arg,"$f",fname);
        arg=Sreplaceall(arg,"$w",whole);
        arg=Sreplaceall(arg,"$s",str);
        arg=Sreplaceall(arg,"$e",ext);
        arg=Sreplaceall(arg,"$l",strlow);
        arg=Sreplaceall(arg,"$p",">");
        arg=Sreplaceall(arg,"$q","\"");
        #else
        arg=Sreplaceall(arg,"\%n",Sformat("%03i",i));
        arg=Sreplaceall(arg,"\%a","&");
        arg=Sreplaceall(arg,"\%f",fname);
        arg=Sreplaceall(arg,"\%w",whole);
        arg=Sreplaceall(arg,"\%s",str);
        arg=Sreplaceall(arg,"\%e",ext);
        arg=Sreplaceall(arg,"\%l",strlow);
        arg=Sreplaceall(arg,"\%p",">");
        arg=Sreplaceall(arg,"\%q","\"");
        #endif
  //      if (showprogress)
  //        printf("%s -> ",arg);
        if (Seq(arg,":")) {
          com=Srls(com);
          if (!quiet)
            printf("# (%03i/%i) %s\n",i,fs.len,com);
          if (shell)
            coms.add(com);
          else
            system(com);
          com="";
        } else
          com=Sconc(com," ",arg);
  /*      if (showprogress) {
          printf("%s\n>",com);
          for (int k=1;k<=ls.len;k++)
            printf("%s ",ls.num(k));
          printf("\n");
        }*/
      }
      com=Srls(com);
      if (!quiet)
        printf("# (%03i/%i) %s\n",i,fs.len,com);
      if (progress!=NULL)
        fprintf(progress,"-%03i/%i- %s\n",i,fs.len,com);
      if (shell)
        coms.add(com);
      else
        system(com);
      if (shell) {
  //      printf("Executing: %s\n",coms.toString());
        writelinestofile(coms,"forall.bat");
        // system("chmod a+x forall.bat");
        #ifdef DOS
          system("forall.bat");
        #else
          system("source ./forall.bat");
        #endif
      }
    }
  }
  if (progress==NULL) {
//    if (showprogress)
      printf("Could not write a progress report!\n");
  } else {
    fclose(progress);
    #ifdef DOS
      system("del progress.fa");
    #else
      system("rm -f progress.fa");
    #endif
  }
  if (shell)
    system("rm forall.bat");

}
