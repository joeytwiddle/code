#define Region Banana
#include <joeylib.h>
#define Region Region

#include <sys/ipc.h>
#include <sys/shm.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>		/* For the visual matching */
#include <X11/extensions/XShm.h>

#include <stdlib.h>
#include <stdio.h>

/* These are for the CPU time measurement. */
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "xvvis.h"

#include <sched.h>

// struct Fly {
	// V3d pos,dir;
	// float v;
	// Fly() {
		// pos=V3d::random();
		// dir=V3d::cross(pos,V3d::random());
		// v=0.02;
	// }
	// void move() {
		// pos=(pos+dir*v).norm();
		// dir=-V3d::cross(V3d::cross(dir,pos),pos);
	// }
// };

struct Fly {
	V3d pos,axis;
	float v;
	Matrix rotm;
	Fly() {
		pos=V3d::random();
		axis=V3d::cross(pos,V3d::random());
		v=0.04;
		rotm=Matrix::rotation(axis,v);
	}
	void move() {
		pos=rotm*pos;
		// dir=-V3d::cross(V3d::cross(dir,pos),pos);
	}
};

int flies=100;

Fly *fly;

int col;

long unsigned int black,white;

int scrwid,scrhei;

void init() {
	fly=new Fly[flies];
	for (int i=0;i<flies;i++)
		fly[i]=Fly();
	col=0;
}

int DrawFractal (XImage * img, XColor rgb[], int colors, int w, int h)
{
  int i, j;
		// if (col==0) {
						// for (int i=0;i<scrwid;i++)
			// for (int j=0;j<scrhei;j++)
				// img->f.put_pixel(img,i,j,black);
		// // col = ( col + 1 ) % colors;
		// col=1;
	// }
  // for (j = 0; j < h; j++)
    // for (i = 0; i < w; i++) {
      // img->f.put_pixel (img, i, j, rgb[0].pixel);
      // // img->f.put_pixel (img, i, j, rgb[(i + j * w) % colors].pixel);
    // }
	col++;
	for (i=0;i<flies;i++) {
		Fly f=fly[i];
		// f.move();
		int px=scrwid/2+f.pos.x*scrwid/2*0.9;
		int py=scrhei/2+f.pos.y*scrhei/2*0.9;
		img->f.put_pixel(img,px,py,black);
		fly[i].move();
		f=fly[i];
		px=scrwid/2+f.pos.x*scrwid/2*0.9;
		py=scrhei/2+f.pos.y*scrhei/2*0.9;
		img->f.put_pixel(img,px,py,rgb[i%colors].pixel);
	}
  return 0;
}

/* Some globals */
int can_use_mitshm = 1;
int mitshm_major_code;
int mitshm_minor_code;

/* error handler to catch MITSHM problems */
int mitshm_handler (Display * d, XErrorEvent * ev)
{
  can_use_mitshm = 0;
  return 0;
}

/* The meaning of these are pretty much given */
#define COLORS	8
#define WIN_W	640
#define WIN_H	640

  Display *d;
  XImage *img = NULL;
  XEvent ev;
  XShmSegmentInfo shminfo;
  int screen, should_quit = 0;
  GC gc;
  XVisualInfo vis, *vlist;
  int match;
  struct rusage resource_utilization;
  int shared_pixmaps;		/* dummy */

int main (void)
{
  int i;
  int allocateOK;
  Colormap colormap;
  Window win;
  XColor rgb[COLORS];


	
	scrwid=WIN_W;
	scrhei=WIN_H;
	init();
	

  d = XOpenDisplay (NULL);

  if (!d)
    fputs ("Couldn't open display\n", stderr), exit (1);

  screen = DefaultScreen (d);
  gc = DefaultGC (d, screen);

  /* Find a visual */

  vis.screen = screen;
  vlist = XGetVisualInfo (d, VisualScreenMask, &vis, &match);

  if (!vlist)
    fputs ("No matched visuals\n", stderr), exit (1);

  vis = vlist[0];
  XFree (vlist);

  if (vis.colormap_size < COLORS)
    fputs ("Colormap is too small.\n", stderr), exit (1);

  for (i = 0; i < COLORS; i++) {	/* Set up the colors gradient */
    rgb[i].red = 65535 * (1.0 - 1.0 * i / COLORS);
    rgb[i].green = 65535 * (1.0 - 1.0 * i / COLORS);;
    rgb[i].blue = 65535 * (1.0 * i / COLORS);;
    rgb[i].flags = DoRed | DoGreen | DoBlue;
  }

  if ( get_class(vis) % 2 == 1) {	/* The odd numbers can redefine colors */
    unsigned long color[COLORS];

    colormap = DefaultColormap (d, screen);
    /* Allocate cells */
    allocateOK = (XAllocColorCells (d, colormap, 1,
				    NULL, 0, color, COLORS) != 0);
    if (!allocateOK) {
      XInstallColormap (d, colormap);
      // fputs("Couldn't allocate enough colors cells.\n", 
      // stderr), exit(1);
    } else {

      /* Modify the colorcells */
      for (i = 0; i < COLORS; i++)
				rgb[i].pixel = color[i];

      // This doesn't work for installed colormap!

      XStoreColors (d, colormap, rgb, COLORS);

    }

		black = XBlackPixel(d,screen);
		white = XWhitePixel(d,screen);

  } else if ( get_class(vis) == TrueColor) {
    /* This will lookup the color and sets the rgb[i].pixel value */
    for (i = 0; i < COLORS; i++)
      XAllocColor (d, DefaultColormap (d, screen), rgb + i);
  } else
    fprintf (stderr, "Not content with visual class %d.\n",
	     get_class(vis) ), exit (1);

  /* Find out if MITSHM is supported and useable */
  printf ("MITSHM: ");

  if (XShmQueryVersion (d, &mitshm_major_code,
			&mitshm_minor_code, &shared_pixmaps)) {
    int (*handler) (Display *, XErrorEvent *);
    img = XShmCreateImage (d, vis.visual,
			   vis.depth, XShmPixmapFormat (d),
			   NULL, &shminfo, WIN_W, WIN_H);
    shminfo.shmid = shmget (IPC_PRIVATE,
			    img->bytes_per_line * img->height,
			    IPC_CREAT | 0777);
    shminfo.shmaddr = (char *)shmat (shminfo.shmid, 0, 0);
		img->data = (char *)shminfo.shmaddr;

    handler = XSetErrorHandler (mitshm_handler);
    XShmAttach (d, &shminfo);	/* Tell the server to attach */
    XSync (d, 0);
    XSetErrorHandler (handler);

    shmctl (shminfo.shmid, IPC_RMID, 0);
    /* Mark this shm segment for deletion at once. The segment will
     * automatically become released when both the server and this
     * client have detached from it.
     * (Process termination automagically detach shm segments) */

    if (!can_use_mitshm) {
      shmdt (shminfo.shmaddr);
      img = NULL;
    }
  }

  if (img == NULL) {
    can_use_mitshm = 0;
    /* XInitImage(img); */
    img = XCreateImage (d, vis.visual, vis.depth, ZPixmap,
			0, (char *)malloc (WIN_W * WIN_H), WIN_W, WIN_H, 8, 0);
  }

  if (can_use_mitshm)
    printf ("YES!\n");
  else
    printf ("NO, using fallback instead.\n");

  DrawFractal (img, rgb, COLORS, WIN_W, WIN_H);

  win = XCreateSimpleWindow (d, DefaultRootWindow (d),
			     0, 0, WIN_W, WIN_H, 0,
			     WhitePixel (d, screen), BlackPixel (d, screen));
  XSelectInput (d, win, ButtonPressMask | ExposureMask);
  XMapWindow (d, win);

  printf ("Click in the fractal window to terminate the program.\n");

  while (!should_quit) {

		// for (int i=0;i<flies;i++)
			// fly[i].move();
		DrawFractal(img,rgb,COLORS,WIN_W,WIN_H);
      if (can_use_mitshm)
				XShmPutImage (d, win, gc, img, 0, 0, 0, 0, WIN_W, WIN_H, True);
      else
				XPutImage (d, win, gc, img, 0, 0, 0, 0, WIN_W, WIN_H);
			XFlush(d);
	    XSync (d, 0); // this stopped it going mad
		// if (col>100) {
			// printf(".\n");
		  // wait(0.01);
		// }
		// sched_yield();
	
    // XNextEvent (d, &ev);
    // switch (ev.type) {
    // case ButtonPress:
      // should_quit = 1;
      // break;
    // case Expose:
      // if (can_use_mitshm)
				// XShmPutImage (d, win, gc, img, 0, 0, 0, 0, WIN_W, WIN_H, True);
      // else
				// XPutImage (d, win, gc, img, 0, 0, 0, 0, WIN_W, WIN_H);
      // break;
    // default:
			// break;
    // }
  }

  if (get_class(vis) % 2 == 1 || get_class(vis) == TrueColor) {
    unsigned long color[COLORS];

    if (allocateOK) {
      for (i = 0; i < COLORS; i++)
				color[i] = rgb[i].pixel;
      XFreeColors (d, colormap, color, COLORS, 0);
    }				/* Allocated colors freed */
  } else {
    XUninstallColormap (d, colormap);
  }

  if (can_use_mitshm) {
    XShmDetach (d, &shminfo);	/* Server detached */
    XDestroyImage (img);	/* Image struct freed */
    shmdt (shminfo.shmaddr);	/* We're detached */
  } else
    XDestroyImage (img);	/* Image struct freed */

  XDestroyWindow (d, win);	/* Window removed */
  XCloseDisplay (d);		/* Display disconnected */

  /* So you can see how your computer compares to your friend's */
  getrusage (RUSAGE_SELF, &resource_utilization);
  printf ("CPU seconds consumed: %ds and %dµs\n",
	  (int) resource_utilization.ru_utime.tv_sec,
	  (int) resource_utilization.ru_utime.tv_usec);

  return 0;
}
