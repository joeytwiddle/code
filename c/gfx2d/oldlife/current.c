/* main.c */

/* Just a short test program to open two windows and close them again 
 * when a mouse button is pressed.
 * I might write something useful soon.
 */

#include <stdlib.h>
#include <math.h>
#include "c:\joey\code\c\mylib\mylib.c"
#include <stdio.h>

const radius=3;
const SCR_WIDTH=320;
const SCR_HEIGHT=200;

int t;

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
  if ( (i>radius) && (i<SCR_WIDTH-radius) && (j>radius) && (j<SCR_HEIGHT-radius)) {
    
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
    q[j][i]=count(p,j,i);
  }
  }
  return q;
}

void show_life(p)
unsigned char **p;
{
	int i,j;
	
	for (i=0; i<SCR_HEIGHT; i++) {
	for (j=0; j<SCR_WIDTH; j++) {
    if (p[j][i]==1)
      putpixel(screen,j,i,15);
    else
      putpixel(screen,j,i,0);
	}
	}
}

int main(int argc, char **argv)
{
	int i,j;
	unsigned char **p;
  char *firstparm;

  firstparm=argv[1];
  srand(firstparm[0]+firstparm[1]+firstparm[2]);

  allegrosetup(SCR_WIDTH,SCR_HEIGHT);

	t=total();
	
	p=uchar_array(SCR_WIDTH,SCR_HEIGHT);
	
	for (i=0; i<SCR_HEIGHT; i++) {
	for (j=0; j<SCR_WIDTH; j++) {
	  if ((rand() & 1) == 1)
      p[j][i]=1;
	  else
      p[j][i]=0;
	}
	}
	
	/* Start Program */
  while(!key[KEY_ESC])
	{
		  p=update_life(p);
      show_life(p);
	}
}
