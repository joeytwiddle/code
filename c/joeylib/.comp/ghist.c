#ifndef ghist_C
  #define ghist_C

  #include <ghist.h>

// Starts of class GHist


  // Variable declared in .h file
  // Variable declared in .h file
   GHist::GHist() {
  }
   GHist::GHist(int s) {
    m=Map1d<int>(s,0);
    // printf("done");
  }
  void GHist::addocc(float f) {
    int i=(m.width-1)*chop(f,0,1);
    // printf("Trying to add to %i\n",i);
    // printf("at %i",m.pos[i]);
    m.pos[i]++;
    total++;
  }
  int GHist::difference(GHist *o) {
    int sum=0;
    for (int i=0;i<m.width;i++)
    sum+=mysquare(m.pos[i]-o->m.pos[i]);
    return sum;
  }
  float GHist::variance() { // Histogram variance
    float ave=0;
    float cnt=0;
    int i;
    for (i=0;i<m.width;i++) {
      ave+=m.pos[i];
      cnt+=m.pos[i];
    }
    ave=ave/cnt;
    float sum=0;
    for (i=0;i<m.width;i++)
    sum+=mysquare((float)m.pos[i]-ave);
    return sum/(float)m.width/(float)cnt;
  }
  float GHist::glvariance() { // Greylevel variance
    float ave=0;
    float cnt=0;
    int i;
    for (i=0;i<m.width;i++) {
      ave+=m.pos[i]*(float)i;
      cnt+=m.pos[i];
    }
    ave=ave/cnt;
    float sum=0;
    for (i=0;i<m.width;i++)
    sum+=m.pos[i]*mysquare((float)i-ave);
    return sum/(float)m.width/(float)cnt;
  }
  void GHist::smooth(int r) {
    GHist tmp=GHist(m.width);
    int i;
    for (i=0;i<m.width;i++) {
      int s=0;
      int c=0;
      for (int j=-r;j<=r;j++) {
        int di=i+j;
        if (di>=0 && di<m.width) {
          s+=m.pos[di];
          c++;
        }
      }
      tmp.m.pos[i]=(float)s/(float)c;
    }
    for (i=0;i<m.width;i++)
    m.pos[i]=tmp.m.pos[i];
  }
  void GHist::freedom() {
    m.freedom();
  }
// End class 


float tofloat(GHist g) {
  return g.variance();
}

float tofloat(GHist *g) {
  return g->variance();
}
#endif
