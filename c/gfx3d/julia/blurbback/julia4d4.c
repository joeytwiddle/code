/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


// 4d julia sliced through d-axis
// Rotated view

#include <joeylib.c>

// z=z^2+c

#define smallest 0.001
#define forget 0.5
#define rotation -pi/5.0
#define elevation pi/16.0

Quaternion current;

int julquat(Quaternion z) {
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

Quaternion searchline(Quaternion front,Quaternion back) {
  Quaternion t=front-back;
  if (t.mod()<smallest)
    return back;
  Quaternion mid=(front+back)/2.0;
  // if (mid.c>forget)
  // return Quaternion(0,0,forget,0);
  if (julquat(mid)==0)
    return searchline(front,mid);
  else
  return searchline(mid,back);
}

void main() {
  int scrwid=320;
  int scrhei=200;
  int indent=(scrwid-scrhei)/2;
  float cenx=0;
  float ceny=0;
  float wid=1.0;
  float hei=1.0;
  float left=cenx-wid;
  float right=cenx+wid;
  float top=ceny-hei;
  float bottom=ceny+hei;
  float front=-1.0;
  float back=1.0;
  float scale=2.0/(2.0*front);
  float crazy=0.1234567;
  int steps=1000;
  int jump=1;
  JBmp *b=new JBmp(scrwid,scrhei);
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  _farsetsel(screen->seg);
  randomise();
  Map2d<float> *map=new Map2d<float>(scrhei,scrhei);
  current=*new Quaternion(floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5));
  do {
    current=current*Quaternion(0.99,0,0,0)+Quaternion(floatrnd(-.05,.05),floatrnd(-.05,.05),floatrnd(-.05,.05),floatrnd(-.05,.05));
    
    float dslice=floatrnd(-.3,.3);
    current=*new Quaternion(floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5),floatrnd(-.5,.5));
  current=*new Quaternion(-0.488193901,0.572800587,0,0);
    
    for (int i=0;i<scrhei;i+=jump) {
      for (int j=0;j<scrhei;j+=jump) {
        // b->setpixelud(indent+i,j,255);
        // b->writetoscreen();
        int col=0;
        V3d tmpnear=V3d(left+2.0*wid*i/(float)scrhei,top+2.0*hei*j/(float)scrhei,front);
        V3d tmpfar=V3d(tmpnear.x*2.0,tmpnear.y*2.0,back);
        tmpnear=V3d::rotate(tmpnear,V3d(0,1,0),rotation);
        tmpfar=V3d::rotate(tmpfar,V3d(0,1,0),rotation);
        tmpnear=V3d::rotate(tmpnear,V3d(1,0,0),elevation);
        tmpfar=V3d::rotate(tmpfar,V3d(1,0,0),elevation);
        Quaternion near=Quaternion(tmpnear.x,tmpnear.y,tmpnear.z,dslice);
        Quaternion far=Quaternion(tmpfar.x,tmpfar.y,tmpfar.z,dslice);
        map->pos[i][j]=crazy;
        Quaternion last=near-(far-near);
        for (int k=0;k<steps;k++) {
          float dk=(float)k/(float)steps;
          Quaternion next=near*(1.0-dk)+far*dk;
          if (julquat(next)==0) {
            Quaternion res=searchline(last,next);
            // col=255.0*(1.0-(res.c-front)/(forget-front));
            map->pos[i][j]=res.c;
            break;
          }
          last=next;
        }
        if (i>=jump && j>=jump) {
          if (map->pos[i-jump][j-jump]==crazy || map->pos[i-jump][j]==crazy || map->pos[i][j-jump]==crazy)
            col=0;
          else {
            float diff=2.0*map->pos[i-jump][j-jump]-map->pos[i-jump][j]-map->pos[i][j-jump];
            col=chop(128.0+diff*1600.0,0,255);
          }
          for (int x=0;x<jump;x++) {
            for (int y=0;y<jump;y++) {
              b->setpixel(indent+i+x,j+y,col);
            }
          }
        }
        if (key[KEY_SPACE])
          exit(0);
      }
      b->writetoscreen();
    }
    PALETTE pal;
    get_palette(pal);
    save_bitmap(getnextfilename("bmp"),screen,pal);
  } while (!key[KEY_SPACE]);
  allegro_exit();
}
