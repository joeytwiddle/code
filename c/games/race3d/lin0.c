#define SCRWID 320
#define SCRHEI 200
#define SCRBPS 24
#define desiredFramesPerSecond 40
// #define KEEP_TO_FPS

// #define ALLEGRO
#define LEAVE_EVENTS_ALONE
#include "../../gfx/sdl/stripped-useful.c"
#undef new
#undef mysgn
#undef sgn
#undef square
#include <joeylib.h>

#define REDEFINE
#include "define.c"

#include "keys.c"

#define scrwid SCRWID
#define scrhei SCRHEI

float brightness=0;
float change=1;
float fogdepth=20.0;
//float fogdepth=4.0;
bool planar=false;
int numps=300;
float partrad=0.007;
int tunnelps=40;
float tunnelrad=0.4;

float turnability=0.10;
float forcevel=0.03;
float markerrange=3.0;
int taillen=1200;

int numWaves=15;
int waveAmp=10.0;

V3d pos;
Ori ori;
Octree<V3d> octree=Octree<V3d>(V3d(0,0,0),50);
//&Octree<V3d>::posforv3d);

JBmp left=JBmp(scrwid,scrhei);
JBmp right=JBmp(scrwid,scrhei);
JBmp b=JBmp(scrwid,scrhei);

class Wave {
public:
  int f;
  V3d line;
  float o,a;
  Wave() {
    f=intrnd(1,7);
    line=V3d::random();
    if (planar)
      line.y=0;
    o=myrnd()*2.0*pi;
    a=myrnd()*waveAmp;
  }
  V3d contribution(float thru) {
    return a*line*mysin(((float)f)*(thru*2.0*pi-o));
    // return V3d::random();
  }
  void display() {
    printf("%i %f %f\n",f,o,a);
  }
};

List<Wave> waves=List<Wave>();

void plotsphere(V3d cen,int c) {
  int x,y;
  float p=PPgetunitnoadd(cen);
  if (p) {
    float rad=partrad*p;
    PPgetscrposnoadd(&cen,&PPlefteye,&x,&y);
    left.filledcircle(x,y,rad,16*c); // Fundamental change!
    PPgetscrposnoadd(&cen,&PPrighteye,&x,&y);
    right.filledcircle(x,y,rad,16*c);
    // b.filledcircle(x,y,,c);
    // plotpart2(part[i],-eyewid,sgn(c)*b))
    // plotpart2(part[i],eyewid,sgn(c)*(128+b)))
  }
}

void plotline(V3d a,V3d b,float i) {
  PPlinenoadd(&left,PPlefteye,a,b,15*i*16);
  PPlinenoadd(&right,PPrighteye,a,b,(int)(15*i)*16);
}

V3d getpos(float thru) {
  V3d v=V3d::o;
  for (int i=1;i<=waves.len;i++) {
    Wave w=waves.num(i);
    v=v+w.contribution(thru);
  }
  return v;
}

V3d prepare(V3d v) {
  return (v-pos).disorientate(ori);
}

void line(V3d a,V3d b) {
  plotline(prepare(a),prepare(b),1);
}

void plotscene() {
  left.clear(0);
  right.clear(0);
  ori.quickorisetup();
  Frustrum f=Frustrum(pos,ori,0.01,fogdepth);
  // printf("pos = %s\nori = %s\n",pos.toString(),ori.toString());
//  List<V3d> particles=*octree.getsimplelist();
//  List<V3d> particles=*octree.getclippedlist(f);
  OrderedList<V3d> particles=*octree.getorderedlist(f);
  //line(f.left.pos,f.left.pos+f.left.nor/5.0);
//  line(f.left.pos,f.top.pos);
//  line(f.right.pos,f.top.pos);
//  line(f.bottom.pos,f.right.pos);
//  line(f.left.pos,f.bottom.pos);
  // printf("%i %i\n",octree.totalobjs,particles.len);
  for (int i=particles.len;i>=1;i--) {
    int x,y;
    V3d p=particles.num(i);
    V3d cen=prepare(p);
    int c=15.0-chop(14.0*cen.z/fogdepth,0,14);
    // int c=7.0-chop(6.0*(i/particles.len),0,6);
    plotsphere(cen,c);
  }
}    

void writescreen() {
  // Or screens
  for (int i=0;i<scrwid;i++) {
    for (int j=0;j<scrhei;j++) {
      // b.bmp[j][i]=(left.bmp[j][i] | right.bmp[j][i]);
		// screen_setPixel(i,j,b.bmp[j][i]);
		// screen_setPixel(i,j,intrnd(0,255));
		// if (left.bmp[j][i]!=0)
			// printf("%i\n",left.bmp[j][i]);
		// screen_setPixel(i,j,screen_MapRGB(left.bmp[j][i],right.bmp[j][i],intrnd(0,255)));
		screen_setPixel(i,j,screen_MapRGB(left.bmp[j][i],right.bmp[j][i],left.bmp[j][i]));
    }
  }
  // b.writetoscreen();
	SDL_UpdateRect(screen,0,0,SCRWID,SCRHEI);
}

void init() {
	printf("WARNING: There are awful memory leaks so DON'T PLAY FOR TOO LONG!\n");
  setuptriglookup();

#ifdef REDEFINE
	fp=fopen("incl.c","wa");
	if (fp==NULL)
		printf("Failed to open file incl.c\n");
	printSurfaceDetails(fp,"screen",screen);
	fclose(fp);
#endif

  // allegrosetup(scrwid,scrhei);
  // makepalette(&greypalette);
  // mypalette(255,0,0,0);
  // mypalette(0,0,0,0);
  // mypalette(255,1,1,1);
  // mypalette(128,0,0,0);
  for (int r=0;r<16;r++) {
    for (int g=0;g<16;g++) {
      int palc=r+g*16;
      // mypalette(palc,(r==0?0:brightness+0.2+change*0.5*r/15.0),(g==0?0:brightness+change*0.1+0.2*g/15.0),(g==0?0:brightness+change*0.2+0.6*g/15.0));
      // Groovy colours mypalette(palc,(r==0?0:0.1+0.6*r/15.0),(g==0?0:0.1+0.2*g/15.0),(g==0?0:0.1+0.7*g/15.0));
      // Good colours: mypalette(palc,(r==0?0:0.3+0.4*r/15.0),(g==0?0:0.1+0.2*g/15.0),(g==0?0:0.3+0.5*g/15.0));
    }
  }

  // Set up track
  randomise();
  for (int i=0;i<=numWaves;i++) {
    waves+Wave();
    // waves.num(i).display();
  }

  for (float thru=0;thru<1.0;) {
    for (int i=0;i<tunnelps;i++) {
    V3d here=getpos(thru);
    V3d forward=getpos(thru+0.00001)-here;
    V3d up=V3d::normcross(V3d::crazy,forward);
    V3d right=V3d::normcross(forward,up);
      float t=2*pi*(float)i/(float)tunnelps;
      float s=sin(t);
      float c=cos(t);
      V3d v=here+tunnelrad*(s*up+c*right);
      octree.add(v);
		thru+=1.0/(float)numps/(float)tunnelps;
    }
  }

  for (int i=1;i<2000;i++) {
    octree.add(waveAmp*(float)numWaves/10.0*V3d(floatrnd(-1,1),floatrnd(-1,1),floatrnd(-1,1)));
  }

  ori=Ori(V3d(1,0,0),V3d(0,1,0));
  pos=getpos(0);
    float pd=2.5;
    PPsetup(scrwid,scrhei,pd);

  SDL_Event event;
  // Display track
  float t=0;
  do {
    t=t+0.03;
    V3d from=V3d::rotate(6.0*V3d::k,V3d::j,t);
    ori.forcez(from.neg());
    pos=from;
    plotscene();
    writescreen();
       while(SDL_PollEvent(&event)){  /* Loop until there are no events left on the queue */
         switch(event.type){  /* Process the appropiate event type */
           case SDL_KEYDOWN:  /* Handle a KEYDOWN event */
				  t=-1;
				 }
		 }
  } while (t>=0);

  printf("Enough of that, let's play!\n");

  // Race
  starttimer();
}

  V3d vel=V3d(0,0,0);
  V3d acc=V3d(0,0,0);
  float droll=0;
  float dyaw=0;
  float dpitch=0;
  int frame=0;

  float thru=0;
  float marker=0;
  V3d *tail=new V3d[taillen];
  int tailpos=0;

  void doframe() {
    thru+=0.001;
    // V3d last=V3d(pos.x,pos.y,pos.z);
    // V3d pos=getpos(thru);
    // V3d next=getpos(thru+0.00001);
    // V3d newz=next-pos;
    // ori.forcez(newz);
    frame++;
    // float pd=1.6+1.3*sin(2*pi*frame/1000.0);
    
    plotscene();
    
    // Plot and move marker
    V3d m;
    for (int i=1;i<=15;i++) {
      m=V3d::disorientate(getpos(marker)-pos,ori);
      if (m.mod()<markerrange)
        marker+=0.0002;
    }
    
    float u=PPgetunitnoadd(m);
    bool plot=false;
    if (u) {
      int x,y;
      float rad=0.12*u;
      if (PPgetscrposnoadd(m,PPlefteye,&x,&y))
      if (left.inimage(x,y)) {
        plot=true;
        left.opencircle(x,y,rad,15*16);
        left.opencircle(x,y,rad/2,15*16);
      }
      if (PPgetscrposnoadd(m,PPrighteye,&x,&y))
      if (right.inimage(x,y)) {
        plot=true;
        right.opencircle(x,y,rad,15*16);
        right.opencircle(x,y,rad/2,15*16);
      }
    }
    if (!plot) {
      V2d v=scrwid*2*V2d(m.x,m.y).norm();
      if (abs(v.x)>scrwid/2) {
        float change=(float)scrwid/2.0/abs(v.x);
        v=change*v;
      }
      if (abs(v.y)>scrhei/2) {
        float change=(float)scrhei/2.0/abs(v.y);
        v=change*v;
      }
      v=v+V2d(scrwid/2,scrhei/2);
      left.opencircle(v,5,15*16);
      right.opencircle(v,5,15*16);
    }
    
    // Pull player towards marker
    // if (m.mod()>markerrange+.1) {
      V3d pulldir=getpos(marker)-pos;
      // vel=vel+pulldir*0.02;
      // float amount=(V3d::normdot(ori.z(),pulldir)+5.0)/6.0;
      float amount=chop(0.99-(m.mod()-markerrange)/5.0,0,1);
      V3d newz=ori.z()*amount+(1.0-amount)*pulldir;
      ori.forcez(newz);
      // vel=vel*amount;
      // 
      // 
//    }
    
    // Draw and update tail
    V3d last=(tail[tailpos]-pos).disorientate(ori);
    for (int k=1;k<taillen;k++) {
      int j=mymod(tailpos+k,taillen);
      V3d next=(tail[j]-pos).disorientate(ori);
      plotline(last,next,(float)k/(float)taillen);
      last=next;
    }
    
    if ((frame % 2)==0) {
      tail[tailpos]=pos-ori.qz;
      tailpos=mymod(tailpos+1,taillen);
    }
    
  writescreen();
    
    // acc=hang(acc,V3d::origin,0.95,0.01);
    // Movement
    float angvel=turnability;

	 SDL_Event event;
		while (SDL_PollEvent(&event)) {
	 		observeEvent(event);
			// printf("KEY: %i\n",event.key.keysym.sym);
		 			if (event.type == SDL_QUIT) {
						// printf("Received SDL_QUIT event.\n");
						keepLooping=false;
					}
		}

    if (F10pressed) {
		// printf("User pressed F10: exiting.\n");
		 keepLooping=false;
	 }
	if (zpressed || LCTRLpressed) {
		// printf("%s %f\n",ori.z().toString(),forcevel);
      vel=vel+ori.z()*forcevel;
	}
    else
    angvel=turnability*2.0;
    if (UPpressed)
      dpitch=dpitch+angvel;
    if (DOWNpressed)
      dpitch=dpitch-angvel;
    if (LEFTpressed)
      if (xpressed || ALTpressed)
      droll=droll-angvel;
    else
    dyaw=dyaw-angvel;
    if (RIGHTpressed)
      if (xpressed || ALTpressed)
      droll=droll+angvel;
    else
    dyaw=dyaw+angvel;
    vel=hang(vel,V3d::o,0.92,0);
    pos=pos+vel;
    droll=hang(droll,0,0.9,0);
    dyaw=hang(dyaw,0,0.9,0);
    dpitch=hang(dpitch,0,0.9,0);
    ori.roll(droll/5.0);
    ori.yaw(dyaw/5.0);
    ori.pitch(dpitch/5.0);
    
    framedone();
  // } while (true); // !key[KEY_SPACE] && !key[KEY_ESC]);
  // savetimer();
  // // allegro_exit();
  // displayframespersecond();
}
