/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


// 4d julia sliced through d-axis
// Boundary tracing

// z=z^2+c

#include <joeylib.c>

#define unknown 0
#define insideset 1
#define insidesetmbb 2 // (may be boundary - unchecked)
#define boundary 3
#define outsideset 4


int scrwid=320;
int scrhei=200;
int indent=(scrwid-scrhei)/2;
float cenx=0;
float ceny=0;
float cenz=0;
float wid=1.0;
float hei=1.0;
float dep=1.0;
float left=cenx-wid;
float right=cenx+wid;
float top=ceny-hei;
float bottom=ceny+hei;
float front=cenz-dep;
float back=cenz+dep;
float dslice=floatrnd(-.3,.3);
float crazy=0.1234567;
int spacequant=250;
int jump=1;

Map3d<int> *map=new Map3d<int>(spacequant,spacequant,spacequant,&gimmezeroint);
Quaternion current=*new Quaternion(floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5));

int julquat(Quaternion z) {
  // - Returns 0 if in set, otherwise num iterations
  int k=0;
  bool over=false;
  do {
    k++;
    z=z*z+current;
    if (k>255)
      over=true;
  } while (!over && !(z.mod()>2.0));
  if (over)
    k=0;
  return k;
}

Quaternion maptoqspace(int x,int y,int z) {
  return Quaternion(left+(float)x*2.0*wid/(float)spacequant,top+(float)y*2.0*hei/(float)spacequant,front+(float)z*2.0*dep/(float)spacequant,dslice);
}

int stateat(int x,int y,int z) {
  if (map->pos[x][y][z]==unknown) {
    int res=julquat(maptoqspace(x,y,z));
    if (res>0)
      map->pos[x][y][z]=outsideset;
    else
    map->pos[x][y][z]=insidesetmbb;
  }
  printf(Sconcat("Quat ",Sconcat(maptoqspace(x,y,z).toString()," gets %i.\n")),map->pos[x][y][z]);
  return map->pos[x][y][z];
}

bool isboundary(int x,int y,int z) {
  // - Assumed in set.  Boundary if any neighbours are outside set
  if (stateat(x,y,z)==insidesetmbb) {
    for (int i=-1;i<=1;i++)
    for (int j=-1;j<=1;j++)
    for (int k=-1;k<=1;k++)
    if (i!=0 || j!=0 || k!=0)
      if (stateat(x+i,y+j,z+k)==outsideset) {
      map->pos[x][y][z]=boundary;
      putpixel(screen,x,y,z);
      return true;
    }
    map->pos[x][y][z]=insideset;
  }
  return (stateat(x,y,z)==boundary);
}

void search(int x,int y,int z) {
//  putpixel(screen,x,y,255);
//  if (key[KEY_ESC])
//    exit(0);
  for (int i=-1;i<=1;i++)
  for (int j=-1;j<=1;j++)
  for (int k=-1;k<=1;k++)
  if (i!=0 || j!=0 || k!=0)
    if (isboundary(x+i,y+j,z+k))
    search(x+i,y+j,z+k);
}

void main() {
//  allegrosetup(scrwid,scrhei);
//  makepalette(&greypalette);
//  _farsetsel(screen->seg);
  // To get started searching boundary, must first find a boundary point
  int x=spacequant/2;
  int y=spacequant/2;
  int z=-1;
  int r;
  do {
    z++;
//    putpixel(screen,z,0,128+64);
    r=stateat(x,y,z);
  } while (r!=boundary && z<spacequant);
  // We've found a boundary point, start searching
  search(x,y,z);
}
