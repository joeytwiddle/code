/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


// 4d julia sliced through d-axis
// Boundary tracing using recursion
// Crashes after depth ~6500

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
float wid=2.0;
float hei=2.0;
float dep=2.0;
float left=cenx-wid;
float right=cenx+wid;
float top=ceny-hei;
float bottom=ceny+hei;
float front=cenz-dep;
float back=cenz+dep;
float dslice;
float crazy=0.1234567;
int spacequant=100;
//int spacequant=20;
int jump=1;
int depth=0;

Map3d<int> *map=new Map3d<int>(spacequant,spacequant,spacequant,&gimmezeroint);
Quaternion current=*new Quaternion(floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5));

int julquat(Quaternion z) {
  // - Returns 0 if in set, otherwise num iterations
  register int k=0;
  register bool over=false;
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
  //printf(Sconcat("Q ",Sconcat(maptoqspace(x,y,z).toString()," -> %i.\n")),map->pos[x][y][z]);
  return map->pos[x][y][z];
}

bool isboundary(int x,int y,int z) {
  // - Assumed in set.  Boundary if any neighbours are outside set
  if (stateat(x,y,z)==insidesetmbb) {
    if (stateat(x-1,y,z)==outsideset ||
        stateat(x+1,y,z)==outsideset ||
        stateat(x,y-1,z)==outsideset ||
        stateat(x,y+1,z)==outsideset ||
        stateat(x,y,z-1)==outsideset ||
        stateat(x,y,z+1)==outsideset) {
      map->pos[x][y][z]=boundary;
      return true;
    }
    map->pos[x][y][z]=insideset;
  }
  return (stateat(x,y,z)==boundary);
}

void search(int x,int y,int z) {
//  putpixel(screen,x,y,255);
  printf("%i,",depth++);
  putpixel(screen,x,y,255);
  if (key[KEY_SPACE])
    exit(0);
  for (int i=-1;i<=1;i++)
  for (int j=-1;j<=1;j++)
  for (int k=-1;k<=1;k++)
  if (i!=0 || j!=0 || k!=0) {
    int dx=x+i;
    int dy=y+j;
    int dz=z+k;
    if (dx>=1 && dx<spacequant-1 && dy>=1 && dy<spacequant-1 && dz>=1 && dz<spacequant-1)
      if (map->pos[dx][dy][dz]==unknown || map->pos[dx][dy][dz]==insidesetmbb)
        if (isboundary(dx,dy,dz))
          search(dx,dy,dz);
  }
  printf("%i:",depth--);
  putpixel(screen,x,y,255.0*(1-(float)z/(float)spacequant));
}

void main() {
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  _farsetsel(screen->seg);
  // To get started searching boundary, must first find a boundary point
  randomise();
  dslice=floatrnd(-.3,.3);
  int x=spacequant/2;
  int y=spacequant/2;
  int z=0;
  int r;
  do {
    z++;
    putpixel(screen,z,0,128+64);
    //printf("%i,%i,%i : ",x,y,z);
//    r=stateat(x,y,z);
  } while (!isboundary(x,y,z) && z<spacequant);
  // We've found a boundary point, start searching
  search(x,y,z);
//  while (!key[KEY_SPACE]) { }
}
