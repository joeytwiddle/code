// Quadrilateral finder and sorter, uses quadfind.c

// #define NOGRAPHICS

#include <joeylib.c>

float cdclip;
float creepintoline;
float dotabove;
float minlen;
float oiscale;

JBmp jbmp;//=JBmp(640,480);

RGBmp *oi;
Map2d<float> *mag,*angs;

#include "quadfind.c"

String ql(List<Polygon2d> *ps) {
  String s="";
  for (int i=1;i<=ps->len;i++)
    s=Sconc(s,ps->p2num(i)->name);
  return s;
}

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
    offness=100.0*v.mod()/(float)cnt;
  // Using square offness=10.0*v.mod()*v.mod()/(float)cnt;
//    offness=v.mod();
  else
  offness=10000000;
  // Ignoring area, not
  // offness=100.0*v.mod()/10000.0;
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

  randomise();
  
  ArgParser a=ArgParser(argc,argv);  
  String fname=a.argafter("-i","GF lines filename","ls.gf");
  cdclip=a.floatafter("-ed","Maximum end distance of lines",30);
  creepintoline=a.floatafter("-ic","Intersection can creep in this much of the line",0.02);
  dotabove=a.floatafter("-pd","Parallelity: dot product must be above this",0.5);
  minlen=a.floatafter("-ml","Minimum length of a line",20);
  bool displaying=!a.argexists("nod","Don't display");
  String imname=a.argafter("-oi","Original image","none");
  oiscale=a.floatafter("-ois","Scale to original image",2);
  bool overlaylines=a.argexists("-ol","Overlay lines");
  int imwidth=a.intafter("-rqw","width of recovered quad images (pixels)",200);
  a.done();

  // Specific to my images
  if (Sstarts(imname,"image"))
    oiscale=2.5;
  if (Sstarts(imname,"img") || Slen(Sbefore(imname,"."))=2)
    oiscale=2.0;
  
  // Read list from file and find quads (looks easy!)
  List<QuadsLine2d> ls=readgfset(fname);
  helloworld();
  printf("Finding quads\n");
  List<Polygon2d> qs=findquads(ls);
  
  printf("df...");
  JFont coolfont=JFont(0.5);
  printf("fd...\n");

  if (!Seq(imname,"none")) {
    
    printf("Trying to read original file (%s)...",imname);
    oi=RGBmp::readfile(imname);
    Map2d<float> *gs=oi->getgreyscale();

  jbmp=JBmp(oi->width*0.5,oi->height*0.5);
  jbmp.clear(255);
  for (int i=1;i<=ls.len;i++) {
    QuadsLine2d l=ls.num(i)*oiscale*0.5;
    jbmp.line(l,0);
    //jbmp.thickline(l,1,255);
  }
  jbmp.writefile("lines.bmp");

  // Simple quad map
  Map2d<bool> jb=Map2d<bool>(oi->width*0.5,oi->height*0.5,255);
  for (int i=1;i<=qs.len;i++) {
    // jb.clear();
    Polygon2d p=qs.num(i);
    for (int j=1;j<=p.vs.len;j++) {
      jb.line(p.linefrom(j)*0.5*oiscale,0);
    }
  }
  jb.writefile("quads.bmp");
  
    // Edge detection
    printf("Doing edge detection\n");
    gs->edgedetection(Map2d<float>::sobel(),&mag,&angs);
    // angs->display();
    // exit(0);
    
    qs.applycall(&scalepoly); // No need for oiscale after this point
    
    for (int i=1;i<=qs.len;i++) {
      Polygon2d *quad=qs.p2num(i);
      quad->name=Schr(Sasc("A")+(i-1)%26);
    }

    // Overlay lines on original
    if (overlaylines) {
    RGBmp *linesover=oi->scaledby(0.5);
    for (int i=1;i<=ls.len;i++)
      linesover->thickline(0.5*oiscale*ls.num(i),myRGB(0,0,0),3);
    for (int i=1;i<=ls.len;i++)
      linesover->line(0.5*oiscale*ls.num(i),myRGB(0,255,255));
    linesover->writefile("linesover.bmp");
    }
    
    // Overlay quads on original
    RGBmp *rgbmp=oi->scaledby(0.5);
    for (int i=1;i<=qs.len;i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp->thickline(0.5*p.linefrom(j),myRGB(0,0,0),3);
      }
    }
    for (int i=1;i<=qs.len;i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp->line(0.5*p.linefrom(j),myRGB(0,255,255));
      }
    }
    rgbmp->writefile("overlay.bmp");

    RGBmp *detailed=new RGBmp(oi->width/2.0,oi->height/2.0,myRGB(255,255,255));//oi->scaledby(1);
    for (int i=1;i<=qs.len;i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        detailed->thickline(p.linefrom(j)*0.5,myRGB(0,0,0),1);
      }
//      V2d t=(p.vs.num(1)+p.vs.num(2)+p.vs.num(3)+p.vs.num(4))/4.0/2.0+V2d::random()*100/2.0;
      V2d t=V2d(myrnd()*detailed->width,myrnd()*detailed->height);
      for (int k=1;k<=4;k++) {
      V2d c=p.vs.num(k)/2.0;
      detailed->line(c,t,myRGB(0,0,0));
      }
      String data=Sformat("%s",p.name);
      coolfont.centerString(data,detailed,t.x,t.y,8,myRGB(0,0,0));
    }
    detailed->writefile("data.bmp");

    List<String> left;
    List<String> right;
    for (int i=1;i<=qs.len;i++) {
      Polygon2d *p=qs.p2num(i);
      String s=Sformat("%s score %.3f, area %.3f",p->name,scorefor(p),p->area());
      if (p->centroid().x<oi->width/2.0)
        left.add(s);
      else
        right.add(s);
    }
    List<String> data;
    data.add("-- Left:");
    data.add(left);
    data.add("-- Right:");
    data.add(right);
    writelinestofile(data,"data.txt");

    /* printf("Ordering\n");
       qs=qs.qsort(&offness);
       printf("Done ordering.\n");*/
    
    printf("Removing overlapping quads (%i) ...\n",qs.len);
    data.add("-- Rejection:");
    // Check all containers first
    for (int i=1;i<=qs.len;i++) {
      Polygon2d *quad=qs.p2num(i);
      bool done=false;
      for (int j=1;j<=qs.len && !done;j++) { // Compare to all others (one-directional)
        printf("Comparing (%i/%i) to %i : ",i,qs.len,j);
        if (i!=j) {
          
          Polygon2d *oquad=qs.p2num(j);
          data.add(Sformat("Comparing %s to %s %i %i",quad->name,oquad->name,i,j));
          
          bool overlaps=quad->overlaps(oquad,0.99);
          if (overlaps==Polygon2d::container) { // quad envelops oquad
            if (scorefor(quad)>0.8*scorefor(oquad)) {
    //          data.add(ql(&qs));
              data.add(Sformat("%s contained %s and rejected it",quad->name,oquad->name));
              qs.remnum(j);
              j--;
              if (i>j) {
                i--;
                done=true;
              }
              printf("Contained quad rejected");
      //        data.add(ql(&qs));
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
        //      data.add(ql(&qs));
            data.add(Sformat("%s beat %s.",quad->name,oquad->name));
            qs.remnum(j);
            j--;
            printf("Second quad rejected");
          //    data.add(ql(&qs));
          } else {
            //  data.add(ql(&qs));
            data.add(Sformat("%s beat %s,",oquad->name,quad->name));
            qs.remnum(i);
            i--;
            done=true;
            printf("First quad rejected");
              //data.add(ql(&qs));
          }
        }
        
        printf("\n");
      }
    }
    printf("Done: now (%i) quads.\n",qs.len);
    
    // Overlay again
    RGBmp *rgbmp2=oi->scaledby(0.5);
    for (int i=1;i<=qs.len;i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp2->thickline(0.5*p.linefrom(j),myRGB(0,0,0),3);
      }
    }
    for (int i=1;i<=qs.len;i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp2->line(0.5*p.linefrom(j),myRGB(0,255,255));
      }
      V2d t=0.5*p.centroid();
      // rgbmp2->line(oiscale*p.num(1),t,V3d(1,0,0));
      String data=Sformat("%.3f",scorefor(qs.p2num(i)));
      //coolfont.centerString(data,rgbmp2,t.x,t.y,9,myRGB(255,0,0));
    }
    rgbmp2->writefile("overlay2.bmp");
  
  // Simple quad map again
  Map2d<bool> jbq=Map2d<bool>(oi->width*0.5,oi->height*0.5,255);
  for (int i=1;i<=qs.len;i++) {
    // jb.clear();
    Polygon2d p=qs.num(i);
    for (int j=1;j<=p.vs.len;j++) {
      jbq.line(p.linefrom(j)*0.5,0);
    }
  }
  jbq.writefile("quads2.bmp");
      
    // Recover text
    List<RGBmp *> rec;
    // oi->display();
    for (int i=1;i<=qs.len && i<=5;i++) {
      List<V2d> *l=&qs.num(i).vs;
      RGBmp *n=oi->recoverquad(l,1,imwidth);
      n->writefile(getnextfilename("recquad","bmp"));
      rec.add(n);
    }
    
    if (displaying) {
//      waitforkeypress();
      jbmp.display();
      jb.display();
      for (int i=1;i<=rec.len;i++)
      rec.num(i)->display();
    }

    data.add("-- Final quads:");
    for (int i=1;i<=qs.len;i++) {
      Polygon2d *p=qs.p2num(i);
      String s=Sformat("%s %.3f",p->name,scorefor(p));
      data.add(s);
    }
    writelinestofile(data,"data.txt");
  
  }

//  allegroexit();
  printf("%i quadrilaterals found\n",qs.len);
  
}
