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

  List<String> ls=readlinesfromfile(fname);
  int count=0;
  for (int i=1;i<=ls.len;i++) {
    String l=ls.num(i);
    int j;
    if ((j=Sinstr(l,"<blockquote>"))>0) {
      printf("Found blockquote line %i (%i): %s\n",i,j,l);
      int k=i-1;
      int m;
      String n;
      do {
        n=ls.num(k);
        m=max(Sinstrlast(ls.num(k),"<br>"),Sinstrlast(ls.num(k),"</p>"));
        printf("m=%i for line %s\n",m,n);
        if (m==0)
          k--;
      } while (m==0);
      printf("Found </P> or <BR> line %i (%i): %s\n",k,m,n);
      // Assume success, proceed with insertion
      count++;
      String click=Sformat("einaus%i",count);
      String block=Sformat("einaus%iblock",count);
      // Prepend <p>... with begin span
      String s1=Sconc("<span id=\"",click,"\" class=\"Outline\" style=\"cursor:hand;\">");
//      n=Sinsert(n,m,s1);
      ls.replace(k,Sconc(s1,n,"</span>"));
      // Prepend blockquote
      String s2=Sconc("<span id=\"",block,"\" style=\"display:None\">");
      ls.replace(i,Sinsert(l,j,s2));
      int o=i;
      for (;Sinstr(ls.num(o),"</blockquote>")==0;o++) { }
      printf("Found end of blockquote line %i: %s\n",o,ls.num(o));
      ls.replace(o,Sconc(ls.num(o),"</span>"));
      i=o;
    }
  }
  int i;
  for (i=1;Sinstr(ls.num(i),"</body>")==0;i++) { }
  ls.insert(i,readlinesfromfile("einausinsert.html"));
  writelinestofile("einaus.html",ls);
  printf("Found %i blocks\n",count);

}
