/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

bool contains(uchar x,uchar y) {
  return (x & y)==y;
}

class Time {
public:
  int hour,minute,second;
  Time() { }
  Time(int h,int m,int s) {
    hour=h;
    minute=m;
    second=s;
  }
  String toString() {
    return Sformat("%2i:%2i:%2i",hour,minute,second);
  }
};

class Date {
public:
  int day,month,year;
  Date() { }
  Date(int d,int m,int y) {
    day=d;
    month=m;
    year=y;
  }
  String toString() {
    return Sformat("%2i/%2i/%4i",day,month,year);
  }
};

int FImaxname=0;
class FileInfo {
public:
  String dir;
  String name;
  int size;
  bool directory,readonly,archived,system,hidden;
  Date date;
  Time time;
  FileInfo() { }
  FileInfo(String n,int sz,bool d,bool r,bool a,bool s,bool h,Date dt,Time tm) {
    dir=Snew("");
    name=Snew(n);
    if (Slen(name)>FImaxname)
      FImaxname=Slen(name);
    size=sz;
    directory=d;
    readonly=r;
    archived=a;
    system=s;
    hidden=h;
    date=dt;
    time=tm;
  }
  String toString() {
    String s=Snew("");
    if (directory)
      s=Sconc(s,"D");
    if (readonly)
      s=Sconc(s,"R");
    if (archived)
      s=Sconc(s,"A");
    if (system)
      s=Sconc(s,"S");
    if (hidden)
      s=Sconc(s,"H");
    String format=Sreplacein("%s%Is : %8i (%s) %s %s","I",Sformat("%i",FImaxname));
    return Sformat(format,dir,name,size,s,date.toString(),time.toString());
  }
};

void temp(String s) {
    printf("[%s]:%i\n",s,Slen(s));
    for (int i=0;i<=10;i++) {
      int j=s[i];
      printf("%i,",j);
    }
    printf("2\n");
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  bool tmp=a.argexists("/s");
  String pattern=a.arg(1);
  a.done();

  List<FileInfo> fs;

  struct ffblk f;
  int ffops=FA_HIDDEN | FA_SYSTEM;
  if (tmp)
    ffops=ffops | FA_DIREC;
  bool done = findfirst(pattern, &f, ffops);
  while (!done) {

    FileInfo fi=FileInfo(f.ff_name,f.ff_fsize,contains(f.ff_attrib,FA_DIREC),contains(f.ff_attrib,FA_RDONLY),contains(f.ff_attrib,FA_ARCH),contains(f.ff_attrib,FA_SYSTEM),contains(f.ff_attrib,FA_HIDDEN),
      Date((f.ff_fdate >>  5) & 0x0f,(f.ff_fdate & 0x1f),((f.ff_fdate >> 9) & 0x7f) + 1980),
      Time((f.ff_ftime >> 11) & 0x1f,(f.ff_ftime >>  5) & 0x3f,(f.ff_ftime & 0x1f) * 2));

    fs.add(fi);

    printf("%10u %2u:%02u:%02u %2u/%02u/%4u %20s Atrs: %i\n",
    f.ff_fsize,
    (f.ff_ftime >> 11) & 0x1f,
    (f.ff_ftime >>  5) & 0x3f,
    (f.ff_ftime & 0x1f) * 2,
    (f.ff_fdate >>  5) & 0x0f,
    (f.ff_fdate & 0x1f),
    ((f.ff_fdate >> 9) & 0x7f) + 1980,
    f.ff_name,
    (int)f.ff_attrib);
    done = findnext(&f);
  }

  for (int i=1;i<=fs.len;i++)
    printf("%s\n",fs.num(i).toString());
    

}
