class RGBHistogram {
public:
  int quant,numsamps;
  uchar ***rgb;
  
  RGBHistogram() {
    int q=6;
    quant=q;
    numsamps=0;
    rgb=new (uchar**)[q];
    for (int i=0;i<q;i++) {
      rgb[i]=new (uchar*)[q];
      for (int j=0;j<q;j++) {
        rgb[i][j]=new uchar[q];
        for (int k=0;k<q;k++)
        rgb[i][j][k]=0;
      }
    }
  }
  
  RGBHistogram(int q) {
    quant=q;
    numsamps=0;
    rgb=new (uchar**)[q];
    for (int i=0;i<q;i++) {
      rgb[i]=new (uchar*)[q];
      for (int j=0;j<q;j++) {
        rgb[i][j]=new uchar[q];
        for (int k=0;k<q;k++)
        rgb[i][j][k]=0;
      }
    }
  }
  
  float correlation() {

    List<float> rs=List<float>(quant*quant*quant);
    List<float> gs=List<float>(quant*quant*quant);
    List<float> bs=List<float>(quant*quant*quant);
    int cnt=0;
    for (int i=0;i<quant;i++)
    for (int j=0;j<quant;j++)
    for (int k=0;k<quant;k++)
    for (int l=1;l<=rgb[i][j][k];l++) {
      rs.add(i);
      gs.add(j);
      bs.add(k);
      cnt++;
    }

    float xx=corrS(rs,rs);
    float yy=corrS(gs,gs);
    float zz=corrS(bs,bs);
    float xy=corrS(rs,gs);
    float yz=corrS(gs,bs);
    float zx=corrS(bs,rs);

    float tmp= xy/sqrt(xx*yy) *
               yz/sqrt(yy*zz) *
               zx/sqrt(zz*xx);
//    float tmp=lscorrelation(rs,gs)*lscorrelation(gs,bs)*lscorrelation(bs,rs);
    rs.freedom();
    gs.freedom();
    bs.freedom();
//    printf("(%f) ",tmp);
    return tmp;
  }

    /*

    // Find averages
    long c=0;
    float ax=0,ay=0,az=0;
    for (int i=0;i<quant;i++)
    for (int j=0;j<quant;j++)
    for (int k=0;k<quant;k++) {
      int r=rgb[i][j][k];
      c=c+r;
      ax=ax+r*i;
      ay=ay+r*j;
      az=az+r*k;
    }
    ax=ax/(float)c;
    ay=ay/(float)c;
    az=az/(float)c;
    float sxyz=0,sxxx=0,syyy=0,szzz=0;
    for (i=0;i<quant;i++)
    for (int j=0;j<quant;j++)
    for (int k=0;k<quant;k++) {
      sxyz=sxyz+(float)rgb[i][j][k]*((float)i-ax)*((float)j-ay)*((float)k-az)/(float)quant/(float)quant/(float)quant;
      sxxx=sxxx+(float)rgb[i][j][k]*((float)i-ax)*((float)i-ax)*((float)i-ax)/(float)quant/(float)quant/(float)quant;
      syyy=syyy+(float)rgb[i][j][k]*((float)j-ay)*((float)j-ay)*((float)j-ay)/(float)quant/(float)quant/(float)quant;
      szzz=szzz+(float)rgb[i][j][k]*((float)k-az)*((float)k-az)*((float)k-az)/(float)quant/(float)quant/(float)quant;
    }
    // printf("<%f %f,%f,%f> ",sxyz,sxxx,syyy,szzz);
    return (sxyz*sxyz*sxyz)/(sxxx*syyy*szzz);
    // return (sxxx*syyy*szzz)/(sxyz*sxyz*sxyz);
  }   */
  
  float variance() {
    long c=0;
    float ax=0,ay=0,az=0;
    for (int i=0;i<quant;i++)
    for (int j=0;j<quant;j++)
    for (int k=0;k<quant;k++) {
      int r=rgb[i][j][k];
      c=c+r;
      ax=ax+r*i;
      ay=ay+r*j;
      az=az+r*k;
    }
    ax=ax/(float)c;
    ay=ay/(float)c;
    az=az/(float)c;
    float var=0;
    for (int i=0;i<quant;i++)
    for (int j=0;j<quant;j++)
    for (int k=0;k<quant;k++) {
      var=var+rgb[i][j][k]*mysquare((V3d((float)i-ax,(float)j-ay,(float)k-az)).mod());
    }
    var=var/(float)quant/(float)quant/(float)quant/c;
    return var;
  }
  
  void addocc(myRGB *c) {
    int r=(int)(quant*c->r/256);
    int g=(int)(quant*c->g/256);
    int b=(int)(quant*c->b/256);
    if (r<0 || r>=quant || g<0 || g>=quant || b<0 || b>=quant) {
      printf("Trying to add to RGBHistogram outside range: %i %i %i\n",c->r,c->g,c->b);
      exit(0);
    }
    rgb[r][g][b]=rgb[r][g][b]+1;
    numsamps++;
  }
  /* RGBHistogram *normsmoothed(int radius) {
       RGBHistogram *n=new RGBHistogram(quant);
       int count=0;
       for (int dx=-radius;dx<=radius;dx++) {
         int height=(int)sqrt(radius*radius-dx*dx);
         for (int dy=-height;dy<=height;dy++) {
           int depth=(int)sqrt(radius*radius-dx*dx-dy*dy);
           for (int dz=-height;dz<=height;dz++) {
             count++;
           }
         }
       }
       for (int i=radius;i<quant-radius;i++)
       for (int j=radius;j<quant-radius;j++)
       for (int k=radius;k<quant-radius;k++) {
         float sum=0;
         for (int dx=-radius;dx<=radius;dx++) {
           int height=(int)sqrt(radius*radius-dx*dx);
           for (int dy=-height;dy<=height;dy++) {
             int depth=(int)sqrt(radius*radius-dx*dx-dy*dy);
             for (int dz=-height;dz<=height;dz++) {
               sum=sum+rgb[i+dx][j+dy][k+dx];
             }
           }
         }
         n->rgb[i][j][k]=(int)sum/count;
       }
       // n.done();
       return n;
     }*/
  
  RGBHistogram *normsmoothed(int radius) {
    RGBHistogram *n=new RGBHistogram(quant);
    for (int i=0;i<quant;i++)
    for (int j=0;j<quant;j++)
    for (int k=0;k<quant;k++) {
      int count=0;
      float sum=0;
      for (int dx=-radius;dx<=radius;dx++) {
        int height=(int)sqrt(radius*radius-dx*dx);
        for (int dy=-height;dy<=height;dy++) {
          int depth=(int)sqrt(radius*radius-dx*dx-dy*dy);
          for (int dz=-height;dz<=height;dz++) {
            int px=i+dx; int py=j+dy; int pz=k+dz;
            if (px>=0 && px<quant && py>=0 && py<quant && pz>=0 && pz<quant) {
              sum=sum+rgb[px][py][pz];
              count++;
            }
          }
        }
      }
      n->rgb[i][j][k]=(int)sum/count;
    }
    // n->done();
    return n;
  }
  
  // Gaussian
  RGBHistogram *smoothed(int radius) {
    RGBHistogram *n=new RGBHistogram(quant);
    float gaus[radius+1][radius+1][radius+1];
    float count=0;
    for (int dx=-radius;dx<=radius;dx++) {
      int height=(int)sqrt(radius*radius-dx*dx);
      for (int dy=-height;dy<=height;dy++) {
        int depth=(int)sqrt(radius*radius-dx*dx-dy*dy);
        for (int dz=-height;dz<=height;dz++) {
          gaus[abs(dx)][abs(dy)][abs(dz)]=gaussian(sqrt(dx*dx+dy*dy+dz*dz)/radius);
          count=count+gaus[abs(dx)][abs(dy)][abs(dz)];
        }
      }
    }
    for (int i=radius;i<quant-radius;i++)
    for (int j=radius;j<quant-radius;j++)
    for (int k=radius;k<quant-radius;k++) {
      float sum=0;
      for (int dx=-radius;dx<=radius;dx++) {
        int height=(int)sqrt(radius*radius-dx*dx);
        for (int dy=-height;dy<=height;dy++) {
          int depth=(int)sqrt(radius*radius-dx*dx-dy*dy);
          for (int dz=-height;dz<=height;dz++) {
            sum=sum+gaus[abs(dx)][abs(dy)][abs(dz)]*rgb[i+dx][j+dy][k+dx];
          }
        }
      }
      n->rgb[i][j][k]=(int)sum/count;
    }
    // n.done();
    return n;
  }
  static float difference(RGBHistogram a,RGBHistogram b) {
    if (a.quant!=b.quant)
      myerror("Can't find difference between two differently quantised histograms.");
    float diff=0;
    for (int i=0;i<a.quant;i++)
    for (int j=0;j<a.quant;j++)
    for (int k=0;k<a.quant;k++)
    diff=diff+mysquare(a.rgb[i][j][k]-b.rgb[i][j][k]);
    return (float)diff/(float)a.quant/(float)a.quant/(float)a.quant;
  }
  void display() {
    #ifdef ALLEGRO
    int winwid=640,winhei=480;
    set_color_depth(24);
    allegrosetup(winwid,winhei);
    PALETTE pal;
    generate_332_palette(pal);
    set_palette(pal);
    float rot=pi/4,elev=pi/8,rotspeed=pi/64;
    int smallize=largest();
    PPsetup(winwid,winhei,3);
    BITMAP *b=create_bitmap(winwid,winhei);
    do {
      clear(b);
      Matrix m1=Matrix();
      m1.makerotation(V3d(0,1,0),rot);
      Matrix m2=Matrix();
      m2.makerotation(V3d(1,0,0),elev);
      for (int i=0;i<quant;i++) {
        for (int j=0;j<quant;j++) {
          for (int k=0;k<quant;k++) {
            V3d c=V3d(i,j,k);
            c=c/c.getlongestside();
            c=c*(float)rgb[i][j][k]/smallize*255;
            // c=c*255;
            c.chop(0,255);
            if (c.getlongestside()>64) {
              V3d v=2*(V3d(i,j,k)/quant-V3d(.5,.5,.5));
              // v=V3d::rotate(v,V3d(0,1,0),rot);
              // v=V3d::rotate(v,V3d(1,0,0),elev);
              v=m1*v;
              v=m2*v;
              V3d u=v+V3d(.02,0,0);
              int x,y,rx,dx;
              PPgetscrpos(v,&x,&y);
              PPgetscrpos(u,&rx,&dx);
              dx=rx-x;
              circle(b,x,y,dx,makecol(c.x,c.y,c.z));
            }
          }
        }
      }
      blit(b,screen,0,0,0,0,winwid,winhei);
      if (key[KEY_LEFT])
        rot-=rotspeed;
      if (key[KEY_RIGHT])
        rot+=rotspeed;
      if (key[KEY_UP])
        elev-=rotspeed;
      if (key[KEY_DOWN])
        elev+=rotspeed;
      if (key[KEY_Z])
        smallize=smallize*1.2;
      if (key[KEY_X])
        smallize=smallize/1.2;
    } while (!key[KEY_ESC]);
    #endif
  }
  int largest() {
    int largest=0;
    for (int i=0;i<quant;i++) {
      for (int j=0;j<quant;j++) {
        for (int k=0;k<quant;k++) {
          if (rgb[i][j][k]>largest)
            largest=rgb[i][j][k];
        }
      }
    }
    return largest;
  }
  int maximumat(int i,int j,int k) {
    int rad=2;
    if (i-rad<0 || i+rad>=quant || j-rad<0 || j+rad>=quant || k-rad<0 || k+rad>=quant)
      return 0;
    int tmp=rgb[i][j][k];
    if (tmp<2)
      return 0;
    for (int x=-rad;x<=rad;x++)
    for (int y=-rad;y<=rad;y++)
    for (int z=-rad;z<=rad;z++)
    if (x!=0 && y!=0 && z!=0)
      if (rgb[i+x][j+y][k+z]>tmp)
      return 0;
    return 1;
  }
  int eightmaximumat(int i,int j,int k) {
    int tmp=rgb[i][j][k];
    for (int x=-1;x<=1;x+=2)
    for (int y=-1;y<=1;y+=2)
    for (int z=-1;z<=1;z+=2)
    if (rgb[i+x][j+y][k+z]>=tmp)
      return 0;
    return 1;
  }
  int fourmaximumat(int i,int j,int k) {
    return (rgb[i][j][k] > rgb[i-1][j][k] &&
    rgb[i][j][k] > rgb[i+1][j][k] &&
    rgb[i][j][k] > rgb[i][j-1][k] &&
    rgb[i][j][k] > rgb[i][j+1][k] &&
    rgb[i][j][k] > rgb[i][j][k-1] &&
    rgb[i][j][k] > rgb[i][j][k+1]
    );
  }
};

float tofloat(RGBHistogram r) {
  return r.correlation();
}
