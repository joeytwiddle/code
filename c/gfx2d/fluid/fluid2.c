/* Swirly
   Ooh baby yeah this is a wicked program.
*/

#include <stdlib.h>
#include "BWlib.h"
#include <math.h>

const float pi=3.1415926535897932385;
const numps=100; /* Don't forget to change down below too! */
const SCR_WIDTH=256;
const SCR_HEIGHT=256;
const left=-1;
const right=1;
const top=1;
const bottom=-1;
const int speed=1;
const gridsize=20;
const float browniansize=0.001;
const traillength=50;

typedef struct {
  float x,y;
} vector;

typedef struct {
  vector pos;
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
}


void printvect(vector v)
{
  fprintf(stderr,"(%f",v.x);
  fprintf(stderr,",%f)",v.y);
}


void printpart(part* p)
{
  printvect(p->pos);
}


vector newvect(float x,float y)
{
  vector p;
  
  p.x=x;
  p.y=y;
  return p;
}


vector multvect(float a,vector v)
{
  vector w;
  
  w.x=a*v.x;
  w.y=a*v.y;
  return w;
}


vector addvects(vector u, vector v)
{
  vector w;
  
  w.x=u.x+v.x;
  w.y=u.y+v.y;
  return w;
}


vector subvects(vector u, vector v)
{
  return addvects(u,multvect(-1,v));
}


vector rotatevect(vector u,double a)
{
  vector v;
  
  v.x=u.x*cos(a)-u.y*sin(a);
  v.y=+u.x*sin(a)+u.y*cos(a);
  return v;
}


float radius(vector v)
{
  return sqrt(v.x*v.x+v.y*v.y);
}


vector normalise(vector v)
{
  return multvect(1/radius(v),v);
}


int equalvects(vector u,vector v)
{
  if ((u.x==v.x) && (u.y==v.y))
    return 1;
  else
    return 0;
}



vector forceat(vector v)
{
  return multvect(0.1*(0.7-radius(v))/10,rotatevect(normalise(v),frame*2*pi/200));
}



/*
vector forceat(vector v)
{
  return multvect((cos(sin(frame)*frame*pi/2000)+0.7-radius(v))*speed*0.01,multvect(sin((frame*2*pi)/(rand())),rotatevect(normalise(v),speed*frame*pi/300)));
}
*/


/*
vector forceat(vector v)
{
  vector vminusp,vminusq;
  
  if ((v.x==0) && (v.y==0))
    return newvect(0,0);
  else
    vminusp=addvects(v,multvect(-1,p));
    vminusq=addvects(v,multvect(-1,q));
    return multvect(0.001,addvects(multvect(2*(1-radius(vminusp))*sin(frame*2*pi/745),normalise(vminusp)),
         multvect(3*(1-radius(vminusq))*sin(frame*2*pi/987),normalise(vminusq))));
}
*/


vector inoutattractor(vector v,vector a,float strength,int period)
{
  if (equalvects(v,a))
    return newvect(0,0);
  else
    return multvect(strength*(1-radius(subvects(v,a)))*sin(frame*2*pi/period),normalise(subvects(v,a)));
}

/*
vector forceat(vector v)
{
  vector vminusp,vminusq;
  
  if (equalvects(v,newvect(0,0)))
    return newvect(0,0);
  else
    return multvect(0.001,addvects(rotatevect(normalise(v),pi/2+pi/32+0*sin(frame*2*pi/10)),
           addvects(inoutattractor(v,p,2,856),inoutattractor(v,q,3,1381))));
}*/


void brownian(part *p)
{
  p->pos.x=p->pos.x+(drand48()*2.0-1.0)*browniansize;
  p->pos.y=p->pos.y+(drand48()*2.0-1.0)*browniansize;
}

void movepart(part* p)
{
  
  brownian(p);
  p->pos=addvects(p->pos,multvect(speed,forceat(p->pos)));
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
  
  h=addvects(u,d);
  BWDrawLine(window2,scrx(u),scry(u),scrx(h),scry(h));
}


void plotfield()
{
  int i,j;
  vector u;
  
  BWClearArea(window2, 0, 0, SCR_WIDTH, SCR_HEIGHT);
  for (i=0; i<=gridsize; i++) {
  for (j=0; j<=gridsize; j++) {
    u=newvect(-1.0+2.0*i/gridsize,1.0-2.0*j/gridsize);
    plotarrow(u,multvect(1000/gridsize,forceat(u)));
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
  window2 = BWCreateNewWindow(display, argc, argv);
  window1 = BWCreateNewWindow(display, argc, argv);

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
