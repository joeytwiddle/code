#define QUIET

#include <joeylib.c>

bool quiet;

void expect(String a,String b) {
  if (!Seq(a,b))
    error("Got %s expected %s",b,a);
}

void main(int argc,String *argv) {
  
  String batname=Sconc(JPATH(),"/tmp/forall.bat");

  ArgParser a=ArgParser(argc,argv);
//    a.comment("Syntax: forall <files> do <command>\n");
//    a.comment("    or: forall in <filename> do <command>\n");
    a.comment("  <command> may use %f as the filename,");
    a.comment("    %s as the filename stripped of extensions,");
    a.comment("    %e as the filename's extension,");
    a.comment("    %l as %f in lower case,");
    a.comment("    %L as %w in lower case,");
    a.comment("    %w as the whole path of the filename,");
    a.comment("    %n as the number of the file being processed,");
    a.comment("    %a as & , %p as > , %q as \" ,");
    a.comment("    and : to seperate commands.");
    #ifdef DOS
      a.comment("    For DOS, use $ instead of %");
    #endif
  bool dirsonly=a.argexists("-dirsonly","execute command on directories only");
  bool filesonly=a.argexists("-filesonly","execute command on non-directories only");
  bool linkeddirsok=a.argexists("-followdirs","follow sym linked directories");
  bool linkedfilesok=!a.argexists("-nofollowfiles","don't follow sym linked files");
  bool showprogress=a.argexists("-showprog","show progress");
  bool shell=a.argexists("-shell","run commands through a shell file");
  a.comment("    A forall command in shell mode is not the same in non-shell mode");
  a.comment("    since in the shell program the state will persist between each file.");
  bool ccomments =!a.argexists("-//","[in]: ignore // commenting");
  bool shcomments=!a.argexists("-#"," [in]: ignore # commenting");
  quiet=a.argexists("-stealth","don't print stuff");
  int padzeros=a.intafter("-padzeros","pad %n with this many 0s on left",3);
  String padzeroformat=Sformat("%s0%ii","%",padzeros);
  bool checkforfilename=false;
  if (argc<3 || a.helpon) {
    a.com="forall <files> do <command>   OR\nforall in <filename> do <command>";
    a.helpon=true;
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
        if (isdir(s)) {
          if (filesonly)
            okay=false;
          if (islink(s) && !linkeddirsok)
            okay=false;
        } else { // not dir => file
          if (dirsonly)
            okay=false;
          if (islink(s) && !linkedfilesok)
            okay=false;
        }
        if ( (dirsonly || filesonly) &&
             !fileexists(s) )
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
  progress=fopen("forall.progress","w");
  List<String> coms;
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
      String wholelow=Stolower(whole);
      /* Remove all '.'s
      int k;
      while ((k=Sinstr(str,"."))>0) {
        str=Sleft(str,k-1);
      } */
      int k=Sinstrlast(fname,".");
      String str=( k<=0 ? fname : Sleft(fname,k-1) );
      String ext=( k<=0 ? Snew("") : Sfrom(fname,k+1) );
      // List<String> coms;
      if (!shell)
        coms.clear();
      String com="";
      for (int j=1;j<=ls.len;j++) {
        String arg=ls.num(j);
  //      if (showprogress)
  //        printf("%s -> ",arg);
        #ifdef DOS
        arg=Sreplaceall(arg,"$n",Sformat(padzeroformat,i));
        arg=Sreplaceall(arg,"$a","&");
        arg=Sreplaceall(arg,"$f",fname);
        arg=Sreplaceall(arg,"$w",whole);
        arg=Sreplaceall(arg,"$s",str);
        arg=Sreplaceall(arg,"$e",ext);
        arg=Sreplaceall(arg,"$l",strlow);
        arg=Sreplaceall(arg,"$L",wholelow);
        arg=Sreplaceall(arg,"$p",">");
        arg=Sreplaceall(arg,"$q","\"");
        #else
        arg=Sreplaceall(arg,"\%n",Sformat(padzeroformat,i));
        arg=Sreplaceall(arg,"\%a","&");
        arg=Sreplaceall(arg,"\%f",fname);
        arg=Sreplaceall(arg,"\%w",whole);
        arg=Sreplaceall(arg,"\%s",str);
        arg=Sreplaceall(arg,"\%e",ext);
        arg=Sreplaceall(arg,"\%l",strlow);
        arg=Sreplaceall(arg,"\%L",wholelow);
        arg=Sreplaceall(arg,"\%p",">");
        arg=Sreplaceall(arg,"\%q","\"");
        #endif
  //      if (showprogress)
  //        printf("%s -> ",arg);
        if (Seq(arg,":")) {
          com=Srls(com);
          if (!quiet) {
            // printf("# (%03i/%i) %s\n",i,fs.len,com);
            String output=Sformat("# (%03i/%i) %s",i,fs.len,com);
            if (shell) // Print it later when the shell is being executed
              coms.add(Sconc("echo \"",Stoechoformat(output),"\""));
            else
              printf("%s\n",output);
          }
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
          if (!quiet) {
            String output=Sformat("# (%03i/%i) %s",i,fs.len,com);
            if (shell) // Print it later when the shell is being executed
              coms.add(Sconc("echo \"",Stoechoformat(output),"\""));
            else
              printf("%s\n",output);
          }
      if (progress!=NULL)
        fprintf(progress,"-%03i/%i- %s\n",i,fs.len,com);
      if (shell)
        coms.add(com);
      else
        system(com);
    }
  }
      if (shell) {
        if (coms.len>0) {
          writelinestofile(coms,batname);
          #ifdef DOS
            system(batname);
            system(Sconc("del ",batname));
          #else
            system(Sconc("chmod a+x ",batname));
            int err=system(Sconc("",batname));
            if (err!=0) {
              List<String> ls=readlinesfromfile(batname);
              printf("Error in shellscript!\n");
              /* printf("Error in:\n");
              for (int i=0;i<ls.len;i++)
                printf("  %s\n",ls.get(i)); */
            }
          #endif
        }
          //}
//          if (fileexists(batname))
//            system(Sconc("rm ",batname));
        if (!quiet)
          printf("done.\n");
      }
  if (progress==NULL) {
    if (showprogress)
      printf("Could not write a progress report!\n");
  } else {
    fclose(progress);
    #ifdef DOS
      system("del forall.progress");
    #else
      system("rm -f forall.progress");
    #endif
  }

}
