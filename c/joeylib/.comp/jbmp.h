#ifndef jbmp_H
  #define jbmp_H

// #define pi 3.1415926535897932385

#ifdef ALLEGRO
  #include <sys/farptr.h>
  #include <sys/segments.h>
  #include <sys/movedata.h>
#endif

int smileymouthpolarfn(float t,float r); // Method


int heartpolarfn(float t,float r); // Method


int flowerpolarfn(float t,float r); // Method


/* float starpolarfn(float t) {
     t=mymod2(t,-pi/5,pi/5)/(pi/5);
     return 0.2+0.8*myabs(t);
   }*/

int starpolarfn(float th,float r); // Method


#define MAXJBMPWIDTH 2048
#define JBMP_PIXEL_SIZE sizeof(unsigned char)

class JBmp : public Writeable {
public:
  unsigned char **bmp; // Exists // For Dos allegro
  unsigned char **spare; // Exists
  int width,height; // Exists
  unsigned char buffer[MAXJBMPWIDTH]; // Exists
   JBmp(); // Method

  void setupgrid(int w,int h); // Method

   JBmp(int w,int h); // Method

  void clear(int c); // Method

   JBmp(int w,int h,int c); // Method

  void clear(); // Method

  void blitline(int y); // Method

  void blit(); // Method

  void writetoscreen(); // Method

  void display(); // Method

  /* void filledcircle(int cx,int cy,int r,int c) {
       int x,h,y,px,py;
       for (x=-r;x<=r;x++) {
         h=(int)sqrt(mysquare(r)-mysquare(x));
         for (y=-h;y<=h;y++) {
           px=cx+x; py=cy+y;
           if (px>=0 && px<width && py>=0 && py<height)
             bmp[py][px]=(unsigned char)c;
         }
       }
     }*/
  void filledcircle(int cx,int cy,float r,int c); // Method

  void filledcircle(V2d cen,float r,int c); // Method

  void filledcirclenodarker(int cx,int cy,int r,int c); // Method

  void shadedcircle(int cx,int cy,int r,int inc,int outc); // Method

  void shadedcirclenodarker(int cx,int cy,int r,int inc,int outc); // Method

  void addcircle(int cx,int cy,int r,int c); // Method

  // r = .5 + 2 * t / pi + .5 * SIN(1.5 * t)
  void filledpolar(int (*fn)(float,float),int cx,int cy,int r,float rot,int c); // Method

  void flower(int cx,int cy,int r,float rot,int cp,int cc); // Method

  void smiley(int cx,int cy,int r,float rot,int c); // Method

  void incpixel(int x,int y,int n); // Method

  bool inbmp(int x,int y); // Method

  bool inimage(int x,int y); // Method

  void incpixel(int x,int y); // Method

  void setpixel(int x,int y,int c); // Method

  void setpixelnc(int x,int y,int c); // Method

  void setpixelud(int x,int y,int c); // Method

  void setpixelnodarker(int x,int y,int c); // Method

  void addtopixel(int x,int y,int c); // Method

  int pointat(int x,int y); // Method

  void setrectangle(int l,int t,int w,int h,int c); // Method

  void hliner(int x,int y,int dx,int c); // Method

  void hline(int x,int y,int xb,int c); // Method

  void vliner(int x,int y,int dy,int c); // Method

  void vline(int x,int y,int yb,int c); // Method

  void line(int x1,int y1,int x2,int y2,int c); // Method

  void line(V2d a,V2d b,int c); // Method

  void line(Line2d l,int c); // Method

  void line(Line2d *l,int c); // Method

  void sthickline(V2d a,V2d b,float w,int c); // Method

  void thickline(V2d a,V2d b,float w,int c); // Method

  void thickline(int xa,int ya,int xb,int yb,float w,int c); // Method

  void thickline(Line2d l,int w,int c); // Method

  void thicklinev(V2d a,V2d b,float w,int c); // Method

  void thicklineh(V2d a,V2d b,float w,int c); // Method

  void rthickline(V2d a,V2d b,float w,int c); // Method

  void liner(V2d a,V2d b,int c); // Method

  int point(int x,int y); // Method

  void interpolate(int l,int t,int r,int b,int nwcol,int necol,int swcol,int secol); // Method


  Map2d<uchar> tomap(); // Method


  void writefile(String fname); // Method

  void intvline(int x,int ya,int yb,int ca,int cb); // Method

  void intertri(V2d a,V2d b,V2d c,int ca,int cb,int cc); // Method

  void intertri(int xa,int ya,int xb,int yb,int xc,int yc,int ca,int cb,int cc); // Method

  void filltri(int xa,int ya,int xb,int yb,int xc,int yc,int c); // Method

/*  void filltri(int xa,int ya,int xb,int yb,int xc,int yc,int c) {
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
    if (start<0)
      start=0;
    for (int x=start;x<=xb;x++) {
      float xthruab;
      if (xb==xa)
        xthruab=0;
      else
      xthruab=(float)(x-xa)/(xb-xa);
      float xthruac;
      if (xa==xc)
        xthruac=0.5;
      else
      xthruac=(float)(x-xa)/(xc-xa);
      int ylow=(int)((float)ya+xthruab*(float)(yb-ya)+0.5);
      int yhei=(int)((float)ya+xthruac*(float)(yc-ya)+0.5);
      if (ylow>yhei) {
        int tmp=yhei;
        yhei=ylow;
        ylow=tmp;
      }
      vline(x,ylow,yhei,c);
    }
    int end=xc;
    if (end>=width)
      end=width-1;
    for (int x=xb;x<=end;x++) {
      float xthrubc;
      if (xb==xc)
        xthrubc=0.5;
      else
      xthrubc=(float)(x-xb)/(xc-xb);
      float xthruac;
      if (xa==xc)
        xthruac=0;
      else
      xthruac=(float)(x-xa)/(xc-xa);
      int ylow=(int)((float)yb+xthrubc*(float)(yc-yb)+0.5);
      int yhei=(int)((float)ya+xthruac*(float)(yc-ya)+0.5);
      if (ylow>yhei) {
        int tmp=yhei;
        yhei=ylow;
        ylow=tmp;
      }
      vline(x,ylow,yhei,c);
    }
  }*/
  void filltri(V2d a,V2d b,V2d c,int d); // Method

  void fillpoly(Polygon2d p,int c); // Method

  
  /* if (ya>yb) {
       swapints(&xa,&xb); swapints(&ya,&yb);
     }
     if (yb>yc) {
       swapints(&xb,&xc); swapints(&yb,&yc);
     }
     if (ya>yc) {
       swapints(&xa,&xc); swapints(&ya,&yc);
     }
     for (int y=ya;y<=yb;y++) {
       float f;
       if (diff(yb,ya)==0)
         f=0;
       else
       f=(float)(y-ya)/(float)(yb-ya);
       int lx=(float)xa*(float)(1.0-f)+f*(float)xb;
       float g;
       if (diff(ya,yc)==0)
         g=0;
       else
       g=(float)(y-ya)/(float)(yc-ya);
       int rx=(float)xa*(float)(1.0-g)+g*(float)xc;
       hline(lx,y,rx,c);
     }
     for (int y=yb;y<=yc;y++) {
       float f;
       if (diff(yb,yc)==0)
         f=0;
       else
       f=(float)(y-yb)/(float)(yc-yb);
       int lx=(float)xb*(float)(1.0-f)+f*(float)xc;
       float g;
       if (diff(yb,ya)==0)
         g=0;
       else
       g=(float)(y-yb)/(float)(ya-yb);
       int rx=(float)xb*(float)(1.0-g)+g*(float)xa;
       hline(lx,y,rx,c);
     }
   }*/
void fillquad(int xa,int ya,int xb,int yb,int xc,int yc,int xd,int yd,int c); // Method

void fillquad(V2d a,V2d b,V2d c,V2d d,int z); // Method

void writeframe(); // Method

void saveframe(); // Method

void fadeby(int c); // Method

void fillstar(int x,int y,int r,float dth,int c); // Method

void opencircle(int x,int y,float r,int c); // Method

void opencircle(V2d cen,float r,int c); // Method

uchar chopbmp(int x,int y); // Method

/* void smooth(int r) {
     JBmp n=JBmp(width,height);
     List<V2d> ps=v2dsincircle(0,0,r);
     for (int x=0;x<width;x++)
     for (int y=0;y<height;y++) {
       int c=0;
       int cnt=0;
       for (int i=1;i<=ps.len;i++) {
         V2d p=V2d(x,y)+ps.num(i);
         if (inbmp(p)) {
           c+=bmp[(int)p.y][(int)p.x];
           cnt++;
         }
       }
       n.bmp[y][x]=c/cnt;
     }
     *this=n;
void smooth(int r) {
  JBmp n=JBmp(width,height);
  for (register int x=0;x<width;x++)
  for (register int y=0;y<height;y++) {
    int c=0;
    int cnt=0;
    for (register int i=-r;i<=r;i++) {
      int h=sqrt(r*r-i*i);
      for (register int j=-h;j<=h;j++) {
        int dx=x+i;
        int dy=y+j;
        if (inbmp(dx,dy)) {
          c+=bmp[dy][dx];
          cnt++;
        }
      }
    }
    n.bmp[y][x]=c/cnt;
  }
  *this=n;
}
   }*/
void smooth(int r); // Method

int fsrad; // Exists
int *hs; // Exists
int cnt; // Exists
void setupfastsmooth(int r); // Method

void fastsmooth(); // Method

void randomfastsmooth(); // Method

bool inbmp(V2d p); // Method

};      

/* 
 void foralldo(void (*fn)(ListObject)) {
   for (int i=1;i<=len;i++) {
     fn(list[i-1]);
   }
 }
 void foralldo(void (*fn)(ListObject),String s) {
   for (int i=1;i<=len;i++) {
     fn(list[i-1]);
     if (i<len)
       printf(s);
   }
 }
*/

#endif
