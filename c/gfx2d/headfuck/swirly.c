/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

#include <joeylib.c>

int scrwid=320;
int scrhei=200;

uchar exor(uchar x,uchar y) {
  if (x==y)
    return 0;
  else
  return 255;
}

class Effect2d {
public:
  V2d pos,vel;
  V2d cen,off;
  float rotvel;
  float amp;
  virtual int valat(int x,int y) {
    return intrnd(0,255);
  }
  virtual float smoothvalat(int x,int y) {
    return intrnd(0,255);
  }
  virtual void evolve() {
  }
  virtual void adjust(JBmp *img) {
    for (int i=0;i<scrwid;i++)
    for (int j=0;j<scrhei;j++)
      if (valat(i,j)!=0)
        img->bmp[j][i]=exor(img->bmp[j][i],255);
  }
};

class Radial : public Effect2d {
public:
  float rad,radvel;
  Radial() {
    pos=V2d(floatrnd(0,scrwid),floatrnd(0,scrhei));
    vel=V2d::o;
    rad=floatrnd(10,40);
    radvel=0;
    cen=V2d(scrwid/2,scrhei/2)+10.0*V2d::randomcircle();
    off=10.0*V2d::randomcircle();
    rotvel=floatrnd(0,2*pi/40.0);
  }
  virtual int valat(int x,int y) {
    float d=V2d::dist(V2d(x,y),pos);
    int c=d/rad;
    if (alternate(c))
      return 255;
    else
    return 0;
  }
  virtual float smoothvalat(int x,int y) {
    float d=V2d::dist(V2d(x,y),pos);
//    return gaussian(d/200.0)*mysin(d/rad*2*pi);
    return log(d/200.0)*mysin(d/rad*2*pi);
  }
  virtual void evolve() {
    pos=pos+vel;
    hang(&pos,V2d(scrwid/2,scrhei/2),0.99,0);
    vel=hang(vel,V2d::o,0.99,0.3);
    hang(&radvel,0,0.995,0.2);
    radvel=myabs(radvel);
    rad+=radvel;
    hang(&rad,10,0.97);
  }
};

class Stripey : public Radial {
public:
  float ang,angvel;
  Stripey() {
    Radial::Radial();
    pos=V2d(floatrnd(0,scrwid),floatrnd(0,scrhei));
    vel=V2d::o;
//    rad=floatrnd(20,40);
    radvel=0;
    ang=myrnd(2*pi);
    angvel=0;
  }
  int valat(int x,int y) {
    V2d tofar=V2d(x,y)-pos;
    float d=V2d::dot(V2d::angle(ang),tofar);
    if (alternate(d,rad))
      return 255;
    else
    return 0;
  }
  float smoothvalat(int x,int y) {
    V2d tofar=V2d(x,y)-pos;
    float d=V2d::dot(V2d::angle(ang),tofar);
    return mysin(d/rad*2*pi);
//    return mysin(d/rad*2*pi)/d*100.0;
//    return gaussian(d/100.0)*mysin(d/rad*2*pi)/d*100.0;
  }
  void evolve() {
    Radial::evolve();
//    pos=pos+vel;
//    vel=hang(vel,V2d::o,0.999,0.02);
    ang+=angvel;
    hang(&angvel,0,0.995,0.001);
  }
};

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  bool writingframes=a.argexists("-wf","write frames");
  a.done();
  
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  PPsetup(scrwid,scrhei,2.0);
  
  JBmp img=JBmp(scrwid,scrhei);
  
  List<Effect2d *> es;
  randomise();
  for (int i=1;i<=5;i++) {
//    int j=intrnd(1,2);
    if (odd(i))
      es.add(new Radial());
    else
      es.add(new Stripey());
    //es.num(es.len)->amp=floatrnd(0,1);
    es.num(es.len)->amp=1;
  }
  
  for (int i=1;i<=es.len;i++)
    for (int j=1;j<=200;j++)
      es.num(i)->evolve();

  int frame=0;

  while (!keypressed()) {

    frame++;

    // Make new image
/*    img.clear();
    for (int k=1;k<=es.len;k++) {
      Effect2d *e=es.num(k);
      e->adjust(&img);
    }*/
    for (int i=0;i<scrwid;i++)
    for (int j=0;j<scrhei;j++) {
      float f=0;
      for (int k=1;k<=es.len;k++) {
        Effect2d *e=es.num(k);
        f=f+e->smoothvalat(i,j)*e->amp;
      }
//      img.bmp[j][i]=( f>0 ? 0 : 255 );
//      img.bmp[j][i]=ucharchop(255*mysquare(f)/es.len);
      img.bmp[j][i]=ucharchop(255*f/es.len);
    }

    // Show new image
    img.writetoscreen();
    
    // Evolve drivers
    for (int i=1;i<=es.len;i++)
      for (int j=1;j<=5;j++) {
        Effect2d *e=es.num(i);
        e->evolve();
//        e->pos=e->cen+V2d::rotate(e->off,e->rotvel*(float)frame);
      }

    if (writingframes)
      img.writeframe();
    
  }

}
