#ifndef writeable_C
  #define writeable_C

  #include <writeable.h>

// Starts of class Writeable


  // Variable declared in .h file

   void Writeable::setpos(int i,int j,int c) {
    error("Write::setpos(int) not cast before write");
  }
   void Writeable::setpos(int i,int j,myRGB c) {
    error("Write::setpos(myRGB) not cast before write");
  }
/*  template <class CT>
  void setpos(int x,int y,CT c) {
    error("Writeable::setpos(x,y,c) Must be overwritten!");
  }*/
  template <class CT>
  void Writeable::setpos(Pixel p,CT c) {
    setpos(p.x,p.y,c);
  }
  template <class CT>
  void Writeable::setpixel(int x,int y,CT c) {
    setpos(x,y,c);
  }
  template <class CT>
  void Writeable::setpixel(Pixel p,CT c) {
    setpos(p.x,p.y,c);
  }

  template <class CT>
  void Writeable::line(int x1, int y1, int x2, int y2,CT r) {
    Line2d l=Line2d(V2d(x1,y1),V2d(x2,y2));
    String s=Sformat("Clipping %s...\n",l.toString());
//    l.clipbyrectangle(0,0,width-1,height-1);
//    printf("  got %s...\n",l.toString());
    x1=l.a.x;
    x2=l.b.x;
    y1=l.a.y;
    y2=l.b.y;

//    if (inbmp(x1,y1) && inbmp(x2,y2)) {
      List<Pixel> ps=Pixel::line(Pixel(x1,y1),Pixel(x2,y2));
      for (int i=1;i<=ps.len;i++) {
//        printf("Setting pixel %s\n",ps.num(i).toString());
        setpixel(ps.num(i),r);
      }
      ps.freedom();
//    } else
//      printf("%sDidn't write line because got %s when clipped\n",s,l.toString());

    free(s);
  }
  template <class CT>
  void Writeable::line(Pixel a,Pixel b,CT c) {
  // #ifndef UNIX
    line(a.x,a.y,b.x,b.y,c);
  // #endif
  }
  template <class CT>
  void Writeable::line(V2d a,V2d b,CT c) {
  // #ifndef UNIX
    line(a.x,a.y,b.x,b.y,c);
  // #endif
  }

  template <class CT>
  void Writeable::cross(V2d cen,int s,CT c) {
    line(cen+V2d(s,s),cen-V2d(s,s),c);
    line(cen+V2d(-s,s),cen-V2d(-s,s),c);
  }

  template <class CT>
  void Writeable::thickcross(V2d cen,int s,float wid,CT c) {
    thickline(cen+V2d(s,s),cen-V2d(s,s),c,wid);
    thickline(cen+V2d(-s,s),cen-V2d(-s,s),c,wid);
  }

  template <class CT>
  void Writeable::vline(int x,int ya,int yb,CT c) {
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
//      V3d c=pull(ca,atob,cb);
      setpixel(x,i,c);
    }
  }

  template <class CT> // Silly! Should keep w with a and b!
  void Writeable::thickline(V2d a,V2d b,CT r,float w) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a+p,a-p,b+p,r);
    filltri(a-p,b+p,b-p,r);
  }

  template <class CT>
  void Writeable::thicklineh(V2d a, V2d b, float w, CT c) {
    V2d p=V2d(0,w/2.0+1);
    V2d a2b=V2d(w/2.0-1,0)*mysgn(b.x-a.x);
    filltri(a-a2b+p,a-a2b-p,b+a2b+p,c);
    filltri(a-a2b-p,b+a2b+p,b+a2b-p,c);
//    error("Writeable::hthickline not cast before write");
  }
  template <class CT>
  void Writeable::thicklinev(V2d a, V2d b, float w, CT c) {
    V2d p=V2d(w/2.0,0);
    V2d a2b=V2d(0,w/2.0)*mysgn(b.y-a.y);
    filltri(a-a2b+p,a-a2b-p,b+a2b+p,c);
    filltri(a-a2b-p,b+a2b+p,b+a2b-p,c);
//    error("Writeable::vthickline not cast before write");
  }
  template <class CT>
  void Writeable::rthickline(V2d a, V2d b, float w, CT c) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a+p,a-p,b+p,c);
    filltri(a-p,b+p,b-p,c);
    filledcircle(a,w/2.0,c);
    filledcircle(b,w/2.0,c);
//    error("Writeable::rthickline not cast before write");
  }
  template <class CT>
  void Writeable::sthickline(V2d a, V2d b, float w, CT c) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a+p,a-p,b+p,c);
    filltri(a-p,b+p,b-p,c);
//    error("Writeable::sthickline not cast before write");
  }
  template <class CT>
  void Writeable::filltri(V2d a, V2d b, V2d c, CT col) {
    filltri(a.x,a.y,b.x,b.y,c.x,c.y,col);
//    error("Writeable::filltri not cast before write");
  }
  template <class CT>
  void Writeable::filltri(int xa,int ya,int xb,int yb,int xc,int yc,CT c) {

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

  template <class CT>
  void Writeable::filledcircle(V2d cen,float r,CT c) {
    filledcircle(cen.x,cen.y,r,c);
  }

  template <class CT>
  void Writeable::filledcircle(int cx,int cy,float r,CT c) {
    int x,h,y,px,py;
    for (x=-r;x<=r;x++) {
      h=(int)sqrt(mysquare(r)-mysquare(x));
      for (y=-h;y<=h;y++) {
        px=cx+x; py=cy+y;
        if (px>=0 && px<width && py>=0 && py<height)
          this->setpos(Pixel(px,py),c);
      }
    }
  }

template <class CT>
void Writeable::opencircle(int x,int y,float r,CT c) {
  Pixel last=Pixel(x,y+r);
  int steps=4+r/2;
  for (int i=1;i<=steps;i++) {
    float a=2.0*pi*(float)i/(float)steps;
    Pixel next=Pixel(x+r*sin(a),y+r*cos(a));
    line(last,next,c);
    last=next;
  }
}



//  virtual void opencircle(Pixel p,int r,

/*  template <class CT>
  virtual void filledcircle(int cx,int cy,float r,CT c) {
    printf("Filling circle %i %i %f (%f) %i %i\n",cx,cy,r,c,width,height);
    int x,h,y,px,py;
    for (x=-r;x<=r;x++) {
      h=(int)sqrt(mysquare(r)-mysquare(x));
      for (y=-h;y<=h;y++) {
        px=cx+x; py=cy+y;
        if (px>=0 && px<width && py>=0 && py<height)
          setpos(Pixel(px,py),c);
      }
    }
  }
  template <class CT>
  virtual void filledcircle (V2d v, float f, CT c) {
    this->filledcircle((int)v.x,(int)v.y,f,r);;
  }*/

   void Writeable::arrow(Pixel a,Pixel b,myRGB c) {
    this->line(a,b,c);
    V2d diff=V2d(b-a);
    V2d perp=V2d(diff.y,-diff.x);
    V2d l=V2d(a)+0.9*diff+perp*0.1*diff.mag();
    V2d r=V2d(a)+0.9*diff-perp*0.1*diff.mag();
    this->line(b,Pixel(l),c);
    this->line(b,Pixel(r),c);
  }

   void Writeable::plotandfree(List<Pixel> ps,int c) {
    for (int i=1;i<=ps.len;i++)
      setpos(ps.num(i),c);
    ps.freedom();
  }

   void Writeable::plotandfree(List<Pixel> ps,myRGB c) {
    for (int i=1;i<=ps.len;i++)
      this->setpos(ps.num(i),c);
    ps.freedom();
  }

  template<class ColType>
  void Writeable::cross(V2d cen,float s,ColType c) {
    line(cen+V2d(-s,-s),cen+V2d(s,s),c);
    line(cen+V2d(-s,s),cen+V2d(s,-s),c);
  }

// End class 


/*
void plotandfree(Writeable *w,List<Pixel> ps,int c) {
    for (int i=1;i<=ps.len;i++)
      w->setpos(ps.num(i),c);
    ps.freedom();
  }

void plotandfree(Writeable *w,List<Pixel> ps,myRGB c) {
    for (int i=1;i<=ps.len;i++)
      w->setpos(ps.num(i),c);
    ps.freedom();
  }
*/

// Keep getting error setpos not cast before write!

template<class Object>
void plotandfree(Object *w,List<Pixel> ps,int c) {
    for (int i=1;i<=ps.len;i++)
      w->setpos(ps.num(i),c);
    ps.freedom();
  }

template<class Object>
void plotandfree(Object *w,List<Pixel> ps,myRGB c) {
    for (int i=1;i<=ps.len;i++)
      w->setpos(ps.num(i),c);
    ps.freedom();
  }

#endif
