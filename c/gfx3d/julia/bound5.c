/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


// 4d julia sliced through d-axis
// Boundary tracing using list

// z=z^2+c

typedef struct {
  int x,y,z;
} Pixel3d;

Pixel3d newPixel3d(int a,int b,int c) {
  Pixel3d *n=new Pixel3d;
  n->x=a;
  n->y=b;
  n->z=c;
  return *n;
}

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
float wid=1.5;
float hei=1.5;
float dep=1.5;
float left=cenx-wid;
float right=cenx+wid;
float top=ceny-hei;
float bottom=ceny+hei;
float front=cenz-dep;
float back=cenz+dep;
float dslice;
float crazy=0.1234567;
int spacequant=100;
int jump=1;
int depth=0;

Map3d<int> *map=new Map3d<int>(spacequant,spacequant,spacequant,&gimmezeroint);
Quaternion current;
Map2d<float> *rend;

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

void render(int rx,int ry) {
              int col;
              if (rend->pos[rx+1][ry]==back || rend->pos[rx][ry+1]==back)
                col=0;
              else {
                float diff=2.0*rend->pos[rx][ry]-rend->pos[rx-1][ry]-rend->pos[rx][ry-1];
                col=chop(128.0+diff*1600.0,0,255);
              }
              putpixel(screen,indent+rx,ry,col);
}

void main() {
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  _farsetsel(screen->seg);
  // To get started searching boundary, must first find a boundary point
  randomise();
  dslice=floatrnd(-.3,.3);
//  current=*new Quaternion(floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5));
  current=*new Quaternion(-0.488193901,0.572800587,0,0);

  rend=new Map2d<float>(spacequant,spacequant,back);

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
  List<Pixel3d> l=List<Pixel3d>(6000);
  l+newPixel3d(x,y,z);
  while (l.len>0) {
    Pixel3d p=l.num(1);
    l.removenumkeeporder(1);
    int x=p.x;
    int y=p.y;
    int z=p.z;
    //printf("%i,",depth++);
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
          if (isboundary(dx,dy,dz)) {
            l+newPixel3d(dx,dy,dz);
            int rx=rend->width*(0.5+((float)dx/(float)spacequant-0.5)/(1.0+(float)dz/spacequant));
            int ry=rend->height*(0.5+((float)dy/(float)spacequant-0.5)/(1.0+(float)dz/spacequant));
            float rz=front+(float)z*2.0*dep/(float)spacequant;
            if (rz<rend->pos[rx][ry]) {
              rend->pos[rx][ry]=rz;
              render(rx,ry);
              render(rx-1,ry);
              render(rx,ry-1);
            }
          }
    }
    //printf("%i:",depth--);
    putpixel(screen,x,y,255.0*(1-(float)z/(float)spacequant));
  }
  while (!key[KEY_SPACE]) { }
  allegro_exit();
}
