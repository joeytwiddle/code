// Quadrilateral finder and sorter

// #define NOGRAPHICS

#include <joeylib.c>

float cdclip;
float creepintoline;
float dotabove;
float minlen;
float oiscale;

JBmp jbmp=JBmp(640,480);

RGBmp *oi;
Map2d<float> *mag,*angs;

#include "quadfind.c"

float areaofpoly(Polygon2d p) {
  return p.area();
}

void scalepoly(Polygon2d *p) {
  p->scale(oiscale);
}

float offness(Polygon2d *p) {
  if (p->regionised==NULL) {
    p->regionised=new Region(p->scaleaboutcentroid(0.9),oi->width,oi->height);
//    p->regionised->contract(10);
  }
  Region r=*p->regionised;
  r.makelist();
  V2d v=V2d(0,0);
  int cnt=0;
  for (int i=1;i<=r.list->len;i++) {
    int x=r.list->num(i).x;
    int y=r.list->num(i).y;
    if (angs->inmap(x,y)) {
      v=v+mag->pos[x][y]*V2d::rotate(V2d(1,0),angs->pos[x][y]);
      cnt++;
    }
  }
  float offness;
  if (cnt>0)
    offness=1000.0*v.mod()/(float)cnt*10000.0/(float)r.list->len;
  else
  offness=100000;
  // printf("Quad has offness %f count %i area %f\n",offness,cnt,p.area());
  r.freedom();
  return offness;
}

float scorefor(Polygon2d *p) {
  if (p->score==-1)  {
    printf("<working...");
    p->score=1.0/offness(p);
    printf("done>");
    return p->score;
  }
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);  
  String fname=a.argafter("-i","GF lines filename","ls.gf");
  cdclip=a.floatafter("-ed","Maximum end distance of lines",30);
  creepintoline=a.floatafter("-ic","Intersection can creep in this much of a the line",0.02);
  dotabove=a.floatafter("-pd","Parallelity: dot product must be above this",0.5);
  minlen=a.floatafter("-ml","Minimum length of a line",20);
  bool displaying=!a.argexists("nod","Don't display");
  String imname=a.argafter("-oi","Original image","none");
  oiscale=a.floatafter("-ois","Scale to original image",2);

  JFont coolfont=JFont("c:/joey/code/c/gfx2d/jfont/cool.jf",0.4);
  printf("fd...\n");
  // Read list from file and find quads (looks easy!)
  List<QuadsLine2d> ls=readgfset(fname);
  helloworld();
  printf("Finding quads\n");
  List<Polygon2d> qs=findquads(ls);
  
  // Simple quad map
  Map2d<bool> jb=Map2d<bool>(640,480,15);
  for (int i=1;i<=qs.len && !key[KEY_ESC];i++) {
    // jb.clear();
    Polygon2d p=qs.num(i);
    for (int j=1;j<=p.vs.len;j++) {
      jb.line(p.linefrom(j),0);
    }
  }
  jb.writefile("quads.bmp");
  
  if (displaying) {
    jbmp.display();
    jb.display();
    allegroexit();
  }
  
  if (!Seq(imname,"none")) {
    
    oi=RGBmp::readfile(imname);
    Map2d<float> *gs=oi->getgreyscale();
    
    // Overlay quads on original
    RGBmp *rgbmp=oi->scaledby(0.5);
    for (int i=1;i<=qs.len && !key[KEY_ESC];i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp->thickline(0.5*oiscale*p.linefrom(j),myRGB(0,0,0),3);
      }
    }
    for (int i=1;i<=qs.len && !key[KEY_ESC];i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp->line(0.5*oiscale*p.linefrom(j),myRGB(0,255,255));
      }
    }
    rgbmp->writefile("overlay.bmp");

    // Edge detection
    printf("Doing edge detection\n");
    gs->edgedetection(Map2d<float>::sobel(),&mag,&angs);
    // angs->display();
    // exit(0);
    
    RGBmp *detailed=oi->scaledby(1);
    for (int i=1;i<=qs.len && !key[KEY_ESC];i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        detailed->line(oiscale*p.linefrom(j),myRGB(0,255,0));
      }
    }
    detailed->writefile("data.bmp");
    
    /* printf("Ordering\n");
       qs=qs.qsort(&offness);
       printf("Done ordering.\n");*/
    
    qs.applycall(&scalepoly);
    
    // Should check all containers first
    printf("Removing overlapping quads (%i) ...\n",qs.len);
    for (int i=1;i<=qs.len;i++) {
      Polygon2d *quad=qs.p2num(i);
      bool done=false;
      for (int j=1;j<=qs.len && !done;j++) { // Compare to all others (one-directional)
        printf("Comparing (%i/%i) to %i : ",i,qs.len,j);
        if (i!=j) {
          
          Polygon2d *oquad=qs.p2num(j);
          
          bool overlaps=quad->overlaps(oquad,0.95);
          if (overlaps==Polygon2d::container) { // quad envelops oquad
            if (scorefor(quad)>0.8*scorefor(oquad)) {
              qs.remnum(j);
              j--;
              if (i>j)
                i--;
              printf("Contained quad rejected");
            }
          }
          
        }

        printf("\n");
      }
    }
    // Then overlapping in general
    for (int i=1;i<=qs.len;i++) {
      Polygon2d *quad=qs.p2num(i);
      bool done=false;
      for (int j=i+1;j<=qs.len && !done;j++) { // bi-derectional
        printf("Comparing (%i/%i) to %i : ",i,qs.len,j);
        Polygon2d *oquad=qs.p2num(j);
        
        bool overlaps=quad->overlaps(oquad);
        if (overlaps) {
          if (scorefor(quad)>scorefor(oquad)) {
            qs.remnum(j);
            j--;
            printf("Second quad rejected");
          } else {
            qs.remnum(i);
            i--;
            done=true;
            printf("First quad rejected");
          }
        }
        
        printf("\n");
      }
    }
    printf("Done: now (%i) quads.\n",qs.len);
    
    // Overlay again
    RGBmp *rgbmp2=oi->scaledby(0.5);
    for (int i=1;i<=qs.len && !key[KEY_ESC];i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp2->thickline(0.5*p.linefrom(j),myRGB(0,0,0),3);
      }
    }
    for (int i=1;i<=qs.len && !key[KEY_ESC];i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp2->line(0.5*p.linefrom(j),myRGB(0,255,255));
      }
      V2d t=p.centroid();
      rgbmp2->line(oiscale*p.num(1),t,V3d(1,0,0));
      String data=Sformat("Score %f Area %f",scorefor(qs.p2num(i)),p.area());
      coolfont.centerString(data,rgbmp2,t.x,t.y,12,myRGB(0,255,0));
    }
    rgbmp2->writefile("overlay2.bmp");
    
    // Recover text
    List<RGBmp *> rec;
    // oi->display();
    for (int i=1;i<=qs.len && i<=5;i++) {
      List<V2d> *l=&qs.num(i).vs;
      RGBmp *n=oi->recoverquad(l,1);
      n->writefile(getnextfilename("recquad","bmp"));
      rec.add(n);
    }
    
    if (displaying) {
      waitforkeypress();
      jbmp.display();
      jb.display();
      for (int i=1;i<=rec.len && !key[KEY_ESC];i++)
      rec.num(i)->display();
    }
  }
  
  allegroexit();
  printf("%i quadrilaterals found\n",qs.len);
  
}
