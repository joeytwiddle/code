/* main.c */

/* Just a short test program to open two windows and close them again 
 * when a mouse button is pressed.
 * I might write something useful soon.
 */

#include "c:\joey\code\c\mylib\mylib.c"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

const numps=20;
const SCR_WIDTH=320;
const SCR_HEIGHT=200;

unsigned char ** uchar_array(int w,int h) {
  int i;
  unsigned char ** p;
  p=calloc(w,sizeof(unsigned char *));
  for (i=0;i<w;i++) {
    p[i]=calloc(h,sizeof(unsigned char));
  }
  return p;
}

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

void show_life(p)
unsigned char **p;
{
	int i,j;
	
//  BWClearArea(this_window, 0, 0, SCR_WIDTH, SCR_HEIGHT);
	for (i=0; i<SCR_HEIGHT; i++) {
	for (j=0; j<SCR_WIDTH; j++) {
    if (p[i][j]==1)
      putpixel(screen,i,j,1);
//      BWDrawPoint(this_window,j,i);
	}
	}
}

int main(int argc, char **argv)
{
	int i,j;
	int events;
  unsigned char **p;

	srand(874984);

  allegrosetup(SCR_WIDTH,SCR_HEIGHT);

  p=uchar_array(SCR_WIDTH,SCR_HEIGHT);
	
	for (i=0; i<SCR_HEIGHT; i++) {
	for (j=0; j<SCR_WIDTH; j++) {
	  if ((rand() & 1) == 1)
      p[j][i]=1;
	  else
      p[j][i]=0;
	}
	}
	
  while(!key[KEY_ESC])
	{
        p=update_life(p);
        show_life(p);
	}
}
