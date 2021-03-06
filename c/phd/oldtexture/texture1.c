#include <joeylib.c>

  int botres=5; // botres*2^n=topres must hold for some n (to make check for botres in neighbour classifier work)
  int topres=80;
  float notext=0.1;
  float istext=0.9;

class Seg {
public:
  Map2d<V3d> *map;
  float (*classifyfn)(List<Pixel> *,Map2d<V3d> *);
  Map2d<bool> *bin;

  float classify(Region *r,Map2d<V3d> *m) {
    return classifyfn(r->getlist(),m);
  }
  float classify(List<Pixel> *l,Map2d<V3d> *m) {
    return classifyfn(l,m);
  }
  Seg(Map2d<V3d> *m,float (*c)(List<Pixel> *,Map2d<V3d> *)) {
    map=m;
    classifyfn=c;
  }
  Map2d<bool> *classifytopdown() {
    bin=new Map2d<bool>(map->width,map->height,false);
    topdown(0,0,map->width-1,map->height-1);
    return bin;
  }
  void topdown(int l,int t,int r,int b) {
//printf("a %i %i %i %i\n",l,t,r-l,b-t);
    List<Pixel> vs;
    for (int i=l;i<r;i++)
    for (int j=t;j<b;j++)
      vs.add(newPixel(i,j));
    float c=classify(&vs,map);
    vs.freedom();
    if (r-l>botres || b-t>botres) {
      if (c>istext) {
        bin->setrectangle(l,t,r-l,b-t,depower(r-l,2));
      }
      else if (c>notext) {
        int mx=(l+r)/2;
        int my=(t+b)/2;
        topdown(l,t,mx,my);
        topdown(mx,t,r,my);
        topdown(l,my,mx,b);
        topdown(mx,my,r,b);
      }
    } else if(c>0.5)
      bin->setrectangle(l,t,r-l,b-t,true);
  }
  Map2d<bool> *classifyneighbours() {
    #define toavoid 0
    #define tocheck 1
    #define totext 2
    Map2d<uchar> *todo=new Map2d<uchar>(map->width/topres,map->height/topres,tocheck);
    for (int res=topres;res>=botres;res=res/2) {
      Map2d<uchar> *todonext=todo->scaledby(1);
      for (int i=0;i<todo->width;i++)
      for (int j=0;j<todo->height;j++) {
        if (todo->pos[i][j]==tocheck) {
          List<Pixel> vs;
          for (int x=i*res;x<(i+1)*res;x++)
          for (int y=j*res;y<(j+1)*res;y++)
            vs.add(newPixel(x,y));
          float c=classify(&vs,map);
          vs.freedom();
          if (res>botres) {
          if (c>istext) {
            todonext->pos[i][j]=totext;
            for (int x=-1;x<=1;x++)
            for (int y=-1;y<=1;y++)
              if (x!=0 || y!=0)
                if (todonext->getpixel(i+x,j+y)!=totext)
                  todonext->setpixel(i+x,j+y,tocheck);
          } else if (c>notext)
            todonext->setpixel(i,j,tocheck);
          else
            todonext->setpixel(i,j,toavoid);
          } else {
            if (c>0.5)
              todonext->setpixel(i,j,totext);
          }
        }
        if (todonext->pos[i][j]==totext) {
          for (int x=-1;x<=1;x++)
            for (int y=-1;y<=1;y++)
              if (x!=0 || y!=0)
                if (todonext->getpixel(i+x,j+y)!=totext)
                  todonext->setpixel(i+x,j+y,tocheck);
        }
      }
      destroy(todo);
      todonext->scaledto(map->width/botres,map->height/botres)->writefile(getnextfilename("level","bmp"));
      todo=todonext->scaledby(2);
      destroy(todonext);
    }
    return todo->threshold(0.5);
  }
};

float classifyrnd(List<Pixel> *ps,Map2d<V3d> *m) {
  return myrnd();
}

Map2d<bool> *expected;

float classifytrain(List<Pixel> *ps,Map2d<V3d> *m) {
  int cnt=0;
  for (int i=1;i<=ps->len;i++) {
    Pixel p=ps->num(i);
//    printf("%i %i in %i %i\n",p.x,p.y,expected->width,expected->height);
    if (expected->getpos(p.x,p.y))
      cnt++;
  }
  return (float)cnt/(float)ps->len+floatrnd(-.2,.2);
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  String iname=a.getarg();
  String fname=Sconc("home/joey/images/ ",iname,".bmp");
  String ename=Sconc("home/joey/images/ exp",iname,".bmp");
  a.done();

  randomise();
  
  Map2d<V3d> *map=RGBmp::readfile(fname)->getv3ds();

  expected=Map2d<float>::readfile(ename)->scaleto(map->width,map->height)->threshold(0.5);
  
  printf("Starting segmentation...\n");
  Seg seg=Seg(map,&classifytrain);
  Map2d<bool> *b=seg.classifyneighbours();
  b->scaleto(expected->width,expected->height)->writefile("textseg.bmp");
  
}
