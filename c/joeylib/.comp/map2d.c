#ifndef map2d_C
  #define map2d_C

  #include <map2d.h>

// Class declaration of existence: RGBmp


// Class declaration of existence: Region


// Class declaration of existence: Map2d


//static RGBmp *RGBmp::fromv3ds(Map2d<V3d>);

/* Skipping declared method template <class TObject>
void anguswritebmp8(String fn_image,Map2d<TObject> *map,float low,float scale) */


// Class declaration of existence: SubMap2d


/* Skipping declared method template <class Object>
SubMap2d<Object> *shortcutSubMap2d(int,int,int,int,Map2d<Object> *) */


// Template class must appear in .h file


//#ifdef DOS
  
  // Template class must appear in .h file

  
  template <class Object>
  SubMap2d<Object> *shortcutSubMap2d(int a,int b,int c,int d,Map2d<Object> *e) {
    return new SubMap2d<Object>(a,b,c,d,e);
  }

//#endif

// Starts of class MapRGB

// End class 


template<class Object>
Map2d<bool> Map1d<Object>::draw() {
  int l=largest();
  if (l<=0)
    l=123;
  Map2d<bool> b=Map2d<bool>(width,l,true);
  for (int i=0;i<width;i++)
    b.line(i,b.height,i,b.height-getpos(i),false);
  return b;
}

Map2d<bool> drawCorrelator(Correlator cc) {
  int left=cc.xs.min()-5;
  int right=cc.xs.max()+5;
  int top=cc.ys.min()-5;
  int bottom=cc.ys.max()+5;
  if (bottom<0)
    bottom=0;
  if (top>0)
    top=0;
  int wid=right-left;
  int hei=bottom-top;
  float total=cc.totalweight();
  int mw = ( wid>hei ? 400 : 400*wid/hei );
  int mh = ( wid>hei ? 400*hei/wid : 400 );
  Map2d<bool> m=Map2d<bool>(mw,mh,false);
  for (int i=1;i<=cc.xs.len;i++) {
    Pixel p=Pixel(m.width*(cc.xs.num(i)-left)/wid,
                  m.height*(cc.ys.num(i)-top)/hei);
    p.y=m.height-1-p.y; // invert height so y goes up!
    m.cross(p,5*cc.ws.num(i)*cc.xs.len/total,true);
  }
  V2d c=cc.centroid();
  V2d mc=V2d(m.width*(c.x-left)/wid,m.height*(c.y-top)/hei);
  mc.y=m.height-1-mc.y;
  V2d dir=V2d::angle(cc.angle())*200;
  dir.y=-dir.y;
  m.line(mc-dir,mc+dir,true);
  return m;
}


#endif
