class Seg {
public:
  float (*classifyfn)(List<Pixel> *);
  Map2d<bool> *bin;

  float classify(Region *r) {
    return classifyfn(r->getlist());
  }
  float classify(List<Pixel> *l) {
    return classifyfn(l);
  }
  Seg() { }
  Seg(float (*c)(List<Pixel> *)) {
    classifyfn=c;
  }
  Map2d<bool> *classifytopdown() { // Big blocks sometimes wrongly classified
    bin=new Map2d<bool>(image->width,image->height,false);
    topdown(0,0,image->width-1,image->height-1);
    return bin;
  }
  void topdown(int l,int t,int r,int b) {
//printf("a %i %i %i %i\n",l,t,r-l,b-t);
    List<Pixel> vs;
    for (int i=l;i<r;i++)
    for (int j=t;j<b;j++)
      vs.add(newPixel(i,j));
    float c=classify(&vs);
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
    Map2d<uchar> *todo=new Map2d<uchar>(image->width/topres,image->height/topres,tocheck);
    for (int res=topres;res>=botres;res=res/2) {
      Map2d<uchar> *todonext=todo->scaledby(1);
      Map2d<float> *score=new Map2d<float>(todo->width,todo->height);
      for (int i=0;i<todo->width;i++)
      for (int j=0;j<todo->height;j++) {
          List<Pixel> vs;
          for (int x=i*res;x<(i+1)*res;x++)
          for (int y=j*res;y<(j+1)*res;y++)
            vs.add(newPixel(x,y));
          float c=classify(&vs);
//          float c=measurehiststability(i*res,j*res,(i+1)*res-1,(j+1)*res-1);
          score->setpos(i,j,c);
          vs.freedom();
        if (todo->getpos(i,j)==tocheck) {
          if (res>botres) {
          if (c>istext) {
            todonext->setpos(i,j,totext);
            for (int x=-1;x<=1;x++)
            for (int y=-1;y<=1;y++)
              if (x!=0 || y!=0)
                if (todonext->inmap(i+x,j+y))
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
        if (todonext->getpos(i,j)==totext) {
          for (int x=-1;x<=1;x++)
            for (int y=-1;y<=1;y++)
              if (x!=0 || y!=0)
                if (todonext->inmap(i+x,j+y))
                  if (todonext->getpixel(i+x,j+y)!=totext)
                    todonext->setpixel(i+x,j+y,tocheck);
        }
      }
      destroy(todo);
      int f=getnextfilenum();
      todonext->scaledto(image->width/botres,image->height/botres)->writefiletop(Sformat("level%i.bmp",f),2);
      score->scaledto(image->width/botres,image->height/botres)->writefile(Sformat("measure%i.bmp",f));
      destroy(score);
      todo=todonext->scaledby(2);
      destroy(todonext);
    }
    return todo->threshold(0.3);
  }
  Map2d<bool> *classifywindows() {
    Map2d<bool> *b=new Map2d<bool>(image->width,image->height,false);
    Map2d<float> *fi=new Map2d<float>(image->width,image->height,0.0);
    float thresh=( !twooutnodes ? 0.9 : 0.0 );
    int cntwinds=0;
    printf("Scanning %i X %i\n",image->width/windres,image->height/windres);
    for (int i=0;i<=image->width;i+=windres) {
//    putc('.',stdout);
    if (((i/windres) % 50) == 0)
      printf("%i / %i\n",i,image->width);
    for (int j=0;j<=image->height;j+=windres) {
      List<Pixel> *ps=new List<Pixel>(1);
      ps->add(Pixel(i+windres/2,j+windres/2));
      if (image->inmap(ps->num(1))) {
//      printf("List length %i\n",ps->len);
//      float c=measureedgeanglecancelling(ps);
      float c=classify(ps);
      destroy(ps);
      for (int x=i;x<i+windres;x++)
      for (int y=j;y<j+windres;y++) {
        b->setpos(x,y,(c>thresh));
        fi->setpos(x,y,c);
      }
      }
      cntwinds++;
    }
    }
    // b->writefile("windowmeas.bmp");
    printf("Scanned %i windows\n",cntwinds);
    fi->writefile("windowmeas.bmp");
    return b;
  }
      
};

