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
  virtual int valat(int x,int y) {
    return intrnd(0,255);
  }
  virtual void evolve() {
  }
};

class Radial : public Effect2d {
public:
  V2d pos,vel;
  float rad,radvel;
  Radial() {
    pos=V2d(floatrnd(0,scrwid),floatrnd(0,scrhei));
    vel=V2d::o;
    rad=floatrnd(10,20);
    radvel=0;
  }
  virtual int valat(int x,int y) {
    float d=V2d::dist(V2d(x,y),pos);
    int c=d/rad;
    if (even(c))
      return 255;
    else
    return 0;
  }
  virtual void evolve() {
    pos=pos+vel;
    vel=hang(vel,V2d::o,0.99,0.2);
  }
};

class Stripey : public Radial {
public:
  float ang,angvel;
  Stripey() {
//    Radial::Radial();
    pos=V2d(floatrnd(0,scrwid),floatrnd(0,scrhei));
    vel=V2d::o;
    rad=floatrnd(10,20);
    radvel=0;
    ang=myrnd(2*pi);
    angvel=0;
  }
  int valat(int x,int y) {
    V2d tofar=V2d(x,y)-pos;
    float d=V2d::normdot(V2d::angle(ang),tofar);
    int c=d/rad;
    if (even(c))
      return 255;
    else
    return 0;
  }
  void evolve() {
//    Radial::evolve();
    pos=pos+vel;
    vel=hang(vel,V2d::o,0.99,0.2);
    ang+=angvel;
    hang(&angvel,0,0.99,0.1);
  }
};

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  PPsetup(scrwid,scrhei,2.0);
  
  JBmp img=JBmp(scrwid,scrhei);
  
  List<Effect2d *> es;
  randomise();
  for (int i=1;i<=3;i++) {
    int j=intrnd(1,2);
    if (j==1)
      es.add(new Radial());
    if (j==2)
      es.add(new Stripey());
  }
  
  for (int i=1;i<=es.len;i++)
    for (int j=1;j<=200;j++)
      es.num(i)->evolve();
  
  while (!keypressed()) {
    
    // Show new image
    img.clear();
    for (int i=0;i<scrwid;i++)
    for (int j=0;j<scrhei;j++)
    for (int k=1;k<=es.len;k++) {
      Effect2d *e=es.num(k);
      img.bmp[j][i]=( exor(img.bmp[j][i],e->valat(i,j)) );
    }
    img.writetoscreen();
    
    // Evolve drivers
    for (int i=1;i<=es.len;i++)
      for (int j=1;j<=5;j++)
        es.num(i)->evolve();
    
  }
  
}
