class VolRend {
public:
  Map2d<float> *rend;
  JBmp *b;
  float crazy;
  float smallest;
  bool (*invol)(V3d);
  int steps,coldifftol,scrwid,scrhei,res;
  float back;
  
  V3d searchline(V3d front,V3d back,bool wanted) {
    V3d t,mid;
    t=(front-back);
    if (t.mod()<smallest)
      return back;
    mid=((front+back)/2.0);
    if (invol(mid)==wanted)
      return searchline(front,mid,wanted);
    else
    return searchline(mid,back,wanted);
  }
  
  float getdepth(int i,int j) {
    if (!rend->inmap(i,j))
      return back;
    if (rend->pos[i][j]==crazy) {
      // Shoot ray into image
      V3d near=V3d((i-scrhei/2)*2.0/scrhei,(j-scrhei/2)*2.0/scrhei,-2);
      V3d far=V3d(near.x*2.0,near.y*2.0,2.0);
      rend->pos[i][j]=back;
      V3d next,last=near;
      bool lastin=(invol(near));
      
      for (int k=1;k<steps;k++) {
        float dk=(float)k/(float)steps;
        next=near*(1.0-dk)+far*dk;
        if (invol(next)!=lastin) {
          rend->pos[i][j]=searchline(last,next,!lastin).z;
          break;
        }
        last=next;
      }
    }
    return rend->pos[i][j];
  }
  
  int getcol(int rx,int ry) {
    int col;
    float d=getdepth(rx,ry);
    if (d==back)
      col=0;
    else {
      float diff=2.0*d-getdepth(rx-1,ry)-getdepth(rx,ry-1);
      col=chop(128.0-diff*4800.0,0,255);
    }
    return col;
  }
  
  void fillsquare(int x,int y,int w,int h) {
    #ifdef ALLEGRO
  if (key[KEY_SPACE])
    exit(0);
    #endif
  int nwcol=getcol(x,y);
    int necol=getcol(x+w,y);
    int swcol=getcol(x,y+h);
    int secol=getcol(x+w,y+h);
    int cx=x+w/2;
    int cy=y+h/2;
    int ave=(nwcol+necol+swcol+secol)/4;
    int m=max(max(diff(nwcol,ave),diff(necol,ave)),max(diff(swcol,ave),diff(secol,ave)));
    if (m<coldifftol || max(w,h)<=res) {
      b->interpolate(x,y,x+w,y+h,nwcol,necol,swcol,secol);
    } else {
      fillsquare(x,y,w/2,h/2);
      fillsquare(cx,y,w/2,h/2);
      fillsquare(x,cy,w/2,h/2);  
      fillsquare(cx,cy,w/2,h/2);
    }
  }
  
  VolRend(int w,int h,bool (*f)(V3d),int cdt,float jump,int s,float acc,float ba,int r) {
  crazy=-12345.6789;
    scrwid=w;
    scrhei=h;
    b=new JBmp(w,h);
    rend=new Map2d<float>(w,h,crazy);
    coldifftol=cdt;
    smallest=acc;
    invol=f;
    steps=s;
    back=ba;
    res=r;
    for (int i=0;i<scrhei;i+=jump) {
      for (int j=0;j<scrhei;j+=jump) {
        fillsquare(i,j,jump,jump);
      }
      b->writetoscreen();
    }
  }
  
};
