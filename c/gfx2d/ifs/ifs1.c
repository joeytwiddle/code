/* IFS */

#include <stdlib.h>
#include "../BWlib/BWlib.h"
#include <math.h>
#include <sys/types.h>
#include <time.h>

const SCR_WIDTH=256;
const SCR_HEIGHT=256;
const left=-1;
const right=1;
const top=1;
const bottom=-1;
const ts=4;
const ps=500;
const float change=0.01;

BasicWin window1,window2;
float v[20][6];
int p,showts;
float x[10000],y[10000];
float cx,cy,nx,ny;
time_t *tloc;


void setuptrans()
{
  int t,n;
  
  for (t=0; t<ts; t++) {
  for (n=0; n<4; n++) {
    v[t][n]=2.0*drand48()-1.0;
  }
  v[t][4]=drand48()-0.5;
  v[t][5]=drand48()-0.5;
  }
}


int scrx(float x)
{
  return SCR_WIDTH*(x-left)/(right-left);
}


int scry(float y)
{
  return SCR_HEIGHT*(y-top)/(bottom-top);
}


void transform(float x, float y, int t)
{
  nx=x*v[t][0]+y*v[t][1]+v[t][4];
  ny=y*v[t][2]+y*v[t][3]+v[t][5];
}


void newpoint()
{
  int t;
  
  t=(int)(drand48()*ts);
  
  transform(cx,cy,t);
  
  BWClearArea(window1,scrx(x[p]),scry(y[p]),1,1);
  BWDrawPoint(window1,scrx(nx),scry(ny));
  
  cx=nx; cy=ny;
  x[p]=cx; y[p]=cy;
  
  p=p+1;
  if (p>=ps)
    p=0;
}


void plottrans()
{
  int t,x1,x2,x3,x4,y1,y2,y3,y4;
  
  BWClearArea(window2,0,0,SCR_WIDTH,SCR_HEIGHT);
  
  for (t=0; t<ts; t++) {
    transform(1,1,t);  x1=scrx(nx); y1=scry(ny);
    transform(-1,1,t);  x2=scrx(nx); y2=scry(ny);
    transform(-1,-1,t);  x3=scrx(nx); y3=scry(ny);
    transform(1,-1,t);  x4=scrx(nx); y4=scry(ny);
    BWDrawLine(window2,x1,y1,x2,y2);
    BWDrawLine(window2,x2,y2,x3,y3);
    BWDrawLine(window2,x3,y3,x4,y4);
    BWDrawLine(window2,x4,y4,x1,y1);
  }
}


void movetrans()
{
  int t,n;
  
  t=(int)(drand48()*ts);
  n=(int)(drand48()*6);
  
  v[t][n]=v[t][n]+(drand48()-0.5)*change;
  
  if (showts==1)
    plottrans();
}


int main(int argc, char **argv)
{
  Display *display;
  int events;
  
  srand48(time(tloc));

  setuptrans();
  p=0;
  cx=drand48()*2.0-1.0;
  cy=drand48()*2.0-1.0;

  /* Connect to the server for display */
  display = BWConnectToServer();
  window1 = BWCreateNewWindow(display, argc, argv, 882, 4);
  window2 = BWCreateNewWindow(display, argc, argv, 882, 324);

  while(1) {
    events = BWCheckEvents(window1);
    switch (events) {
      /* Window exposed - redraw */
      case EV_EXPOSED:
        {
	  break;
	}
	/* Mouse button pressed - close both windows and exit*/
      case EV_BUTTON: {
        BWCloseWindow(window1);
        exit(0);
      }
      default: {
	newpoint();
	movetrans();
        break;
      }
    }
    events=BWCheckEvents(window2);
    if (events==EV_EXPOSED) {
      if (showts==1)
        showts=0;
      else
        showts=1;
    }
  }


}
