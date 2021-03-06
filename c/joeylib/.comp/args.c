#ifndef args_C
  #define args_C

  #include <args.h>

String safegetenv(String s) {
  String e=getenv(s);
  if (e==NULL)
    error("Could not get environment variable %s",s);
  return e;
}

String JPATH() {
  return safegetenv("JPATH");
}

// Deprecated: non-class Argument Parser
int Jargc;
char **Jargv;
void Jargsetup(int argc,char **argv) {
  Jargc=argc;
  Jargv=argv;
}
String Jgetargafter(String pre,String fail) {
  for (int i=0;i<Jargc-1;i++) {
    if (Seq(Jargv[i],pre))
      return Jargv[i+1];
  }
  return fail;
}

// Starts of class ArgParser


  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
  
   ArgParser::ArgParser() {
  }
  
   ArgParser::ArgParser(int c,char **v) {
    argc=c;
    argv=v;
    helpon=(argexists("-h") || argexists("--help"));
    com=Sconc(arg(0)," ");
    // for (int i=0;i<c;i++)
    // printf(">%s<\n",v[i]);
    #ifndef QUIET
//      printf("%s\n",arg(0));
    #endif
  }
  
  String ArgParser::strafter(String pre,String why,String fail) {
//    com=Sconc(com,"[",pre," <",why,">] ");
    com=Sconc(com,"[",pre,"..] ");
    for (int i=0;i<argc-1;i++) {
      if (Seq(argv[i],pre)) {
        opts.add(Sformat("  %s <%s> = %s",pre,why,argv[i+1]));
        String r=Snew(argv[i+1]);
        remargs(i,2);
        return r;
      }
    }
    // if (Slen(fail)==0)
    // error("Did not find %s : %s",pre,why);
//    if (helpon)
    if (Slen(why)==0)
      opts.add(Sformat("  %s <unknown> = %s (default)",pre,fail));
    else
      opts.add(Sformat("  %s <%s> = %s (default)",pre,why,fail));
    return fail;
  }
  
  String ArgParser::strafter(String pre,String fail) { // fail is why
    return strafter(pre,fail,"");
  }
  
  /* String strafter(String pre,String fail) {
       return strafter(pre,"",fail);
     }*/
  
  String ArgParser::getargafter(String pre,String fail) {
    return strafter(pre,fail);
  }
  
  String ArgParser::getargafter(String pre,String why,String fail) {
    return strafter(pre,why,fail);
  }
  
  String ArgParser::argafter(String pre,String fail) {
    return strafter(pre,fail);
  }
  
  String ArgParser::argafter(String pre,String why,String fail) {
    return strafter(pre,why,fail);
  }
  
  float ArgParser::floatafter(String pre,String why,float fail) {
    String s=strafter(pre,why,Sformat("%f",fail));
    float f;
    sscanf(s,"%f",&f);
    return f;
  }
  
  float ArgParser::floatafter(String pre,float fail) {
    return floatafter(pre,"",fail);
  }
  
  int ArgParser::intafter(String pre,String why,int fail) {
    String s=strafter(pre,why,Sformat("%i",fail));
    int f;
    sscanf(s,"%i",&f);
    return f;
  }
  
  int ArgParser::intafter(String pre,int fail) {
    return intafter(pre,"",fail);
  }
  
  bool ArgParser::argexists(String x,String d, bool remove) {
    com=Sconc(com,"[",x,"] ");
    for (int i=0;i<argc;i++) {
      if (Seq(argv[i],x)) {
        if (helpon)
          opts.add(Sformat("  %s : %s  ==  on",x,d));
        if (remove)
          remarg(i);
        return true;
      }
    }
    opts.add(Sformat("  %s : %s",x,d));
    return false;
  }
  
  bool ArgParser::argexists(String x,String d) {
    return argexists(x,d,true);
  }

  bool ArgParser::argexists(String x) {
    for (int i=0;i<argc;i++) {
      if (Seq(argv[i],x)) {
        remarg(i);
        return true;
      }
    }
    return false;
  }


  
  String ArgParser::argnum(int i) {
//    com=Sconc(com,"<> ");
    if (i<0 || i>=argc) {
//      opts.add(Sformat("Argument missing",i));
      helpon=true;
      return "";
      // error("Trying to get argument out of range %i [0-%i]",i,argc);
    }
    String s=Snew(argv[i]);
    return s;
  }
  
  String ArgParser::arg(int i) {
    return argnum(i);
  }
  
  String ArgParser::getarg(int i) { // Removes argument
    String s=arg(i);
    remarg(i);
    return s;
  }

  String ArgParser::getarg() { // Removes argument
    return getarg(1);
  }

  String ArgParser::arg(String w) { // Removes argument
    String s=getarg(1);
    if (Slen(s)==0)
      opts.add(Sformat("You must provide <%s>!",w));
    com=Sconc(com,"<",w,"> ");
    return s;
  }

  String ArgParser::getarg(String w) { // Removes argument
    return arg(w);
  }

  String ArgParser::argor(String w,String o) {
    com=Sconc(com,"[<",w,">] ");
    if (argsleft()) {
      // printf("Think args left.\n");
      String t=getarg(w);
      opts.add(Sformat("  <%s> = %s",w,t));
      return t;
    }
    opts.add(Sformat("  <%s> = %s (default)",w,o));
    return o;
  }
  
  String ArgParser::argor(String o) {
    return argor("",o);
  }
  
  void ArgParser::remarg(int i) {
    // Silly idea considering the way we return strings!
//    printf("Removing argument %s\n",argv[i]);
    for (int j=i+1;j<argc;j++)
      argv[j-1]=argv[j];
    argc--;
  }
  
  void ArgParser::remargs(int i,int j) {
    for (int k=1;k<=j;k++)
      remarg(i);
  }
  
  int ArgParser::argsleft() {
    if (argc>1)
      return (argc-1);
    else
      return false;
//    return argc>1;
  }

  void ArgParser::error(String s) {
    helpon=true;
    opts.add(s);
  }
  
  void ArgParser::done() {
    if (argc>1)
      helpon=true; // since unused arguments left
    if (helpon) {
      printf("%s\n",com);
      printf("Options:\n");
      for (int i=1;i<=opts.len;i++)
        printf("%s\n",opts.num(i));
      if (argc>1) {
        String s="";
        for (int i=1;i<argc;i++)
          s=Sconc(s,Sconc(" ",argv[i]));
        printf("Unused arguments: %s\n",Sfrom(s,2));
      }
      exit(0);
    }
  }
  
  void ArgParser::showargs() {
    for (int i=0;i<argc;i++) {
      printf("%i:%s ",i,arg(i));
    }
    printf("\n");
  }
  
  void ArgParser::comment(String s) {
//    opts.add(Sconc(" ",s));
    opts.add(s);
  }
  
// End class 


#endif
