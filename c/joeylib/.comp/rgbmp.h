#ifndef rgbmp_H
  #define rgbmp_H

// See quad function at the bottom

#include <stdio.h>
#define uchar unsigned char

// too slow!
/*
float PRdist=0.1;
class PerpRecv {
public:
  V2d a,b,c,d;
  V2d vv,vh;
  PerpRecv(V2d aa,V2d bb,V2d cc,V2d dd) {
    a=aa;
    b=bb;
    c=cc;
    d=dd;
    vh=Line2d(a,b).intersection(Line2d(d,c));
    vv=Line2d(a,d).intersection(Line2d(b,c));
  }
  V2d find(float e,float s,float l,float r,float t,float b,V2d aa,V2d bb,V2d cc,V2d dd) {
    V2d cen=Line2d(aa,cc).intersection(Line2d(bb,dd));
    Line2d hor=Line2d(vh,cen);
    Line2d ver=Line2d(vv,cen);
    V2d pp=Line2d(aa,dd).intersection(hor);
    V2d rr=Line2d(bb,cc).intersection(hor);
    V2d qq=Line2d(aa,bb).intersection(ver);
    V2d ss=Line2d(dd,cc).intersection(ver);
    float cx=(l+r)/2.0;
    float cy=(t+b)/2.0;
//    printf("%f %f / %f %f\n",e,s,cx,cy);
    if (diff(e,cx)>PRdist) {
//      printf("x\n");
      if (e<cx)
        return find(e,s,l,cx,t,b,aa,qq,ss,dd);
      else
        return find(e,s,cx,r,t,b,qq,bb,cc,ss);
    }
    if (diff(s,cy)>PRdist) {
//      printf("y\n");
      if (s<cy)
        return find(e,s,l,r,t,cy,aa,bb,rr,pp);
      else
        return find(e,s,l,r,cy,b,pp,rr,cc,dd);
    }
    return V2d(cx,cy);
  }
  V2d sourceof(float e,float s) {
//    printf("*\n");
    return find(e,s,0,1,0,1,a,b,c,d);
  }
};
*/

extern float PRdist; // Variable initialised in .c file
class PerpRecv {
public:
  V2d a,b,c,d; // Exists
  V2d vv,vh; // Exists
   PerpRecv(V2d aa,V2d bb,V2d cc,V2d dd); // Method

  V2d find(float e,float s,float l,float r,float t,float b,V2d aa,V2d bb,V2d cc,V2d dd); // Method

  V2d sourceof(float e,float s); // Method

};

class Texture {
public:
  virtual myRGB colat(V3d w); // Method

};

int gimmezero(); // Method


unsigned int getshort(FILE *fp); // Method

unsigned int getint(FILE *fp); // Method

void putshort(FILE *fp,int i); // Method

void putint(FILE *fp, int i); // Method


 class RGBmp;

RGBmp *angusreadbmp8(char *fn_image); // Method

RGBmp *angusreadbmp24(char *fn_image); // Method

void anguswritebmp24(char *fn_image,RGBmp *r); // Method

// void writeppm(char *fn_image,RGBmp *r);

/*class PerpRecv {
public:
  V2d a,b,c,d;
  V2d vv,vh;
  int pow;
  RGBmp *o,*to;
  PerpRecv(RGBmp *oo,V2d aa,V2d bb,V2d cc,V2d dd,int pp) {
    o=oo;
    a=aa;
    b=bb;
    c=cc;
    d=dd;
    pow=pp;
    vh=Line2d(a,b).intersection(Line2d(d,c));
    vv=Line2d(a,d).intersection(Line2d(b,c));
  }
  void recover(float l,float t,float r,float b,V2d aa,V2d bb,V2d cc,V2d dd,int p);
  RGBmp *recover();
};*/
  
class RGBmp : public Writeable {
public:
  int width,height; // Exists
  myRGB **bmp; // Exists

   RGBmp(); // Method

   RGBmp(int w,int h); // Method

   RGBmp(int w,int h,myRGB r); // Method


  myRGB getpos(int x,int y); // Method


  myRGB getpos(Pixel p); // Method


  void make(int w,int h); // Method


  void applygreyscale(); // Method

  
  void applycontrast(float f); // Method

  
  void applybrightness(float f); // Method

  
  Map3d<int> *houghtransform(int angquant,int xquant,int yquant,int spacerad); // Method

  
  Map2d<float> *getedges(int maskrad); // Method

  
  void setrectangle(int left,int top,int wid,int hei,myRGB r); // Method

  
  RGBmp *smoothed(int rad); // Method

  
  int inimage(int x, int y); // Method

  
  
  void thickline(Line2d l,myRGB r,float w); // Method

  void thickline(V2d a,V2d b,myRGB r,float w); // Method


  void thicklineinf(V2d a,V2d b,float w,myRGB r); // Method


  void filltri(V2d a,V2d b,V2d c,myRGB r); // Method


  void line(Line2d l,myRGB r); // Method

  void line(V2d a,V2d b,V3d v); // Method

  void line(V2d a,V2d b,myRGB r); // Method

  void line(Pixel a,Pixel b,myRGB r); // Method

  void line(int x1, int y1, int x2, int y2,myRGB *r); // Method

  
  
  /* void explore(int x,int y,List<List<Pixel> > *l,Map2d<int> *chosen,int c,myRGB *r,float tol,int d) {
       printf("%i ",d);
       if (inimage(x,y)) {
         if (chosen->pos[x][y]==0) {
           myRGB *t=&bmp[x][y];
           if (myRGB::distance(r,t)<tol) {
             chosen->pos[x][y]=c;
             // putpixel(screen,x,y,65535);
             // l->p2num(c)->add(newPixel(x,y));
             explore(x-1,y,l,chosen,c,t,tol,d+1);
             explore(x+1,y,l,chosen,c,t,tol,d+1);
             explore(x,y-1,l,chosen,c,t,tol,d+1);
             explore(x,y+1,l,chosen,c,t,tol,d+1);
           }
         }
       }
     }*/
  
  boolean valid(int x,int y,Map2d<int> *chosen,myRGB *r,float tol); // Method

  
  void explore(int x,int y,List<List<Pixel> > *l,Map2d<int> *chosen,int c,myRGB *r,float tol,int d); // Method

  
  List<List<Pixel> > *getfilledregions(float tolerance); // Method

  
  
  List<List<Pixel> > *oldclassifyscattered(myRGB preferred,float tolerance,int scatrad); // Method

  
  
  
  
  List<List<Pixel> > *classifyallscattered(float tolerance,int scatrad); // Method

  
  
  
  
  
  
  List<List<Pixel> > *classifyscattered(myRGB preferred,float tolerance,int scatrad); // Method

  
  
  
  
  
  
  
  
  
  
  
  void display(); // Method


  void display(int winwid,int winhei); // Method

  void writetoscreen(); // Method

  
  /* RGBmp(String fname) {
       BITMAP *b;
       PALETTE pal;
       printf("gOT HERE.\n");
       b=load_bitmap(fname,pal);
       printf("not here\n");
       width=b->w;
       height=b->h;
       printf("%i %i\n",b->w,b->h);
       bmp=new (myRGB*)[width];
       printf("but here\n");
       for (int i=0;i<width;i++) {
         bmp[i]=new myRGB[height];
         for (int j=0;j<height;j++) {
           // RGB r=pal[getpixel(b,i,j)];
           // bmp[i][j]=myRGB(r.r,r.g,r.b);
         }
       }
       printf("and here!\n");
     }*/
  
  // attempt for DOS
   RGBmp(String fname); // Method


//    error("RGbmp(filename) deprecated");

/*    FILE *fp;
    int fp_type;
    int xdim, ydim;
    int i, c, c1, rv, j, padb;
    unsigned int bfSize, bfOffBits, biSize, biWidth, biHeight, biPlanes;
    unsigned int biBitCount, biCompression, biSizeImage, biXPelsPerMeter;
    unsigned int biYPelsPerMeter, biClrUsed, biClrImportant;
    int cmaplen, mono_cmap;
    
    // fp = openImageIStream(fname, &fp_type);
    fp=fopen(fname,"r");
    
    if (!fp) {
      printf("RGBmp: error opening file %s\n",fname);
      exit(0);
    }
    
    printf("Reading file %s",fname);
    
    c = getc(fp);  c1 = getc(fp);
    if (c!='B' || c1!='M') { 
      fprintf(stderr, "[readBMP24] file type != 'BM'\n"); 
      exit(1);
    }
    
    
    
    bfSize = getint(fp);
    getshort(fp);         
    getshort(fp);
    bfOffBits = getint(fp);
    
    
    biSize          = getint(fp);
    biWidth         = getint(fp);
    biHeight        = getint(fp);
    // printf("RGBmp - size %i width %i height %i\n",biSize,biWidth,biHeight);
    biPlanes        = getshort(fp);
    biBitCount      = getshort(fp);
    biCompression   = getint(fp);
    biSizeImage     = getint(fp);
    biXPelsPerMeter = getint(fp);
    biYPelsPerMeter = getint(fp);
    biClrUsed       = getint(fp);
    biClrImportant  = getint(fp);
    
    xdim = biWidth; ydim = biHeight;
    
    printf(" %i %i %i",biCompression,biClrUsed,biClrImportant);
    
    width=xdim;
    height=ydim;
    bmp=new (myRGB*)[width];
    for (int i=0;i<width;i++) {
      bmp[i]=new myRGB[height];
      for (int j=0;j<height;j++) {
        // bmp[i][j]=myRGB(myrnd2(),myrnd2(),myrnd2());
      }
    }
    
    printf(" width %i height %i\n",xdim,ydim);
    
    if (ferror(fp)) { 
      fprintf(stderr, "[readBMP] EOF reached in file header\n");
      exit(1);
    }
    
    
    if (biBitCount!=24) {
      fprintf(stderr, "[readBMP24] BMP type(%d) not supported\n", biBitCount);
      exit(1);
    }
    
    
    c = biSize - 40;    
    for (i=0; i<c; i++) getc(fp);
    padb = (xdim*9)%4;  
    for (i=(ydim)-1; i >= 0; i--) {
      for (j = 0; j < xdim; j++) {
        uchar b=getc(fp);
        uchar g=getc(fp);
        uchar r=getc(fp);
        // printf("Putting %i %i %i in (%i,%i)\n",r,g,b,j,i);
        // printf("-%i %i %i    ",(int)r,(int)g,(int)b);
        // bmp[j][i]=myRGB((int)r,(int)g,(int)b);
        bmp[j][i]=myRGB((int)r,(int)g,(int)b);
        if (bmp[j][i].r!=r)
          printf("%i %i! ",j,i);
        // printf("/%i %i %i    ",bmp[j][i].r,bmp[j][i].g,bmp[j][i].b);
        // bmp[j][i].print();
        // n.bmp[j][i]=myRGB(myrnd2(),myrnd2(),myrnd2());
        // byte_rgb_img->rgb_img[2][i][j] =  getc(fp);
        // byte_rgb_img->rgb_img[1][i][j] =  getc(fp);
        // byte_rgb_img->rgb_img[0][i][j] =  getc(fp);
      }
      for (j=0; j<padb; j++) getc(fp);
      if (ferror(fp)) { 
        fprintf(stderr, "[readBMP] file truncated\n");
        exit(1);
      }
      
    }
    // closeImageStream(fp, fp_type);
    fclose(fp);
  }*/
  
  void writebmp24(char *fname); // Method

  
  static RGBmp *readfile(String s); // Method

  
  static RGBmp *readppm(String filename); // Method

  
  static RGBmp *readbmp(String fname); // Method

  
  void writefile(String fname); // Method

  
  Map2d<float> *getgreyscale(); // Method

  Map2d<V3d> *getv3ds(); // Method

  
  Map2d<RGBHistogram> *getlocalhistmap(int histrad,float scale); // Method

  

  static RGBmp *toRGBmp(Map2d<float> *m); // Method


  template <class Obj>
  static RGBmp *toRGBmp(Map2d<Obj> *m); // Method



  void setupgrid(Map2d<V2d> *grid,int left,int top,int size,V2d hvp,V2d vvp); // Method

  // This method is accurate excepting estimate of aspect ratio
  RGBmp *recoverquadrilateral(V2d a,V2d b,V2d c,V2d d,int imwidth); // Method




/*
RGBmp *recoverquad(V2d a,V2d b,V2d c,V2d d,int imwidth) {
  
  float aspect=sqrt(V2d::dist(a,b)+V2d::dist(c,d))/sqrt(V2d::dist(a,d)+V2d::dist(c,b));
  int pow=8;
  return PerpRecv(this,a,b,c,d,pow).recover();
}

RGBmp *recoverquad(V2d a,V2d b,V2d c,V2d d,int imwidth) {
  
  float aspect=sqrt(V2d::dist(a,b)+V2d::dist(c,d))/sqrt(V2d::dist(a,d)+V2d::dist(c,b));
  RGBmp *out=new RGBmp(imwidth,imwidth/aspect);
  PerpRecv pr=PerpRecv(a,b,c,d);
  for (int i=0;i<out->width;i++)
  for (int j=0;j<out->height;j++) {
    float e=(float)i/(float)out->width;
    float s=(float)j/(float)out->height;
    printf("%f %f\n",e,s);
    V2d p=pr.sourceof(e,s);
    int px=p.x;
    int py=p.y;
    if (inimage(px,py))
      out->bmp[i][j]=bmp[px][py];
    else
      out->bmp[i][j]=bmp[0][0];
  }
  
  return out;
}
*/

    myRGB getinter(float x,float y); // Method



RGBmp *recoverquad(V2d a,V2d b,V2d c,V2d d,int imwidth); // Method


/* without interpolation
RGBmp *recoverquad(V2d a,V2d b,V2d c,V2d d,int imwidth) {
  float aspect=sqrt(V2d::dist(a,b)+V2d::dist(c,d))/sqrt(V2d::dist(a,d)+V2d::dist(c,b));
  RGBmp *out=new RGBmp(imwidth,imwidth/aspect);
  for (int i=0;i<out->width;i++)
  for (int j=0;j<out->height;j++) {
    float e=(float)i/(float)out->width;
    float s=(float)j/(float)out->height;
    float w=1.0-e;
    float n=1.0-s;
    V2d p=a*n*w+b*n*e+c*s*e+d*s*w;
    int px=p.x;
    int py=p.y;
    if (inimage(px,py))
      out->bmp[i][j]=bmp[px][py];
    else
      out->bmp[i][j]=bmp[0][0];
  }
  
  return out;
}
*/

  RGBmp *linearrecoverquad(List<V2d> *ls,float scale,int imwid); // Method


  // The 3d quad recovery bodge
  // I think the maths works, but it's not confirmed.
  // The results are certainly comparable to those from recoverquadrilateral
  RGBmp *recoverquad(List<V2d> *ls,float scale,int imwid); // Method


  void setpos(int x,int y,V3d c); // Method


  void setpos(int x,int y,myRGB c); // Method


  void setpixel(int x,int y,V3d c); // Method


  void setpixel(int x,int y,myRGB c); // Method


  void setpixel(Pixel p,myRGB c); // Method


  void setpos(Pixel p,myRGB c); // Method


  void intvline(int x,int ya,int yb,V3d ca,V3d cb); // Method


  void intvline(int x,int ya,int yb,V3d ca,V3d cb,Texture *t); // Method


  void intertri(int xa,int ya,int xb,int yb,int xc,int yc,V3d ca,V3d cb,V3d cc,Texture *t); // Method



  void intertri(int xa,int ya,int xb,int yb,int xc,int yc,myRGB ca,myRGB cb,myRGB cc); // Method


  void intertri(V2d a,V2d b,V2d c,myRGB ca,myRGB cb,myRGB cc); // Method


  void intertri(int xa,int ya,int xb,int yb,int xc,int yc,V3d ca,V3d cb,V3d cc); // Method


  void clear(); // Method


  void clear(myRGB c); // Method


  void clear(V3d c); // Method


  bool inbmp(int x,int y); // Method


  RGBmp *scaletowidth(int w); // Method


  RGBmp *scaledby(float s); // Method


  myRGB getposinterpolate(float x,float y); // Method



  static RGBmp *scale(RGBmp *in,float scale); // Method


  RGBmp *inverted(); // Method


  static RGBmp *fromv3ds(Map2d<V3d> *m); // Method


  void line(Line2d *l,myRGB c); // Method

  void sthickline(V2d a,V2d b,float w,myRGB c); // Method

  void thickline(V2d a,V2d b,float w,myRGB c); // Method

  void thickline(int xa,int ya,int xb,int yb,float w,myRGB c); // Method

  void thickline(Line2d l,int w,myRGB c); // Method

  void thicklinev(V2d a,V2d b,float w,myRGB c); // Method

  void thicklineh(V2d a,V2d b,float w,myRGB c); // Method

  void rthickline(V2d a,V2d b,float w,myRGB c); // Method

void opencircle(Pixel p,float r,myRGB c); // Method

void opencircle(V2d v,float r,myRGB c); // Method

void opencircle(int x,int y,float r,myRGB c); // Method

  void filledcircle(int cx,int cy,float r,myRGB c); // Method

  void cross(V2d cen,float s,myRGB c); // Method

  void cross(float x,float y,float s,myRGB c); // Method

  void filledcircle(V2d pos,float r,myRGB c); // Method

  RGBmp *applyfn(myRGB fn(myRGB)); // Method

  
  void freedom(); // Method

  
  myRGB greyify(myRGB r); // Method

  
  void greyify(); // Method
    

};

/* 
   unsigned int getshort(FILE *fp) {
     
     int c, c1;
     c = getc(fp);  c1 = getc(fp);
     return ((unsigned int) c) + (((unsigned int) c1) << 8);
   }
   
   // ******************************************
   unsigned int getint(FILE *fp) {
     
     int c, c1, c2, c3;
     c = getc(fp);  c1 = getc(fp);  c2 = getc(fp);  c3 = getc(fp);
     #ifdef DOS
     return ((unsigned int) c) +
     (((unsigned int) c1) << 8) + 
     (((unsigned int) c2) << 16) +
     (((unsigned int) c3) << 24);
     #else
     return ((unsigned int) c3) +
     (((unsigned int) c2) << 8) + 
     (((unsigned int) c1) << 16) +
     (((unsigned int) c) << 24);
     #endif
   }
   
   void putshort(FILE *fp,int i) {
     
     int c, c1;
     
     c = ((unsigned int ) i) & 0xff;  c1 = (((unsigned int) i)>>8) & 0xff;
     putc(c, fp);   putc(c1,fp);
   }
   
   void putint(FILE *fp, int i) {
     
     int c, c1, c2, c3;
     c  = ((unsigned int ) i)      & 0xff;  
     c1 = (((unsigned int) i)>>8)  & 0xff;
     c2 = (((unsigned int) i)>>16) & 0xff;
     c3 = (((unsigned int) i)>>24) & 0xff;
     
     putc(c, fp);   putc(c1,fp);  putc(c2,fp);  putc(c3,fp);
   }
   
   
   
*/

// #include <writeppm.c>

/*  void PerpRecv::recover(float l,float t,float r,float b,V2d aa,V2d bb,V2d cc,V2d dd,int p) {
    printf("%i\n",p);
    p++;
    float cx=(l+r)/2.0;
    float cy=(t+b)/2.0;
    V2d cen=Line2d(aa,cc).intersection(Line2d(bb,dd));
    Line2d hor=Line2d(vh,cen);
    Line2d ver=Line2d(vv,cen);
    V2d pp=Line2d(aa,dd).intersection(hor);
    V2d rr=Line2d(bb,cc).intersection(hor);
    V2d qq=Line2d(aa,bb).intersection(ver);
    V2d ss=Line2d(dd,cc).intersection(ver);
    int x=cx;
    int y=cy;
    int ox=(int)cen.x;
    int oy=(int)cen.y;
    to->bmp[x][y]=o->bmp[ox][oy];
    if (p<=pow) {
      recover(l,t,cx,cy,aa,qq,cen,pp,p);
      recover(cx,t,r,cy,qq,bb,rr,cen,p);
      recover(l,cy,cx,b,pp,cen,ss,dd,p);
      recover(cx,cy,r,b,cen,rr,cc,ss,p);
    }
  }
  RGBmp *PerpRecv::recover() {
    to=new RGBmp(power(2,pow),power(2,pow));
    recover(0,0,to->width,to->height,a,b,c,d,0);
    return to;
  }
*/





V2d rectangleproject(V3d w,V3d eye); // Method


V2d vvpfromhvpandbaseline(V2d hvp,Line2d baseline,V3d eye,RGBmp origimage); // Method


/* V2d vvpfromhvpandbaselineold(V2d hvp,Line2d baseline,V3d eye) {

  printf("Baseline: %s",baseline.toString());
  if (baseline.a.y<baseline.b.y) {
    printf(" swapping");
    swap(&baseline.a,&baseline.b);
  }
  printf("\n");

  V3d vbot=V3d(baseline.a.x,baseline.a.y,0.0);
  V3d vtop=V3d(baseline.b.x,baseline.b.y,0.0);
  Line3d lbottom=Line3d(eye,vbot);
  Line3d ltop=Line3d(eye,vtop);
  Line3d lhvp=Line3d(eye,V3d(hvp.x,hvp.y,0.0));

  Plane ptop=Plane::fromLines(ltop,lhvp);
  Plane pbottom=Plane::fromLines(lbottom,lhvp);
  Plane pmiddle=Plane::fromLines(lbottom,ltop);

  V3d ptopnor=V3d::cross(vtop-eye,V3d(hvp.x,hvp.y,0.0)-eye);
  V3d pbotnor=V3d::cross(vbot-eye,V3d(hvp.x,hvp.y,0.0)-eye);
  V3d pmidnor=V3d::cross(vtop-eye,vbot-eye);
  //V2d perp=(baseline.b-baseline.a).perp();
  //V3d pmidnor=V3d(perp.x,perp.y,0.0);

  V3d hd=V3d::cross(ptopnor,pbotnor).normalised();
  float normdot=V3d::normdot(hd,pmidnor);
  if (normdot>0.9 || normdot<-0.9)
    printf("*** WARNING!  ");
  printf("hd and pmidnor are similar by %f\n",normdot);
  V3d vd=V3d::cross(hd,pmidnor).normalised();

  if (hd.x<0)
    hd=-hd;
  if (vd.y>0)
    vd=-vd;
  printf("HD: %s\n",hd.toString());
  printf("VD: %s\n",vd.toString());

  V2d tl=rectangleproject(vtop-50.0*hd,eye);
  V2d tr=rectangleproject(vtop+50.0*hd,eye);
  V2d bl=rectangleproject(vtop-50.0*hd-50.0*vd,eye);
  V2d br=rectangleproject(vtop+50.0*hd-50.0*vd,eye);

  // Given the two swaps above, this shouldn't be necessary!
  printf("%s %s\n%s %s\n",tl.toString(),tr.toString(),bl.toString(),br.toString());
  if ((tl+bl).x>(tr+br).x) {
    printf("***** SWAPPING X!\n");
    swap(&tl,&tr);
    swap(&bl,&br);
  }
  if ((tl+tr).y>(bl+br).y) {
    printf("***** SWAPPING Y!\n");
    swap(&tl,&bl);
    swap(&tr,&br);
  }
  printf("after\n%s %s\n%s %s\n",tl.toString(),tr.toString(),bl.toString(),br.toString());

  V2d vvp=Line2d(bl,tl).intersect(Line2d(br,tr));

  return vvp;

} */

List<V3d> rectanglefromquadrilateral(List<V2d> v2ds,V3d eye); // Method



List<V3d> rectanglefromquadrilateralgroovy(List<V2d> v2ds,V3d eye); // Method


#endif
