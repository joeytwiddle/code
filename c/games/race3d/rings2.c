#include <joeylib.c>

#define scrwid 320
#define scrhei 200

int gridsize=6;
int gjump=2;

JBmp left=JBmp(scrwid,scrhei);
JBmp right=JBmp(scrwid,scrhei);
JBmp b=JBmp(scrwid,scrhei);

class Wave {
public:
  int f;
  V3d line;
  float o,a;
  Wave(int ff,V3d ll,float oo,float aa) {
    f=ff;
    line=ll;
    o=oo;
    a=aa;

  }
  Wave() {
    int k=intrnd(1,40);
    Wave(k,V3d::random(),myrnd()*2.0*pi,myrnd()*0.1);

    f=intrnd(1,5);
    line=V3d::random();
    o=myrnd()*2.0*pi;
    a=myrnd()*6.0;

  }
  V3d contribution(float thru) {
    return a*line*sin(((float)f)*(thru*2.0*pi-o));
//    return V3d::random();
  }
  void display() {
    printf("%i %f %f\n",f,o,a);
  }
};

void plotsphere(V3d cen,int c) {
  int x,y;
  float p=PPgetunitnoadd(cen);
  if (p) {
  float rad=0.04*p;
  PPgetscrposnoadd(cen,PPlefteye,&x,&y);
    left.filledcircle(x,y,rad,c);
  PPgetscrposnoadd(cen,PPrighteye,&x,&y);
    right.filledcircle(x,y,rad,8*c);
//  b.filledcircle(x,y,,c);
//  plotpart2(part[i],-eyewid,sgn(c)*b))
//  plotpart2(part[i],eyewid,sgn(c)*(128+b)))
  }
}

List<Wave> waves=List<Wave>();

V3d getpos(float thru) {
    V3d v=V3d::o;
    for (int i=1;i<=waves.len;i++) {
      Wave w=waves.num(i);
      v=v+w.contribution(thru);
    }
  return v;
}

void main() {
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  mypalette(255,0,0,0);
  mypalette(0,0,0,0);
  mypalette(255,1,1,1);
  mypalette(128,0,0,0);
  randomise();
  for (int i=0;i<=30;i++) {
    waves+Wave();
    //waves.num(i).display();
  }
  for (int r=0;r<8;r++) {
  for (int g=0;g<8;g++) {
    int palc=r+g*8;
    mypalette(palc,(r==0?0:0.2+0.5*r/7.0),(g==0?0:0.1+0.2*g/7.0),(g==0?0:0.2+0.6*g/7.0));
    // Groovy colours mypalette(palc,(r==0?0:0.1+0.6*r/7.0),(g==0?0:0.1+0.2*g/7.0),(g==0?0:0.1+0.7*g/7.0));
    // Good colours: mypalette(palc,(r==0?0:0.3+0.4*r/7.0),(g==0?0:0.1+0.2*g/7.0),(g==0?0:0.3+0.5*g/7.0));
  }
  }
  V3d vel=V3d(0,0,0);
  V3d acc=V3d(0,0,0);
  Ori ori;
  float droll=0;
  float dyaw=0;
  float roll=0;
  float yaw=0;
  float pitch=0;
  int frame=0;
  List<V3d> particles=List<V3d>(gridsize*gridsize*gridsize);
/*  for (int i=1;i<gridsize*gridsize*gridsize;i++) {
    particles+gridsize*V3d(floatrnd(-1,1),floatrnd(-1,1),floatrnd(-1,1));
  }*/
  for (float thru=0;thru<1.0;thru+=0.002) {
    V3d here=getpos(thru);
    V3d forward=getpos(thru+0.00001)-here;
    V3d up=0.7*V3d::normcross(V3d::crazy,forward);
    V3d right=0.7*V3d::normcross(forward,up);
    int ps=8;
    for (int i=0;i<ps;i++) {
      float t=2*pi*(float)i/(float)ps;
      float s=sin(t);
      float c=cos(t);
      particles+(here+s*up+c*right);
    }
    //v.print();
  }
  starttimer();
  float thru=0;
  V3d pos=getpos(thru);
  do {
    thru+=0.001;
//    V3d last=V3d(pos.x,pos.y,pos.z);
    V3d pos=getpos(thru);
    V3d next=getpos(thru+0.00001);
    V3d newz=next-pos;
    ori.forcez(newz);
    frame++;
//    float pd=1.6+1.3*sin(2*pi*frame/1000.0);
    float pd=1.0;
    PPsetup(scrwid,scrhei,pd);
    left.clear(0);
    right.clear(0);
//    b.fadeby(16);
    int ioff=-mydiv(pos.x,gjump);
    int joff=-mydiv(pos.y,gjump);
    int koff=-mydiv(pos.z,gjump);
    ori.quickorisetup();
    // Need to find largest axis, and do loops with that one outside
    int ks=mysgn(ori.qz.z);
    int js=-mysgn(ori.qz.y);
    int is=-mysgn(ori.qz.x);
    for (int i=1;i<=particles.len;i++) {
          int x,y;
          V3d p=particles.num(i);
          V3d cen=p-pos+0.0*ori.y;
          cen=V3d::disorientate(cen,ori);
          int c=7.0-chop(6.0*(cen.z+gridsize)/(float)(gridsize*2),0,6);
//          int c=7.0-chop(6.0*(i/particles.len),0,6);
          plotsphere(cen,c);
        }
    for (int i=0;i<scrwid;i++) {
    for (int j=0;j<scrhei;j++) {
      b.bmp[j][i]=(left.bmp[j][i] | right.bmp[j][i]);
    }
    }
    b.writetoscreen();
//    acc=hang(acc,V3d::origin,0.95,0.01);
//    vel=hang(vel,(ori.qz/5.0).neg(),0.9,0)+acc;
//    pos=pos+vel;
//    droll=hang(droll,0,0.9,0.01);
//    dyaw=hang(dyaw,0,0.95,0.01);
//    roll=hang(roll,0,0.95,0)+droll;
//    yaw=hang(yaw,0,0.92,0)+dyaw;
//    pitch=hang(pitch,0,0.999,0.01);
//    pos=pos+ori.qz*4;
//    ori.roll(roll/5.0);
//    ori.yaw(yaw/5.0);
//    pos=pos-ori.z()*4;
    framedone();
  } while (!key[KEY_SPACE] && !key[KEY_ESC]);
  savetimer();
  allegro_exit();
  displayframespersecond();
}
