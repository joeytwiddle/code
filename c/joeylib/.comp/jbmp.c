#ifndef jbmp_C
  #define jbmp_C

  #include <jbmp.h>

// #define pi 3.1415926535897932385

int smileymouthpolarfn(float t,float r) {
  return (myabs(t)>3*pi/5 && r>0.5);
}

int heartpolarfn(float t,float r) {
  if (t<0)
    t=-t;
  return (r<(.1 + 2 * t / pi + .5 * sin(2 * t))/3);
}

int flowerpolarfn(float t,float r) {
  return (r<myabs(sin(5*t/2)));
}

/* float starpolarfn(float t) {
     t=mymod2(t,-pi/5,pi/5)/(pi/5);
     return 0.2+0.8*myabs(t);
   }*/

int starpolarfn(float th,float r) {
  th=mymod2(th,-pi/5,pi/5);
  th=myabs(th);
  float m=4*(cos(4*pi/5)-1)/sin(4*pi/5);
  // float t=m/(tan(th)-1);
  // float t=tan(th)/(1-m*tan(th));
  float t=1/(1/tan(th)-m);
  return (r<sqrt(t*t*(1+m*m)+2*m*t+1));
}

// Starts of class JBmp


  // Variable declared in .h file
  // Variable declared in .h file
  // Variable declared in .h file
   JBmp::JBmp() {
  }
  void JBmp::setupgrid(int w,int h) {
    int x,y;
    width=w;
    height=h;
    bmp=new (unsigned char*)[height];
    for (y=0;y<h;y++) {
      bmp[y]=new unsigned char[width];
      for (x=0;x<w;x++) {
        // bmp[y][x]=(float)256*myrnd();
        bmp[y][x]=0;
      }
    }
    spare=new (unsigned char*)[height];
    for (y=0;y<h;y++) {
      spare[y]=new unsigned char[width];
      for (x=0;x<w;x++) {
        // spare[y][x]=(float)256*myrnd();
        spare[y][x]=0;
      }
    }
  }
   JBmp::JBmp(int w,int h) {
    setupgrid(w,h);
  }
  void JBmp::clear(int c) {
    for (int x=0;x<width;x++)
    for (int y=0;y<height;y++)
    bmp[y][x]=c;
  }
   JBmp::JBmp(int w,int h,int c) {
    setupgrid(w,h);
    clear(c);
  }
  void JBmp::clear() {
    clear(0);
  }
  void JBmp::blit() {
    #ifdef ALLEGRO
    #ifndef NOGRAPHICS
    int y;
    for (y=0; y<height; y++) {
      movedata(_my_ds(), bmp[y], screen->seg, bmp_write_line(screen,y), width);
    }
    #endif
    #endif
  }
  void JBmp::writetoscreen() {
    blit();
  }
  void JBmp::display() {
  #ifdef ALLEGRO
    allegrosetup(width,height,8);
    writetoscreen();
    waitforkeypress();
    #endif
  }
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
  void JBmp::filledcircle(int cx,int cy,float r,int c) {
    int x,h,y,px,py;
    for (x=-r;x<=r;x++) {
      h=(int)sqrt(mysquare(r)-mysquare(x));
      for (y=-h;y<=h;y++) {
        px=cx+x; py=cy+y;
        if (px>=0 && px<width && py>=0 && py<height)
          bmp[py][px]=(unsigned char)c;
      }
    }
  }
  void JBmp::filledcircle(V2d cen,float r,int c) {
    filledcircle(cen.x,cen.y,r,c);
  }
  void JBmp::filledcirclenodarker(int cx,int cy,int r,int c) {
    int x,h,y,px,py;
    for (x=-r;x<=r;x++) {
      h=(int)sqrt(mysquare(r)-mysquare(x));
      for (y=-h;y<=h;y++) {
        px=cx+x; py=cy+y;
        setpixelnodarker(px,py,c);
      }
    }
  }
  void JBmp::shadedcircle(int cx,int cy,int r,int inc,int outc) {
    int x,h,y,px,py;
    for (x=-r;x<=r;x++) {
      h=(int)sqrt(mysquare(r)-mysquare(x));
      for (y=-h;y<=h;y++) {
        px=cx+x; py=cy+y;
        if (inimage(px,py)) {
          float f=(float)sqrt(x*x+y*y)/(float)r;
          bmp[py][px]=(unsigned char)inc+f*(outc-inc);
        }
      }
    }
  }
  void JBmp::shadedcirclenodarker(int cx,int cy,int r,int inc,int outc) {
    int x,h,y,px,py;
    for (x=-r;x<=r;x++) {
      h=(int)sqrt(mysquare(r)-mysquare(x));
      for (y=-h;y<=h;y++) {
        px=cx+x; py=cy+y;
        if (inimage(px,py)) {
          float f=(float)sqrt(x*x+y*y)/(float)r;
          setpixelnodarker(px,py,(unsigned char)inc+f*(outc-inc));
        }
      }
    }
  }
  void JBmp::addcircle(int cx,int cy,int r,int c) {
    int x,h,y,px,py;
    for (x=-r;x<=r;x++) {
      h=(int)sqrt(mysquare(r)-mysquare(x));
      for (y=-h;y<=h;y++) {
        px=cx+x; py=cy+y;
        if (px>=0 && px<width && py>=0 && py<height)
          bmp[py][px]=ucharchop(bmp[py][px]+c);
      }
    }
  }
  // r = .5 + 2 * t / pi + .5 * SIN(1.5 * t)
  void JBmp::filledpolar(int (*fn)(float,float),int cx,int cy,int r,float rot,int c) {
    for (int x=-r;x<=r;x++) {
      int h=(int)sqrt(mysquare(r)-mysquare(x));
      for (int y=-h;y<=h;y++) {
        float ang=mymod2(rot+myangle(x,y),-pi,pi);
        float dist=((float)x*x+y*y)/(r*r);
        if (fn(ang,dist)) {
          int px=cx+x; int py=cy+y;
          if (px>=0 && px<width && py>=0 && py<height)
            bmp[py][px]=(unsigned char)(c);
        }
      }
    }
  }
  void JBmp::flower(int cx,int cy,int r,float rot,int cp,int cc) {
    filledpolar(&flowerpolarfn,cx,cy,r,rot,cp);
    filledcircle(cx,cy,(int)(r/3.5),cc);
  }
  void JBmp::smiley(int cx,int cy,int r,float rot,int c) {
    filledpolar(&smileymouthpolarfn,cx,cy,r,rot,c);
    filledcircle(cx+r*(float)sin(-rot+pi/4),cy+r*(float)cos(-rot+pi/4),r/3,c);
    filledcircle(cx+r*(float)sin(-rot-pi/4),cy+r*(float)cos(-rot-pi/4),r/3,c);
  }
  void JBmp::incpixel(int x,int y,int n) {
    if (x>=0 && x<width && y>=0 && y<height)
      if (bmp[y][x]<255)
      bmp[y][x]=bmp[y][x]+n;
  }
  bool JBmp::inbmp(int x,int y) {
    return (x>=0 && x<width && y>=0 && y<height);
  }
  bool JBmp::inimage(int x,int y) {
    return inbmp(x,y);
  }
  void JBmp::incpixel(int x,int y) {
    incpixel(x,y,1);
  }
  void JBmp::setpixel(int x,int y,int c) {
    if (inbmp(x,y))
      bmp[y][x]=c;
  }
  void JBmp::setpixelnc(int x,int y,int c) {
    bmp[y][x]=c;
  }
  void JBmp::setpixelud(int x,int y,int c) {
    if (x>=0 && x<width && y>=0 && y<height) {
      bmp[y][x]=c;
      #ifdef ALLEGRO
      movedata(_my_ds(), bmp[y], screen->seg, bmp_write_line(screen,y), width);
      #endif
    }
  }
  void JBmp::setpixelnodarker(int x,int y,int c) {
    if (x>=0 && x<width && y>=0 && y<height)
      if (bmp[y][x]<c)
      bmp[y][x]=c;
  }
  void JBmp::addtopixel(int x,int y,int c) {
    setpixel(x,y,bmp[y][x]+c);
  }
  int JBmp::pointat(int x,int y) {
    int rx=mymod(x,width);
    int ry=mymod(y,height);
    return bmp[ry][rx];
  }
  void JBmp::setrectangle(int l,int t,int w,int h,int c) {
    if (w<0) {
      l=l+w; w=-w;
    }
    if (h<0) {
      t=t+h; h=-h;
    }
    if (w==0)
      w=1;
    if (h==0)
      h=1;
    for (int i=l;i<l+w;i++)
    for (int j=t;j<t+h;j++)
    setpixel(i,j,c);
  }
  void JBmp::hliner(int x,int y,int dx,int c) {
    setrectangle(x,y,dx,1,c);
  }
  void JBmp::hline(int x,int y,int xb,int c) {
    hliner(x,y,xb-x+1,c);
  }
  void JBmp::vliner(int x,int y,int dy,int c) {
    setrectangle(x,y,1,dy,c);
  }
  void JBmp::vline(int x,int y,int yb,int c) {
    vliner(x,y,yb-y+1,c);
  }
  void JBmp::line(int x1,int y1,int x2,int y2,int c) {
    int x,y,tmp;
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
  void JBmp::line(V2d a,V2d b,int c) {
    line(a.x,a.y,b.x,b.y,c);
  }
  void JBmp::line(Line2d l,int c) {
    line(l.a,l.b,c);
  }
  void JBmp::line(Line2d *l,int c) {
    line(l->a,l->b,c);
  }
  void JBmp::sthickline(V2d a,V2d b,float w,int c) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a+p,a-p,b+p,c);
    filltri(a-p,b+p,b-p,c);
  }
  void JBmp::thickline(V2d a,V2d b,float w,int c) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a-a2b+p,a-a2b-p,b+a2b+p,c);
    filltri(a-a2b-p,b+a2b+p,b+a2b-p,c);
  }
  void JBmp::thickline(int xa,int ya,int xb,int yb,float w,int c) {
    thickline(V2d(xa,ya),V2d(xb,yb),w,c);
  }
  void JBmp::thickline(Line2d l,int w,int c) {
    thickline(l.a,l.b,w,c);
  }
  void JBmp::thicklinev(V2d a,V2d b,float w,int c) {
    V2d p=V2d(w/2.0,0);
    V2d a2b=V2d(0,w/2.0)*mysgn(b.y-a.y);
    filltri(a-a2b+p,a-a2b-p,b+a2b+p,c);
    filltri(a-a2b-p,b+a2b+p,b+a2b-p,c);
  }
  void JBmp::thicklineh(V2d a,V2d b,float w,int c) {
    V2d p=V2d(0,w/2.0+1);
    V2d a2b=V2d(w/2.0-1,0)*mysgn(b.x-a.x);
    filltri(a-a2b+p,a-a2b-p,b+a2b+p,c);
    filltri(a-a2b-p,b+a2b+p,b+a2b-p,c);
  }
  void JBmp::rthickline(V2d a,V2d b,float w,int c) {
    V2d a2b=w/2.0*(b-a).norm();
    V2d p=a2b.perp();
    filltri(a+p,a-p,b+p,c);
    filltri(a-p,b+p,b-p,c);
    filledcircle(a,w/2.0,c);
    filledcircle(b,w/2.0,c);
  }
  void JBmp::liner(V2d a,V2d b,int c) {
    line(a,a+b,c);
  }
  int JBmp::point(int x,int y) {
    if (inbmp(x,y))
      return bmp[y][x];
    else
    return -1;
  }
  void JBmp::interpolate(int l,int t,int r,int b,int nwcol,int necol,int swcol,int secol) {
    for (int i=0;i<=r-l;i++)
    for (int k=0;k<=b-t;k++) {
      float e=(float)i/(float)(r-l);
      float w=1.0-e;
      float s=(float)k/(float)(b-t);
      float n=1.0-s;
      setpixel(l+i,t+k,ucharmod(s*e*secol+n*w*nwcol+s*w*swcol+n*e*necol));
    }
  }

  Map2d<uchar> JBmp::tomap() { // Both dependent on same map: can't destroy both!
    Map2d<uchar> m=Map2d<uchar>(height,width);
    m.pos=bmp;
    return *m.transpose();
  }

  void JBmp::writefile(String fname) {
    Map2d<uchar> m=Map2d<uchar>(height,width);
    m.pos=bmp;
//    m.display();
//    printf("...\n");
    m.transpose()->writefile(fname);
    /*
    #ifdef ALLEGRO
    set_color_depth(8);
    BITMAP *lbmp=create_bitmap(width,height);
    PALETTE pal;
    get_palette(pal);
    for (int i=0;i<width;i++) {
      for (int j=0;j<height;j++) {
        putpixel(lbmp,i,j,bmp[j][i]);
      }
    }
    save_bitmap(fname,lbmp,pal);
    #else  
    printf("JBmp::writefile() not implemented under UNIX");
    #endif*/
  }
  void JBmp::intvline(int x,int ya,int yb,int ca,int cb) {
    float atob;
    bool done=false;
    int s=sgn(yb-ya);
    for (int i=ya;!done;i+=s) {
      if (i==yb)
        done=true;
      if (ya==yb)
        atob=0;
      else
        atob=(float)(i-ya)/(float)(yb-ya);
      int c=pull(ca,atob,cb);
      setpixel(x,i,c);
    }
  }
  void JBmp::intertri(V2d a,V2d b,V2d c,int ca,int cb,int cc) {
    intertri(a.x,a.y,b.x,b.y,c.x,c.y,ca,cb,cc);
  }
  void JBmp::intertri(int xa,int ya,int xb,int yb,int xc,int yc,int ca,int cb,int cc) {
    if (xa>xb) {
      swapints(&xa,&xb); swapints(&ya,&yb);
      swapints(&ca,&cb);
    }
    if (xb>xc) {
      swapints(&xb,&xc); swapints(&yb,&yc);
      swapints(&cb,&cc);
    }
    if (xa>xb) {
      swapints(&xa,&xb); swapints(&ya,&yb);
      swapints(&ca,&cb);
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
      int cab=pull(ca,xthruab,cb);
      int cac=pull(ca,xthruac,cc);
      intvline(x,ylow,yhei,cab,cac);
    }
    int end=xc;
    if (xc>width)
      end=width;
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
      int cbc=pull(cb,xthrubc,cc);
      int cac=pull(ca,xthruac,cc);
      intvline(x,ylow,yhei,cbc,cac);
    }
  }
  void JBmp::filltri(int xa,int ya,int xb,int yb,int xc,int yc,int c) {
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
      int ylow=(int)((float)yb+xthrubc*(float)(yc-yb)+0.5);
      int yhei=(int)((float)ya+xthruac*(float)(yc-ya)+0.5);
      if (ylow>yhei) {
        int tmp=yhei;
        yhei=ylow;
        ylow=tmp;
      }
      vline(x,ylow,yhei,c);
    }
  }
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
  void JBmp::filltri(V2d a,V2d b,V2d c,int d) {
    filltri(a.x,a.y,b.x,b.y,c.x,c.y,d);
  }
  void JBmp::fillpoly(Polygon2d p,int c) {
    List<Polygon2d> l=p.splittotris();
    for (int i=1;i<=l.len;i++)
      filltri(l.num(i).num(1),l.num(i).num(2),l.num(i).num(3),c);
    l.freedom();
  }
  
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
void JBmp::fillquad(int xa,int ya,int xb,int yb,int xc,int yc,int xd,int yd,int c) {
  filltri(xa,ya,xb,yb,xc,yc,c);
  filltri(xa,ya,xc,yc,xd,yd,c);
  /* line(xa,ya,xb,yb,c);
     line(xa,ya,xc,yc,c);
     line(xd,yd,xb,yb,c);
     line(xc,yc,xd,yd,c);*/
}
void JBmp::fillquad(V2d a,V2d b,V2d c,V2d d,int z) {
  filltri(a,b,c,z);
  filltri(a,c,d,z);
//  line(a,b,z);
//  line(b,c,z);
//  line(c,d,z);
//  line(d,a,z);
}
void JBmp::writeframe() {
  writefile(getnextfilename("bmp"));
}
void JBmp::saveframe() {
  writeframe();
}
void JBmp::fadeby(int c) {
  for (int i=0;i<width;i++)
  for (int j=0;j<height;j++) {
    int k=bmp[j][i];
    if (k>c)
      bmp[j][i]-=c;
    else
    if (k>0)
      bmp[j][i]=0;
  }
}
void JBmp::fillstar(int x,int y,int r,float dth,int c) {
  V2d pos=V2d(x,y);
  for (int i=0;i<5;i++) {
    float th=dth+i*2*pi/5.0;
    V2d top=V2d::rotate(V2d(0,r),th);
    V2d botright=V2d::rotate(V2d(0,r*0.3),th+2.0*pi*1.0/10.0);
    V2d botleft=V2d::rotate(V2d(0,r*0.3),th-2.0*pi*1.0/10.0);
    filltri(pos+top,pos+botright,pos+botleft,c);
    filltri(pos,pos+botright,pos+botleft,c);
  }
}
void JBmp::opencircle(int x,int y,float r,int c) {
  V2d last=V2d(x,y+r);
  int steps=4+r/2;
  for (int i=1;i<=steps;i++) {
    float a=2.0*pi*(float)i/(float)steps;
    V2d next=V2d(x+r*sin(a),y+r*cos(a));
    line(last,next,c);
    last=next;
  }
}
void JBmp::opencircle(V2d cen,float r,int c) {
  opencircle(cen.x,cen.y,r,c);
}
uchar JBmp::chopbmp(int x,int y) {
  x=intmod(x,width);
  y=intmod(y,height);
  return bmp[y][x];
}
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
void JBmp::smooth(int r) {
  int *hs=new int[r+1];
  int cnt=0;
  for (register int i=0;i<=r;i++) {
    hs[i]=sqrt(r*r-i*i);
    if (i>0)
      cnt+=hs[i]*4+2;
    else
      cnt+=hs[i]*2+1;
  }
  for (register int x=r;x<width-r;x++)
  for (register int y=r;y<height-r;y++) {
    int c=0;
    for (register int i=-r;i<=r;i++) {
      for (register int j=-hs[abs(i)];j<=hs[abs(i)];j++) {
        int dx=x+i;
        int dy=y+j;
        c+=(int)bmp[dy][dx];
      }
    }
    spare[y][x]=(int)c/(int)cnt;
  }
  unsigned char **tmp=spare;
  spare=bmp;
  bmp=tmp;
}
// Variable declared in .h file
// Variable declared in .h file
// Variable declared in .h file
void JBmp::setupfastsmooth(int r) {
  fsrad=r;
  cnt=0;
  hs=new int[r+1];
  for (register int i=0;i<=r;i++) {
    hs[i]=sqrt(r*r-i*i);
    if (i>0)
      cnt+=hs[i]*4+2;
    else
      cnt+=hs[i]*2+1;
  }
}
void JBmp::fastsmooth() {
  for (register int i=0;i<fsrad;i++) {
    for (register int x=0;x<width;x++) {
      spare[i][x]=0;
      spare[height-1-i][x]=0;
    }
    for (register int y=0;y<height;y++) {
      spare[y][i]=0;
      spare[y][width-1-i]=0;
    }
  }
  for (register int x=fsrad;x<width-fsrad;x++)
  for (register int y=fsrad;y<height-fsrad;y++) {
    int c=0;
    for (register int i=-fsrad;i<=fsrad;i++) {
      for (register int j=-hs[abs(i)];j<=hs[abs(i)];j++) {
        int dx=x+i;
        int dy=y+j;
        c+=(int)bmp[dy][dx];
      }
    }
    spare[y][x]=(int)c/(int)cnt;
  }
  unsigned char **tmp=spare;
  spare=bmp;
  bmp=tmp;
}
void JBmp::randomfastsmooth() {
  for (register int i=0;i<fsrad;i++) {
    for (register int x=0;x<width;x++) {
      spare[i][x]=0;
      spare[height-1-i][x]=0;
    }
    for (register int y=0;y<height;y++) {
      spare[y][i]=0;
      spare[y][width-1-i]=0;
    }
  }
  for (register int x=fsrad;x<width-fsrad;x++)
  for (register int y=fsrad;y<height-fsrad;y++) {
    int c=0;
  int cnt=0;
    for (register int i=-fsrad;i<=fsrad;i++) {
      for (register int j=-hs[abs(i)];j<=hs[abs(i)];j++) {
        if (myrnd()<0.1 || cnt==0) {
        int dx=x+i;
        int dy=y+j;
        c+=(int)bmp[dy][dx];
        cnt++;
        }
      }
    }
    spare[y][x]=(int)c/(int)cnt;
  }
  unsigned char **tmp=spare;
  spare=bmp;
  bmp=tmp;
}
bool JBmp::inbmp(V2d p) {
  return inbmp(p.x,p.y);
}
// End class 
      

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
