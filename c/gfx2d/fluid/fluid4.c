/* Swirly
   Ooh baby yeah this is a wicked program.
*/

#include <stdlib.h>
#include "\joey\code\others\bwlib\BWlib.h"
#include <math.h>

const float pi=3.1415926535897932385;
const numps=100; /* Don't forget to change down below too! */
const SCR_WIDTH=256;
const SCR_HEIGHT=256;
const left=-1;
const right=1;
const top=1;
const bottom=-1;
const int speed=2;
const gridsize=20;
const float browniansize=0.0001;
const traillength=1;
const float dampening=0.8;
const float strength=0.0001;

typedef struct {
  float x,y;
} vector;

typedef struct {
  vector pos;
  vector vel;
} part;


part particle[1000];
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


void nl()
{
  fprintf(stderr,"\n");
}


void msg(char *c)
{
  fprintf(stderr,c); nl;
}


void randpart(part* v)
{
  v->pos.x=drand48()*2-1;
  v->pos.y=drand48()*2-1;
  v->vel.x=0;
  v->vel.y=0;
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


vector V(float x,float y)
{
  vector p;
  
  p.x=x;
  p.y=y;
  return p;
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


vector Vrotate(vector u,double a)
{
  vector v;
  
  v.x=u.x*cos(a)-u.y*sin(a);
  v.y=+u.x*sin(a)+u.y*cos(a);
  return v;
}


float square(float x)
{
  return x*x;
}


float radius(vector v)
{
  return sqrt(v.x*v.x+v.y*v.y);
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


vector massivebody(vector v)
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


vector inoutattractor(vector v,vector a,float strength,int period)
{
  if (Vequal(v,a))
    return V(0,0);
  else
    return Vmult(strength*(1-radius(Vsub(v,a)))*sin(frame*2*pi/period),Vnorm(Vsub(v,a)));
}

vector forceat(vector v)
{
  vector vminusp,vminusq;
  
  if (Vequal(v,V(0,0)))
    return V(0,0);
  else
    return Vmult(0.001,Vadd(Vrotate(Vnorm(v),pi/2+pi/32+0*sin(frame*2*pi/10)),
           Vadd(inoutattractor(v,p,3,85),inoutattractor(v,q,2,381))));
}


void brownian(part *p)
{
  p->vel.x=p->vel.x+(drand48()*2.0-1.0)*browniansize;
  p->vel.y=p->vel.y+(drand48()*2.0-1.0)*browniansize;
}

void movepart(part* pa)
{
  vector tmp;
  
  tmp=forceat(pa->pos);
  brownian(pa);
  if (Vequal(pa->pos,q) || Vequal(pa->pos,p))
    randpart(pa);
  else 
    pa->vel=Vmult(dampening,Vadd(pa->vel,Vmult(speed,tmp)));
    pa->pos=Vadd(pa->pos,pa->vel);
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
  int i;
  
  for (i=0; i<numps; i++) {
    movepart(&particle[i]);
  }
}


void plotarrow(vector u,vector d)
{
  vector h; /* Head of arrow */
  
  h=Vadd(u,d);
  if (!(Vequal(d,V(0,0))))
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
      plotarrow(u,Vmult(1000/gridsize,forceat(u)));
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
  
  srand48(874984);
  frame=0;

  /* Connect to the server for display */
  display = BWConnectToServer();

  /* Open the windows */
  window1 = BWCreateNewWindow(display, argc, argv, 882, 4);
  window2 = BWCreateNewWindow(display, argc, argv, 882, 324);

  for (i=0; i<numps; i++) {
    randpart(&particle[i]);
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
          plotfield();
        frame=frame+speed;
        break;
      }
    }
  }


}
