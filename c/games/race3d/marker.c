#include <joeylib.c>

#define scrwid 320
#define scrhei 200

int gridsize=6;
int gjump=2;

float angvel=0.2;
float forcevel=0.015;

int ps=1;
float rad=0;

float markerrange=8.0;
float wellrange=10.0;

JBmp left=JBmp(scrwid,scrhei);
JBmp right=JBmp(scrwid,scrhei);
JBmp b=JBmp(scrwid,scrhei);

void plotline(V3d a,V3d b) {
  PPline(&left,PPlefteye,a,b,7);
  PPline(&right,PPrighteye,a,b,7*8);
}

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
    
    f=intrnd(1,10);
    line=V3d::random();
    o=myrnd()*2.0*pi;
    a=myrnd()*7.0;
    
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
    float rad=0.06*p;
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
  V3d pos=getpos(thru);
  int marker=1;
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
    float maxdist=100000;
    int maxp;
    for (int i=1;i<=particles.len;i++) {
      V3d p=particles.num(i);
      V3d cen=p-pos+0.0*ori.y;
      cen=V3d::disorientate(cen,ori);
      int c=7.0-chop(6.0*(cen.z+gridsize)/(float)(gridsize*2),0,6);
      // int c=7.0-chop(6.0*(i/particles.len),0,6);
      plotsphere(cen,c);
      
      if (i==marker) { // || i==1+mymod(marker+7,particles.len)) {
        int x,y;
        float u=PPgetunitnoadd(cen);
        if (u) {
          float rad=0.08*u;
          PPgetscrposnoadd(cen,(PPlefteye+PPrighteye)/2.0,&x,&y);
          left.opencircle(x,y,rad,7);
          right.opencircle(x,y,rad,7*8);
        }
      }
    }
    plotline(V3d(-0.1,0,0),V3d(0,0,0.5));
    plotline(V3d(0.1,0,0),V3d(0,0,0.5));
    plotline(V3d(0,-0.1,0),V3d(0,0,0.5));
    plotline(V3d(0,0.1,0),V3d(0,0,0.5));
    
    for (int i=0;i<scrwid;i++) {
      for (int j=0;j<scrhei;j++) {
        b.bmp[j][i]=(left.bmp[j][i] | right.bmp[j][i]);
      }
    }
    // Simple crosshair
    // b.hline(scrwid/2-10,scrhei/2,scrwid/2+10,255);
    // b.vline(scrwid/2,scrhei/2-10,scrhei/2+10,255);
    b.writetoscreen();
    // acc=hang(acc,V3d::origin,0.95,0.01);
    
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


  if (V3d::dist(pos,particles.num(marker))>wellrange) {
    int pullmarker=mymod(marker,particles.len);

    V3d pulldir=particles.num(pullmarker)-pos;
    
    vel=vel+pulldir*0.02;
    // pos=hang(pos,particles.num(pullmarker),0.9,0)+vel;
    
    }

    V3d at=particles.num(mymod2(marker+10,1,particles.len+1))-pos;
    V3d newz=ori.z()*.99+.01*at;
    ori.forcez(newz);

    vel=hang(vel,V3d::o,0.98,0);
    pos=pos+vel;

    droll=hang(droll,0,0.9,0);
    dyaw=hang(dyaw,0,0.9,0);
    dpitch=hang(dpitch,0,0.9,0);
    
    ori.roll(droll/5.0);
    ori.yaw(dyaw/5.0);
    ori.pitch(dpitch/5.0);
    
    if (V3d::dist(pos,particles.num(marker))<markerrange) {
      marker=1+mymod(marker,particles.len);
    }
    
    framedone();
  } while (!key[KEY_SPACE] && !key[KEY_ESC]);
  savetimer();
  allegro_exit();
  displayframespersecond();
}
