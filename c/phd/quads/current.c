// Quadrilateral finder and sorter, uses quadfind.c

// #define NOGRAPHICS

#include <joeylib.c>

float cdclip;
float creepintoline;
float dotabove;
float minlen;
float oiscale;
bool dividebyarea;
float favourlarge;
bool oldmethod;
bool dimbg;

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
  float cnt=0;
  int pixcnt=0;
  for (int i=1;i<=r.list->len;i++) {
    int x=r.list->num(i).x;
    int y=r.list->num(i).y;
    if (angs->inmap(x,y)) {
      v=v+mag->pos[x][y]*V2d::rotate(V2d(1,0),angs->pos[x][y]);
      if (oldmethod)
        cnt++;
      else
        cnt+=mag->pos[x][y];
      pixcnt++;
    }
  }
  float offness;
  // Using square offness=10.0*v.mod()*v.mod()/(float)cnt;
  // offness=v.mod();
  if (cnt>0) {
    return 100000.0*v.mod()/cnt/(float)pixcnt;
    offness=v.mod();
    if (dividebyarea)
      offness=offness/(float)cnt;
    else
      offness=offness/10000.0;
    offness=1000.0*offness/(float)pixcnt;
  } else
  offness=10000000;
  // printf("Quad has offness %f count %i area %f\n",offness,cnt,p.area());
  r.freedom();
  return offness;
}

float scorefor(Polygon2d *p) {
  if (p->score==-1)  {
    printf("<working...");
//    if (oldmethod)
      p->score=log(1.0/offness(p));
//    else
//      p->score=-offness(p)*100.0;
    printf("done>");
    return p->score;
  }
}

myRGB dodim(myRGB r) {
  if (dimbg)
    return myRGB(0,0,(int)((float)(r.r+r.g+r.b)/3.0));
  else
    return r;
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
//  float favourlarge=a.floatafter("-fl","Favour large by",0.8); // old method
  float favourlarge=a.floatafter("-fl","Favour large by",1.2);
  dividebyarea=!a.argexists("-dd","Don't divide V by area");
  oldmethod=a.argexists("-osm","Old stats method (area not sum mags, score reciprocal not -ve)");
  bool turquoise=a.argexists("-tur","turquoise overlaid lines, not white");
  dimbg=a.argexists("-dbg","dim background");
  a.done();

  myRGB overcol=myRGB( ( turquoise ? 0 : 255 ), 255,255);

  // Specific to my images
  String tmp=imname;
  printf("Auto scale %s? ",tmp);
  if (Sinstrlast(tmp,"/"))
    tmp=Sfrom(tmp,Sinstrlast(tmp,"/")+1);
  if (Sinstrlast(tmp,"\\"))
    tmp=Sfrom(tmp,Sinstrlast(tmp,"\\")+1);
  if (Sstarts(tmp,"image")) {
    printf("2.5");
    oiscale=2.5;
  }
  if (Sstarts(tmp,"img") || Slen(Sbefore(tmp,"."))==2) {
    printf("2.0");
    oiscale=2.0;
  }
  printf("\n");
  
  // Read list from file and find quads (looks easy!)
  printf("0\n");
  List<QuadsLine2d> ls=readgfset(fname);
  printf("a\n");
  printf("Finding quads\n");
  List<Polygon2d> qs=findquads(ls);
  
  printf("df...");
  JFont coolfont=JFont(0.5);
  printf("fd...\n");

  printf("b\n");

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
    RGBmp *linesover=oi->applyfn(&dodim);
    for (int i=1;i<=ls.len;i++)
      linesover->thickline(oiscale*ls.num(i),myRGB(0,0,0),8);
    for (int i=1;i<=ls.len;i++)
      linesover->thickline(oiscale*ls.num(i),overcol,2);
    linesover->scaledby(0.5)->writefile("linesover.bmp");
    }
    
    // Overlay quads on original
    RGBmp *rgbmp=oi->applyfn(&dodim);
    for (int i=1;i<=qs.len;i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp->thickline(p.linefrom(j),myRGB(0,0,0),8);
      }
    }
    for (int i=1;i<=qs.len;i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp->thickline(p.linefrom(j),overcol,2);
      }
    }
    rgbmp->scaledby(0.5)->writefile("overlay.bmp");

    RGBmp *detailed=new RGBmp(oi->width/2.0,oi->height/2.0,myRGB(255,255,255));//oi->scaledby(1);
    for (int i=1;i<=qs.len;i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        detailed->line(p.linefrom(j)*0.5,myRGB(0,0,0));
      }
//      V2d t=(p.vs.num(1)+p.vs.num(2)+p.vs.num(3)+p.vs.num(4))/4.0/2.0+V2d::random()*100/2.0;
      V2d t=V2d(myrnd()*detailed->width,myrnd()*detailed->height);
      for (int k=1;k<=4;k++) {
      V2d c=p.vs.num(k)/2.0;
      detailed->line(c,t,myRGB(0,0,0));
      }
      String data=Sformat("%s",p.name);
//      coolfont.centerString(data,detailed,t.x,t.y,8,myRGB(0,0,0));
    }
    detailed->writefile("data.bmp");

/*    List<String> left;
    List<String> right;
    for (int i=1;i<=qs.len;i++) {
      Polygon2d *p=qs.p2num(i);
      String s=Sformat("%s score %.3f, area %.3f",p->name,scorefor(p),p->area());
      if (p->centroid().x<oi->width/2.0)
        left.add(s);
      else
        right.add(s);
    } */
    
    List<String> data;
/*  //    data.add(Sformat("GF %s td=%.2f ti=%.2d tp=%.2f minlen=%.2f Image %s scale=%.2f  widthofrecquad=%.2f favourlarge=%.2f",fname,cdclip,creepintoline,dotabove,minlen,imname,oiscale,favourlarge));
    String tmp="";
    if (dividebyarea)
      tmp=Sconc(tmp,"Dividing V by area.  ");
    data.add(tmp);
    data.add("");
    data.add("-- Left:");
    data.add(left);
    data.add("-- Right:");
    data.add(right);
    writelinestofile(data,"data.txt"); */

    /* printf("Ordering\n");
       qs=qs.qsort(&offness);
       printf("Done ordering.\n");*/
    
  printf("c\n");

/*
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
    } */
/*  Commented because it crashes - but Should Be In!
    // Then overlapping in general
    for (int i=1;i<=qs.len;i++) {
      printf("A\n");
      Polygon2d *quad=qs.p2num(i);
      bool done=false;
      printf("B\n");
      for (int j=i+1;j<=qs.len && !done;j++) { // bi-derectional
        printf("Comparing (%i/%i) to %i : ",i,qs.len,j);
        Polygon2d *oquad=qs.p2num(j);

      printf("C\n");
        bool overlaps=quad->overlaps(oquad);
      printf("D\n");
        if (overlaps) {
          if (scorefor(quad)>scorefor(oquad)) {
            printf("Second quad rejected");
            //      data.add(ql(&qs));
            data.add(Sformat("%s beat %s.",quad->name,oquad->name));
            qs.remnum(j);
            j--;
          //    data.add(ql(&qs));
          } else {
            printf("First quad rejected");
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
      printf("Z\n");
    }
    printf("Done: now (%i) quads.\n",qs.len);
    */


    // Recover text
    List<RGBmp *> rec;
    // oi->display();
    for (int k=1;k<=qs.len && k<=5;k++) {
      List<V2d> l=qs.p2num(k)->vs;
      V3d eye=V3d(oi->width/2,oi->height/2,-1000);
      List<V3d> ws=rectanglefromquadrilateral(l,eye);
    V3d tl=ws.num(1);
    V3d tr=ws.num(2);
    V3d br=ws.num(3);
    V3d bl=ws.num(4);
    float aspect=(V3d::dist(tr,br)+V3d::dist(bl,tl))/(V3d::dist(tl,tr)+V3d::dist(br,bl));
    int size=max(V3d::dist(tl,tr),V3d::dist(bl,br))*4;
    if (size>500)
      size=500;
    RGBmp n=RGBmp(size,size*aspect);
    for (int i=0;i<n.width;i++)
    for (int j=0;j<n.height;j++) {
      float xthru=(float)i/(float)(n.width-1);
      float ythru=(float)j/(float)(n.height-1);
      V3d world=tl+xthru*(tr-tl)+ythru*(bl-tl);
      V2d hit=rectangleproject(world,eye);
      n.setpos(i,j,oi->getinter(hit.x,hit.y));
    }
      n.writefile(getnextfilename("recquad","bmp"));
      // rec.add(n);
    }
    


      printf("A\n");
    // Overlay again
    RGBmp *rgbmp2=oi->applyfn(&dodim);
    for (int i=1;i<=qs.len;i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp2->thickline(p.linefrom(j),myRGB(0,0,0),8);
      }
    }
    for (int i=1;i<=qs.len;i++) {
      Polygon2d p=qs.num(i);
      for (int j=1;j<=p.vs.len;j++) {
        rgbmp2->thickline(p.linefrom(j),overcol,2);
      }
      V2d t=p.centroid();
      // rgbmp2->line(oiscale*p.num(1),t,V3d(1,0,0));
      String data=Sformat("%.3f",scorefor(qs.p2num(i)));
      //coolfont.centerString(data,rgbmp2,t.x,t.y,9,myRGB(255,0,0));
    }
    rgbmp2->scaledby(0.5)->writefile("overlay2.bmp");
  
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
      
  printf("d\n");

    data.add("-- Final quads:");
    for (int i=1;i<=qs.len;i++) {
      Polygon2d *p=qs.p2num(i);
      String s=Sformat("%s %.3f",p->name,scorefor(p));
      data.add(s);
    }
    writelinestofile(data,"data.txt");
  

    if (displaying) {
//      waitforkeypress();
      jbmp.display();
      jb.display();
      for (int i=1;i<=rec.len;i++)
        rec.num(i)->display();
    }

  }

//  allegroexit();
  printf("%i quadrilaterals found\n",qs.len);
  
}
