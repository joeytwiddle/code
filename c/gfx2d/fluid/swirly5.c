/* Swirly (repulsors with wibbly)
   Needs some playing around with variables.
*/

#include <stdlib.h>
#include "../BWlib/BWlib.h"
#include <math.h>

const float pi=3.1415926535897932385;
const numps=30; /* Don't forget to change down below too! */
const numrs=30; /* Don't forget to change down below too! */
const SCR_WIDTH=256;
const SCR_HEIGHT=256;
const left=-1;
const right=1;
const top=1;
const bottom=-1;
const int speed=1;
const float strength=0.8;
const gridsize=50;
const float arrowsize=1.0;
const float browniansize=0.00;
const traillength=10;
const float dampening=0.7;
const float holderstrength=0.10;
const float walkdamp=0.99;
const float walkrrnd=0.008;
const float walktrnd=0.3;

typedef struct {
  float x,y;
} vector;

typedef struct {
  vector pos;
  vector vel;
  float walkr;
  float walkt;
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
}


void randrepulsor(rep* r)
{
  r->pos.x=drand48()*2-1;
  r->pos.y=drand48()*2-1;
  r->str=sqrt(drand48())*0.002;
  r->dstr=r->str;
  r->off=drand48()*2*pi;
  r->fre=drand48()/200/square(r->str*1000);
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
    return Vmult((str+dstr*sin(off+frame*fre))/square(radius(Vsub(v,r))),Vnorm(Vsub(v,r)));
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


void movepart(part* pa)
{
  vector tmp;
  
  tmp=forceat(pa->pos);
  brownian(pa);
  pa->vel=Vadd(Vmult(dampening,pa->vel),Vmult(strength,tmp));
  pa->walkr=pa->walkr*walkdamp+(drand48()*2-1)*walkrrnd;
  pa->walkt=pa->walkt+(drand48()*2-1)*walktrnd;
  pa->pos=Vadd(pa->pos,Vadd(pa->vel,Cart(pa->walkr,pa->walkt)));
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
  
    movepart(&particle[i]);
  
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


void plotarrow(vector u,vector d)
{
  vector h; /* Head of arrow */
  
/*  if (!(Vequal(d,V(0,0))))*/
    h=Vadd(u,d);
    BWDrawLine(window2,scrx(u),scry(u),scrx(h),scry(h));
}


void plotfield()
{
  int i,j;
  vector u;
  
  BWClearArea(window2, 0, 0, SCR_WIDTH, SCR_HEIGHT);
  for (i=0; i<=gridsize; i++) {
  for (j=0; j<=gridsize; j++) {
    u=V(-1.0+2.0*i/gridsize,1.0-2.0*j/gridsize);
    if (radius(forceat(u))<0.01)
      plotarrow(u,Vmult(arrowsize*1000/gridsize,forceat(u)));
  }
  }
}


void plotrepulsors()
{
  int i;
  float j;
  rep r;
  
  BWClearArea(window2, 0, 0, SCR_WIDTH, SCR_HEIGHT);
  for (i=0; i<numrs; i++) {
    r=repulsor[i];
    for (j=0.0; j<2.0*pi; j=j+2.0*pi/8) {
      plotarrow(r.pos,Vmult(r.str+r.dstr*sin(r.off+frame*r.fre),Cart(80.0,j)));
    }
  }
}


int main(int argc, char **argv)
{
  int i,stillhappy;
  Display *display;
  int events;
  
  p.x=-0.9; p.y=0;
  q.x=0.9; q.y=0;
  
  srand48(8749847);
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
        plotparts();
        moveparts();
        if (mymod(frame,32)==0)
/*          plotfield();*/
          plotrepulsors();
        frame=frame+speed;
        break;
      }
    }
  }


}
