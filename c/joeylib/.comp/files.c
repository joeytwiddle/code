#ifndef files_C
  #define files_C

  #include <files.h>

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int numfileswritten=0;

int getnextfilenum() {
  numfileswritten++;
  return numfileswritten;
}

String getnextfilename() {
  return Sformat("%03i",getnextfilenum());
}

String getnextfilename(String ext) {
  String all=getnextfilename();
  return Sconcat(all,Sconcat(".",ext));
}

String getnextfilename(String pre,String ext) {
  return Sformat("%s%s.%s",pre,getnextfilename(),ext);
}

void unfuckfile(String fname,String oname) {
  FILE *fp=fopen(fname,"r");
  if (fp==NULL)
    error("Problem reading %s\n",fname);
  FILE *op=fopen(oname,"w");
  char c,lastc;
  int i;
  int lasti;
  while ((i=getc(fp))!=EOF) {
    c=(char)i;
    if (i>=32 && i<=126)
      printf("%c",c);
    else
      printf("<%i>",i);
    if (i==10 && lasti!=13) {
      printf("[%i %i]",lasti,i);
    } else {
      putc(c,op);
    }
    lastc=c;
    if (i==10)
      i=13;
    lasti=i;
  }
  fclose(fp);
  fclose(op);
}    

int close(FILE *fp) {
  return fclose(fp);
}

List<String> readlinesfromfile(String fname,String seperators,boolean trim) {
  #ifndef QUIET
#ifdef NOISY_OPEN_TXT
  fprintf(stderr,"Opening text file %s\n",fname);
#endif
  #endif
  #ifdef DEBUG
    fprintf(stderr,"Reading lines from file...\n");
  #endif
  FILE *fp=fopen(fname,"r");
  if (fp==NULL)
    error("Problem reading %s\n",fname);
  List<String> ls;
  int i;
  char c;
  List<char> cs;
  while ((i=getc(fp))!=EOF) {
    c=(char)i;
    if ((int)c==13)
      c=(char)10;
    #ifdef DEBUG
      printf("%c",(char)c);
    #endif
    if (Sinstr(seperators,c)>0) {
      String s=Snew(cs.len);
      for (int i=1;i<=cs.len;i++)
        s[i-1]=cs.num(i);
      if (trim)
        s=Strim(s);
      ls.add(s);
      cs.len=0;
    } else
    cs.add(c);
  }
  String s=Snew(cs.len);
  for (int i=1;i<=cs.len;i++)
    s[i-1]=cs.num(i);
  if (trim)
    s=Strim(s);
  cs.freedom();
  ls.add(s);
  close(fp);
  #ifdef DEBUG
    printf("\n\n");
  #endif
  return ls;
}

List<String> readlinesfromfile(String fname,String seperators) {
  return readlinesfromfile(fname,seperators,false);
}


List<String> mail2list(String fname) {
        String seperators="\n"; // {(char)10};
  #ifndef QUIET
#ifdef NOISY_OPEN_TXT
  fprintf(stderr,"Opening text file %s\n",fname);
#endif
  #endif
  #ifdef DEBUG
    fprintf(stderr,"Reading lines from file...\n");
  #endif
  FILE *fp=fopen(fname,"r");
  if (fp==NULL)
    error("Problem reading %s\n",fname);
  List<String> ls;
  char c;
  List<char> cs;
  int i;
  while ((i=getc(fp))!=EOF) {
    c=(char)i;
    #ifdef DEBUG
      printf("%c",(char)c);
    #endif
    if (Sinstr(seperators,toString(c))>0) {
      String s=Snew(cs.len);
      for (int i=1;i<=cs.len;i++)
        s[i-1]=cs.num(i);
      ls.add(s);
      cs.len=0;
    } else
    cs.add(c);
  }
  String s=Snew(cs.len);
  for (int i=1;i<=cs.len;i++)
    s[i-1]=cs.num(i);
  cs.freedom();
  ls.add(s);
  close(fp);
  #ifdef DEBUG
    printf("\n\n");
  #endif
  return ls;
}

List<String> readlinesfromfile(String fname) {
  return readlinesfromfile(fname,"\n");
}

/* Inefficient due to Sconc
List<String> readlinesfromfile(String fname) {
  // printf("Opening %s\n",fname);
  #ifdef DEBUG
    printf("Reading lines from file...\n");
  #endif
  FILE *fp=fopen(fname,"r");
  if (fp==NULL)
    error("Problem reading %s\n",fname);
  List<String> ls;
  char c;
  String s="";
  int i;
  while ((i=getc(fp))!=EOF) {
    c=(char)i;
    #ifdef DEBUG
      printf("*");
    #endif
    if (c=='\n') {
      ls.add(s);
      // printf("* %s\n",s);
      s="";
    } else
    s=Sconc(s,chtoString(c));
  }
  ls.add(s);
  fclose(fp);
  #ifdef DEBUG
    printf("\n\n");
  #endif
  return ls;
}*/

void writelinestofile(List<String> ls,String fname,String sep) {
  FILE *fp=fopen(fname,"w");
  #ifndef QUIET
  printf("Writing text file %s (%i lines)\n",fname,ls.len);
  #endif
  for (int i=1;i<=ls.len;i++) {
    // fprintf(fp,ls.num(i)); // This is very bad if the String contains '%' '\' etc.
    fprintf(fp,"%s",ls.num(i));
    if (i<ls.len)
      fprintf(fp,sep);
  }
  fclose(fp);
}

void writelinestofile(List<String> ls,String fname) {
  writelinestofile(ls,fname,"\n");
}

void writelinestofile(String fname,List<String> ls) {
  writelinestofile(ls,fname,"\n");
}

void writelinestofile(List<String> *ls,String fname) {
  writelinestofile(*ls,fname,"\n");
}

void writelinestofile(String fname,List<String> *ls) {
  writelinestofile(*ls,fname,"\n");
}

String getlinefromfile(FILE *fp) {
  int i=getc(fp);
  if (i==EOF)
    return "EOF";
  char c=(char)i;
//  printf("!\n");
  String s=chtoString(c);
  while ((i=getc(fp))!=EOF) {
    c=(char)i;
    if (c=='\n')
      return s;
    else
    s=Sconc(s,chtoString(c));
  }
  return s;
}

bool JFilesRecursive=true;

bool islink(String fname) {
  #ifdef DOS
    return false;
  #else
        struct stat s;
        if (lstat(fname,&s)==-1) { // Use lstat so we know if we've hit a link (other stat's dereference to destination file!)
          #ifndef QUIET
          printf("Problem analysing %s\n",fname);
          #endif
          return false;
        } else {
          //printf("%s is a link = %i\n",fname,S_ISLNK(s.st_mode));
          if (S_ISLNK(s.st_mode))
            return true;
          else
            return false;
        }
//  return true;
  #endif
}

bool isdir(String fname) {
  struct stat s;
  if (stat(fname,&s)==-1) {
    #ifndef QUIET
      printf("Problem analysing %s\n",fname);
    #endif
    return false;
  } else {
    if (S_ISDIR(s.st_mode))
      return true;
    else
      return false;
  }
/* #ifdef DOS
    if (access(fname, D_OK)==0) {
      printf("%s is a directory\n",fname);
      return true;
    } else {
      printf("%s is not a directory\n",fname);
      return false;
    }
  #endif */
}

#ifdef DJGPP

List<String> getfilesrecursive(String dn) {
  if (Seq(Sright(dn,1),"/") && Slen(dn)>1)
    dn=Sleft(dn,Slen(dn)-1);
  List<String> fs;
  DIR *dp=opendir(dn);
  struct dirent *ep;
  if (dp!=NULL) {
    while (ep=readdir(dp)) {
      String fname=Snew(ep->d_name);
//      printf("%s\n",fname);
      if (!Seq(fname,".") && !Seq(fname,"..")) {
        if (!Seq(dn,"."))
          fname=Sconc(dn,"/",fname);
        struct stat s;
        if (stat(fname,&s)==-1) {
          #ifndef QUIET
            printf("Problem analysing %s\n",fname);
          #endif
        } else
          if (S_ISDIR(s.st_mode))
            fname=Sconc(fname,"/");
        fs.add(fname);
        if (Seq(Sright(fname,1),"/"))
          fs.add(getfilesrecursive(fname));
      }
    }
    closedir(dp);
  } else
    printf("Could not open directory %s\n",dn);
//printf("%i\n",fs.len);  
return fs;
}

List<String> getfilesindir(String dn) {
#ifdef DEBUG
  printf("gfid: %s\n",dn);
  #endif
  if (Slen(dn)==0)
    dn=".";
  if (Seq(Sright(dn,1),"/") && Slen(dn)>1)
    dn=Sleft(dn,Slen(dn)-1);
  List<String> fs;
  DIR *dp=opendir(dn);
  struct dirent *ep;
  if (dp!=NULL) {
    while (ep=readdir(dp)) {
      String fname=Snew(ep->d_name);
      if (!Seq(fname,".") && !Seq(fname,"..")) {
      #ifdef DEBUG
      printf("[%s ",fname);
      #endif
        struct stat s;
        if (stat(Sconc(dn,"/",fname),&s)==-1)
        if (stat(Sconc(dn,"/",fname),&s)==-1)
          printf("Problem analysing %s\n",fname);
        else
          if (S_ISDIR(s.st_mode))
            fname=Sconc(fname,"/");
        fs.add(fname);
      }
    }
    closedir(dp);
  } else
    printf("Could not open directory %s\n",dn);
//printf("%i\n",fs.len);  
return fs;
}

#ifndef DOS
List<String> getfiles(String dn,String format) {
//  printf("Trying to find %s in %s\n",format,dn);
  if (Seq(Sright(dn,1),"/") && Slen(dn)>1)
    dn=Sleft(dn,Slen(dn)-1);
  List<String> fs=getfilesindir(dn);
  for (int i=1;i<=fs.len;i++) {
//    printf("%s %s\n",fs.num(i),format);
    if (Smatches(fs.num(i),format)) {
      if (Slen(dn)>0 && !Seq(dn,".") && !Seq(dn,"/"))
        fs.setnum(i,Sconc(dn,"/",fs.num(i)));
    } else {
      fs.removenum(i);
      i--;
    }
  }
  if (JFilesRecursive) {
  for (int i=1;i<=fs.len;i++) {
    String fn=fs.num(i);
    if (Seq(Sright(fn,1),"/"))
      fs.add(getfiles(fn,format));
  }
  }
//  printf("!%i\n",fs.len);
  return fs;
}

List<String> getfiles(String p) {
  if (Seq(Sright(p,1),"/"))
    p=Sleft(p,Slen(p)-1);
  int i=max(Sinstrlast(p,"/"),Sinstrlast(p,"\\"));
  if (i==0)
    return getfiles(".",p);
  else {
    String f=Sfrom(p,i+1);
    String d=Sleft(p,i-1);
    return getfiles(d,f);
  }
}
#endif

int getfilesize(String fname) {
  if (Seq(Sright(fname,1),"/") && Slen(fname)>1)
    fname=Sleft(fname,Slen(fname)-1);
  struct stat s;
  if (stat(fname,&s)==0)
    if (S_ISDIR(s.st_mode)) {
      int sz=0;
      List<String> ls=getfilesindir(fname);
      for (int i=1;i<=ls.len;i++)
        sz+=getfilesize(Sconc(fname,"/",ls.num(i)));
      return sz;
    } else
      return s.st_size;
  else
    return -1;
}

// Starts of class FileRec


  // Variable declared in .h file
  // Variable declared in .h file
   FileRec::FileRec() {
  }
   FileRec::FileRec(String f,int s) {
    fname=Snew(f);
    size=s;
  }
   void FileRec::display() {
    printf("%s  %i\n",fname,size);
  }
   String FileRec::type() {
    return "file";
  }
// End class 


float getsize(FileRec *r) {
  return r->size;
}

// Starts of class DirRec


  // Variable declared in .h file
   DirRec::DirRec() {
  }
   DirRec::DirRec(String dir) {
    fname=Snew(dir);
    if (!Seq(Sright(dir,1),"/"))
      dir=Sconc(dir,"/");
#ifdef DEBUG
    printf("Finding dir %s\n",dir);
#endif
    List<String> ls=getfilesindir(dir);
    int s=0;
    for (int i=1;i<=ls.len;i++) {
#ifdef DEBUG
      printf("%s\n",ls.num(i));
#endif
      String f=ls.num(i);
      if (islink(Sconc(dir,f))) {
        // printf("File %s ignored because it's a link!\n",f);
      } else {
        if (isdir(Sconc(dir,f)) && !Seq(Sright(f,1),"/"))
          f=Sconc(f,"/");
        if (Seq(Sright(f,1),"/")) {
          fs.add(new DirRec(Sconc(dir,f)));
        } else
          fs.add(new FileRec(f,getfilesize(Sconc(dir,f))));
        s=s+fs.num(fs.len)->size;
      }
    }
    size=s;
  }
  void DirRec::display() {
    printf("%s (%i) %i\n",fname,fs.len,size);
    for (int i=1;i<=fs.len;i++)
      fs.num(i)->display();
  }
  void DirRec::displayratio() {
    fs.sortby(&getsize);
    printf("%i files, %i bytes\n",fs.len,size);
    int largest=0;
    for (int i=1;i<=fs.len;i++)
      if (Slen(fs.num(i)->fname)>largest)
        largest=Slen(fs.num(i)->fname);
    largest=largest+3;
    for (int i=1;i<=fs.len;i++) {
      printf("%s%s%.2f\n",fs.num(i)->fname,Srep(" ",largest-Slen(fs.num(i)->fname)),(float)fs.num(i)->size*100.0/(float)size);
//      printf("%i %i\n",fs.num(i)->size,size);
    }
  }
   String DirRec::type() {
    return "directory";
  }
// End class 


#endif

void removefile(String f) {
  if (unlink(f)!=0) {
    error(Sformat("File %s NOT removed!\n",f));
  }
/*  #ifdef POSIX
    String s=Sformat("rm %s",f);
//    printf(">>> %s\n",s);
    system(s);
  #endif */
}

String wholepath() {
  int size = 100;
  char *buffer = (char *) malloc (size);
     
  while (1) {
           char *value = getcwd (buffer, size);
           if (value != 0)
             return buffer;
           size *= 2;
           free (buffer);
           buffer = (char *) malloc (size);
  }
}

List<char> *readbinaryfile(String fname) {
  List<char> *l=new List<char>();
  FILE *fp=fopen(fname,"rb");
  if (fp==NULL)
    error("Problem reading %s\n",fname);
  int c;
  while ((c=getc(fp))!=EOF) {
      l->add((int)c);
  }
  fclose(fp);
      #ifdef DEBUG
      printf("File %s read, %i bytes\n",fname,l->len);
      #endif
  return l;
}

void writebinaryfile(String fname,List<char> *c) {
  FILE *fp=fopen(fname,"wb");
  for (int i=1;i<=c->len;i++)
    fputc(c->num(i),fp);
  fclose(fp);
}

void deletefile(String fname) {
  #ifdef DOS
    system(Sconc("del ",fname));
  #else
    system(Sconc("rm ",fname));
  #endif
}

void copyfile(String fname,String tname) {
  #ifdef DOS
    system(Sconc("copy ",fname," ",tname));
  #else
    system(Sconc("cp ",fname," ",tname));
  #endif
}

/* Usage:
bool zipped=unzipfile(fname);
...
if (zipped)
  zipfile(fname);
*/

bool unzipfile(String s) {
  if (Seq(".gz",Sright(s,3))) {
    system(Sformat("gunzip %s",s));
    *s=*Sleft(s,Slen(s)-3);
    return true;
  } else
    return false;
}

void zipfile(String s) {
  system(Sformat("gzip %s",s));
}

bool fileexists(String fname) {
  struct stat s;
  if (stat(fname,&s)==-1) {
    #ifndef QUIET
      printf("Problem analysing %s\n",fname);
    #endif
    return false;
  } else {
    return true;
  }
/*  FILE *f=fopen(s,"r");
  if (f==NULL)
    return false;
  fclose(f);
  return true;*/
}

/*
List<String> filesmatching(String s) {
  String dir,pat;
  if (Sinstrlast(s,"/")>0) {
    dir=Sbeforelast(s,"/");
    pat=Safterlast(s,"/");
  } else {
    dir=".";
    pat=s;
  }
  DirRec dr=DirRec(dir);

*/

void writeinttofile(String fname, int i) {
  List<String> ls;
  ls.add(Sfromint(i));
  writelinestofile(ls,fname);
  ls.freedom();
}

int readintfromfile(String fname) {
  List<String> ls=readlinesfromfile(fname);
  int i=Stoint(ls.num(1));
  ls.freedom();
  return i;
}

void writefloattofile(String fname, float i) {
  List<String> ls;
  ls.add(Sfromfloat(i));
  writelinestofile(ls,fname);
  ls.freedom();
}

float readfloatfromfile(String fname) {
  List<String> ls=readlinesfromfile(fname);
  float i=Stofloat(ls.num(1));
  ls.freedom();
  return i;
}

#endif
