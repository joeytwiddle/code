 class RGBmp;

 class Region;

template <class Object>
 class Map2d;

//static RGBmp *RGBmp::fromv3ds(Map2d<V3d>);

template <class TObject>
void anguswritebmp8(String fn_image,Map2d<TObject> *map,float low,float scale); // Method


template <class Object>
 class SubMap2d;

template <class Object>
SubMap2d<Object> *shortcutSubMap2d(int,int,int,int,Map2d<Object> *); // Method


// Template class appears in original format:
template <class Object>
class Map2d : public Writeable {
public:
//  int width,height;
  Object **pos;
  
  Map2d() {
    #ifdef DEBUG
    printf("Empty map created.\n");
    #endif
  }
  Map2d(int x,int y,Object o) {
    width=x;
    height=y;
    pos=new (Object *)[width];
    for (int i=0;i<width;i++) {
      pos[i]=new (Object)[height];
      for (int j=0;j<height;j++)
      pos[i][j]=o;
    }
  }
  Map2d(int x,int y,Object (*fn)()) {
    width=x;
    height=y;
    pos=new (Object *)[width];
    for (int i=0;i<width;i++) {
      pos[i]=new (Object)[height];
      for (int j=0;j<height;j++)
      pos[i][j]=fn();
    }
  }
  Map2d(int x,int y,Object (*fn)(int,int)) {
    width=x;
    height=y;
    pos=new (Object *)[width];
    for (int i=0;i<width;i++) {
      pos[i]=new (Object)[height];
      for (int j=0;j<height;j++)
      pos[i][j]=fn(i,j);
    }
  }
  Map2d(int x,int y) {
    width=x;
    height=y;
    pos=new (Object *)[width];
    for (int i=0;i<width;i++) {
      pos[i]=new (Object)[height];
      // Clear
//      for (int j=0;j<height;j++)
//        pos[i][j]=0;
    }
  }
  void houghlinethroughwithends(int x,int y,int acc,Map2d<int> *left,Map2d<int> *right) {
    for (int a=0;a<height;a++) {
      float cosa=cos(2.0*pi*a/(float)height);
      float sina=sin(2.0*pi*a/(float)height);
      int r=(((float)x*cosa+(float)y*sina));
      if (r>=0 && r<width) {
        pos[r][a]+=acc;
        if (x<left->pos[r][a])
          left->pos[r][a]=x;
        if (y>right->pos[r][a])
          right->pos[r][a]=x; // y? what's this all about
      }
    }
  }
  void houghlinethrough(int x,int y,int acc,float scale,float oscale) {
    for (int a=0;a<height;a++) {
      // ((x,y)-r(cosa,sina)).(cosa,sina)=0
      // xcosa+ysina-rcos2a-rsin2a=0
      // r=(xcosa+ysina)/(cos2a+sin2a)
      float cosa=cos(2*pi*a/(float)height);
      float sina=sin(2*pi*a/(float)height);
      // float cos2a=cosa*cosa;
      // float sin2a=sina*sina;
      int r=width*scale*((float)x*cosa+(float)y*sina);
      r=r+width/2;
      // printf("%i %i\n",a,r);
      if (r>=0 && r<width)
        pos[r][a]+=acc;
    }
  }

  void houghlinethrough(Pixel p,Map2d<bool> *src) {
    int dx=p.x-src->width/2;
    int dy=p.y-src->height/2;
    int max=pythag(src->width,src->height);
    for (int a=0;a<height;a++) {
      float aa=2.0*pi*a/(float)height;
      float r=V2d::dot(V2d::angle(aa),V2d(dx,dy));
      int rr=r*width/max;
      if (rr>=0 && rr<width) {
//        Line2d l=houghlinefor(a,rr,src);
//        l.clipbyrectangle(0,0,src->width-1,src->height-1);
        int acc=1;
//  printf("ACC %i/%i %i/%i -> %i' %i\n",a,height,rr,width,(int)(aa*360/2/pi),(int)r);
        pos[rr][a]+=acc;
      }
    }
  }

  Line2d houghlinefor(int a,int r,Map2d<bool> *src) {
    int max=pythag(src->width,src->height);
    float aa=(float)a*2.0*pi/(float)height;
    float rr=(float)r*(float)max/(float)width;
  printf("%i/%i %i/%i -> %i' %i\n",a,height,r,width,(int)(aa*360/2/pi),(int)rr);
    V2d start=V2d(src->width/2,src->height/2);
    start=start+(V2d::angle(aa)*rr);
//    return Line2d(start,V2d(src->width/2,src->height/2));
    V2d move=V2d::angle(aa+pi/2.0)*max/8.0;
    return Line2d(start-move,start+move);
  }

  float largest() {
    float tmp,high=tofloat(pos[0][0]);
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++) {
      tmp=tofloat(pos[i][j]);
      if (tmp>high)
        high=tmp;
    }
    return high;
  }
  float smallest(List<V2d> *vs) {
    float tmp,low=+inf;
    for (int i=1;i<=vs->len;i++) {
      V2d v=vs->num(i);
      int a=v.x;
      int b=v.y;
      if (inmap(a,b)) {
        tmp=pos[a][b];
        if (tmp<low)
          low=tmp;
      }
    }
    return low;
  }
  float largest(List<V2d> *vs) {
    float tmp,high=-inf;
    for (int i=1;i<=vs->len;i++) {
      V2d v=vs->num(i);
      int a=v.x;
      int b=v.y;
      if (inmap(a,b)) {
        tmp=pos[a][b];
        if (tmp>high)
          high=tmp;
      }
    }
    return high;
  }
  
  float smallest() {
    float tmp,low=tofloat(pos[0][0]);
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++) {
      tmp=tofloat(pos[i][j]);
      if (tmp<low)
        low=tmp;
    }
    return low;
  }
  
  bool maximumat(int i,int j,int rad) {
    if (i-rad<0 || i+rad>=width || j-rad<0 || j+rad>=height)
      return false;
    int tmp=pos[i][j];
    if (tmp<2)
      return false;
    for (int x=-rad;x<=rad;x++)
    for (int y=-rad;y<=rad;y++)
    if (x!=0 && y!=0)
      if (pos[i+x][j+y]>tmp)
      return false;
    return true;
  }
  
  bool inmap(int x, int y) {
    
    if ((x>=0) && (x<width) && (y>=0) && (y<height))
      return true;
    else
    return false;
  }
  
  bool inmap(Pixel p) {
    return inmap(p.x,p.y);
  }
  
  
  Map2d<float> *smoothed(int rad) {
    Map2d<float> *s=new Map2d<float>(width,height);
    List<Pixel> ps=pixelsincircle(0,0,rad);
    printf("Smoothing (%i)\n",s->width);
    for (int i=0;i<s->width;i++) {
      printf(".");
      for (int j=0;j<s->height;j++) {
        int c=0;
        Object o=0;
        for (int k=1;k<=ps.len;k++) {
          int dx=i+ps.p2num(k)->x;
          int dy=j+ps.p2num(k)->y;
          if (inmap(dx,dy)) {
            c++;
            o=o+pos[dx][dy];
          }
        }
        s->pos[i][j]=(float)o/c;
      }
    }
    printf("\n");
    return s;
  }
  
  
  void display() {
    
    float colscale,low,high,tmp;
    
    // Find the lowest and highest floats
    low=smallest();
    high=largest();
    
    colscale=255.0/(high-low);
    
    int scrwid,scrhei;
    int mode[3][2]={{320,200},{640,480},{1024,768}};
      int i=0;
      bool found=false;
      do {
        scrwid=mode[i][0];
        scrhei=mode[i][1];
        if (scrwid>=width && scrhei>=height)
          found=true;
        if (i==2)
          found=true;
        if (!found)
          i++;
      } while (!found);

      #ifdef ALLEGRO
      set_color_depth(8);
      allegrosetup(scrwid,scrhei);
      PALETTE pal;
      generate_332_palette(pal);
      set_palette(pal);
      makepalette(&greypalette);
      
      // Find aspect
      
      int left,top,wid,hei;
      float scale;
      
      if (width/scrwid>height/scrhei) {
        left=0;
        scale=(float)scrwid/width;
        top=(int)(scrhei/2-height*scale/2);
        wid=scrwid;
        hei=(int)(height*scale);
      } else {
        top=0;
        scale=(float)scrhei/height;
        left=(int)(scrwid/2-width*scale/2);
        wid=(int)(width*scale);
        hei=scrhei;
      }
      
      for (int y=0;y<hei;y++)
      for (int x=0;x<wid;x++) {
        int dx=(int)((float)x/scale);
        int dy=(int)((float)y/scale);
        int r=(tofloat(pos[dx][dy])-low)*colscale;
        int g=r;
        int b=r;
//        putpixel(screen,left+x,top+y,makecol(r,g,b));
        putpixel(screen,left+x,top+y,r);
      }
      
      textout(screen, font, Sformat("%i,%i (%f-%f)",width,height,low,high), 0, 0, 255);
      
      while (!key[KEY_ESC] && !key[KEY_SPACE]) { 
      }
      
      #endif
      
    }
    
    // void writefile(String s) {
      // if (Sright(s,3)=="pgm")
      // writepgm(s);
    // }
    
    void writepgm(String fname,float low,float scale) {
      unsigned char *data,*dp;
      int c,i,j,datasize,level;
      FILE *fp;
      
      if(!(fp=fopen(fname,"wb"))) {
        fprintf(stderr,"writepgm: error opening output file\n");
        exit(0);
      }
      
      fprintf(fp,"P5\n");
      
      fprintf(fp,"%d %d\n",width,height);
      
      fprintf(fp,"%d\n",255);
      
      datasize=width*height;
      
      if(!(data=(unsigned char *)calloc(sizeof(char),datasize))){
        fprintf(stderr,"writepgm: calloc error\n");
        exit(0);
      }
      
      dp=data;
      for(i=0;i<height;i++)
      for(j=0;j<width;j++)
      *dp++=(uchar)(scale*(tofloat(pos[j][i])-low));
      
      if(!fwrite(data,sizeof(unsigned char),datasize,fp)) {
        fprintf(stderr,"writepgm: write error\n");
        exit(0);
      }
      
      free(data);
      
      fclose(fp);
    }
    
    static Map2d<float> *readbmp(String fname) {
      // printf("Reading %s\n",fname);
      return RGBmp::readfile(fname)->getgreyscale();
      /* 
         #ifdef ALLEGRO
         PALETTE p;
         BITMAP *bmp=load_bitmap(fname,p);
         // printf("Loaded\n");
         Map2d<float> *n=new Map2d<float>(bmp->w,bmp->h);
         for (int i=0;i<bmp->w;i++)
         for (int j=0;j<bmp->h;j++)
         n->pos[i][j]=(float)getpixel(bmp,i,j)/255.0;
         // n->pos[i][j]=(float)bmp->line[j][i]/255.0;
         return n;
         #endif
      */
    }
    
    static Map2d<float> *readfile(String s) {
      #ifndef QUIET
        printf("Reading greyscale image %s\n",s);
      #endif
      if (Seq(Sright(s,4),".pgm"))
        return readpgm(s);
      if (Seq(Sright(s,3),"bmp"))
        return readbmp(s);
    }

/*    static Map2d<int> *readfile(String s,int t) {
      Map2d<float> *m=Map2d<float>::readfile(s);
      Map2d<int> *n=new Map2d<int>(m->width,m->height);
      for (int i=0;i<n->width;i++)
      for (int j=0;j<n->height;j++)
        n->setpos(i,j,m->getpos(i,j)*t);
      destroy(m);
      return n;
    }*/

//    template<class NObject>
#ifndef DOS
    static Map2d<Object> *readfile(String s,Object t) {
      Map2d<float> *m=Map2d<float>::readfile(s);
      Map2d<Object> *n=new Map2d<Object>(m->width,m->height);
      for (int i=0;i<n->width;i++)
      for (int j=0;j<n->height;j++)
        n->setpos(i,j,(Object)m->getpos(i,j)*t);
      destroy(m);
      return n;
    }
#endif

    void writefileandscale(String fname) {
      writefile(fname);
      writefloattofile(Sconc(fname,".scale"),largest());
    }

//    template<class NObject>
    static Map2d<Object> *readfileandscale(String fname) {
      Object n=readfloatfromfile(Sconc(fname,".scale"));
      return Map2d<Object>::readfile(fname,n);
    }

    static Map2d<float> *readpgm(String fname) {
      printf("Trying to read pgm %s\n",fname);
      unsigned char *data;
      int i,j,datasize,c;
      int typeno,maxvalue;
      char dumchar;
      FILE *fp;
      int *width,*height;
      
      fprintf(stderr,"readpgm: opening %s\n",fname);
      
      fp=fopen(fname,"rb");
      if(!fp) {
        fprintf(stderr,"readpgm: error opening file\n");
        exit(0);
      }
      
      if(fscanf(fp,"P%d\n",&typeno)!=1) {
        fprintf(stderr,"readpgm: error reading typeno\n");
        exit(0);
      }
      
      fprintf(stderr,"readpgm: ppm type P%d\n",typeno);
      
      while(fscanf(fp,"#%c",&dumchar)) {
        while(fscanf(fp,"%c",&dumchar),dumchar!='\n') printf("%c",dumchar);
        printf("\n");
      }
      
      if(fscanf(fp,"%d %d\n",width,height)!=2) {
        fprintf(stderr,"readpgm: error reading width,height\n");
        exit(0);
      }
      
      if(fscanf(fp,"%d",&maxvalue)!=1) {
        fprintf(stderr,"readpgm: error reading maxvalue\n");
        exit(0);
      }
      /* fix 14.10.97: read newline separately to avoid skipping bytes of
         image data corresponding to form-feeds
      */
      fgetc(fp);
      
      fprintf(stderr,"readpgm: w %d  h %d mv %d\n",*width,*height,maxvalue);
      
      datasize=(*width)*(*height);
      
      Map2d<float> *n=new Map2d<float>(*width,*height);
      
      switch(typeno) {
        
        case 5:        /*  rawbits pgm type */
        
        // p=(unsigned char **)calloc((*height),sizeof(unsigned char *));
        
        data=(unsigned char *)calloc(datasize,sizeof(unsigned char));
        if(!data) {
          fprintf(stderr,"readppm: calloc error 3\n");
          exit(0);
        }
        if(fread(data,sizeof(unsigned char),datasize,fp)!=datasize) {
          fprintf(stderr,"readppm: error reading data\n");
          exit(0);
        }
        
        c=0;
        for(i=0;i<(*height);i++)
        for(j=0;j<(*width);j++)
        n->pos[j][i]=(float)(data[c++]/255.0);
        
        free(data);
        break;
        default:
        fprintf(stderr,"readppm: sorry ppm type not implemented\n");
        exit(0);
      }
      
      fclose(fp);
      
      return n;
    }
    Map2d<bool> *realthreshold(float threshold,Region *r) {
      Map2d<bool> *n=new Map2d<bool>(width,height,false);
      for (int i=r->leftmost();i<=r->rightmost();i++) {
        for (int j=r->topmost();j<=r->bottommost();j++) {
          n->pos[i][j]=(pos[i][j]>threshold);
        }
      }
      return n;
    }
    Map2d<bool> *realthreshold(float threshold) {
      Map2d<bool> *n=new Map2d<bool>(width,height);
      for (int i=0;i<width;i++) {
        for (int j=0;j<height;j++) {
          n->pos[i][j]=(tofloat(pos[i][j])>threshold);
        }
      }
      return n;
    }
    Map2d<bool> *threshold(float t) {
      float threshold=smallest()+(largest()-smallest())*t;
      return realthreshold(threshold);
    }
    void writefiletop(String fname,float top) {
      float colscale=255.0/top;
      writefile(fname,0.0,colscale);
    }
    void writefile(String fname) {
      float low=smallest();
      float high=largest();
      float colscale=255.0/(high-low);
      writefile(fname,low,colscale);
    }
    void writefile(String fname,float low,float colscale) {
      #ifndef QUIET
        printf("Writing greyscale image %s (%3f,%3f).\n",fname,low,255.0/colscale+low);
      #endif
      bool zip=Seq(Sright(fname,3),".gz");
      if (zip)
        fname=Sleft(fname,Slen(fname)-3);
      String ext=Sright(fname,4);
      if (Seq(ext,".pgm"))
        writepgm(fname,low,colscale);
      if (Seq(ext,".bmp"))
        anguswritebmp8(fname,this,low,colscale);
      if (zip)
        zipfile(fname);
    }
    /* printf("Writing map %s\n",fname);
       set_color_depth(8);
       BITMAP *bmp=create_bitmap(width,height);
       RGB *pal=makegreyallegpalette();
       for (int i=0;i<width;i++)
       for (int j=0;j<height;j++) {
         uchar r=ucharchop(colscale*(pos[i][j]-low));
         putpixel(bmp,i,j,r);
       }
       save_bitmap(fname,bmp,pal);*/
    void clear(Object o) {
      for (int i=0;i<width;i++)
      for (int j=0;j<height;j++)
      pos[i][j]=o;
    }
    template <class FObject>

/* Needs fixing around edges:
    Map2d<Object> *filterby(Map2d<FObject> *f,Object outside) {
      Map2d<Object> *n=new Map2d<Object>(width,height);
      // Do central
      printf("Filtering main...\n");
      for (int i=0;i<n->width-f->width+1;i++)
      for (int j=0;j<n->height-f->height+1;j++) {
        Object o=outside;
        for (int p=0;p<f->width;p++)
        for (int q=0;q<f->height;q++)
        o=o+pos[i+p][j+q]*f->pos[p][q];
        n->pos[f->width/2+i][f->height/2+j]=o;
      }
      // Now do edges
      printf("Filtering border...\n");
      Object total=outside;
      for (int i=0;i<f->width;i++)
      for (int j=0;j<f->height;j++)
        total=total+f->getpos(i,j);
      for (int i=0;i<n->width;i++)
      for (int j=0;j<n->height;j++) {
        if (i<f->width/2 || i>width-f->width/2 || j<f->height/2 || j>height-f->height/2) {
          Object o=outside;
          float count=0;
          for (int p=0;p<f->width;p++)
          for (int q=0;q<f->height;q++) {
            if (inmap(i+p-f->width/2,j+q-f->height/2)) {
              o=o+pos[i+p-f->width/2][j+q-f->height/2]*f->pos[p][q];
              count=count+f->pos[p][q];
            }
          }
          n->pos[i][j]=o*total/count;
        }
      }
      return n;
    }
*/

    Map2d<Object> *filterby(const Map2d<FObject> *f,Object outside) {
      Map2d<Object> *n=new Map2d<Object>(width,height);
      for (int i=0;i<n->width-f->width+1;i++)
      for (int j=0;j<n->height-f->height+1;j++) {
        Object o=outside;
        for (int p=0;p<f->width;p++)
        for (int q=0;q<f->height;q++)
        o=o+pos[i+p][j+q]*f->pos[p][q];
        n->pos[f->width/2+i][f->height/2+j]=o;
      }
      return n;
    }

/*    template <class FObject>
    float applyfilter(const Map2d<FObject> *f,int x,int y) {
    }*/

    template <class FObject>
    Map2d<Object> *filterby(Map2d<FObject> *f) {
      return filterby(f,0);
    }
    template <class FObject>
    Map2d<Object> *filterby(const Map2d<FObject> *f) {
      return filterby(f,(Object)0);
    }
    template <class FObject>
    Map2d<Object> *filterbywithdestroy(Map2d<FObject> *f,Object outside) {
      Map2d<Object> *n=new Map2d<Object>(width,height,outside);
      for (int i=0;i<n->width-f->width+1;i++)
      for (int j=0;j<n->height-f->height+1;j++) {
        Object o=outside;
        for (int p=0;p<f->width;p++)
        for (int q=0;q<f->height;q++) {
          #ifdef DEBUG
          printf("A");
          #endif
          Object tmp=pos[i+p][j+q]*f->pos[p][q];
          #ifdef DEBUG
          printf("b");
          #endif
          Object newo=(o+tmp);
          #ifdef DEBUG
          printf("c");
          #endif
          tmp.freedom();
          o.freedom();
          o=newo;
          #ifdef DEBUG
          printf("Z");
          #endif
        }
        n->pos[f->width/2+i][f->height/2+j]=o;
//        o.freedom();
//        printf("%i ",Vcount);
      }
      return n;
    }
    template <class FObject>
    Map2d<Object> *filterbywithdestroy(Map2d<FObject> *f) {
      return filterbywithdestroy(f,0);
    }
    template <class FObject>
    Map2d<float> *filterbywithdestroygetgs(Map2d<FObject> *f,Object outside) {
      Map2d<float> *n=new Map2d<float>(width,height,tofloat(outside));
      for (int i=0;i<n->width-f->width+1;i++)
      for (int j=0;j<n->height-f->height+1;j++) {
        Object o=outside;
        for (int p=0;p<f->width;p++)
        for (int q=0;q<f->height;q++) {
          #ifdef DEBUG
          printf("A");
          #endif
          Object tmp=pos[i+p][j+q]*f->pos[p][q];
          #ifdef DEBUG
          printf("b");
          #endif
          Object newo=(o+&tmp);
          #ifdef DEBUG
          printf("c");
          #endif
          tmp.freedom();
          o.freedom();
          o=newo;
          #ifdef DEBUG
          printf("Z");
          #endif
        }
        n->pos[f->width/2+i][f->height/2+j]=tofloat(o);
        o.freedom();
//        printf("%i ",Vcount);
      }
      return n;
    }
    template <class FObject>
    Map2d<float> *filterbywithdestroygetgs(Map2d<FObject> *f) {
      return filterbywithdestroygetgs(f,0);
    }
    Map2d<Object> *transpose() {
      Map2d<Object> *n=new Map2d<Object>(height,width);
      for (int i=0;i<width;i++)
      for (int j=0;j<height;j++)
      n->pos[j][i]=pos[i][j];
      return n;
    }
    template <class FObject>
    Map2d<FObject> *applyfn(FObject fn(Object)) {
      Map2d<FObject> *n=new Map2d<FObject>(width,height);
      for (int i=0;i<width;i++)
      for (int j=0;j<height;j++)
      n->pos[i][j]=fn(pos[i][j]);
      return n;
    }
    static Map2d<Object> *applyop(Map2d<Object> *a,Object fn(Object,Object),Map2d<Object> *b) {
      Map2d<Object> *n=new Map2d<Object>(max(a->width,b->width),max(a->height,b->height));
      float axs=(float)a->width/(float)n->width;
      float bxs=(float)b->width/(float)n->width;
      float ays=(float)a->height/(float)n->height;
      float bys=(float)b->height/(float)n->height;
      for (int i=0;i<n->width;i++)
      for (int j=0;j<n->height;j++)
      n->pos[i][j]=fn(a->pos[(int)(axs*(float)i)][(int)(ays*(float)j)],b->pos[(int)(bxs*(float)i)][(int)(bys*(float)j)]);
      return n;
    }
    Map2d<Object> *applyop(Object fn(Object,Object),Map2d<Object> *b) {
      return Map2d::applyop(this,fn,b);
    }
    
    void edgedetection(Map2d<float> *filter,Map2d<float> **mag,Map2d<float> **ang) {
      Map2d<Object> *hc=this->filterby(filter);
      Map2d<Object> *vc=this->filterby(filter->transpose());
      Map2d<float> *hcb=hc->getgreyscale();
      Map2d<float> *vcb=vc->getgreyscale();
      *mag=hcb->applyop(&hypotenuse,vcb);
      *ang=hcb->applyop(&myangle,vcb);
      destroy(hc);
      destroy(vc);
      destroy(hcb);
      destroy(vcb);
    }
    
    void edgedetection(Map2d<float> *filter,Map2d<float> **mag,Map2d<float> **ang,Object outside) {
      Map2d<Object> *hc=this->filterby(filter,outside);
      Map2d<Object> *vc=this->filterby(filter->transpose(),outside);
      Map2d<float> *hcb=hc->getgreyscale();
      Map2d<float> *vcb=vc->getgreyscale();
      *mag=hcb->applyop(&hypotenuse,vcb);
      *ang=hcb->applyop(&myangle,vcb);
      destroy(hc);
      destroy(vc);
      destroy(hcb);
      destroy(vcb);
    }
    
    void edgedetectionwithdestroy(Map2d<float> *filter,Map2d<float> **mag,Map2d<float> **ang,Object outside) {
      printf("Filtering...\n");
      Map2d<Object> *hc=this->filterbywithdestroy(filter,outside);
      Map2d<Object> *vc=this->filterbywithdestroy(filter->transpose(),outside);
      printf("Getting greyscale...\n");
      Map2d<float> *hcb=hc->getgreyscale();
      Map2d<float> *vcb=vc->getgreyscale();
      *mag=hcb->applyop(&hypotenuse,vcb);
      *ang=hcb->applyop(&myangle,vcb);
      destroy(hc);
      destroy(vc);
      destroy(hcb);
      destroy(vcb);
    }
    
    void edgedetectionwithdestroygetgs(Map2d<float> *filter,Map2d<float> **mag,Map2d<float> **ang,Object outside) {
      printf("Filtering and getting greyscale...\n");
      Map2d<float> *hcb=this->filterbywithdestroygetgs(filter,outside);
      Map2d<float> *vcb=this->filterbywithdestroygetgs(filter->transpose(),outside);
      *mag=hcb->applyop(&hypotenuse,vcb);
      *ang=hcb->applyop(&myangle,vcb);
      destroy(hcb);
      destroy(vcb);
    }
    
    void vedgedetection(Map2d<float> *filter,Map2d<float> **mag,Map2d<float> **ang) {
      Map2d<float> *hc=this->filterby(filter,V3d::o)->applyfn(&V3d::smod);
      Map2d<float> *vc=this->filterby(filter->transpose(),V3d::o)->applyfn(&V3d::smod);
      *mag=hc->applyop(&hypotenuse,vc);
      *ang=hc->applyop(&myangle,vc);
      destroy(hc);
      destroy(vc);
    }
    
    static Map2d<float> *sobel() {
      Map2d<float> *n=new Map2d<float>(3,3,0.0);
      n->pos[0][0]=n->pos[0][2]=-1;
      n->pos[0][1]=-2;
      n->pos[2][0]=n->pos[2][2]=1;
      n->pos[2][1]=2;
      return n;
    }
    
    static Map2d<float> *thinsobel() {
      Map2d<float> *n=new Map2d<float>(2,3,0.0);
      n->pos[0][0]=n->pos[0][2]=-1;
      n->pos[0][1]=-2;
      n->pos[1][0]=n->pos[1][2]=1;
      n->pos[1][1]=2;
      return n;
    }
    
    static Map2d<float> *diagonal() {
      Map2d<float> *n=new Map2d<float>(2,2,0.0);
      n->pos[0][0]=-1;
      n->pos[1][1]=1;
      return n;
    }
    
    static Map2d<float> *simple() {
      Map2d<float> *simpleh=new Map2d<float>(2,1,0.0);
      simpleh->pos[0][0]=-1;
      simpleh->pos[1][0]=1;
      return simpleh;
    }
    
    static Map2d<float> *unit() {
      Map2d<float> *simpleh=new Map2d<float>(1,1,0.0);
      simpleh->pos[0][0]=1;
      return simpleh;
    }

    static Map2d<float> *gaussian(float r) {
      int rr=r*3;
      Map2d<float> *h=new Map2d<float>(rr,rr,0.0);
      for (int i=0;i<rr;i++)
      for (int j=0;j<rr;j++)
        h->setpos(i,j,::gaussian(V2d(i-rr/2,j-rr/2).mag()/r));
      return h;
    }

    static Map2d<float> *gabor(V2d dir,float size,float freq) {
      int fsize=size*3;
      Map2d<float> *simpleh=new Map2d<float>(fsize,fsize,0.0);
      for (int i=0;i<fsize;i++)
      for (int j=0;j<fsize;j++)
        simpleh->setpos(i,j,::gabor(V2d(i-fsize/2,j-fsize/2),dir,size,freq));
      return simpleh;
    }
    
    Map2d<float> *getdistributionvariance(int angq,int angrad) {
      Map2d<float> *angdist=new Map2d<float>(width,height,0.0);
      for (int x=0;x<width;x++) {
        for (int y=0;y<height;y++) {
          float *angcnt=new float[angq];
          for (int i=0;i<angq;i++)
          angcnt[i]=0;
          // float sum=0;
          for (int i=-angrad;i<=angrad;i++) {
            int h=sqrt(angrad*angrad-i*i);
            for (int j=-h;j<=h;j++) {
              int dx=(x+i);
              int dy=(y+j);
              if (inmap(dx,dy)) {
                int a=angq*(pos[dx][dy]+pi)/(2.0*pi);
                // angcnt[a]+=change->pos[dx][dy];
                // sum+=change->pos[dx][dy];
                angcnt[a]+=1;
              }
            }
          }
          float ave=0;
          // for (int i=0;i<angq;i++)
          // angcnt[i]=angcnt[i]/sum;
          for (int i=0;i<angq;i++)
          ave+=angcnt[i]/(float)angq;
          float var=0;
          for (int i=0;i<angq;i++)
          var+=mysquare(angcnt[i]-ave);
          angdist->pos[x][y]=var;
          // printf("%f %f\n",ave,var);
        }
        printf(">");
      }
      printf("\n");
      return angdist;
    }
    
    Map2d<float> *getdistributionvariancewithweighting(Map2d<float> *change,int angq,int angrad) {
      Map2d<float> *angdist=new Map2d<float>(width,height,0.0);
      for (int x=0;x<width;x++) {
        for (int y=0;y<height;y++) {
          float *angcnt=new float[angq];
          for (int i=0;i<angq;i++)
          angcnt[i]=0;
          float sum=0;
          for (int i=-angrad;i<=angrad;i++) {
            int h=sqrt(angrad*angrad-i*i);
            for (int j=-h;j<=h;j++) {
              int dx=(x+i);
              int dy=(y+j);
              if (inmap(dx,dy)) {
                int a=angq*(pos[dx][dy]+pi)/(2.0*pi);
                angcnt[a]+=change->pos[dx][dy];
                sum+=change->pos[dx][dy];
                // angcnt[a]+=1;
              }
            }
          }
          float ave=0;
          // for (int i=0;i<angq;i++)
          // angcnt[i]=angcnt[i]/sum;
          for (int i=0;i<angq;i++)
          ave+=angcnt[i]/(float)angq;
          float var=0;
          for (int i=0;i<angq;i++)
          var+=mysquare(angcnt[i]-ave);
          angdist->pos[x][y]=var/sum;
          // printf("%f %f\n",ave,var);
          free(angcnt);
        }
        printf(">");
      }
      printf("\n");
      return angdist;
    }
    
    Map2d<float> *getmydistributionvariancewithweighting(Map2d<float> *change,int angrad,float scale) {
      Map2d<float> *angdist=new Map2d<float>(width*scale,height*scale,0.0);
      for (int x=0;x<scale*width;x++) {
        for (int y=0;y<scale*height;y++) {
          V2d p=V2d(0,0);
          float cnt=0;
          for (int i=-angrad;i<=angrad;i++) {
            int h=sqrt(angrad*angrad-i*i);
            for (int j=-h;j<=h;j++) {
              int dx=(x/scale+i);
              int dy=(y/scale+j);
              if (inmap(dx,dy)) {
                float a=pos[dx][dy];
                V2d n=V2d(sin(a),cos(a));
                p=p+change->pos[dx][dy]*n;
                cnt+=change->pos[dx][dy];
              }
            }
          }
          angdist->pos[x][y]=p.mod()/cnt;
        }
        printf(">");
      }
      printf("\n");
      return angdist;
    }
    
    Map2d<bool> *adaptivethreshold(float t) {

      // First, generate a histogram
      float low=smallest();
      float high=largest();
      int hq=256;
      int hist[hq];
      for (int i=0;i<hq;i++)
      hist[i]=0;
      for (int x=0;x<width;x++)
      for (int y=0;y<height;y++) {
        int i=((float)hq-1.0)*((float)pos[x][y]-low)/(high-low);
        hist[i]++;
      }
      int total=width*height;

      // Where is the threshold?
      int cnt=0;
      for (int i=0;i<hq;i++) {
        cnt+=hist[i];
        if (cnt>=t*total)
          return realthreshold(low+((float)i+1.0)*(high-low)/(float)hq);
      }
      return false;
    }

    Map2d<bool> *makeregion(List<V2d> *vs) {
      Map2d<bool> *n=new Map2d<bool>(width,height,false);
      for (int i=1;i<=vs->len;i++) {
        V2d v=vs->num(i);
        int x=v.x;
        int y=v.y;
        if (inmap(x,y)) {
          n->pos[x][y]=true;
        }
      }
      return n;
    }

    float adaptivethresholdnum(float t,List<V2d> *vs) {

      // First, generate a histogram
      float low=smallest(vs);
      float high=largest(vs);
      int hq=256;
      int hist[hq];
      for (int i=0;i<hq;i++)
        hist[i]=0;
      int total=0;
      // printf("Starting adaptive threshold %i\n",vs->len);
      for (int i=1;i<=vs->len;i++) {
        // printf("%i ",i);
        V2d v=vs->num(i);
        int x=v.x;
        int y=v.y;
        if (inmap(x,y)) {
          int j=((float)hq-1.0)*((float)pos[x][y]-low)/(high-low);
          hist[j]++;
          total++;
        } else
          printf("adaptive threshold: (%i,%i) out\n",x,y);
      }
      printf("%i used from %i\n",total,vs->len);

      // Where is the threshold?
      int cnt=0;
      for (int i=0;i<hq;i++) {
        cnt+=hist[i];
        if (cnt>=t*total) {
          return low+((float)i+1.0)*(high-low)/(float)hq;
        }
      }

      return false;
    }

    float adaptivethresholdnum(float t,Region *r) {

      Map2d<bool> *m=r->map;
      
      // First, generate a histogram
      float low=m->smallest();
      float high=m->largest();
      int hq=256;
      int hist[hq];
      for (int i=0;i<hq;i++)
        hist[i]=0;
      int total=0;
      // printf("Starting adaptive threshold %i\n",vs->len);
      for (int x=r->leftmost();x<=r->rightmost();x++)
      for (int y=r->topmost();y<=r->bottommost();y++) {
        if (m->pos[x][y]>0.5) {
          int j=((float)hq-1.0)*((float)pos[x][y]-low)/(high-low);
          hist[j]++;
          total++;
        }
      }
//      printf("%i used from %i\n",total,vs->len);

      // Where is the threshold?
      int cnt=0;
      for (int i=0;i<hq;i++) {
        cnt+=hist[i];
        if (cnt>=t*total) {
          return low+((float)i+1.0)*(high-low)/(float)hq;
        }
      }

      return false;
    }

    Map2d<bool> *adaptivethreshold(float t,List<V2d> *vs) {
      printf("Performing adaptive threshold %f on a region\n",t);
      return realthreshold(adaptivethresholdnum(t,vs));
    }
    Map2d<bool> *adaptivethreshold(float t,Region *r) {
      float n=adaptivethresholdnum(t,r);
      printf("Adaptive threshold %f on region size %i, real t=%f.\n",t,r->list->len,n);
      return realthreshold(n,r);
    }
    
    List<Pixel> *getlist() {
      List<Pixel> *l=new List<Pixel>();
// horrid!      for (int x=leftmost();x<=rightmost();x++)
//      for (int y=topmost();y<=bottommost();y++) {
      for (int x=0;x<width;x++)
      for (int y=0;y<width;y++) {
        if (pos[x][y]>0.5) {
          l->add(Pixel(x,y));
//          printf("%i,%i/",x,y);
        }
      }
      return l;
    }
    
    
    void setpos(int x,int y,Object o) {
      if (inmap(x,y))
        pos[x][y]=o;
//      setpixel(x,y,o);
    }

    void setpixel(int x,int y,Object o) {
//      printf("Setting pixel %i %i %f\n",x,y,o);
      if (inmap(x,y))
        pos[x][y]=o;
    }

    void setpos(Pixel p,Object o) {
      setpos(p.x,p.y,o);
    }
    
    void setpixel(Pixel p,Object o) {
      setpos(p.x,p.y,o);
    }
    
  void setrectangle(int l,int t,int w,int h,Object c) {
    if (w<0) {
      l=l+w; w=-w;
    }
    if (h<0) {
      t=t+h; h=-h;
    }
    for (int i=l;i<l+w;i++)
    for (int j=t;j<t+h;j++)
    setpixel(i,j,c);
  }
  void hliner(int x,int y,int dx,Object c) {
    setrectangle(x,y,dx,1,c);
  }
  void hline(int x,int y,int xb,Object c) {
    hliner(x,y,xb-x,c);
  }
  void vliner(int x,int y,int dy,Object c) {
    setrectangle(x,y,1,dy,c);
  }
  void vline(int x,int y,int yb,Object c) {
    vliner(x,y,yb-y,c);
  }
  void line(Pixel pa,Pixel pb,Object c) {
    line(pa.x,pa.y,pb.x,pb.y,c);
  }
    void line(int x1,int y1,int x2,int y2,Object c) {
      int x,y,tmp;
//      printf("a%i %i \n",x1,x2);
      if (diff(x1,x2)>diff(y1,y2)) {
        if (x1>x2) {
          tmp=x1; x1=x2; x2=tmp;
          tmp=y1; y1=y2; y2=tmp;
        }
        for (x=x1; x<=x2; x++) {
          y=y1+(y2-y1)*(x-x1)/(x2-x1);
          setpixel(x,y,c);
        }
      } else {
        if (diff(y1,y2)==0)
          setpixel(x1,y1,c);
        else {
          if (y1>y2) {
            tmp=x1; x1=x2; x2=tmp;
            tmp=y1; y1=y2; y2=tmp;
          }
          for (y=y1; y<=y2; y++) {
            x=x1+(x2-x1)*(y-y1)/(y2-y1);
            setpixel(x,y,c);
          }
        }
      }
      // printf("Drawing line %i %i %i %i\n",x1,y1,x2,y2);
    }
    void line(V2d a,V2d b,Object c) {
      line(a.x,a.y,b.x,b.y,c);
    }
    void line(Line2d l,Object o) {
      line(l.a,l.b,o);
    }
    
  void cross(V2d cen,float s,Object c) {
    line(cen+V2d(-s,-s),cen+V2d(s,s),c);
    line(cen+V2d(-s,s),cen+V2d(s,-s),c);
  }

  void filltri(V2d a,V2d b,V2d c,Object col) {
    filltri(a.x,a.y,b.x,b.y,c.x,c.y,col);
  }
  void filltri(int xa,int ya,int xb,int yb,int xc,int yc,Object c) {
    if (xa>xb) {
      swapints(&xa,&xb); swapints(&ya,&yb);
    }
    if (xb>xc) {
      swapints(&xb,&xc); swapints(&yb,&yc);
    }
    if (xa>xb) {
      swapints(&xa,&xb); swapints(&ya,&yb);
    }
    int start=xa;
    if (xa<0)
      start=0;
    for (int x=start;x<=xb;x++) {
      float xthruab;
      if (xb==xa)
        xthruab=0;
      else
      xthruab=(float)(x-xa)/(xb-xa);
      float xthruac;
      if (xa==xc)
        xthruac=0;
      else
      xthruac=(float)(x-xa)/(xc-xa);
      int ylow=(int)(ya+xthruab*(float)(yb-ya));
      int yhei=(int)(ya+xthruac*(float)(yc-ya));
      if (ylow>yhei) {
        int tmp=yhei;
        yhei=ylow;
        ylow=tmp;
      }
      vline(x,ylow,yhei,c);
    }
    int end=xc;
    for (int x=xb;x<=end;x++) {
      float xthrubc;
      if (xb==xc)
        xthrubc=0;
      else
      xthrubc=(float)(x-xb)/(xc-xb);
      float xthruac;
      if (xa==xc)
        xthruac=0;
      else
      xthruac=(float)(x-xa)/(xc-xa);
      int ylow=(int)(yb+xthrubc*(float)(yc-yb));
      int yhei=(int)(ya+xthruac*(float)(yc-ya));
      if (ylow>yhei) {
        int tmp=yhei;
        yhei=ylow;
        ylow=tmp;
      }
      // System.out.println(x+","+ylow+" - "+yhei);
      // System.out.println("    "+xthruab+" "+xthruac);
      vline(x,ylow,yhei,c);
    }
  }

  void plotpoly(Polygon2d p,Object o) {
    List<Polygon2d> l=p.splittotris();
    for (int i=1;i<=l.len;i++) {
      List<V2d> vs=l.num(i).vs;
      filltri(vs.num(1),vs.num(2),vs.num(3),o);
    }
  }

    int leftmost() {
      for (int i=0;i<width;i++)
      for (int j=0;j<height;j++)
      if (pos[i][j]>0.5)
        return i;
      return width;
    }
    int rightmost() {
      for (int i=width-1;i>=0;i--)
      for (int j=0;j<height;j++)
      if (pos[i][j]>0.5)
        return i;
      return 0;
    }
    int topmost() {
      for (int j=0;j<height;j++)
      for (int i=0;i<width;i++)
      if (pos[i][j]>0.5)
        return j;
      return height;
    }
    int bottommost() {
      for (int j=height-1;j>=0;j--)
      for (int i=0;i<width;i++)
      if (pos[i][j]>0.5)
        return j;
      return 0;
    }
    Map2d<bool> *crop() {
      int l=leftmost();
      int r=rightmost();
      int t=topmost();
      int b=bottommost();
      /* Map2d<bool> *n=new Map2d<bool>(r-l+1,b-t+1);
         for (int i=0;i<n->width;i++)
         for (int j=0;j<n->height;j++)
         n->pos[i][j]=pos[l+i][t+j];
         return n;*/
      return shortcutSubMap2d<bool>(l,r,r-l+1,b-t+1,this);
//      printf("Map2d::crop - not doing\n");
      return NULL;
    }
    
    bool npixelwithinrad(int i,int j,int r) {
      for (int x=-r;x<=r;x++) {
        int h=sqrt(r*r-x*x);
        for (int y=-h;y<=h;y++) {
          int dx=i+x;
          int dy=j+y;
          if (inmap(dx,dy))
            if (pos[dx][dy]<0.5)
            return false;
        }
      }
      return true;
    }
    
    Map2d<bool> *contract(int r) {
      Map2d<bool> *n=new Map2d<bool>(width,height);
      for (int i=0;i<n->width;i++)
      for (int j=0;j<n->height;j++) {
        if (pos[i][j] && npixelwithinrad(i,j,r))
          n->pos[i][j]=true;
        else
          n->pos[i][j]=false;
      }
      return n;
    }
    
    bool pixelwithinrad(int i,int j,int r) {
      for (int x=-r;x<=r;x++) {
        int h=sqrt(r*r-x*x);
        for (int y=-h;y<=h;y++) {
          int dx=i+x;
          int dy=j+y;
          if (inmap(dx,dy))
            if (pos[dx][dy]>0.5)
            return true;
        }
      }
      return false;
    }
    
    Map2d<bool> *expand(int r) {
      Map2d<bool> *n=new Map2d<bool>(width,height);
      for (int i=0;i<n->width;i++)
      for (int j=0;j<n->height;j++) {
        if (pixelwithinrad(i,j,r))
          n->pos[i][j]=true;
        else
          n->pos[i][j]=false;
      }
      return n;
    }
        
    void freedom() {
      for (int i=0;i<width;i++)
        free(pos[i]);
      free(pos);
    }
    
    Map2d<Object> *scaleto(int w,int h) { // Anti-aliases
      Map2d<Object> *n=new Map2d<Object>(w,h);
      for (int i=0;i<w;i++)
      for (int j=0;j<h;j++) {
        int x=i*width/w;
        int y=j*height/h;
        int rx=(i+1)*width/w;
        int by=(j+1)*height/h;
        if (rx==x || by==y)
          n->pos[i][j]=pos[x][y];
        else { // Came from >1 pixels so anti-alias:
          Object o=0;
          for (int k=x;k<rx;k++)
          for (int l=y;l<by;l++)
            o=o+pos[k][l];
          n->pos[i][j]=o/(float)((rx-x)*(by-y));
        }
      }
      return n;
    }

    Map2d<bool> binscaleby(float s) { // Anti-aliases
      return *binscaleto(width*s,height*s);
    }
    Map2d<bool> *binscaleto(int w,int h) { // Anti-aliases
      Map2d<bool> *n=new Map2d<bool>(w,h);
      for (int i=0;i<w;i++)
      for (int j=0;j<h;j++) {
        int x=i*width/w;
        int y=j*height/h;
        n->pos[i][j]=(pos[x][y]>0.5);
      }
      return n;
    }

    Map2d<Object> *scaledto(int w,int h) {
      return scaleto(w,h);
    }

    Map2d<Object> *scaleby(float s) {
      return scaleto(width*s,height*s);
    }
    
    Map2d<Object> *scaledby(float s) {
      return scaleby(s);
    }

    RGBmp *getrgbs() {
      return RGBmp::fromv3ds(this);
    }

    void fill(Region *r,Object o) {
      List<V2d> *vs=r->getlist();
      for (int i=1;i<=vs->len;i++) {
        V2d v=vs->num(i);
        if (inmap(v.x,v.y))
          pos[(int)v.x][(int)v.y]=o;
      }
    }

    Map2d<float> *getgreyscale() {
      // Doesn't work if Object is Vector<float>
      // It seems to now!
      Map2d<float> *n=new Map2d<float>(width,height);
      for (int i=0;i<width;i++)
      for (int j=0;j<height;j++) {
//        printf("%f ",tofloat((Object)pos[i][j]));
        n->pos[i][j]=tofloat(pos[i][j]);
      }
      return n;
    }

    Object get(int x,int y) {
      if (inmap(x,y))
        return pos[x][y];
      else
        return Object();
        // return (Object)-1;
    } // recent change to handle V2ds
      // but what will it do to ints?!
      // ints return 0 instead of -1
      // Look out for this in code
      // which checks for -1 rather
      // than using inmap(x,y)?

    Object getinter(float x,float y) {
      int lx=(int)x;
      int ly=(int)y;
      float e=x-lx;
      float s=y-ly;
      float w=1.0-e;
      float n=1.0-s;
      return n*w*get(lx,ly)+n*e*get(lx+1,ly)+s*w*get(lx,ly+1)+s*e*get(lx+1,ly+1);
    }

    void put(int x,int y,Object o) {
      if (inmap(x,y))
        pos[x][y]=o;
    }
    
    float getaverage() {
      float o=0;

     for (int i=0;i<width;i++)
      for (int j=0;j<height;j++)
        o=o+pos[i][j];
      return o/(float)width/(float)height;
    }
    
    Object getpixel(int x,int y) {
      return get(x,y);
    }

   Object getpos(int x,int y) {
     return getpixel(x,y);
   }

  Object getposclip(int x,int y) {
    if (x<0)
      x=0;
    if (y<0)
      y=0;
    if (x>=width)
      x=width-1;
    if (y>=height)
    	y=height-1;
    return getpixel(x,y);
  }

   Object getpos(Pixel p) {
     return getpixel(p.x,p.y);
   }
   
   Object getpixel(Pixel p) {
     return getpixel(p.x,p.y);
   }
    
   Map2d<bool> *inverse() {
//     return applyfn(&invert);
     return getgreyscale()->applyfn(&invertfn);
   }
   
    // template <class FObject>
    static bool invertfn(float x) {
      return (tofloat(x)>0.5 ? true : false);
    }

   void invert() {
     for (int i=0;i<width;i++)
     for (int j=0;j<height;j++)
       setpos(i,j,!getpos(i,j));
   }

  void intvline(int x,int ya,int yb,Object ca,Object cb) {
    float atob;
    bool done=false;
    int s=sgn(yb-ya);
    for (register int i=ya;!done;i+=s) {
      if (i==yb)
        done=true;
      if (ya==yb)
        atob=0;
      else
        atob=(float)(i-ya)/(float)(yb-ya);
      Object c=pull(ca,atob,cb);
      setpixel(x,i,c);
    }
  }
   
   void intertri(Pixel a,Pixel b,Pixel c,Object ca,Object cb,Object cc) {
    int xa=a.x;
    int xb=b.x;
    int xc=c.x;
    int ya=a.y;
    int yb=b.y;
    int yc=c.y;
    if (xa>xb) {
      swap(&xa,&xb); swap(&ya,&yb);
      swap(&ca,&cb);
    }
    if (xb>xc) {
      swap(&xb,&xc); swap(&yb,&yc);
      swap(&cb,&cc);
    }
    if (xa>xb) {
      swap(&xa,&xb); swap(&ya,&yb);
      swap(&ca,&cb);
    }
    int start=xa;
    if (xa<0)
      start=0;
    for (register int x=start;x<=xb;x++) {
      float xthruab;
      if (xb==xa)
        xthruab=0;
      else
      xthruab=(float)(x-xa)/(xb-xa);
      float xthruac;
      if (xa==xc)
        xthruac=0;
      else
      xthruac=(float)(x-xa)/(xc-xa);
      int ylow=(int)(ya+xthruab*(float)(yb-ya));
      int yhei=(int)(ya+xthruac*(float)(yc-ya));
      intvline(x,ylow,yhei,ca+xthruab*(cb-ca),ca+xthruac*(cc-ca));
    }
    int end=xc;
    if (xc>width)
      end=width;
    for (register int x=xb;x<=end;x++) {
      float xthrubc;
      if (xb==xc)
        xthrubc=0;
      else
      xthrubc=(float)(x-xb)/(xc-xb);
      float xthruac;
      if (xa==xc)
        xthruac=0;
      else
      xthruac=(float)(x-xa)/(xc-xa);
      int ylow=(int)(yb+xthrubc*(float)(yc-yb));
      int yhei=(int)(ya+xthruac*(float)(yc-ya));
      intvline(x,ylow,yhei,cb+xthrubc*(cc-cb),ca+xthruac*(cc-ca));
    }
  }

   void intertri(V2d a,V2d v,V2d c,Object oa,Object ob,Object oc) {
     intertri(Pixel(a),Pixel(v),Pixel(c),oa,ob,oc);
   }
   
void opencircle(int x,int y,float r,Object c) {
  V2d last=V2d(x,y+r);
  int steps=4+r/2;
  for (int i=1;i<=steps;i++) {   
    float a=2.0*pi*(float)i/(float)steps;
    V2d next=V2d(x+r*sin(a),y+r*cos(a));
    line(last,next,c);
    last=next;
  }
}
void circle(int x,int y,float r,Object c) {
  opencircle(x,y,r,c);
}
void opencircle(Pixel p,float r,Object c) {
  opencircle(p.x,p.y,r,c);
}
void circle(Pixel p,float r,Object c) {
  opencircle(p.x,p.y,r,c);
}

  void clipby(Region *r,bool b) {
    for (int x=0;x<width;x++)
    for (int y=0;y<height;y++)
      if (r->getmap()->getpos(x,y)==false)
        setpos(x,y,b);
  }

  void searchreplace(Object s,Object r) {
    for (int x=0;x<width;x++)
    for (int y=0;y<height;y++)
      if (getpos(x,y)==s)
        setpos(x,y,r);
  }


#ifndef DOS
    static Map2d<bool> *readbinfile(String s) {
      Map2d<float> *tmp=Map2d<float>::readfile(s);
      Map2d<bool> *n=tmp->threshold(0.5);
      destroy(tmp);
      return n;
    }
#endif

  RGBmp hueify() {
    RGBmp n=RGBmp(width,height,myRGB::black);
    int l=(int)largest();
    List<myRGB> m=List<myRGB>(l);
    for (int i=1;i<=l;i++) {
      myRGB r=myRGB::hue(myrnd());
      if (myrnd()>0.5)
        r=r+myRGB::white*myrnd()*0.6;
      else
        r=r*(0.4+myrnd()*0.6);
      if (i==255)
        r=myRGB::white;
      m.add(r);
    }
    for (int x=0;x<width;x++)
    for (int y=0;y<height;y++)
      if (getpos(x,y)>0)
        n.setpos(x,y,m.num(getpos(x,y)));
    m.freedom();
    return n;
  }






  void thickcross(V2d cen,int s,float wid,Object c) {
    thickline(cen+V2d(s,s),cen-V2d(s,s),c,wid);
    thickline(cen+V2d(-s,s),cen-V2d(-s,s),c,wid);
  }

  void thickline(V2d a,V2d b,Object r,float w) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a+p,a-p,b+p,r);
    filltri(a-p,b+p,b-p,r);
  }














    
    
    
    // Entering code after this can do weird things to the compiler
    
    
    virtual void search(Map2d<bool> *togo,List<V2d> *l,List<V2d> *s) {
      // myerror("No method defined for Map2d<Object>::search()\n");
      while (s->len>0) {
        V2d v=s->num(1);
        int i=v.x;
        int j=v.y;
        s->removenum(1);
        if (togo->inmap(i,j))
          if (togo->pos[i][j]) {
          togo->pos[i][j]=false;
          l->add(V2d(i,j));
          s->add(V2d(i-1,j));
          s->add(V2d(i+1,j));
          s->add(V2d(i,j-1));
          s->add(V2d(i,j+1));
        }
      }
    }
    virtual List< List<V2d> > *getregions() {
      List< List<V2d> > *rs=new List< List<V2d> >();
      Map2d<bool> togo=*this->threshold(0.5);
      for (int i=0;i<width;i++)
      for (int j=0;j<height;j++) {
        if (togo.pos[i][j]) {
          rs->add(List<V2d>());
          List<V2d> s=List<V2d>();
          s.add(V2d(i,j));
          search(&togo,rs->p2num(rs->len),&s);
        }
      }
      return rs;
      // myerror("No method defined for Map2d<Object>::getregions()\n");
    }
    void realsearch(Map2d<bool> *togo,Region *r,List<Pixel> *s);
    List<Region *> *getrealregions(bool oftype);
    List<Region *> *getrealregions();
//    template <class Object>
    Map2d<int> getregionmap(Object oftype);
    /* virtual Map2d<Object> *inverse() {
         myerror("No method defined for Map2d<Object>::inverse()\n");
       }*/
    List<Region> collectregions();
  };// End template class


//#ifdef DOS
  
  // Template class appears in original format:
template <class Object>
  class SubMap2d : public Map2d<Object> {
  public:
    int top,left;
    SubMap2d(int t,int l,int w,int h,Map2d<Object> *m) {
      top=t; left=l; width=w; height=h;
      pos=new (Object *)[width];
      for (int i=0;i<width;i++) {
        pos[i]=new (Object)[height];
        for (int j=0;j<height;j++)
        pos[i][j]=m->pos[left+i][top+j];
      }
    }
  };// End template class

  
  template <class Object>
  SubMap2d<Object> *shortcutSubMap2d(int a,int b,int c,int d,Map2d<Object> *e); // Method


//#endif

class MapRGB : public Map2d<myRGB> {
};

/* Skipping later :: template<class Object>
Map2d<bool> Map1d<Object>::draw(); // Method
*/


Map2d<bool> drawCorrelator(Correlator cc); // Method


