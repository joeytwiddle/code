/* Swirly

   Ooh baby yeah this is a wicked program.

*/

#include <stdlib.h>
#include "../BWlib/BWlib.h"
#include <math.h>

const float pi=3.1415926535897932385;
const numps=20;
const SCR_WIDTH=256;
const SCR_HEIGHT=256;
const left=-1;
const right=1;
const top=1;
const bottom=-1;


typedef struct {
  float x,y;
} vector;

typedef struct {
  vector pos;
} part;


part particle[20];
BasicWin window1;
int frame;


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


vector rotatevect(vector u,double a)
{
  vector v;
  
  v.x=u.x*cos(a)+u.y*sin(a);
  v.y=-u.x*sin(a)+u.y*cos(a);
  return v;
}


vector forceat(vector v)
{
  return multvect(0.01,rotatevect(v,frame*pi/300));
}


void movepart(part* p)
{
  vector push;
  
  push=forceat(p->pos);
  p->pos=addvects(p->pos,push);
}


void plotpart(part* p)
{
  int x,y;
  
  x=SCR_WIDTH*((p->pos.x)-left)/(right-left);
  y=SCR_HEIGHT*((p->pos.y)-top)/(bottom-top);
  BWDrawPoint(window1,x,y);
}


void plotparts()
{
  int i;
  
  if ((frame & 4)==0)
    BWClearArea(window1, 0, 0, SCR_WIDTH, SCR_HEIGHT);
  for (i=0; i<numps; i++) {
    plotpart(&particle[i]);
  }
}


void moveparts()
{
  int i;
  
  nl;
  for (i=0; i<numps; i++) {
    movepart(&particle[i]);
  }
}


int main(int argc, char **argv)
{
  int i,stillhappy;
  Display *display;
  int events;
  
  srand48(874984);
  frame=0;

  /* Connect to the server for display */
  display = BWConnectToServer();

  /* Open the windows */
  window1 = BWCreateNewWindow(display, argc, argv);

  for (i=0; i<numps; i++) {
    randpart(&particle[i]);
    printpart(&particle[i]); nl();
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
        frame++;
        break;
      }
    }
  }


}
