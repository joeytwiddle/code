/* main.c */

/* Just a short test program to open two windows and close them again 
 * when a mouse button is pressed.
 * I might write something useful soon.
 */

#include <stdlib.h>
#include "BWlib.h"
#include <math.h>
#include "/usr/local/course/csadc/iplib/imagproc.h"

const radius=3;
const SCR_WIDTH=211;
const SCR_HEIGHT=211;

int t;

void nl()
{
	fprintf(stderr,"\n");
}

int total()
{
  int d,x,y,z;
  
  d=0;
  for (x=-radius; x<=radius; x++) {
  z=sqrt(radius*radius-x*x);
  for (y=-z; y<=z; y++) {
    d++;
  }
  }
  return d;
}

int neighbour(unsigned char **p,int i,int j)
{
  if ((i>=0) && (i<SCR_WIDTH) && (j>=0) && (j<SCR_HEIGHT))
    return p[i][j];
  else
    return 0;
}

 int count(unsigned char **p, int i, int j)
{
  int c,x,y,z;
  
  c=0;
  
  /* If not near edge then don't bother checking for going off side. */
  if ( (i>radius) && (i<SCR_WIDTH-radius) && (j>radius) && (j<SCR_WIDTH-radius)) {
    
    for (x=-radius; x<=radius; x++) {
    z=sqrt(radius*radius-x*x);
    for (y=-z; y<=z; y++) {
      c=c+p[i+x][j+y];
    }
    }

  /* Otherwise do. */
  } else {
  
    for (x=-radius; x<=radius; x++) {
    z=sqrt(radius*radius-x*x);
    for (y=-z; y<=z; y++) {
      if (neighbour(p,i+x,j+y)==1)
        c++;
    }
    }

  }
  
  if ( c<(t*0.5) )
    return 0;
  else
    return 1;
  
}

unsigned char **update_life(unsigned char **p)
{
  int c,i,j;
  unsigned char **q;
  
  q=uchar_array(SCR_WIDTH,SCR_HEIGHT);
  
  for (i=0; i<SCR_HEIGHT; i++) {
  for (j=0; j<SCR_WIDTH; j++) {
    q[i][j]=count(p,i,j);
  }
  }
  return q;
}

void show_life(this_window, p)
BasicWin this_window;
unsigned char **p;
{
	int i,j;
	
	BWClearArea(this_window, 0, 0, SCR_WIDTH, SCR_HEIGHT);
	for (i=0; i<SCR_HEIGHT; i++) {
	for (j=0; j<SCR_WIDTH; j++) {
	  if (p[i][j]==1)
	    BWDrawPoint(this_window,j,i);
	}
	}
}

int main(int argc, char **argv)
{
	int i,j;
	Display *display;
	BasicWin window1;
	BasicWin window2;
	int events;
	unsigned char **p;
  char *firstparm;

  firstparm=argv[1];
  srand(firstparm[0]+firstparm[1]+firstparm[2]);

	t=total();
	
	p=uchar_array(SCR_WIDTH,SCR_HEIGHT);
	
	for (i=0; i<SCR_HEIGHT; i++) {
	for (j=0; j<SCR_WIDTH; j++) {
	  if ((rand() & 1) == 1)
	    p[i][j]=1;
	  else
	    p[i][j]=0;
	}
	}
	
	/* Connect to the server for display */
	display = BWConnectToServer();

	/* Open the windows */
	window1 = BWCreateNewWindow(display, argc, argv);

	/* Start Program */
	while(1)
	{
	/* Check window events */
	events = BWCheckEvents(window1);
	
	switch (events)
		{
		/* Window exposed - redraw */
		case EV_EXPOSED:
			{

			  show_life(window1,p);
	
			break;
			}

		/* Mouse button pressed - close both windows and exit*/
		case EV_BUTTON:
			{
			BWCloseWindow(window1);
			exit(0);
			}

		default: {
		  p=update_life(p);
		  show_life(window1,p);
		  break;
		}
		
		}
	}
}
