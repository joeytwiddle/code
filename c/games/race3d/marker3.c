#include <joeylib.c>

#define scrwid 320
#define scrhei 200

int gridsize=6;
int gjump=2;

float angvel=0.1;
float forcevel=0.03;

float markerrange=3.0;

JBmp left=JBmp(scrwid,scrhei);
JBmp right=JBmp(scrwid,scrhei);
JBmp b=JBmp(scrwid,scrhei);

class Wave {
public:
  int f;
  V3d line;
  float o,a;
  Wave() {
    f=intrnd(1,10);
    line=V3d::random();
    o=myrnd()*2.0*pi;
    a=myrnd()*3.0;
  }
  V3d contribution(float thru) {
    return a*line*sin(((float)f)*(thru*2.0*pi-o));
    // return V3d::random();
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
    // b.filledcircle(x,y,,c);
    // plotpart2(part[i],-eyewid,sgn(c)*b))
    // plotpart2(part[i],eyewid,sgn(c)*(128+b)))
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
  for (int i=0;i<=10;i++) {
    waves+Wave();
    // waves.num(i).display();
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
  float dpitch=0;
  int frame=0;
  List<V3d> particles=List<V3d>(gridsize*gridsize*gridsize);
  /* for (int i=1;i<gridsize*gridsize*gridsize;i++) {
       particles+gridsize*V3d(floatrnd(-1,1),floatrnd(-1,1),floatrnd(-1,1));
     }*/
  int ps=1;
  float rad=0;
  for (float thru=0;thru<1.0;thru+=0.002) {
    V3d here=getpos(thru);
    V3d forward=getpos(thru+0.00001)-here;
    V3d up=V3d::normcross(V3d::crazy,forward);
    V3d right=V3d::normcross(forward,up);
    for (int i=0;i<ps;i++) {
      float t=2*pi*(float)i/(float)ps;
      float s=sin(t);
      float c=cos(t);
      particles+(here+rad*(s*up+c*right));
    }
    // v.print();
  }
  starttimer();
  float thru=0;
  float marker=0;
  V3d pos=getpos(marker);
  do {
    thru+=0.001;
    // V3d last=V3d(pos.x,pos.y,pos.z);
    // V3d pos=getpos(thru);
    // V3d next=getpos(thru+0.00001);
    // V3d newz=next-pos;
    // ori.forcez(newz);
    frame++;
    // float pd=1.6+1.3*sin(2*pi*frame/1000.0);
    float pd=2.0;
    PPsetup(scrwid,scrhei,pd);
    left.clear(0);
    right.clear(0);
    // b.fadeby(16);
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
      // int c=7.0-chop(6.0*(i/particles.len),0,6);
      plotsphere(cen,c);
    }

    // Plot and move marker
    V3d m;
    for (int i=1;i<=10;i++) {
      m=V3d::disorientate(getpos(marker)-pos,ori);
      if (m.mod()<markerrange)
      marker+=0.0002;
    }

//    if (m.mod()>markerrange+.1) {
    V3d pulldir=getpos(marker)-pos;
//    vel=vel+pulldir*0.02;
//    float amount=(V3d::normdot(ori.z(),pulldir)+5.0)/6.0;
    float amount=chop(0.99-(m.mod()-markerrange)/5.0,0,1);
    V3d newz=ori.z()*amount+(1.0-amount)*pulldir;
    ori.forcez(newz);
//    vel=vel*amount;
//    }
        int x,y;
        float u=PPgetunitnoadd(m);
        if (u) {
          float rad=0.08*u;
          PPgetscrposnoadd(m,PPlefteye,&x,&y);
          left.opencircle(x,y,rad,7);
          PPgetscrposnoadd(m,PPrighteye,&x,&y);
          right.opencircle(x,y,rad,7*8);
//          PPgetscrposnoadd(m,(PPlefteye+PPrighteye)/2.0,&x,&y);
//          left.opencircle(x,y,rad,7);
//          right.opencircle(x,y,rad,7*8);
        }

    // Or screens
    for (int i=0;i<scrwid;i++) {
      for (int j=0;j<scrhei;j++) {
        b.bmp[j][i]=(left.bmp[j][i] | right.bmp[j][i]);
      }
    }
    b.writetoscreen();
    // acc=hang(acc,V3d::origin,0.95,0.01);

    // Movement
    if (key[KEY_UP])
      dpitch=dpitch+angvel;
    if (key[KEY_DOWN])
      dpitch=dpitch-angvel;
    if (key[KEY_LEFT])
      if (key[KEY_ALT])
      droll=droll-angvel;
    else
    dyaw=dyaw-angvel;
    if (key[KEY_RIGHT])
      if (key[KEY_ALT])
      droll=droll+angvel;
    else
    dyaw=dyaw+angvel;
    if (key[KEY_LCONTROL])
      vel=vel+ori.z()*forcevel;
    vel=hang(vel,V3d::o,0.9,0);
    pos=pos+vel;
    droll=hang(droll,0,0.9,0);
    dyaw=hang(dyaw,0,0.9,0);
    dpitch=hang(dpitch,0,0.9,0);
    ori.roll(droll/5.0);
    ori.yaw(dyaw/5.0);
    ori.pitch(dpitch/5.0);
    
    framedone();
  } while (!key[KEY_SPACE] && !key[KEY_ESC]);
  savetimer();
  allegro_exit();
  displayframespersecond();
}
