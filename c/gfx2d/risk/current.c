#include <joeylib.c>

void main() {
  int numps=5;
  int scrwid=320;
  int scrhei=200;
  JBmp jbmp=JBmp(scrwid,scrhei);
  randomise();
  for (int i=0;i<scrwid;i++)
  for (int j=0;j<scrhei;j++) {
    jbmp.bmp[j][i]=intrnd(0,numps-1);
//    printf("%i,",jbmp.bmp[j][i]);
  }
  allegrosetup(scrwid,scrhei);
  while (!keypressed()) {
    jbmp.writetoscreen();
    JBmp n=JBmp(scrwid,scrhei);
    for (int x=0;x<scrwid;x++)
    for (int y=0;y<scrhei;y++) {
      int cnt[numps];
      for (int i=0;i<numps;i++)
        cnt[i]=0;
      for (int i=-1;i<=1;i++)
      for (int j=-1;j<=1;j++)
        if (!(i==0 && j==0)) {
//          if (jbmp.inbmp(x+i,y+j)) {
//            int n=jbmp.bmp[y+j][x+i];
          int n=jbmp.chopbmp(i+x,j+y);
          if (n>=0 && n<numps)
            cnt[n]=cnt[n]+1;
          else
            myerror(Sformat("%i out of colour bounds\n",n));
//          }
        }
      int winner=-1;
      int c=-1;
      for (int i=0;i<numps;i++)
        if (cnt[i]>c) {
          winner=i;
          c=cnt[i];
        }
      if (winner>-1)
        n.bmp[y][x]=winner;
      else
        myerror("nw");
    }
    for (int x=0;x<scrwid;x++)
    for (int y=0;y<scrhei;y++)
      jbmp.bmp[y][x]=n.bmp[y][x];
//    jbmp=n;
  }
}
