// Vector change

/* All my source code is freely distributable under the GNU public licence.
I would be delighted to hear if have made use of this code.
If you make money with this code, please give me some!
If you find this code useful, or have any queries, please feel free to
contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
Paul "Joey" Clark, hacking for humanity, Feb 99
www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

class Cluster {
public:
  Vnd pos;
  float str; // = number of points if all weighted 1
  Cluster() { }
  Cluster(Vnd p) {
    pos=p;
    str=1.0;
  }
  void add(Vnd *v) {
    #ifdef DEBUG
      printf("A");
    #endif
    Vnd newpos=pos*str;
    #ifdef DEBUG
      printf("b");
    #endif
    Vnd newpos2=newpos+v;
    #ifdef DEBUG
      printf("c");
    #endif
    str=str+1.0;
    Vnd newpos3=newpos2/str;
    #ifdef DEBUG
      printf("d");
    #endif
    pos.freedom();
    newpos.freedom();
    newpos2.freedom();
    pos=newpos3;
    #ifdef DEBUG
      printf("Z");
    #endif
//    newpos2.freedom(); // very dodge!
  }
}; 

void main(int argc,String *argv) {

ArgParser a=ArgParser(argc,argv);
String clusttype=a.argafter("-ct","cluster type (grid/line/random)","line");
int clustquant=a.intafter("-cq","cluster quantisation",3);
bool findchange=a.argexists("-vc","Calculate vector change");
List<String> maps;
while (a.argsleft()) {
  maps.add(a.getarg(1));
//  printf("%s\n",maps.num(maps.len));
}
a.done();

randomise();

//maps.add("histvar.bmp");
//maps.add("myeadvis.bmp");
//  maps.add("histdiff.bmp");
//  maps.add("histcor.bmp");
//  maps.add("eadvis.bmp");

List<Map2d<float> *> ms;
for (int i=1;i<=maps.len;i++)
ms.add(Map2d<float>::readfile(maps.num(i)));

int width=ms.num(1)->width;
int height=ms.num(1)->height;

if (width*height>80000)
  printf("Warning:  Size %i may be too large!\n",width*height);

printf("Generating $i-d image/n",maps.len);
Map2d<Vnd > *map=new Map2d<Vnd >(width,height,Vnd(maps.len));
for (int x=0;x<width;x++)
for (int y=0;y<height;y++)
map->pos[x][y]=Vnd(maps.len);
for (int i=1;i<=maps.len;i++) {
Map2d<float> *lmap=ms.num(i);
for (int x=0;x<width;x++)
for (int y=0;y<height;y++) {
int lx=x*lmap->width/width;
int ly=y*lmap->height/height;
map->pos[x][y].ds[i-1]=lmap->pos[lx][ly];
}
}

List<Cluster> cs;
printf("Generating clusters...\n");
if (Seq(clusttype,"grid")) {
// Clusters in a grid
List<Vnd> *vs=new List<Vnd>();
vs->add(Vnd(maps.len));
for (int i=1;i<=maps.len;i++) {
  List<Vnd> *newvs=new List<Vnd>();
  for (int j=1;j<=vs->len;j++) {
    Vnd v=vs->num(j);
    for (int k=1;k<=clustquant;k++) {
      float d=(float)(k-1)/(float)(clustquant-1);
      printf("%f ",d);
      v=Vnd(v);
      v.setaxis(i,d);
      newvs->add(v);
    }
  }
//  destroy(vs);
  vs=newvs;
}
for (int i=1;i<=vs->len;i++) {
  cs.add(Cluster(vs->num(i)));
}
}

if (Seq(clusttype,"line")) {
// Clusters in a simple diagonal line
for (int i=1;i<=clustquant;i++) {
  printf("*\n");
  Vnd v=Vnd(maps.len);
  for (int j=1;j<=maps.len;j++) {
    v.setaxis(j,(float)(i-1)/(float)(clustquant-1));
  }
  cs.add(Cluster(v));
}
}

if (Seq(clusttype,"random")) {
for (int i=1;i<=clustquant;i++) {
  printf("*\n");
  Vnd v=Vnd(maps.len);
  for (int j=1;j<=maps.len;j++) {
    v.setaxis(j,myrnd());
  }
  cs.add(Cluster(v));
}
}
  
// Mix up data
/* // Mixing up the actual vectors
printf("Mixing up data...\n");
OrderedList<Vnd> ol;
for (int i=0;i<width;i++) {
  for (int j=0;j<height;j++) {
//  printf("*\n");
    ol.add(&map->pos[i][j],myrnd());
  }
}
List<Vnd> data=ol.getascendinglist();*/

// Mixing up the image pixels
OrderedList<V2d> ol;
for (int i=0;i<width;i++) {
  for (int j=0;j<height;j++) {
//  printf("*\n");
    ol.add(new V2d(i,j),myrnd());
  }
}
List<V2d> data=ol.getascendinglist();

/* // Doing it with vectors
printf("Parsing data, forming clusters...\n");
for (int i=1;i<=data.len;i++) {
  if (i%1000==0)
    printf("%i/%i...\n",i,data.len);
  Vnd v=*data.num(i);
  // Find closest cluster
  int closest=1;
  float dist=Vnd::dist(cs.num(1).pos,v);
  for (int j=2;j<=cs.len;j++) {
    float ldist=Vnd::dist(cs.num(j).pos,v);
    if (ldist<dist) {
      closest=j;
      dist=ldist;
    }
  }
  cs.p2num(closest)->add(v);
}*/

// Doing it with pixels
printf("Parsing data, choosing clusters...\n");
RGBmp rgbmp2=RGBmp(width,height);
for (int i=1;i<=data.len;i++) {
  if (i%1000==0)
    printf("%i/%i...\n",i,data.len);
  V2d v2=data.num(i);
  int x=v2.x;
  int y=v2.y;
  Vnd *v=&map->pos[x][y];
  // Find closest cluster
  int closest=1;
  float dist=Vnd::dist(&cs.p2num(1)->pos,v);
  for (int j=2;j<=cs.len;j++) {
    float ldist=Vnd::dist(&cs.p2num(j)->pos,v);
    if (ldist<dist) {
      closest=j;
      dist=ldist;
    }
  }
  cs.p2num(closest)->add(v);
  rgbmp2.bmp[x][y]=myRGB::hue((float)closest/(float)cs.len); // /Vnd::dist(v,&cs.p2num(closest)->pos);
}

rgbmp2.writefile("clustgrow.bmp");

for (int i=1;i<=cs.len;i++) {
  if (cs.num(i).str<=1.0) {
    cs.remnum(i);
    i--;
  } else
    printf("Cluster %s strength %f\n",cs.num(i).pos.toString(),cs.num(i).str);
}

// Classify image
printf("Classifying image...\n");
RGBmp rgbmp=RGBmp(width,height);
for (int x=0;x<width;x++)
for (int y=0;y<height;y++) {
  Vnd *v=&map->pos[x][y];
  // Find closest cluster
  int closest=1;
  float dist=Vnd::dist(&cs.p2num(1)->pos,v);
  for (int j=2;j<=cs.len;j++) {
    float ldist=Vnd::dist(&cs.p2num(j)->pos,v);
    if (ldist<dist) {
      closest=j;
      dist=ldist;
    }
  }
  rgbmp.bmp[x][y]=myRGB::hue((float)closest/(float)cs.len)/4.0/Vnd::dist(v,&cs.p2num(closest)->pos);
}
rgbmp.writefile("clusters.bmp");

// exit(0);

if (findchange) {

// Edge detection doesn't clean up Vnd's

printf("Finding difference in image...");
Map2d<float> *change,*angs;
map->edgedetectionwithdestroygetgs(Map2d<float>::sobel(),&change,&angs,Vnd(maps.len));
printf("done./n");

for (int i=1;i<=maps.len;i++)
ms.num(i)->display();

// map->getgreyscale()->display();

change->display();

change->writefile("vechange.bmp");

}

/*for (int x=0;x<width;x++)
for (int y=0;y<height;y++)
//printf("%f ",tofloat(map->pos[x][y]));
Map2d<float> *f=Map2d<float>::sobel();
Vnd outside=Vnd(maps.len);
Map2d<Vnd> *n=new Map2d<Vnd>(width,height,outside);
for (int i=0;i<n->width-f->width+1;i++)
for (int j=0;j<n->height-f->height+1;j++) {
Vnd o=outside;
for (int p=0;p<f->width;p++)
for (int q=0;q<f->height;q++) {
printf("%f ",tofloat(map->pos[i+p][j+q]));
o=o+map->pos[i+p][j+q]*f->pos[p][q];
}
n->pos[f->width/2+i][f->height/2+j]=o;
}
n->display();*/

}
