/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#define DEBUG

#include <joeylib.c>

#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

bool contains(uchar x,uchar y) {
  return (x & y)==y;
}

class Time {
public:
  int hour,minute,second;
  Time() { 
  }
  Time(int h,int m,int s) {
    hour=h;
    minute=m;
    second=s;
  }
  String toString() {
    return Sformat("%02i:%02i:%02i",hour,minute,second);
  }
};

class Date {
public:
  int day,month,year;
  Date() { 
  }
  Date(int d,int m,int y) {
    day=d;
    month=m;
    year=y;
  }
  String toString() {
    return Sformat("%02i/%02i/%04i",day,month,year);
  }
};

int FImaxname=0;
int FImaxsize=0;
class FileInfo {
public:
  String dir;
  String name;
  int size;
  bool directory,readonly,archived,system,hidden,link;
  Date date;
  Time time;
  List<FileInfo> ls;
  
  FileInfo() { 
  }
  
  FileInfo(String dr,String n,int sz,bool d,bool r,bool a,bool s,bool h,bool l,Date dt,Time tm) {
    dir=Snew(dr);
    name=Snew(n);
    String tmp=Sconc(tail(dir),name);
    int i=Slen(tmp);
    if (i>FImaxname)
      FImaxname=i;
    size=sz;
    int j=log(sz)/log(10)+1;
    if (j>FImaxsize)
      FImaxsize=j;
    directory=d;
    readonly=r;
    archived=a;
    system=s;
    hidden=h;
    date=dt;
    time=tm;
  }
  
  FileInfo(String dr,String n) {
    dir=Snew(dr);
    name=Snew(n);
    directory=false;
    readonly=false;
    archived=false;
    system=false;
    hidden=false;
  }
   
  void dothing(String *s,bool t,String u) {
    if (t)
      *s=Sconc(*s,u);
    else
    *s=Sconc(*s,Srepeat(" ",Slen(u)));
  }
  
  String toString() {
    String s=Snew("");
    dothing(&s,directory,"D");
#ifdef DOS
    dothing(&s,readonly,"R");
    dothing(&s,archived,"A");
    dothing(&s,system,"S");
    dothing(&s,hidden,"H");
#endif
#ifdef LINUX
    dothing(&s,link,"L");
#endif
    String format="%s %Bi %s %s %s";
    // format=Sreplacein(format,"A",Sformat("%i",FImaxname));
    format=Sreplacein(format,"B",Sformat("%i",FImaxsize));
    String f=tail(dir);
    f=Spadright(f,FImaxname-Slen(name),( directory ? "-" : " " ));
    f=Sconc(f,name);
    String whole=Sformat(format,f,size,date.toString(),time.toString(),s);
    return Sleft(whole,79);
  }
  
  static String tail(String dir) {
    if (!Seq(Sright(dir,1),"\\") && !Seq(Sright(dir,1),"/") && Slen(dir)>0)
      return Sconc(dir,"/");
    else
      return dir;
  }
  
  static List<FileInfo> getfiles(String dir,String pattern,bool subdirs) {

    printf("Directory %s\n",dir);

    List<FileInfo> fs;
    String realpatten=Sconc(tail(dir),pattern);

#ifdef LINUX

    List<String> ls=getfilesindir(dir);
    for (int i=1;i<=ls.len;i++) {
      String fname=ls.num(i);
      printf("Analysing %s\n",fname);
      struct stat s;
      if (stat(fname,&s)==-1) {
        printf("Problem analysing file %s\n",fname);
      } else {
        FileInfo fi=FileInfo(dir,fname);
        fi.size=s.st_size;
        fi.directory=S_ISDIR(s.st_mode);
        fi.link=S_ISLNK(s.st_mode);
        tm tm=*localtime(s.st_atime);
        fi.date=Date(tm.tm_mday,tm.tm_mon,tm.tm_year);
        fi.time=Time(tm.tm_hour,tm.tm_min,tm.tm_sec);
        fs.add(fi);
      }
    }
     
#endif

#ifdef DOS
    
    struct ffblk f;
    int ffops=FA_HIDDEN | FA_SYSTEM | FA_DIREC;
    
    bool done = findfirst(realpattern,&f,ffops);
    while (!done) {

      FileInfo fi=FileInfo(dir,f.ff_name,f.ff_fsize,
         contains(f.ff_attrib,FA_DIREC),contains(f.ff_attrib,FA_RDONLY),contains(f.ff_attrib,FA_ARCH),contains(f.ff_attrib,FA_SYSTEM),contains(f.ff_attrib,FA_HIDDEN),false,
      Date((f.ff_fdate & 0x1f),(f.ff_fdate >>  5) & 0x0f,((f.ff_fdate >> 9) & 0x7f) + 1980),
      Time((f.ff_ftime >> 11) & 0x1f,(f.ff_ftime >>  5) & 0x3f,(f.ff_ftime & 0x1f) * 2));
      
      
      
      if (!Seq(fi.name,".") && !Seq(fi.name,"..")) {
        
        fs.add(fi);
        
        if (fi.directory && subdirs)
          fs.add(getfiles(Sconc(tail(dir),fi.name),pattern,subdirs));
        
      }
      
      /* printf("%10u %2u:%02u:%02u %2u/%02u/%4u %20s Atrs: %i\n",
         f.ff_fsize,
         (f.ff_ftime >> 11) & 0x1f,
         (f.ff_ftime >>  5) & 0x3f,
         (f.ff_ftime & 0x1f) * 2,
         (f.ff_fdate >>  5) & 0x0f,
         (f.ff_fdate & 0x1f),
         ((f.ff_fdate >> 9) & 0x7f) + 1980,
         f.ff_name,
         (int)f.ff_attrib);*/
      
      done = findnext(&f);
    }
    
#endif    

    return fs;
  }
  
  static List<FileInfo> getfiletree(String dir,String pattern,bool subdirs) {
    
#ifdef DOS

    struct ffblk f;
    int ffops=FA_HIDDEN | FA_SYSTEM | FA_DIREC;
    
    List<FileInfo> fs;
    String realpattern=Sconc(tail(dir),pattern);
    // printf("Searching %s\n",realpattern);
    bool done = findfirst(realpattern,&f,ffops);
    while (!done) {
      
      FileInfo fi=FileInfo(dir,f.ff_name,f.ff_fsize,contains(f.ff_attrib,FA_DIREC),contains(f.ff_attrib,FA_RDONLY),contains(f.ff_attrib,FA_ARCH),contains(f.ff_attrib,FA_SYSTEM),contains(f.ff_attrib,FA_HIDDEN),
      Date((f.ff_fdate & 0x1f),(f.ff_fdate >>  5) & 0x0f,((f.ff_fdate >> 9) & 0x7f) + 1980),
      Time((f.ff_ftime >> 11) & 0x1f,(f.ff_ftime >>  5) & 0x3f,(f.ff_ftime & 0x1f) * 2));
      
      if (!Seq(fi.name,".") && !Seq(fi.name,"..")) {
        
        if (fi.directory && subdirs)
          fi.ls=getfiletree(Sconc(tail(dir),fi.name),pattern,subdirs);
        
        fs.add(fi);
        
      }
      
      /* printf("%10u %2u:%02u:%02u %2u/%02u/%4u %20s Atrs: %i\n",
         f.ff_fsize,
         (f.ff_ftime >> 11) & 0x1f,
         (f.ff_ftime >>  5) & 0x3f,
         (f.ff_ftime & 0x1f) * 2,
         (f.ff_fdate >>  5) & 0x0f,
         (f.ff_fdate & 0x1f),
         ((f.ff_fdate >> 9) & 0x7f) + 1980,
         f.ff_name,
         (int)f.ff_attrib);*/
      
      done = findnext(&f);
    }
    
    return fs;
    
    #endif
    
  }

};

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  bool tmp=a.argexists("/s");
  String pattern=a.argor(".");
  a.done();
  
  List<FileInfo> fs=FileInfo::getfiles(getdir(pattern),getfile(pattern),true);
  
  for (int i=1;i<=fs.len;i++)
  printf("%s\n",fs.num(i).toString());
  
}