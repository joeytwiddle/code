/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


// 4d Mandelbrot sliced through d-axis
// Rotated view, raytrace method, random coords

/* All my source code is released under the GNU public licence.
   Please feel free to contact me: pclark@cs.bris.ac.uk
   Paul "Joey" Clark Feb 1999 */

// I would be especially interested to know why the program hangs after
// rendering 4 high-resolution frames sequentially!

#include <joeylib.c>

// z=z^2+c

#define smallest 0.01
#define forget 0.5
#define elevation pi/16.0

float rotation;

  int scrwid=320;
  int scrhei=200; // scrhei is width and height of render image
  int indent=(scrwid-scrhei)/2;

  // Where are we looking in quaternion space?
  float cenx=0.25;
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

  int steps=100; // How finely to search space (depth)
  int jump=1; // Skip pixels for preview
  int frames=30;
  int maxiter=30; // Set very low - we are seeing an early shell of the Julia set

  Map2d<float> *rend=new Map2d<float>(scrhei,scrhei); // depth map
  Quaternion current=*new Quaternion(-0.488193901,0.572800587,0,0); // c in eqn. z <- z^2 + c
  JBmp *b=new JBmp(scrwid,scrhei);

// 0 if in set, otherwise num iterations
int mandquat(Quaternion z) {
  register int k=0;
  Quaternion c=z;
  z=z+current; // current is offset
  do {
    k++;
    z=z*z+c;
  } while (!(k>maxiter) && !(z.mod()>2.0));
  if (k>maxiter)
    k=0;
  return k;
}

// Recursive refined search
Quaternion searchline(Quaternion front,Quaternion back) {
  Quaternion t,mid;
  t.setto(front-back);
  if (t.mod()<smallest)
    return back;
  mid.setto((front+back)/2.0);
  // if (mid.c>forget)
  // return Quaternion(0,0,forget,0);
  if (mandquat(mid)==0)
    return searchline(front,mid);
  else
  return searchline(mid,back);
}

// Light effect for pixel rx,ry
void render(int rx,int ry) {
  if (rx>=jump && ry>=jump && rx<rend->width && ry<rend->height) {
    int col;
//    if (rend->pos[rx][ry]==back && rend->pos[rx+jump][ry]==back && rend->pos[rx][ry+jump]==back)
    if (rend->pos[rx][ry]==back)
      col=0;
    else {
      float diff=2.0*rend->pos[rx][ry]-rend->pos[rx-jump][ry]-rend->pos[rx][ry-jump];
      col=chop(128.0-diff*2400.0,0,255);
    }
//    putpixel(screen,indent+rx,ry,col);
          for (int x=0;x<jump;x++) {
            for (int y=0;y<jump;y++) {
              b->setpixel(indent+rx+x,ry+y,col);
            }
          }
  }
}

int main(int argc, char **argv) {

  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  _farsetsel(screen->seg);
  PALETTE pal;
  get_palette(pal);
  randomise();

  // For making frames for an animation
  Quaternion qfrom=*new Quaternion(-0.12,0.7815,0,0);
  Quaternion qto=*new Quaternion(-1.1396,0.2361,0,0);
//  Quaternion qfrom=*new Quaternion(-0.1075,0.8636,0,0);
//  Quaternion qto=*new Quaternion(0.5,0,0,0);
  V3d tmpnear,tmpfar;
  Quaternion near,far,last,next,res;

  for (int frame=0;frame<=frames;frame++) {

    // Render just one frame of sequence?
    if (argc>1) {
      sscanf(argv[1],"%i",&frame);
      numfileswritten=frame-1;
    }
    // Set up frame's parameters
    float thru=(float)frame/(float)frames;
    current=qfrom*(1.0-thru)+qto*thru;
//    float dslice=0.5*(1.0-thru);
    rotation=-pi/3.0+pi/8.0*thru;

    // Random c
    float dslice=0;
    current=Quaternion(0,0,0,0);
    
    for (int i=0;i<scrhei;i+=jump) {
      for (int j=0;j<scrhei;j+=jump) {

        // Shoot ray into image
        Quaternion displace=Quaternion(cenx,ceny,0,0);
        tmpnear=V3d(-wid+2.0*wid*i/(float)scrhei,-hei+2.0*hei*j/(float)scrhei,front);
        tmpfar=V3d(tmpnear.x*2.0,tmpnear.y*2.0,back);
        tmpnear.changeto(V3d::rotate(tmpnear,V3d(0,1,0),rotation));
        tmpfar.changeto(V3d::rotate(tmpfar,V3d(0,1,0),rotation));
        tmpnear.changeto(V3d::rotate(tmpnear,V3d(1,0,0),elevation));
        tmpfar.changeto(V3d::rotate(tmpfar,V3d(1,0,0),elevation));
        near.setto(Quaternion(tmpnear.x,tmpnear.y,tmpnear.z,dslice));
        far.setto(Quaternion(tmpfar.x,tmpfar.y,tmpfar.z,dslice));
        near.setto(near+displace);
        far.setto(far+displace);
        rend->pos[i][j]=back;
        last.setto(near-(far-near));

        for (int k=0;k<steps;k++) {
          float dk=(float)k/(float)steps;
          next.setto(near*(1.0-dk)+far*dk);
          if (mandquat(next)==0) {
            res.setto(searchline(last,next));
            // col=255.0*(1.0-(res.c-front)/(forget-front));
            rend->pos[i][j]=res.c;
            break;
          }
          last=next;
        }

        render(i,j);

        if (key[KEY_SPACE])
          exit(0);

      }
      b->writetoscreen();
    }

    save_bitmap(getnextfilename("bmp"),screen,pal);

    if (argc>1)
      break;

  }

  allegro_exit();

}
