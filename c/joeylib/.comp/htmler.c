#ifndef htmler_C
  #define htmler_C

  #include <htmler.h>

// Starts of class Pos


  // Variable declared in .h file
   Pos::Pos(int a,int b) {
    r=a; c=b;
  }
  Pos Pos::nextrow() {
    return Pos(r+1,1);
  }
// End class 


// Starts of class HTMLer


  // Variable declared in .h file
   HTMLer::HTMLer() {
  }
   HTMLer::HTMLer(List<String> l) {
    ls=l;
  }
   HTMLer::HTMLer(String fname) {
    ls=readlinesfromfile(fname);
  }
  void HTMLer::start(String t) {
    ls.add("<html>");
    ls.add("<head>");
    ls.add(Sformat("<title>%s</title>",t));
    ls.add(Sformat("<meta name=\"generator\" content=\"Created by joeylib HTMLer (www.joeycentral.freeserve.co.uk/joeylib/)\">"));
    ls.add("</head>");
    ls.add("<body>");
  }
  void HTMLer::start() {
    start("no title");
  }
  void HTMLer::add(String s) {
    ls.add(s);
  }
  void HTMLer::write(String f) {
    writelinestofile(ls,f);
  }
  void HTMLer::end() {
    ls.add("</body>");
    ls.add("</html>");
  }
  String HTMLer::title() {
    return findpattern("<title>*</title>");
  }
  String HTMLer::findpattern(String s) {
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
  int HTMLer::findline(String s) {
    for (int i=1;i<=ls.len;i++) {
      if (Sinstr(ls.num(i),s)>0)
        return i;
    }
  }
  void HTMLer::addtitle() {
    ls.add(Sformat("<center><h1>%s</h1></center>",title()));
  }
  Pos HTMLer::find(String s) {
    for (int i=1;i<=ls.len;i++) {
      int j=Sinstr(ls.num(i),s);
      if (j>0)
        return Pos(i,j);
    }
    return Pos(-1,-1);
  }
  Pos HTMLer::find(String s,Pos p) { // find at or after p
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
  Pos HTMLer::findback(String s,Pos p) { // find at or before p
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
// End class 


#endif
