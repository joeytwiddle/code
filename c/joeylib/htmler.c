class Pos {
public:
  int r,c;
  Pos(int a,int b) {
    r=a; c=b;
  }
  Pos nextrow() {
    return Pos(r+1,1);
  }
};

class HTMLer {
public:
  List<String> ls;
  HTMLer() {
  }
  HTMLer(List<String> l) {
    ls=l;
  }
  HTMLer(String fname) {
    ls=readlinesfromfile(fname);
  }
  void start(String t) {
    ls.add("<html>");
    ls.add("<head>");
    ls.add(Sformat("<title>%s</title>",t));
    ls.add(Sformat("<meta name=\"generator\" content=\"Created by joeylib HTMLer (www.joeycentral.freeserve.co.uk/joeylib/)\">"));
    ls.add("</head>");
    ls.add("<body>");
  }
  void start() {
    start("no title");
  }
  void add(String s) {
    ls.add(s);
  }
  void write(String f) {
    writelinestofile(ls,f);
  }
  void end() {
    ls.add("</body>");
    ls.add("</html>");
  }
  String title() {
    return findpattern("<title>*</title>");
  }
  String findpattern(String s) {
    String left=Sbefore(s,"*");
    String right=Safter(s,"*");
    for (int i=1;i<=ls.len;i++) {
      if (Sinstr(ls.num(i),left)>0) {
        String r=Safter(ls.num(i),left);
        return Sbefore(r,right);
      }
    }
    return NULL;
  }
  int findline(String s) {
    for (int i=1;i<=ls.len;i++) {
      if (Sinstr(ls.num(i),s)>0)
        return i;
    }
  }
  void addtitle() {
    ls.add(Sformat("<center><h1>%s</h1></center>",title()));
  }
  Pos find(String s) {
    for (int i=1;i<=ls.len;i++) {
      int j=Sinstr(ls.num(i),s);
      if (j>0)
        return Pos(i,j);
    }
    return Pos(-1,-1);
  }
  Pos find(String s,Pos p) { // find at or after p
    int k=Sinstr(Sfrom(ls.num(p.r),p.c),s);
    if (k>0)
      return Pos(p.r,p.c+k-1);
    for (int i=p.r+1;i<=ls.len;i++) {
      int j=Sinstr(ls.num(i),s);
      if (j>0)
        return Pos(i,j);
    }
    return Pos(-1,-1);
  }
  Pos findback(String s,Pos p) { // find at or before p
    int k=Sinstrlast(Sleft(ls.num(p.r),p.c+Slen(s)-1),s);
    if (k>0)
      return Pos(p.r,k);
    for (int i=p.r-1;i>0;i--) {
      int j=Sinstrlast(ls.num(i),s);
      if (j>0)
        return Pos(i,j);
    }
    return Pos(-1,-1);
  }
};
