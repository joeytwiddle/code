#ifndef argsold_C
  #define argsold_C

  #include <argsold.h>

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
  
   ArgParser::ArgParser(int c,char **v) {
    argc=c;
    argv=v;
    helpon=argexists("-h");
    if (helpon)
      printf("Arguments:\n");
    // for (int i=0;i<c;i++)
    // printf(">%s<\n",v[i]);
  }
  
  String ArgParser::strafter(String pre,String why,String fail) {
    for (int i=0;i<argc-1;i++) {
      if (!strcmp(argv[i],pre)) {
        printf("%s <%s> = %s\n",pre,why,argv[i+1]);
        String r=Snew(argv[i+1]);
        remargs(i,2);
        return r;
      }
    }
    // if (Slen(fail)==0)
    // error("Did not find %s : %s",pre,why);
    if (helpon)
    if (Slen(why)==0)
      printf("%s <unknown> = default %s\n",pre,fail);
    else
      printf("%s <%s> = default %s\n",pre,why,fail);
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
  
  bool ArgParser::argexists(String x,String d) {
    for (int i=0;i<argc;i++) {
      if (!strcmp(argv[i],x)) {
        printf("%s : %s  ==  on\n",x,d);
        remarg(i);
        return true;
      }
    }
    printf("%s : %s\n",x,d);
    return false;
  }
  
  bool ArgParser::argexists(String x) {
    for (int i=0;i<argc;i++) {
      if (!strcmp(argv[i],x)) {
        remarg(i);
        return true;
      }
    }
    return false;
  }
  
  String ArgParser::argnum(int i) {
    if (i<0 || i>=argc)
      error("Trying to get argument out of range %i [0-%i]",i,argc);
    String s=Snew(argv[i]);
    return s;
  }
  
  String ArgParser::arg(int i) {
    return argnum(i);
  }
  
  String ArgParser::getarg(int i) { // Removes argument
    if (i<0 || i>=argc)
      error("Trying to get argument out of range %i [0-%i]",i,argc);
    String s=Snew(argv[i]);
    remarg(i);
    return s;
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
    return argc-1;
  }
  
  void ArgParser::done() {
    if (argc>1)
      helpon=true; // since error encountered
    if (helpon) {
      String s="";
      if (argc>1) {
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
  
// End class 


#endif
