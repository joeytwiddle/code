/* main.c */

/* Just a short test program to open two windows and close them again 
 * when a mouse button is pressed.
 * I might write something useful soon.
 */

#include <stdlib.h>
#include "BWlib.h"
#include <math.h>
#include "/usr/local/course/csadc/iplib/imagproc.h"

const numps=20;
const SCR_WIDTH=400;
const SCR_HEIGHT=400;

void nl()
{
	fprintf(stderr,"\n");
}

int hasneighbour(unsigned char **p,int i,int j)
{
  if ((i>=0) && (i<SCR_WIDTH) && (j>=0) && (j<SCR_HEIGHT)) {
    if (p[i][j]==1)
      return 1;
  else
    return 0;
  }
}

int count(unsigned char **p, int i, int j)
{
  int c;
  
  c=0;
  c=c+hasneighbour(p,i-1,j);
  c=c+hasneighbour(p,i+1,j);
  c=c+hasneighbour(p,i,j-1);
  c=c+hasneighbour(p,i,j+1);
  c=c+hasneighbour(p,i-1,j+1);
  c=c+hasneighbour(p,i+1,j-1);
  c=c+hasneighbour(p,i-1,j-1);
  c=c+hasneighbour(p,i+1,j+1);
  c=c+hasneighbour(p,i,j);
  return c;
}

unsigned char **update_life(unsigned char **p)
{
  int c,i,j;
  unsigned char **q;
  
  q=uchar_array(SCR_WIDTH,SCR_HEIGHT);
  
  for (i=0; i<SCR_HEIGHT; i++) {
  for (j=0; j<SCR_WIDTH; j++) {
    c=count(p,i,j);
    if (c<5)
      q[i][j]=0;
    else
      q[i][j]=1;
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

	srand(874984);
	
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
/*			BWCloseWindow(window1);
			exit(0);*/
			}

		default: {
			  p=update_life(p);
			  show_life(window1,p);
		
/*		          update_life(p);
		          show_life(window1,p);*/
		
			break;
		}
		
		}
	}
}
