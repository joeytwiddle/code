/* Swirly (repulsors with wibbly)
   repulsors plotted in same window as particles
*/

#include <stdlib.h>
#include "../BWlib/BWlib.h"
#include <math.h>
#include <sys/types.h>
#include <time.h>

time_t *tloc;
const float pi=3.1415926535897932385;
const numps=30; /* Don't forget to change down below too! */
const numrs=50; /* Don't forget to change down below too! */
const SCR_WIDTH=256;
const SCR_HEIGHT=256;
const left=-1;
const right=1;
const top=1;
const bottom=-1;
const int speed=1;
const float strength=0.5;
const gridsize=50;
const float arrowsize=1.0;
const float browniansize=0.00;
const traillength=1;
const float dampening=0.7;
const float holderstrength=0.03;
const float walkrdamp=0.99;
const float walktdamp=0.99;
const float walkdrdamp=0.95;
const float walkdtdamp=0.99;
const float walkdrrnd=0.000005;
const float walkdtrnd=0.002;
const float wibblystrength=6.0;

typedef struct {
  float x,y;
} vector;

typedef struct {
  vector pos;
  vector vel;
  float walkr;
  float walkt;
  float walkdr;
  float walkdt;
} part;

typedef struct {
  vector pos;
  float str;
  float dstr;
  float off;
  float fre;
} rep;

part particle[1000];
rep repulsor[100];
BasicWin window1,window2;
int frame;

vector p,q;


int mymod(int a,int b)
{
  while (a<0 || a>=b) {
    if (a<0)
      a=a+b;
    if (a>=b)
      a=a-b;
  }
  return a;
}


vector V(float x,float y)
{
  vector p;
  
  p.x=x;
  p.y=y;
  return p;
}


void nl()
{
  fprintf(stderr,"\n");
}


void msg(char *c)
{
  fprintf(stderr,c); nl;
}


float square(float x)
{
  return x*x;
}


float radius(vector v)
{
  return sqrt(v.x*v.x+v.y*v.y);
}


void randpart(part* p)
{
  p->pos=V(drand48()*2.0-1.0,drand48()*2.0-1.0);
  p->vel=V(0.0,0.0);
  p->walkr=0.0;
  p->walkt=2.0*pi*drand48();
  p->walkdr=0.0;
  p->walkdt=0.0;
}


void randrepulsor(rep* r)
{
  r->pos.x=drand48()*2-1;
  r->pos.y=drand48()*2-1;
  r->str=square(square(square(drand48())))*0.007;
  r->dstr=r->str;
  r->off=drand48()*2*pi;
  r->fre=drand48()/30;
  r->fre=drand48()/100/sqrt(r->str*100);

  r->str=0.0003;
  r->dstr=r->str;
  r->fre=drand48()/30;
}
  

void Vprint(vector v)
{
  fprintf(stderr,"(%f",v.x);
  fprintf(stderr,",%f)",v.y);
}


void printpart(part* p)
{
  Vprint(p->pos);
}


vector Vmult(float a,vector v)
{
  vector w;
  
  w.x=a*v.x;
  w.y=a*v.y;
  return w;
}


vector Vadd(vector u, vector v)
{
  vector w;
  
  w.x=u.x+v.x;
  w.y=u.y+v.y;
  return w;
}


vector Vsub(vector u, vector v)
{
  return Vadd(u,Vmult(-1,v));
}


vector Vneg(vector v)
{
  return Vmult(-1,v);
}


vector Vrotate(vector u,double a)
{
  vector v;
  
  v.x=u.x*cos(a)-u.y*sin(a);
  v.y=+u.x*sin(a)+u.y*cos(a);
  return v;
}


vector Cart(float r, float t)
{
  return V(r*cos(t),r*sin(t));
}


vector Vnorm(vector v)
{
  return Vmult(1/radius(v),v);
}


int Vequal(vector u,vector v)
{
  if (radius(Vsub(u,v))<0.001)
    return 1;
  else
    return 0;
}


/*
int Vequal(vector u,vector v)
{
  if ((u.x==v.x) && (u.y==v.y))
    return 1;
  else
    return 0;
}
*/

vector pointonline(vector o,vector d,float s)
{
  return Vadd(o,Vmult(s*frame,d));
}


/*vector massivebody(vector v)
{
  float tmp;
  
  tmp=radius(v);
  if (tmp>0.1 && tmp<100)
    return Vmult(strength/square(tmp),Vnorm(V(2*v.x*v.y,square(v.y)-square(v.x))));
  else
    return V(0,0);
}


vector MBmove(vector v, vector m)
{
  return massivebody(Vsub(v,m));
}
*/


vector inoutattractor(vector v,vector a,float strength,int period)
{
  if (Vequal(v,a))
    return V(0,0);
  else
    return Vmult(strength*(1-radius(Vsub(v,a)))*sin(frame*2*pi/period),Vnorm(Vsub(v,a)));
}


/* Massive body
vector forceat(vector v)
{
  vector vminusp,vminusq;
  
  if (Vequal(v,V(0,0)))
    return V(0,0);
  else
    return Vmult(0.001,Vadd(Vrotate(Vnorm(v),pi/2+pi/32+0*sin(frame*2*pi/10)),
           Vadd(inoutattractor(v,p,3,85),inoutattractor(v,q,2,381))));
}
*/


vector wibblyrepulsor(vector v,vector r,float str,float dstr,float off,float fre)
{
  if (Vequal(v,r))
    return V(0,0);
  else {
    return Vmult((str+dstr*sin(off+frame*fre))/radius(Vsub(v,r)),Vnorm(Vsub(v,r)));
  }
}


vector holder(vector v, float strength)
{
  return Vmult(strength*square(radius(v)),Vnorm(Vneg(v)));
}


vector forceat(vector v)
{
  int i;
  rep r;
  vector fa;
/*return V(0,0);*/
  fa=holder(v,holderstrength);
  
  for (i=0; i<numrs; i++) {
    r=repulsor[i];
    fa=Vadd(fa,wibblyrepulsor(v,r.pos,r.str,r.dstr,r.off,r.fre));
  }
  return fa;
}


/*
vector forceat(vector v)
{
  return V(0,0);
}
*/


void brownian(part *p)
{
  p->vel.x=p->vel.x+(drand48()*2.0-1.0)*browniansize;
  p->vel.y=p->vel.y+(drand48()*2.0-1.0)*browniansize;
}


int outofbounds(vector v)
{
  if (v.x<left)
    return 1;
  if (v.x>right)
    return 1;
  if (v.y<bottom)
    return 1;
  if (v.y>top)
    return 1;
  return 0;
}


vector wibbly(part *pa)
{
  pa->walkr=pa->walkr*walkrdamp+pa->walkdr;
  pa->walkt=pa->walkt+pa->walkdt;
  pa->walkdr=pa->walkdr*walkdrdamp+(drand48()*2-1)*walkdrrnd;
  pa->walkdt=pa->walkdt*walkdtdamp+(drand48()*2-1)*walkdtrnd;
  return Cart(pa->walkr,pa->walkt);
}


void movepart(part* pa)
{
  vector tmp;
  
  tmp=forceat(pa->pos);
  brownian(pa);
  pa->vel=Vadd(Vmult(dampening,pa->vel),Vmult(strength,Vadd(tmp,Vmult(wibblystrength,wibbly(pa)))));
  pa->pos=Vadd(pa->pos,pa->vel);
  if (outofbounds(pa->pos))
    randpart(pa);
}


int scrx(vector v)
{
  return SCR_WIDTH*(v.x-left)/(right-left);
}


int scry(vector v)
{
  return SCR_HEIGHT*(v.y-top)/(bottom-top);
}


void plotpart(part* p)
{
  BWDrawPoint(window1,scrx(p->pos),scry(p->pos));
}


void plotparts()
{
  int i;
  
  if (mymod(frame,traillength+1)==0)
    BWClearArea(window1, 0, 0, SCR_WIDTH, SCR_HEIGHT);
  for (i=0; i<numps; i++) {
    plotpart(&particle[i]);
  }
}


void moveparts()
{
  int i,j,new;
  
  for (i=0; i<numps; i++) {
  
    BWClearArea(window1,scrx(particle[i].pos),scry(particle[i].pos),1,1);
    movepart(&particle[i]);
    BWDrawPoint(window1,scrx(particle[i].pos),scry(particle[i].pos));
  
/*    new=0;
    for (j=0; j<numps; j++) {
      if (j!=i)
        if (Vequal(particle[i].pos,particle[j].pos))
          new=1;
    }
    if (outofbounds(particle[i].pos))
      new=1;
    if (new==1)
      randpart(&particle[i]);*/
  
  }
}


void plotarrow(BasicWin window,vector u,vector d)
{
  vector h; /* Head of arrow */
  
  if (radius(d)>0.005) {
    h=Vadd(u,d);
    BWDrawLine(window,scrx(u),scry(u),scrx(h),scry(h));
  }
}


void plotfield()
{
  int i,j;
  vector u;
  
  BWClearArea(window2, 0, 0, SCR_WIDTH, SCR_HEIGHT);
  for (i=0; i<=gridsize; i++) {
  for (j=0; j<=gridsize; j++) {
    u=V(-1.0+2.0*i/gridsize,1.0-2.0*j/gridsize);
/*    if (radius(forceat(u))<0.01) for low-force areas */
    plotarrow(window2,u,Vmult(arrowsize/gridsize,forceat(u)));
  }
  }
}


void plotrepulsor(rep r)
{
  float j;
  
  for (j=0.0; j<2.0*pi; j=j+2.0*pi/8) {
    plotarrow(window1,r.pos,Vmult(r.str+r.dstr*sin(r.off+frame*r.fre),Cart(80.0,j)));
  }
}  


void plotrepulsors()
{
  int i;
  
  BWClearArea(window1, 0, 0, SCR_WIDTH, SCR_HEIGHT);
  for (i=0; i<numrs; i++) {
    plotrepulsor(repulsor[i]);
  }
}


/* Isolation is a measure of loneliness of a repulsor */
float isolation(rep r, int c)
{
  int i;
  float rmin;
  
  rmin=2.0;
  for (i=0; i<c; i++) {
    if (radius(Vsub(r.pos,repulsor[i].pos))<rmin)
      rmin=radius(Vsub(r.pos,repulsor[i].pos));
  }
  return rmin;
}


int main(int argc, char **argv)
{
  int i,j,stillhappy;
  Display *display;
  int events;
  rep tmp;
  
  p.x=-0.9; p.y=0;
  q.x=0.9; q.y=0;
  
  srand48(time(tloc));
  frame=0;

  /* Connect to the server for display */
  display = BWConnectToServer();

  /* Open the windows */
  window1 = BWCreateNewWindow(display, argc, argv, 882, 4);
  window2 = BWCreateNewWindow(display, argc, argv, 882, 324);

  for (i=0; i<numps; i++) {
    randpart(&particle[i]);
  }
  for (i=0; i<numrs; i++) {
    randrepulsor(&repulsor[i]);
  }
/* Choose repulsors spread out
  for (i=0; i<numrs; i++) {
    randrepulsor(&repulsor[i]);
    for (j=0; j<500; j++) {
      randrepulsor(&tmp);
      if (isolation(tmp,i)>isolation(repulsor[i],i)) {
        repulsor[i]=tmp;
        plotrepulsor(tmp);
      }
    }
  }*/
	
  while(1) {
    events = BWCheckEvents(window1);
    switch (events) {
      /* Window exposed - redraw */
      case EV_EXPOSED:
        {
        plotparts();
	break;
	}
	/* Mouse button pressed - close both windows and exit*/
      case EV_BUTTON:
	{
	BWCloseWindow(window1);
	exit(0);
	}
      default: {
/*        plotparts();*/
        moveparts();
        if (mymod(frame,32)==0) {
/*          plotfield();*/
          plotrepulsors();
        }
        frame=frame+speed;
        break;
      }
    }
  }


}
