#include <joeylib.c>

int countaround(JBmp *jb,int i,int j,int c) {
  int cnt=0;
  for (int x=-1;x<=1;x++)
  for (int y=-1;y<=1;y++)
    if (!(x==0 && y==0))
      if (jb->chopbmp(i+x,j+y)==c)
        cnt++;
  return cnt;
}

void main() {
  int numps=5;
  int scrwid=320;
  int scrhei=200;
  JBmp jbmp=JBmp(scrwid,scrhei);
  randomise();
  for (int i=0;i<scrwid;i++)
  for (int j=0;j<scrhei;j++)
    jbmp.bmp[j][i]=intrnd(0,numps-1);
  allegrosetup(scrwid,scrhei);
  while (!keypressed()) {
    jbmp.writetoscreen();
    JBmp n=JBmp(scrwid,scrhei);
    for (int x=0;x<scrwid;x++)
    for (int y=0;y<scrhei;y++) {
      int cnt[numps];
      int best=0;
      int winner=-1;
      for (int i=0;i<numps;i++) {
        int c=countaround(&jbmp,x,y,i);
        cnt[c]++;
        if (cnt[c]>best) {
          best=cnt[c];
          winner=c;
        }
      }
      n.bmp[y][x]=winner;
    }
    jbmp=n;
  }
}
