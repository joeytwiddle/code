/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  String fname=a.arg("HTML file");
  a.done();

  HTMLer h=HTMLer(fname);
  int cnt=0;
  // Find next block
  Pos p=h.find("~open");
  while (p.r>0 && p.c>0) {
    cnt++;
    Pos g=h.findback(">",p);
    printf("Found > at r%ic%i\n",g.r,g.c);
    String w=h.ls.num(g.r);
    w=Sinsert(w,g.c+1,Sformat("<u><span id=\"click%i\" class=\"Outline\" style=\"cursor:hand;\">",cnt));
    h.ls.replace(g.r,w);
    String s=h.ls.num(p.r);
    s=Sreplaceall(s,"~open",Sformat("</span></u> <span id=\"click%iclosed\">&gt;</span><span id=\"click%iopen\" style=\"display:None\">&lt;</span>",cnt,cnt));
    h.ls.replace(p.r,s);
    // Find start and end of blockquote
    Pos q=h.find("<blockquote>",p);
    printf("Found <b> at r%ic%i\n",q.r,q.c);
    Pos r=q;
    int indent=1;
    do {
      //printf("Searching...");
      Pos s=h.find("<blockquote>",r.nextrow());
      Pos e=h.find("</blockquote>",r.nextrow());
      //printf("s r%ic%i e r%ic%i\n",s.r,s.c,r.r,r.c);
      if (s.r==-1 && s.c==-1 && e.r==-1 && e.c==-1)
        error("Could not find <b> or </b> from row %i\n",r.nextrow());
      if (s.r<e.r && s.r>-1) {
        indent++;
        r=s;
      } else {
        indent--;
        r=e;
      }
    } while (indent>0 || (Slen(h.ls.num(r.r+1))==0 && Seq(h.ls.num(r.r+2),"<blockquote>"))); 
    printf("Found corresponding </b> at r%ic%i\n",r.r,r.c);
    String t=h.ls.num(q.r);
    t=Sformat("<span id=\"click%iblock\" style=\"display:None;\">%s",cnt,t);
    h.ls.replace(q.r,t);
    String u=h.ls.num(r.r);
    u=Sformat("%s</span>",u);
    h.ls.replace(r.r,u);
    p=h.find("~open",p.nextrow());

    h.write("einaus.html");
  }
  Pos v=h.find("/body");
  h.ls.insert(v.r,readlinesfromfile("einausinsert.html"));
  h.write("einaus.html");

}
