#ifndef rgbmp_C
  #define rgbmp_C

  #include <rgbmp.h>

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

float PRdist=0.1;
// Starts of class PerpRecv


  // Variable declared in .h file
  // Variable declared in .h file
   PerpRecv::PerpRecv(V2d aa,V2d bb,V2d cc,V2d dd) {
    a=aa;
    b=bb;
    c=cc;
    d=dd;
    vh=Line2d(a,b).intersection(Line2d(d,c));
    vv=Line2d(a,d).intersection(Line2d(b,c));
  }
  V2d PerpRecv::find(float e,float s,float l,float r,float t,float b,V2d aa,V2d bb,V2d cc,V2d dd) {
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
  V2d PerpRecv::sourceof(float e,float s) {
    
  }
// End class 


// Starts of class Texture


   myRGB Texture::colat(V3d w) {
  }
// End class 


int gimmezero() {
  return 0;
}

/* Skipping declared method unsigned int getshort(FILE *fp) */

/* Skipping declared method unsigned int getint(FILE *fp) */

/* Skipping declared method void putshort(FILE *fp,int i) */

/* Skipping declared method void putint(FILE *fp, int i) */


// Class declaration of existence: RGBmp


/* Skipping declared method RGBmp *angusreadbmp8(char *fn_image) */

/* Skipping declared method RGBmp *angusreadbmp24(char *fn_image) */

/* Skipping declared method void anguswritebmp24(char *fn_image,RGBmp *r) */

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
  
// Starts of class RGBmp


  // Variable declared in .h file
  // Variable declared in .h file

   RGBmp::RGBmp() {
    #ifdef DEBUG
      printf("Ensure RGBmp is set before use.\n");
    #endif
  }
   RGBmp::RGBmp(int w,int h) {
    make(w,h);
    clear(myRGB::black);
  }
   RGBmp::RGBmp(int w,int h,myRGB r) {
    make(w,h);
    clear(r);
  }

  myRGB RGBmp::getpos(int x,int y) {
    if (inbmp(x,y))
      return bmp[x][y];
    else
      return myRGB::random();
  }

  myRGB RGBmp::getpos(Pixel p) {
    return getpos(p.x,p.y);
  }

  void RGBmp::make(int w,int h) {
    width=w;
    height=h;
//    printf("Generating new RGBmp size %i %i\n",w,h);
    bmp=new (myRGB*)[width];
    for (int i=0;i<width;i++) {
      // printf("Got here\n");
      bmp[i]=new myRGB[height];
      for (int j=0;j<height;j++) {
        // printf("Going around\n");
        // bmp[i][j]=myRGB(256*myrnd(),256*myrnd(),256*myrnd());
//        bmp[i][j]=myRGB(myrnd(),myrnd(),myrnd());
//        bmp[i][j]=myRGB(0,0,0);
        // bmp[i][j].print();
      }
    }
//    printf("New RGBmp created.\n");
  }

  void RGBmp::applygreyscale() {
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++) {
      bmp[i][j]=(V3d(1,1,1)*V3d::mod(bmp[i][j].v3d())).myrgb();
    }
  }
  
  void RGBmp::applycontrast(float f) {
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++) {
      bmp[i][j]=(V3d(.5,.5,.5)+f*(bmp[i][j].v3d()-V3d(.5,.5,.5))).myrgb();
    }
  }
  
  void RGBmp::applybrightness(float f) {
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++) {
      bmp[i][j]=(bmp[i][j].v3d()*f).myrgb();
    }
  }
  
  Map3d<int> *RGBmp::houghtransform(int angquant,int xquant,int yquant,int spacerad) {
    // Perform Hough transform
    Map3d<int> *map=new Map3d<int>(angquant,xquant,yquant,&gimmezero);
    List<Pixel> l=pixelsincircle(0,0,spacerad);
    for (int x=0;x<width;x++) {
      printf(".");
      for (int y=0;y<height;y++) {
        if (bmp[x][y].brightness()>.5) {
          for (int i=0;i<l.len;i++) {
            Pixel p=l.num(i);
            int mx=x+p.x;
            int my=y+p.y;
            if (arewithinbounds(mx,my,0,0,xquant,yquant)) {
              for (int j=0;j<angquant;j++) {
                if (myabs(myangle((float)p.x,(float)p.y)*angquant/2*pi-j)<angquant/8)
                  (map->pos[j][mx][my])++;
              }
            }
          }
          /* for (int i=-spacerad;i<=spacerad;i++) {
               int h=(int)sqrt(spacerad*spacerad-i*i);
               for (int j=-h;j<=h;j++) {
                 int mx=x+i;
                 int my=y+j;
                 if (arewithinbounds(mx,my,0,0,xquant,yquant)) {
                   for (int j=0;j<angquant;j++) {
                     (map->pos[j][mx][my])++;
                   }
                 }
               }
             }*/
          
        }
      }
    }
    printf("\n");
    return map;
  }
  
  Map2d<float> *RGBmp::getedges(int maskrad) {
    // Let's find the edges!
    Map2d<float> *hc=new Map2d<float>(width,height,&gimmezerofloat);
    Map2d<float> *vc=new Map2d<float>(width,height,&gimmezerofloat);
    Map2d<float> *change=new Map2d<float>(width,height,&gimmezerofloat);
    Map2d<float> *angs=new Map2d<float>(width,height,&gimmezerofloat);
    printf("Got here!\n");
    for (int i=maskrad;i<width-maskrad;i++) {
      for (int j=maskrad;j<height-maskrad;j++) {
        float horchange=myRGB::difference(&bmp[i-1][j],&bmp[i+1][j]);
        float verchange=myRGB::difference(&bmp[i][j-1],&bmp[i][j+1]);
        float totalchange=sqrt(mysquare(horchange)+mysquare(verchange));
        float angchange=myangle(horchange,verchange);
        hc->pos[i][j]=horchange;
        vc->pos[i][j]=verchange;
        change->pos[i][j]=totalchange;
        if (totalchange>0.15)
          angs->pos[i][j]=angchange;
        // change->pos[i][j]=myrnd();
      }
    }
    
    return change;
  }
  
  void RGBmp::setrectangle(int left,int top,int wid,int hei,myRGB r) {
    for (int i=left;i<=left+wid;i++) {
      for (int j=top;j<=top+hei;j++) {
        if (arewithinbounds(i,j,0,0,width,height))
          bmp[i][j]=r;
      }
    }
  }
  
  RGBmp *RGBmp::smoothed(int rad) {
    RGBmp *s=new RGBmp(width,height);
    List<Pixel> ps=pixelsincircle(0,0,rad);    
    for (int i=0;i<s->width;i++) {
      for (int j=0;j<s->height;j++) {
        int c=0;
        V3d v=V3d(0,0,0);
        for (int k=1;k<=ps.len;k++) {
          int dx=i+ps.p2num(k)->x;
          int dy=j+ps.p2num(k)->y;
          if (inimage(dx,dy)) {
            c++;
            v=v+bmp[dx][dy];
          }
        }
        s->bmp[i][j]=(v/c).myrgb();
      }
    }
    return s;
  }
  
  int RGBmp::inimage(int x, int y) {
    
    if ((x>=0) && (x<width) && (y>=0) && (y<height))
      return 1;
    else
    return 0;
  }
  
  
  void RGBmp::thickline(Line2d l,myRGB r,float w) {
    thickline(l.a,l.b,r,w);
  }
  void RGBmp::thickline(V2d a,V2d b,myRGB r,float w) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a+p,a-p,b+p,r);
    filltri(a-p,b+p,b-p,r);
  }

  void RGBmp::thicklineinf(V2d a,V2d b,float w,myRGB r) {
    Line2d l=Line2d(a,b).expandedForever();
    l.clipbyrectangle(0,0,width-1,height-1);
    thickline(l,r,w);
  }

  void RGBmp::filltri(V2d a,V2d b,V2d c,myRGB r) { // inefficient
    intertri(a.x,a.y,b.x,b.y,c.x,c.y,r.v3d(),r.v3d(),r.v3d());
  }

  void RGBmp::line(Line2d l,myRGB r) {
    line(l.a,l.b,r);
  }
  void RGBmp::line(V2d a,V2d b,V3d v) {
    line(a.x,a.y,b.x,b.y,&v.rgb());
  }
  void RGBmp::line(V2d a,V2d b,myRGB r) {
    line(a.x,a.y,b.x,b.y,&r);
  }
  void RGBmp::line(Pixel a,Pixel b,myRGB r) {
    line(a.x,a.y,b.x,b.y,&r);
  }
  void RGBmp::line(int x1, int y1, int x2, int y2,myRGB *r) {

    Line2d l=Line2d(V2d(x1,y1),V2d(x2,y2));
    String s=Sformat("Clipping %s...\n",l.toString());
    l.clipbyrectangle(0,0,width-1,height-1);
//    printf("  got %s...\n",l.toString());
    x1=l.a.x;
    x2=l.b.x;
    y1=l.a.y;
    y2=l.b.y;

//    if (inbmp(x1,y1) && inbmp(x2,y2)) {
      List<Pixel> ps=Pixel::line(Pixel(x1,y1),Pixel(x2,y2));
      for (int i=1;i<=ps.len;i++) {
//        printf("Setting pixel %s\n",ps.num(i).toString());
        setpixel(ps.num(i),*r);
      }
      ps.freedom();
//    } else
//      printf("%sDidn't write line because got %s when clipped\n",s,l.toString());

    free(s);

/*
        // From Map2d
      int x,y,tmp;
//      printf("a%i %i \n",x1,x2);
      if (diff(x1,x2)>diff(y1,y2)) {
        if (x1>x2) {
          tmp=x1; x1=x2; x2=tmp;
          tmp=y1; y1=y2; y2=tmp;
        }
        for (x=x1; x<=x2; x++) {
          y=y1+(y2-y1)*(x-x1)/(x2-x1);
          setpixel(x,y,*r);
        }
      } else {
        if (diff(y1,y2)==0)
          setpixel(x1,y1,*r);
        else {
          if (y1>y2) {
            tmp=x1; x1=x2; x2=tmp;
            tmp=y1; y1=y2; y2=tmp;
          }
          for (y=y1; y<=y2; y++) {
            x=x1+(x2-x1)*(y-y1)/(y2-y1);
            setpixel(x,y,*r);
          }
        }
      }
      // printf("Drawing line %i %i %i %i\n",x1,y1,x2,y2);*/


/* Original for RGBmp
    if (x1==x2 && y1==y2) {
      bmp[x1][y1]=*r;
    } else {
      
      int x,y,tmp;
      
      // If line is more horizontal than vertical
      if (abs(x2-x1)>abs(y2-y1)) {
        if (x1>x2) {
          tmp=x1; x1=x2; x2=tmp;
          tmp=y1; y1=y2; y2=tmp;
        }
        for (x=x1; x<=x2; x++) {
          y=y1+(y2-y1)*(x-x1)/(x2-x1);
          if (inimage(x,y))
            bmp[x][y]=*r;
        }
      } else {
        // Otherwise, line is more vertical than horizontal
        if (y1>y2) {
          tmp=x1; x1=x2; x2=tmp;
          tmp=y1; y1=y2; y2=tmp;
        }
        for (y=y1; y<=y2; y++) {
          x=x1+(x2-x1)*(y-y1)/(y2-y1);
          if (inimage(x,y))
            bmp[x][y]=*r;
        }
      }
    }*/
  }
  
  
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
  
  boolean RGBmp::valid(int x,int y,Map2d<int> *chosen,myRGB *r,float tol) {
    if (!inimage(x,y))
      return false;
    if (chosen->pos[x][y]>0)
      return false;
    myRGB *t=&bmp[x][y];
    if (myRGB::distance(r,t)>=tol)
      return false;
    return true;
  }
  
  void RGBmp::explore(int x,int y,List<List<Pixel> > *l,Map2d<int> *chosen,int c,myRGB *r,float tol,int d) {
    // printf("%i ",d);
    if (valid(x,y,chosen,r,tol)) {
      myRGB *t;
      int left=x;
      t=&bmp[x][y];
      while (left>=0 && valid(left,y,chosen,t,tol)) {
        t=&bmp[left][y];
        left--;
      }
      left++;
      int rght=x;
      t=&bmp[x][y];
      while (rght<width && valid(rght,y,chosen,t,tol)) {
        t=&bmp[rght][y];
        rght++;
      }
      rght--;
      for (int i=left;i<=rght;i++) {
        chosen->pos[i][y]=c;
        // putpixel(screen,i*2,y*2,c);
        l->p2num(c)->add(newPixel(x,y));
      }
      for (int i=left;i<=rght;i++) {
        t=&bmp[i][y];
        explore(x,y-1,l,chosen,c,t,tol,d+1);
        explore(x,y+1,l,chosen,c,t,tol,d+1);
      }
    }
  }
  
  List<List<Pixel> > *RGBmp::getfilledregions(float tolerance) {
    List<List<Pixel> > *l=new List<List<Pixel> >(10000);
    Map2d<int> *chosen=new Map2d<int>(width,height,&gimmezeroint);
    int numchosen=0;
    for (int x=0;x<width;x++) {
      printf(">");
      for (int y=0;y<height;y++) {
        if (chosen->pos[x][y]==0) {
          numchosen++;
          l->add(List<Pixel>());
          // putpixel(screen,x,y,numchosen);
          explore(x,y,l,chosen,numchosen,&bmp[x][y],tolerance,0);
        }
      } 
    }
    printf("\n");
    return l;
  }
  
  
  List<List<Pixel> > *RGBmp::oldclassifyscattered(myRGB preferred,float tolerance,int scatrad) {
    
    List<List<Pixel> > *l=new List<List<Pixel> >(10000);
    Map2d<int> *chosen=new Map2d<int>(width,height,&gimmezeroint);
    List<Pixel> eq=List<Pixel>(10000);
    
    printf("Collecting pixels (%i)\n",width);
    for (int x=0;x<width;x++) {
      printf(".");
      for (int y=0;y<height;y++) {
        
        if (myRGB::distance(&preferred,&bmp[x][y])>tolerance)
          chosen->pos[x][y]=-1;
        else {
          
          List<Pixel> *m;
          
          if (chosen->pos[x][y]==0) {
            
            // Create new class with this pixel
            m=new List<Pixel>();
            l->add(*m); // Add to the main list
            m=l->p2num(l->len);
            m->add(newPixel(x,y));
            chosen->pos[x][y]=l->len;
            // Add other pixels in this class
            
          } else {
            
            // Get the pixel's class
            m=l->p2num(chosen->pos[x][y]);
            
          }
          
          // Scan all neighbouring pixels
          for (int dx=-scatrad;dx<=scatrad;dx++) { // Can change 0s to -scatrad and -h but still slow!
            int h=(int)sqrt(scatrad*scatrad-dx*dx);
            for (int dy=0;dy<=h;dy++) {
              
              if ((dx!=0 || dy!=0) &&
                arewithinbounds(x+dx,y+dy,0,0,width,height)) {
                
                if (myRGB::distance(&preferred,&bmp[x+dx][y+dy])>tolerance)
                  chosen->pos[x+dx][y+dy]=-1;
                else {
                  if (chosen->pos[x+dx][y+dy]==0) {
                    chosen->pos[x+dx][y+dy]=chosen->pos[x][y];
                    m->add(newPixel(x+dx,y+dy));
                  } else {
                    // Slows this down, speeds later up
                    if(chosen->pos[x+dx][y+dy]!=chosen->pos[x][y])
                      if (eq.find(newPixel(chosen->pos[x][y],chosen->pos[x+dx][y+dy]),&equal)==0)
                      eq.add(newPixel(chosen->pos[x][y],chosen->pos[x+dx][y+dy]));
                  }
                  
                }
              }
              
            }
          }
          
        }
        
      }
    }
    printf("\n");
    
    printf("Equivalences: %i\n",eq.len);
    
    // Process list to collect all equivalent lists
    printf("Collecting equivalent pixel lists together (%i)\n",l->len);
    for (int i=1;i<=l->len;i++) {
      
      printf("*");
      
      if (l->num(i).len>0) {
        
        // Find list of classes equivalent to i
        List<int> le=List<int>();
        le.add(i);
        boolean change=-1;
        while (change!=0) {
          change=0;
          for (int j=1;j<=eq.len;j++) {
            int k=le.find(eq.num(j).x);
            if (k>0)
              if (le.find(eq.num(j).y)<1) {
              change=j;
              le.add(eq.num(j).y);
              eq.removenum(j);
              j--;
            }
            k=le.find(eq.num(j).y);
            if (k>0)
              if (le.find(eq.num(j).x)<1) {
              change=j;
              le.add(eq.num(j).x);
              eq.removenum(j);
              j--;
            }
          }
        }
        
        // printf("Set %i has %i equivalences.\n",i,le.len);
        
        // Collect all others into i
        for (int j=2;j<=le.len;j++) {
          l->p2num(i)->add(l->num(le.num(j)));
          // Once collected, set to null
          l->p2num(le.num(j))->len=0;
        }
      }
    }
    printf("\n");
    
    for (int i=1;i<=l->len;i++) {
      if (l->p2num(i)->len==0) {
        l->removenum(i);
        i--;
      }
    }
    
    return l;
  }
  
  
  
  
  List<List<Pixel> > *RGBmp::classifyallscattered(float tolerance,int scatrad) {
    
    List<List<Pixel> > *l=new List<List<Pixel> >(10000);
    Map2d<int> *chosen=new Map2d<int>(width,height,&gimmezeroint);
    List<List<int> > eq=List<List <int> >(10000);
    
    printf("Collecting pixels (%i)\n",width);
    for (int x=0;x<width;x++) {
      printf(".");
      for (int y=0;y<height;y++) {
        
        List<Pixel> *m;
        
        if (chosen->pos[x][y]==0) {
          
          // Create new class with this pixel
          m=new List<Pixel>();
          l->add(*m); // Add to the main list
          eq.add(List<int>());
          m=l->p2num(l->len);
          m->add(newPixel(x,y));
          chosen->pos[x][y]=l->len;
          // Add other pixels in this class
          
        } else {
          
          // Get the pixel's class
          m=l->p2num(chosen->pos[x][y]);
          
        }
        
        // Scan all neighbouring pixels
        for (int dx=-scatrad;dx<=scatrad;dx++) { // Can change 0s to -scatrad and -h but still slow!
          int h=(int)sqrt(scatrad*scatrad-dx*dx);
          for (int dy=0;dy<=h;dy++) {
            
            if ((dx!=0 || dy!=0) &&
              arewithinbounds(x+dx,y+dy,0,0,width,height)) {
              
              if (myRGB::distance(&bmp[x][y],&bmp[x+dx][y+dy])<tolerance) {
                if (chosen->pos[x+dx][y+dy]==0) {
                  chosen->pos[x+dx][y+dy]=chosen->pos[x][y];
                  m->add(newPixel(x+dx,y+dy));
                } else {
                  // Slows this down, speeds later up
                  if(chosen->pos[x+dx][y+dy]!=chosen->pos[x][y]) {
                    int q=min(chosen->pos[x+dx][y+dy],chosen->pos[x][y]);
                    int r=max(chosen->pos[x+dx][y+dy],chosen->pos[x][y]);
                    if (eq.p2num(q)->find(r)==0)
                      eq.p2num(q)->add(r);
                  }
                }
                
              }
            }
            
          }
          
        }
        
      }
    }
    printf("\n");
    
    // printf("Equivalences: %i\n",eq.len);
    
    // Process list to collect all equivalent lists
    printf("Collecting equivalent pixel lists together (%i)\n",l->len);
    for (int i=1;i<=l->len;i++) {
      
      printf("*");
      
      if (l->num(i).len>0) {
        
        // Find list of classes equivalent to i
        List<int> le=List<int>();
        le.add(i);
        boolean change=-1;
        while (change!=0) {
          change=0;
          for (int f=1;f<=le.len;f++) {
            List<int> *scan=eq.p2num(le.num(f));
            for (int j=1;j<=scan->len;j++) {
              int k=scan->num(j);
              if (le.find(k)<1) {
                change=j;
                le.add(k);
              }
            }
            scan->len=0;
          }
        }
        
        // printf("Set %i has %i equivalences.\n",i,le.len);
        
        // Collect all others into i
        for (int j=2;j<=le.len;j++) {
          l->p2num(i)->add(l->num(le.num(j)));
          // Once collected, set to null
          l->p2num(le.num(j))->len=0;
        }
        
      }
    }
    printf("\n");
    
    for (int i=1;i<=l->len;i++) {
      if (l->p2num(i)->len==0) {
        l->removenum(i);
        i--;
      }
    }
    
    return l;
  }
  
  
  
  
  
  
  List<List<Pixel> > *RGBmp::classifyscattered(myRGB preferred,float tolerance,int scatrad) {
    
    List<List<Pixel> > *l=new List<List<Pixel> >(10000);
    Map2d<int> *chosen=new Map2d<int>(width,height,&gimmezeroint);
    List<List<int> > eq=List<List <int> >(10000);
    
    printf("Collecting pixels (%i)\n",width);
    for (int x=0;x<width;x++) {
      printf(".");
      for (int y=0;y<height;y++) {
        
        if (myRGB::distance(&preferred,&bmp[x][y])>tolerance)
          chosen->pos[x][y]=-1;
        else {
          
          List<Pixel> *m;
          
          if (chosen->pos[x][y]==0) {
            
            // Create new class with this pixel
            m=new List<Pixel>();
            l->add(*m); // Add to the main list
            eq.add(List<int>());
            m=l->p2num(l->len);
            m->add(newPixel(x,y));
            chosen->pos[x][y]=l->len;
            // Add other pixels in this class
            
          } else {
            
            // Get the pixel's class
            m=l->p2num(chosen->pos[x][y]);
            
          }
          
          // Scan all neighbouring pixels
          for (int dx=-scatrad;dx<=scatrad;dx++) { // Can change 0s to -scatrad and -h but still slow!
            int h=(int)sqrt(scatrad*scatrad-dx*dx);
            for (int dy=0;dy<=h;dy++) {
              
              if ((dx!=0 || dy!=0) &&
                arewithinbounds(x+dx,y+dy,0,0,width,height)) {
                
                if (myRGB::distance(&preferred,&bmp[x+dx][y+dy])>tolerance)
                  chosen->pos[x+dx][y+dy]=-1;
                else {
                  if (chosen->pos[x+dx][y+dy]==0) {
                    chosen->pos[x+dx][y+dy]=chosen->pos[x][y];
                    m->add(newPixel(x+dx,y+dy));
                  } else {
                    // Slows this down, speeds later up
                    if(chosen->pos[x+dx][y+dy]!=chosen->pos[x][y]) {
                      int q=min(chosen->pos[x+dx][y+dy],chosen->pos[x][y]);
                      int r=max(chosen->pos[x+dx][y+dy],chosen->pos[x][y]);
                      if (eq.p2num(q)->find(r)==0)
                        eq.p2num(q)->add(r);
                    }
                  }
                  
                }
              }
              
            }
          }
          
        }
        
      }
    }
    printf("\n");
    
    // printf("Equivalences: %i\n",eq.len);
    
    // Process list to collect all equivalent lists
    printf("Collecting equivalent pixel lists together (%i)\n",l->len);
    for (int i=1;i<=l->len;i++) {
      
      printf("*");
      
      if (l->num(i).len>0) {
        
        // Find list of classes equivalent to i
        List<int> le=List<int>();
        le.add(i);
        boolean change=-1;
        while (change!=0) {
          change=0;
          for (int f=1;f<=le.len;f++) {
            List<int> *scan=eq.p2num(le.num(f));
            for (int j=1;j<=scan->len;j++) {
              int k=scan->num(j);
              if (le.find(k)<1) {
                change=j;
                le.add(k);
              }
            }
            scan->len=0;
          }
        }
        
        // printf("Set %i has %i equivalences.\n",i,le.len);
        
        // Collect all others into i
        for (int j=2;j<=le.len;j++) {
          l->p2num(i)->add(l->num(le.num(j)));
          // Once collected, set to null
          l->p2num(le.num(j))->len=0;
        }
        
      }
    }
    printf("\n");
    
    for (int i=1;i<=l->len;i++) {
      if (l->p2num(i)->len==0) {
        l->removenum(i);
        i--;
      }
    }
    
    return l;
  }
  
  
  
  
  
  
  
  
  
  
  
  void RGBmp::display() {
    #ifdef ALLEGRO
    display(640,480);
    #endif
  }

  void RGBmp::display(int winwid,int winhei) {
    #ifdef ALLEGRO
  #ifndef NOGRAPHICS
    
    allegrosetup(winwid,winhei,24);
    PALETTE pal;
    generate_332_palette(pal);
    set_palette(pal);
    
    // Find aspect
    
    int left,top,wid,hei;
    float scale;
    
    if (width/winwid>height/winhei) {
      left=0;
      scale=(float)winwid/width;
      top=(int)(winhei/2-height*scale/2);
      wid=winwid;
      hei=(int)(height*scale);
    } else {
      top=0;
      scale=(float)winhei/height;
      left=(int)(winwid/2-width*scale/2);
      wid=(int)(width*scale);
      hei=winhei;
    }
    
    // printf("t%i l%i s%f w%i h%i\n",top,left,scale,wid,hei);
    
    // for(int y=0;y<height;y+=8)
    // for (int x=0;x<width;x+=8)
    // printf("%i %i %i     ",bmp[x][y].r,bmp[x][y].g,bmp[x][y].b);
    // exit(0);
    
    for (int y=0;y<hei;y++)
    for (int x=0;x<wid;x++) {
      int dx=(int)((float)x/scale);
      int dy=(int)((float)y/scale);
      // printf("%i %i   ",dx,dy);
      int r=bmp[dx][dy].r;
      int g=bmp[dx][dy].g;
      int b=bmp[dx][dy].b;
      // printf("%i %i %i     ",r,g,b);
      // r=255*x/wid;
      // g=255*y/hei;
      putpixel(screen,left+x,top+y,makecol(r,g,b));
    }
    
    while (!key[KEY_ESC] && !key[KEY_SPACE]) { 
    }
    
  #endif
  #endif
  }
  void RGBmp::writetoscreen() {
    #ifdef ALLEGRO
    for (int y=0;y<height;y++)
    for (int x=0;x<width;x++) {
      int r=bmp[x][y].r;
      int g=bmp[x][y].g;
      int b=bmp[x][y].b;
      putpixel(screen,x,y,makecol(r,g,b));
    }
    #endif
  }
  
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
   RGBmp::RGBmp(String fname) { //- depreceated, use static RGBmp::readfile(String fname)
    *this=*RGBmp::readfile(fname);
  }

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
  
  void RGBmp::writebmp24(char *fname) {
    
    FILE *fp;
    int fp_type;
    int bperlin;
    int   i,j,c,padb;
    int xdim, ydim;
    
    #ifndef QUIET
      printf("Writing %s width %i height %i\n",fname,width,height);
    #endif
    
    fp = fopen(fname,"w");
    
    xdim = width;
    ydim = height;
    
    bperlin = ((xdim * 24 + 31) / 32) * 4;   
    putc('B', fp);  putc('M', fp);           
    
    
    i = 14 +                     
    40 +                  
    bperlin * ydim;         
    putint(fp, i);
    
    putshort(fp, 0);         
    putshort(fp, 0);         
    putint(fp, 14 + 40);        /* offset from BOfile to BObitmap */ 
    
    putint(fp, 40);             /* biSize: size of bitmap info header */
    putint(fp, xdim);           /* biWidth */
    putint(fp, ydim);           /* biHeight */
    putshort(fp, 1);            /* biPlanes:  must be '1' */
    putshort(fp, 24);                   /* biBitCount: 1,4,8, or 24 */
    putint(fp, 0);              /* biCompression:  BI_RGB, BI_RLE8 or BI_RLE4 */
    putint(fp, bperlin*ydim);   /* biSizeImage:  size of raw image data */
    putint(fp, 0);              /* biXPelsPerMeter: (75dpi * 39" per meter) */
    putint(fp, 0);              /* biYPelsPerMeter: (75dpi * 39" per meter) */
    putint(fp, 0);              /* biClrUsed: # of colors used in cmap */
    putint(fp, 0);              /* biClrImportant: same as above */
    
    padb = (xdim*9)%4;  /* # of pad bytes to write at EOscanline */
    
    // printf("Height %i\n",height);
    for (i = ydim-1; i >= 0; i--) {
      // printf("Writing line %i of %i\n",i,height);
      for (j = 0; j < xdim; j++) {
        putc((char)(bmp[j][i].b), fp);
        putc((char)(bmp[j][i].g), fp);
        putc((char)(bmp[j][i].r), fp);
      }
      
      for (j=0; j<padb; j++) putc(0, fp);
    }
    
    fclose(fp);
    
  }
  
   RGBmp *RGBmp::readfile(String s) {
    bool uzed=unzipfile(s);
    if (uzed)
      printf("Unzipped file. Will zip it up soon\n");
    String ext=Sright(s,4);
    RGBmp *r=NULL;
    if (Seq(ext,".bmp")) {
      r=readbmp(s);
    }
    if (Seq(ext,".ppm"))
      r=readppm(s);
    if (Seq(ext,".pgm")) {
      Map2d<float> *m=Map2d<float>::readfile(s);
      // helloworld();
      // m->display();
      r=RGBmp::toRGBmp(m);
    }
    if (uzed) {
      printf("Re-zipping file...\n");
      zipfile(s);
      printf("done\n");
    }
    if (r!=NULL)
      return r;
    RGBmp *trial;
    trial=readbmp(Sconc(s,".bmp"));
    if (trial!=NULL)
      return trial;
    trial=readppm(Sconc(s,".ppm"));
    if (trial!=NULL)
      return trial;
    error("%s not found (.pgm must be given explicitly).",s);
  }
  
   RGBmp *RGBmp::readppm(String filename) {
    unsigned char *data;
    int *height,*width;
    int x,y,i,datasize,c;
    int typeno,maxvalue;
    char dumchar;
    FILE *fp;
    RGBmp *n;
    
    printf("Got here!\n");
    
    if(!(fp=fopen(filename,"r"))){
      fprintf(stderr,"[readppm] error opening %s\n",filename);
      return NULL;
    }
    
    if(fscanf(fp,"P%d\n",&typeno)!=1) {
      fprintf(stderr,"[readppm] error reading typeno\n");
      return NULL;
    }
    
    while(fscanf(fp,"#%c",&dumchar)) 
    while(fscanf(fp,"%c",&dumchar),dumchar!='\n');
    
    if(fscanf(fp,"%d %d\n",width,height)!=2) {
      fprintf(stderr,"[readppm] error reading width and height\n");
      return NULL;
    }
    
    if(fscanf(fp,"%d",&maxvalue)!=1) {
      fprintf(stderr,"[readppm] error reading maxvalue\n");
      return NULL;
    }
    /* fix 14.10.97: read newline separately to avoid skipping bytes of
       image data corresponding to form-feeds
    */
    fgetc(fp);
    
    datasize=(*width)*(*height)*3;
    switch(typeno) {
      case 6:        
      // p = uchar_array3D(*width, *height);
      n=new RGBmp(*width,*height);
      if(!(data=(unsigned char *)calloc(datasize,sizeof(unsigned char)))){
        fprintf(stderr,"[readppm] calloc error 3\n");
        return NULL;
      }
      if(fread(data,sizeof(unsigned char),datasize,fp)!=datasize) {
        fprintf(stderr,"[readppm] error reading data\n");
        exit(0);
      }
      
      c=0;
      for(y=0;y<(*height);y++)
      for(x=0;x<(*width);x++)
      n->bmp[x][y]=myRGB(data[c++],data[c++],data[c++]);
      
      free(data);
      break;
      
      default:
      fprintf(stderr,"[readppm] sorry ppm type not implemented\n");
      return NULL;
    }
    
    fclose(fp);
    
    n->height=*height;
    n->width=*width;
    
    return n;
    
  }
  
   RGBmp *RGBmp::readbmp(String fname) {
    RGBmp *r;
    r=angusreadbmp8(fname);
    if (r==NULL)
      r=angusreadbmp24(fname);
    if (r==NULL)
      error("RGBmp: Failed to read bitmap %s",fname);
    return r;
  }
  
  void RGBmp::writefile(String fname) {
    #ifndef QUIET
      printf("Writing RGB file %s (%ix%i)\n",fname,width,height);
    #endif
    bool zip=Sequiv(Sright(fname,3),".gz");
    if (zip)
      fname=Sleft(fname,Slen(fname)-3);
    String ext=Sright(fname,4);
    if (Seq(ext,".bmp"))
      anguswritebmp24(fname,this);
    // if (Seq(ext,".ppm"))
      // writeppm(fname,this);
    if (Seq(ext,".pgm"))
      getgreyscale()->writefile(fname);
    if (zip)
      zipfile(fname);
  }
  
  Map2d<float> *RGBmp::getgreyscale() { // Should really use YUV
    Map2d<float> *n=new Map2d<float>(width,height);
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++)
    n->pos[i][j]=V3d::mod(bmp[i][j].v3d())/V3d(1,1,1).mod();
    return n;
  }
  Map2d<V3d> *RGBmp::getv3ds() {
    Map2d<V3d> *n=new Map2d<V3d>(width,height);
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++)
    n->pos[i][j]=bmp[i][j].v3d();
    return n;
  }
  
  Map2d<RGBHistogram> *RGBmp::getlocalhistmap(int histrad,float scale) {
    Map2d<RGBHistogram> *lh=new Map2d<RGBHistogram>(width*scale,height*scale);
    for (int x=0;x<lh->width;x=x+1) {
      printf("-");
      for (int y=0;y<lh->height;y=y+1) {
        // RGBHistogram *r=new RGBHistogram(10);
        for (int i=-histrad;i<=histrad;i++) {
          int h=sqrt(histrad*histrad-i*i);
          for (int j=-h;j<=h;j++) {
            int dx=(float)x/scale+i;
            int dy=(float)y/scale+j;
            if (inimage(dx,dy))
              lh->pos[x][y].addocc(&bmp[dx][dy]);
          }
        }
        lh->pos[x][y]=*lh->pos[x][y].normsmoothed(1);
        // lh->pos[x][y]=*r;
      }
    }
    printf("]\n");
    return lh;
  }
  

   RGBmp *RGBmp::toRGBmp(Map2d<float> *m) {
    RGBmp *n=new RGBmp(m->width,m->height);
    float smallest=m->smallest();
    float largest=m->largest();
    float scale=255.0/(largest-smallest);
    for (int i=0;i<m->width;i++)
    for (int j=0;j<m->height;j++) {
      int c=(tofloat(m->pos[i][j])-smallest)*scale;
      n->bmp[i][j]=myRGB(c,c,c);
    }
    return n;
  }

  template <class Obj>
   RGBmp *RGBmp::toRGBmp(Map2d<Obj> *m) {
    Map2d<float> *f=m->getgreyscale(); // m->applyfn(&tofloat);
    return toRGBmp(f);
  }


  void RGBmp::setupgrid(Map2d<V2d> *grid,int left,int top,int size,V2d hvp,V2d vvp) {
    int hsize=size/2;
    V2d tl=grid->getpos(left,top);
    V2d tr=grid->getpos(left+size,top);
    V2d br=grid->getpos(left+size,top+size);
    V2d bl=grid->getpos(left,top+size);
    V2d cen=Line2d(tl,br).intersect(Line2d(tr,bl));
    V2d lm=Line2d(hvp,cen).intersect(Line2d(tl,bl));
    V2d rm=Line2d(hvp,cen).intersect(Line2d(tr,br));
    V2d tm=Line2d(vvp,cen).intersect(Line2d(tl,tr));
    V2d bm=Line2d(vvp,cen).intersect(Line2d(bl,br));
    grid->setpos(left+hsize,top+hsize,cen);
    grid->setpos(left+hsize,top,tm);
    grid->setpos(left,top+hsize,lm);
    grid->setpos(left+size,top+hsize,rm);
    grid->setpos(left+hsize,top+size,bm);
    if (hsize>1) {
      setupgrid(grid,left,top,hsize,hvp,vvp);
      setupgrid(grid,left+hsize,top,hsize,hvp,vvp);
      setupgrid(grid,left,top+hsize,hsize,hvp,vvp);
      setupgrid(grid,left+hsize,top+hsize,hsize,hvp,vvp);
    }
  }
  // This method is accurate excepting estimate of aspect ratio
  RGBmp *RGBmp::recoverquadrilateral(V2d a,V2d b,V2d c,V2d d,int imwidth) {

    V2d hvp=Line2d(a,b).intersect(Line2d(c,d));
    V2d vvp=Line2d(a,d).intersect(Line2d(b,c));

    // Initially generate a grid of mapping from output rectangle coords
    // to input image coords
    // Recursively generated from the given corners and at each level
    // the centre defined by the intersections of the diagonals
    int max=32; // must be power of 2
    Map2d<V2d> grid=Map2d<V2d>(max+1,max+1,V2d(0,0));
    grid.setpos(0,0,a);
    grid.setpos(max,0,b);
    grid.setpos(max,max,c);
    grid.setpos(0,max,d);
    setupgrid(&grid,0,0,max,hvp,vvp);

    // Now generate output image
    // Usual pathetic aspect ratio estimation
     float aspect=sqrt(V2d::dist(a,b)+V2d::dist(c,d))/sqrt(V2d::dist(a,d)+V2d::dist(c,b));
//    float aspect=sqrt(V2d::dist(a,b)*V2d::dist(c,d))/sqrt(V2d::dist(a,d)*V2d::dist(c,b));
    RGBmp *out=new RGBmp(imwidth,imwidth/aspect);
    for (int i=0;i<out->width;i++)
    for (int j=0;j<out->height;j++) {
      // Find grid position corresponding to this position in the rectangle
      float gx=(float)max*(float)i/(float)out->width;
      float gy=(float)max*(float)j/(float)out->height;
      // Interpolate grid position to get image position
      int gxi=(int)gx;
      int gyi=(int)gy;
      float e=gx-gxi;
      float s=gy-gyi;
      float w=1.0-e;
      float n=1.0-s;
      V2d tl=grid.getpos(gxi,gyi);
      V2d tr=grid.getpos(gxi+1,gyi);
      V2d br=grid.getpos(gxi+1,gyi+1);
      V2d bl=grid.getpos(gxi,gyi+1);
      V2d realpos=tl*n*w+tr*n*e+br*s*e+bl*s*w;
      // Interpolate image pixels
      out->setpos(i,j,getinter(realpos.x,realpos.y));
    }
    return out;

  }



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

    myRGB RGBmp::getinter(float x,float y) {
      int lx=(int)x;
      int ly=(int)y;
      float e=x-lx;
      float s=y-ly;
      float w=1.0-e;
      float n=1.0-s;
      if (inbmp(x,y) && inbmp(x+1,y+1))
        return n*w*bmp[lx][ly]+n*e*bmp[lx+1][ly]+s*w*bmp[lx][ly+1]+s*e*bmp[lx+1][ly+1];
      else
        return myRGB::white; // (myrnd(),myrnd(),myrnd());
    }


RGBmp *RGBmp::recoverquad(V2d a,V2d b,V2d c,V2d d,int imwidth) {
  float aspect=sqrt(V2d::dist(a,b)+V2d::dist(c,d))/sqrt(V2d::dist(a,d)+V2d::dist(c,b));
  RGBmp *out=new RGBmp(imwidth,imwidth/aspect);
  for (int i=0;i<out->width;i++)
  for (int j=0;j<out->height;j++) {
    float e=(float)i/(float)out->width;
    float s=(float)j/(float)out->height;
    float w=1.0-e;
    float n=1.0-s;
    V2d p=a*n*w+b*n*e+c*s*e+d*s*w;
    out->bmp[i][j]=getinter(p.x,p.y);
  }
  return out;
}

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

  RGBmp *RGBmp::linearrecoverquad(List<V2d> *ls,float scale,int imwid) {
    List<V2d> l;
    for (int i=1;i<=4;i++)
      l.add(ls->num(i)*scale);
    // Which is the most NW?
    float nw=l.num(1).x+l.num(1).y;
    int w=1;
    for (int i=2;i<=4;i++) {
      float tnw=l.num(i).x+l.num(i).y;
      if (tnw<nw) {
        nw=tnw;
        w=i;
      }
    }
    int dir=( l.wrapnum(w-1).y<l.wrapnum(w+1).y ? -1 : +1 );
    List<V2d> n;
    for (int i=0;i<4;i++)
      n.add(l.wrapnum(w+dir*i));
    return recoverquad(n.num(1),n.num(2),n.num(3),n.num(4),imwid);
  }

  // The 3d quad recovery bodge
  // I think the maths works, but it's not confirmed.
  // The results are certainly comparable to those from recoverquadrilateral
  RGBmp *RGBmp::recoverquad(List<V2d> *ls,float scale,int imwid) {
    printf("Recovering quad.\n");
    for (int i=1;i<=ls->len;i++)
      printf("%s,",ls->num(i).toString());
    printf("\n");
    List<V2d> l;
    for (int i=1;i<=4;i++)
      l.add(ls->num(i)*scale-V2d(width/2.0,height/2.0));
    // Which is the most NW?
    float nw=l.num(1).x+l.num(1).y;
    int w=1;
    for (int i=2;i<=4;i++) {
      float tnw=l.num(i).x+l.num(i).y;
      if (tnw<nw) {
        nw=tnw;
        w=i;
      }
    }
    int dir=( l.wrapnum(w-1).y<l.wrapnum(w+1).y ? -1 : +1 );
    List<V2d> n;
    for (int i=0;i<4;i++)
      n.add(l.wrapnum(w+dir*i));

    V2d ia=n.num(1);
    V2d ib=n.num(2);
    V2d ic=n.num(3);
    V2d id=n.num(4);
    V2d vh=Line2d(ia,ib).intersection(Line2d(id,ic));
    V2d vv=Line2d(ia,id).intersection(Line2d(ib,ic));
    V3d ph=V3d(vh.x,vh.y,1);
    V3d pv=V3d(vv.x,vv.y,1);
    printf("ph=%s pv=%s\n",ph.toString(),pv.toString());
    V3d a=V3d(ia.x,ia.y,1);
    V3d b=Line3d(a,a+ph).intersect(Line3d(V3d::o,V3d(ib.x,ib.y,1)));
    printf("%s %s\n",ib.toString(),b.perpproj().toString());
    V3d d=Line3d(a,a+pv).intersect(Line3d(V3d::o,V3d(id.x,id.y,1)));
    printf("%s %s\n",id.toString(),d.perpproj().toString());
    V3d c=Line3d(d,d+ph).intersect(Line3d(V3d::o,V3d(ic.x,ic.y,1)));
    printf("%s %s\n",ic.toString(),c.perpproj().toString());
    V3d co=Line3d(b,b+pv).intersect(Line3d(V3d::o,V3d(ic.x,ic.y,1)));
    printf("%s %s\n",ic.toString(),c.perpproj().toString());
//    if (!(co==c))
//      printf("General recovery failure!\n");

    float aspect=sqrt(V3d::dist(a,b)+V3d::dist(c,d))/sqrt(V3d::dist(a,d)+V3d::dist(c,b));
//    printf("%s=%s %s=%s %s=%s %s=%s\n",ia.toString(),a.perpproj().toString(),ib.toString(),b.perpproj().toString(),ic.toString(),c.perpproj().toString(),id.toString(),d.perpproj().toString());
    RGBmp *out=new RGBmp(imwid,imwid/aspect);
    for (int i=0;i<out->width;i++)
    for (int j=0;j<out->height;j++) {
      float e=(float)i/(float)out->width;
      float s=(float)j/(float)out->height;
      float w=1.0-e;
      float n=1.0-s;
      V3d p=a*n*w+b*n*e+c*s*e+d*s*w;
      V2d ip=V2d(width/2.0,height/2.0)+p.perpproj();
      out->bmp[i][j]=getinter(ip.x,ip.y);
    }
    return out;
  }

  void RGBmp::setpos(int x,int y,V3d c) {
    setpos(x,y,c.rgb());
  }

  void RGBmp::setpos(int x,int y,myRGB c) {
    if (inbmp(x,y))
      bmp[x][y]=c;
  }

  void RGBmp::setpixel(int x,int y,V3d c) {
    setpos(x,y,c);
  }

  void RGBmp::setpixel(int x,int y,myRGB c) {
    if (inbmp(x,y)) {
//      printf("Setting pixel %i,%i to %s\n",x,y,c.toString());
//      printf("Setting %i %i (%i,%i)\n",x,y,width,height);
      bmp[x][y]=c;
    }
  }

  void RGBmp::setpixel(Pixel p,myRGB c) {
    setpixel(p.x,p.y,c);
  }

  void RGBmp::setpos(Pixel p,myRGB c) {
    setpixel(p.x,p.y,c);
  }

  void RGBmp::intvline(int x,int ya,int yb,V3d ca,V3d cb) {
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
      V3d c=pull(ca,atob,cb);
      setpixel(x,i,c);
    }
  }

  void RGBmp::intvline(int x,int ya,int yb,V3d ca,V3d cb,Texture *t) {
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
      myRGB c=t->colat(pull(ca,atob,cb));
      setpixel(x,i,c);
    }
  }

  void RGBmp::intertri(int xa,int ya,int xb,int yb,int xc,int yc,V3d ca,V3d cb,V3d cc,Texture *t) {
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
      intvline(x,ylow,yhei,ca+xthruab*(cb-ca),ca+xthruac*(cc-ca),t);
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
      intvline(x,ylow,yhei,cb+xthrubc*(cc-cb),ca+xthruac*(cc-ca),t);
    }
  }


  void RGBmp::intertri(int xa,int ya,int xb,int yb,int xc,int yc,myRGB ca,myRGB cb,myRGB cc) {
    intertri(xa,ya,xb,yb,xc,yc,ca.v3d(),cb.v3d(),cc.v3d());
  }

  void RGBmp::intertri(V2d a,V2d b,V2d c,myRGB ca,myRGB cb,myRGB cc) {
    intertri(a.x,a.y,b.x,b.y,c.x,c.y,ca.v3d(),cb.v3d(),cc.v3d());
  }

  void RGBmp::intertri(int xa,int ya,int xb,int yb,int xc,int yc,V3d ca,V3d cb,V3d cc) {
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
      V3d cab=pull(ca,xthruab,cb);
      V3d cac=pull(ca,xthruac,cc);
      intvline(x,ylow,yhei,cab,cac);
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
      V3d cbc=pull(cb,xthrubc,cc);
      V3d cac=pull(ca,xthruac,cc);
      intvline(x,ylow,yhei,cbc,cac);
    }
  }

  void RGBmp::clear() {
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++)
      bmp[i][j]=myRGB::black;
  }

  void RGBmp::clear(myRGB c) {
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++)
      bmp[i][j]=c;
  }

  void RGBmp::clear(V3d c) {
    clear(c.rgb());
  }

  bool RGBmp::inbmp(int x,int y) {
    return (x>=0 && x<width && y>=0 && y<height);
  }

  RGBmp *RGBmp::scaletowidth(int w) {
    return scale(this,(float)w/(float)width);
  }

  RGBmp *RGBmp::scaledby(float s) {
    return scale(this,s);
  }

  myRGB RGBmp::getposinterpolate(float x,float y) {
    int l=x;
    int t=y;
    float i=x-(float)l;
    float j=y-(float)t;
	 /* A really disgusting hack for gentestimg
	  * due to black bordering.
	  */
	 if (l>=width-5) {
		 l=width-7;
		 i=0;
	 }
	 if (t>=height-5) {
		 t=height-7;
		 j=0;
	 }
    return getpos( l ,t  ) * (1.0-i) * (1.0-j) +
           getpos(l+1,t  ) *   (i)   * (1.0-j) +
           getpos( l ,t+1) * (1.0-i) *   (j) +
           getpos(l+1,t+1) *   (i)   *   (j);
  }


   RGBmp *RGBmp::scale(RGBmp *in,float scale) {
  RGBmp *out=new RGBmp(in->width*scale,in->height*scale);
  printf("hello\n");
  for (int i=0;i<out->width;i++)
  for (int j=0;j<out->height;j++)
  if (scale<1.0) { // Scale image down by averaging
    int l=i*in->width/out->width;
    int t=j*in->height/out->height;
    int r=(i+1)*in->width/out->width;
    int b=(j+1)*in->height/out->height;
    V3d c=V3d::o;
    int cnt=0;
    for (int x=l;x<r;x++)
    for (int y=t;y<b;y++) {
      if (x<in->width && y<in->height) {
        c=c+in->getpos(x,y).v3d();
        cnt++;
      }
    }
    out->setpos(i,j,(c/(float)cnt).myrgb());
  } else { // Scale image up by interpolating
    out->setpos(i,j,in->getposinterpolate(i/scale,j/scale));
  }
  return out;
  }

  RGBmp *RGBmp::inverted() {
    RGBmp *n=new RGBmp(width,height);
    for (int i=0;i<width;i++)
      for (int j=0;j<height;j++)
        n->bmp[i][j]=(V3d(1,1,1)-bmp[i][j].v3d()).myrgb();
    return n;
  }

   RGBmp *RGBmp::fromv3ds(Map2d<V3d> *m) {
    RGBmp *r=new RGBmp(m->width,m->height);
    for (int i=0;i<m->width;i++)
      for (int j=0;j<m->height;j++)
        r->bmp[i][j]=m->pos[i][j].rgb();
    return r;
  }

  void RGBmp::line(Line2d *l,myRGB c) {
    line(l->a,l->b,c);
  }
  void RGBmp::sthickline(V2d a,V2d b,float w,myRGB c) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a+p,a-p,b+p,c);
    filltri(a-p,b+p,b-p,c);
  }
  void RGBmp::thickline(V2d a,V2d b,float w,myRGB c) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a-a2b+p,a-a2b-p,b+a2b+p,c);
    filltri(a-a2b-p,b+a2b+p,b+a2b-p,c);
  }
  void RGBmp::thickline(int xa,int ya,int xb,int yb,float w,myRGB c) {
    thickline(V2d(xa,ya),V2d(xb,yb),w,c);
  }
  void RGBmp::thickline(Line2d l,int w,myRGB c) {
    thickline(l.a,l.b,w,c);
  }
  void RGBmp::thicklinev(V2d a,V2d b,float w,myRGB c) {
    V2d p=V2d(w/2.0,0);
    V2d a2b=V2d(0,w/2.0)*mysgn(b.y-a.y);
    filltri(a-a2b+p,a-a2b-p,b+a2b+p,c);
    filltri(a-a2b-p,b+a2b+p,b+a2b-p,c);
  }
  void RGBmp::thicklineh(V2d a,V2d b,float w,myRGB c) {
    V2d p=V2d(0,w/2.0+1);
    V2d a2b=V2d(w/2.0-1,0)*mysgn(b.x-a.x);
    filltri(a-a2b+p,a-a2b-p,b+a2b+p,c);
    filltri(a-a2b-p,b+a2b+p,b+a2b-p,c);
  }
  void RGBmp::rthickline(V2d a,V2d b,float w,myRGB c) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a+p,a-p,b+p,c);
    filltri(a-p,b+p,b-p,c);
    filledcircle(a,w/2.0,c);
    filledcircle(b,w/2.0,c);
  }
void RGBmp::opencircle(Pixel p,float r,myRGB c) {
  opencircle(p.x,p.y,r,c);
}
void RGBmp::opencircle(V2d v,float r,myRGB c) {
  opencircle(v.x,v.y,r,c);
}
void RGBmp::opencircle(int x,int y,float r,myRGB c) {
  V2d last=V2d(x,y+r);
  int steps=4+r/2;
  for (int i=1;i<=steps;i++) {
    float a=2.0*pi*(float)i/(float)steps;
    V2d next=V2d(x+r*sin(a),y+r*cos(a));
    line(last,next,c);
    last=next;
  }
}
  void RGBmp::filledcircle(int cx,int cy,float r,myRGB c) {
    int x,h,y,px,py;
    for (x=-r;x<=r;x++) {
      h=(int)sqrt(mysquare(r)-mysquare(x));
      for (y=-h;y<=h;y++) {
        px=cx+x; py=cy+y;
        if (px>=0 && px<width && py>=0 && py<height)
          bmp[px][py]=c;
      }
    }
  }
  void RGBmp::cross(V2d cen,float s,myRGB c) {
    line(cen+V2d(-s,-s),cen+V2d(s,s),c);
    line(cen+V2d(-s,s),cen+V2d(s,-s),c);
  }
  void RGBmp::cross(float x,float y,float s,myRGB c) {
    line(V2d(x-s,y-s),V2d(x+s,y+s),c);
    line(V2d(x-s,y+s),V2d(x+s,y-s),c);
  }
  void RGBmp::filledcircle(V2d pos,float r,myRGB c) {
    filledcircle(pos.x,pos.y,r,c);
  }
  RGBmp *RGBmp::applyfn(myRGB fn(myRGB)) {
    RGBmp *n=new RGBmp(width,height);
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++)
      n->bmp[i][j]=fn(bmp[i][j]);
    return n;
  }
  
  void RGBmp::freedom() {
    // printf("Freedom undefined for RGBmp\n");
    for (int i=0;i<width;i++)
      free(bmp[i]);
    free(bmp);
  }
  
  myRGB RGBmp::greyify(myRGB r) {
    float f=r.v3d().mag();
    myRGB n=myRGB(f,f,f);
    return n;
  }
  
  void RGBmp::greyify() {
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++)
      bmp[i][j]=greyify(bmp[i][j]);
  }    

// End class 


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





V2d rectangleproject(V3d w,V3d eye) {
  Line3d l=Line3d(eye,w);
  Plane img=Plane(V3d(0,0,0),V3d(0,0,1));
  V3d hit=img.intersect(l);
  return V2d(hit.x,hit.y);
}

V2d vvpfromhvpandbaseline(V2d hvp,Line2d baseline,V3d eye,RGBmp origimage) {

  // Ensure A is lower in image
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

  V3d ptopnor=V3d::cross(V3d(hvp.x,hvp.y,0.0)-eye,vtop-eye);
  V3d pbotnor=V3d::cross(vbot-eye,V3d(hvp.x,hvp.y,0.0)-eye);
  V3d pmidnor=V3d::cross(vbot-eye,vtop-eye);
  //V2d perp=(baseline.b-baseline.a).perp();
  //V3d pmidnor=V3d(perp.x,perp.y,0.0);

  V3d hd=V3d::cross(ptopnor,pbotnor).normalised();
  // hd points right
  if (hd.x<0) {
    printf("HD X WRONG!\n");
    hd=hd.neg();
  }
  float normdot=V3d::normdot(hd,pmidnor);
  if (normdot>0.9 || normdot<-0.9)
    printf("*** WARNING!  ");
  printf("hd and pmidnor are similar by %f\n",normdot);

  //V2d pmn=(baseline.b-baseline.a).perp();
  //pmidnor=V3d(pmn.x,pmn.y,0.0);
  V3d vd=V3d::cross(hd,pmidnor).normalised();
  // vd.z=-vd.z;
  // vd points up

  if (vd.y>0) {
    printf("VD Y WRONG!\n");
    vd=vd.neg();
  }
  printf("HD: %s\n",hd.toString());
  printf("VD: %s\n",vd.toString());

  V3d aw=vtop-50.0*hd;
  V3d bw=vtop+50.0*hd;
  V3d cw=vtop+50.0*hd-70.0*vd;
  V3d dw=vtop-50.0*hd-70.0*vd;

  V2d tl=rectangleproject(aw,eye);
  V2d tr=rectangleproject(bw,eye);
  V2d br=rectangleproject(cw,eye);
  V2d bl=rectangleproject(dw,eye);

  origimage.thickline(tl,tr,myRGB::red,3);
  origimage.thickline(tr,br,myRGB::red,3);
  origimage.thickline(br,bl,myRGB::red,3);
  origimage.thickline(bl,tl,myRGB::red,3);

  // Given the two swaps above, this shouldn't be necessary!
  printf("%s %s\n%s %s\n",tl.toString(),tr.toString(),bl.toString(),br.toString());
  if ((tl+bl).x>(tr+br).x) {
    printf("***** SWAPPING left side!\n");
    swap(&tl,&tr);
    swap(&bl,&br);
  }
  if ((tl+tr).y>(bl+br).y) {
    printf("***** SWAPPING top/bottom side!\n");
    swap(&tl,&bl);
    swap(&tr,&br);
  }
  printf("after\n%s %s\n%s %s\n",tl.toString(),tr.toString(),bl.toString(),br.toString());

  float score=1.0;
  score=score*(1.0-V3d::normdot(bw-aw,cw-bw));
  score=score*(1.0-V3d::normdot(cw-bw,dw-cw));
  score=score*(1.0-V3d::normdot(dw-cw,aw-dw));
  score=score*(1.0-V3d::normdot(aw-dw,bw-aw));
  printf("--- Score of Rectangularity: %f\n",score);

  V2d vvp=Line2d(bl,tl).intersect(Line2d(br,tr));

  return vvp;

}

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

List<V3d> rectanglefromquadrilateral(List<V2d> v2ds,V3d eye) {
  V2d a=v2ds.num(1); // tl
  V2d b=v2ds.num(2); // tr
  V2d c=v2ds.num(3); // br
  V2d d=v2ds.num(4); // bl

  // Lines through each corner (on which the true 3d corners must lie)
  Line3d al=Line3d(eye,V3d(a.x,a.y,0.0));
  Line3d bl=Line3d(eye,V3d(b.x,b.y,0.0));
  Line3d cl=Line3d(eye,V3d(c.x,c.y,0.0));
  Line3d dl=Line3d(eye,V3d(d.x,d.y,0.0));
  // Plane of left edge
  Plane pl=Plane::fromLines(al,dl);
  printf("Left plane %s\n",pl.toString());

  // Define distance of world corner c
  V3d cw=V3d(c.x,c.y,0.0);
  printf("Defined corner %s\n",cw.toString());

  // Find world corner b using left hand plane on c
  Plane plonr=Plane(cw,pl.nor);
  V3d bw=plonr.intersect(bl);
  printf("Used plane %s to get B %s\n",plonr.toString(),bw.toString());

  // Plane of top edge
  Plane pt=Plane::fromLines(al,bl);
  Plane ptonb=Plane(cw,pt.nor);
  V3d dw=ptonb.intersect(dl);

  // Plane of right edge
  Plane pr=Plane::fromLines(bl,cl);
  Plane pronl=Plane(dw,pr.nor);
  V3d aw=pronl.intersect(al);

  // the other way round (a check)
  Plane pb=Plane::fromLines(cl,dl);
  Plane pbont=Plane(bw,pb.nor);
  V3d aaw=pbont.intersect(al);

  if (!aaw.equals(aw))
    printf(">>>>>>> rgbmp.c: rectanglefromquadrilateral() Should have got %s %s equal!",aw.toString(),aaw.toString());

  printf("Got points with distances %f %f %f %f\n",(aw-eye).mag(),(bw-eye).mag(),(cw-eye).mag(),(dw-eye).mag());

  float score=1.0;
  score=score*(1.0-V3d::normdot(bw-aw,cw-bw));
  score=score*(1.0-V3d::normdot(cw-bw,dw-cw));
  score=score*(1.0-V3d::normdot(dw-cw,aw-dw));
  score=score*(1.0-V3d::normdot(aw-dw,bw-aw));
  printf("Score of rectangularity: %f\n",score);

  List<V3d> ls;
  ls.add(aw);
  ls.add(bw);
  ls.add(cw);
  ls.add(dw);

  return ls;
}


List<V3d> rectanglefromquadrilateralgroovy(List<V2d> v2ds,V3d eye) {
  V2d a=v2ds.num(1); // tl
  V2d b=v2ds.num(2); // tr
  V2d c=v2ds.num(3); // br
  V2d d=v2ds.num(4); // bl

  V2d hvp=Line2d(a,b).intersection(Line2d(c,d));
  V2d vvp=Line2d(b,c).intersection(Line2d(d,a));

  Line3d la=Line3d(eye,V3d(a.x,a.y,0.0));
  Line3d lb=Line3d(eye,V3d(b.x,b.y,0.0));
  Line3d lc=Line3d(eye,V3d(c.x,c.y,0.0));
  Line3d ld=Line3d(eye,V3d(d.x,d.y,0.0));
  Line3d lhvp=Line3d(eye,V3d(hvp.x,hvp.y,0.0));
  Line3d lvvp=Line3d(eye,V3d(hvp.x,hvp.y,0.0));

  Plane pleft=Plane::fromLines(la,ld);
  Plane pright=Plane::fromLines(lb,lc);
  Plane ptop=Plane::fromLines(la,lb);
  Plane pbot=Plane::fromLines(lc,ld);

  V3d hd=V3d::cross(ptop.nor,pbot.nor).normalised();
  V3d vd=V3d::cross(pleft.nor,pright.nor).normalised();

  Plane ptext=Plane(V3d(a.x,a.y,0.0),V3d::normcross(hd,vd));

  V3d aw=ptext.intersect(la);
  V3d bw=ptext.intersect(lb);
  V3d cw=ptext.intersect(lc);
  V3d dw=ptext.intersect(ld);

  printf("Got points with distances %f %f %f %f\n",(aw-eye).mag(),(bw-eye).mag(),(cw-eye).mag(),(dw-eye).mag());

  float score=1.0;
  score=score*(1.0-V3d::normdot(bw-aw,cw-bw));
  score=score*(1.0-V3d::normdot(cw-bw,dw-cw));
  score=score*(1.0-V3d::normdot(dw-cw,aw-dw));
  score=score*(1.0-V3d::normdot(aw-dw,bw-aw));
  printf("Score of rectangularity: %f\n",score);

  // Check against HVP,VVP (not done yet)

  List<V3d> ls;
  ls.add(aw);
  ls.add(bw);
  ls.add(cw);
  ls.add(dw);

  return ls;
}

#endif
