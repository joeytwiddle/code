/* Simple x client that creates and draws into a
 * MITSHM shared XImage 
 * 
 * gcc -o xlib-4 -Wall -O3 xlib-4.c -L/usr/X11R6/lib/ -lX11 -lXext
 * 
 * Written by cel98csn@mds.mdh.se, April 1999
 */
#include <sys/ipc.h>
#include <sys/shm.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>               /* For the visual matching */
#include <X11/extensions/XShm.h>

#include <stdlib.h>
#include <stdio.h>

/* These are for the CPU time measurement. */
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

typedef struct {
   double real, imag;
} complex;

static void complexInit(complex *z, double r, double i)
{
   z->real = r; z->imag = i;
   return;
}

static void complexAdd(complex *z, complex u)
{
   z->real += u.real;
   z->imag += u.imag;
}
static void complexSub(complex *z, complex u)
{
   z->real -= u.real;
   z->imag -= u.imag;
}

static void complexMult(complex *z, complex u)
{
   double tmp;
   tmp = z->real*u.real - z->imag*u.imag;
   z->imag = z->real*u.imag + z->imag*u.real;
   z->real = tmp;
}

static double complexReal(complex *z)
{
   return z->real;
}

static double complexImag(complex *z)
{
   return z->imag;
}

static void complexPrint(complex *z)
{
   printf("%.2g+%.2gi", complexReal(z), complexImag(z));
}

#if NOSSENUS_IS_DUMB
static double complexAbs(complex *z)
{
   return sqrt(z->real*z->real+ z->imag*z->imag);
}
#endif

static double complexAbsSquare(complex *z)
{
   return z->real*z->real+ z->imag*z->imag;
}

/* This is the maximal number of iterations julia will do */
#define MAXDEPTH 200

static int julia(complex z, complex c)
{
   int n;
   
   /* f(z) := z*z - c */
   
   for(n=0; n<MAXDEPTH; n++) 
     {
	complexMult(&z, z);
	complexSub(&z, c);
	/* if(complexAbs(&z)>2.0) */
	if(complexAbsSquare(&z)>4.0)
	  return n;
     } 
   
   return 0;
}

int DrawFractal(XImage *img, XColor rgb[], int colors, int w, int h)
{
   complex initial, point;
   int i, j;
   
   /* Play with this value */
   complexInit(&initial, 0.0, 0.0);
   
   printf("Rendering [%3d,%3d] fractal, please wait.\n", w, h); 
   
   for(j=0; j<h; j++)
     for(i=0; i<w; i++) 
     {
	int c;
	complexInit(&point, 2.5*(i-w/3)/w, 2.1*(j-h/2)/h);
	c = julia(initial, point);
	if(i%25==0) { /* print out every 25th pixel calculated */
	   printf("[%4d,%4d] = %4d\r", i, j, c);
	}
	img->f.put_pixel(img, i, j, rgb[c%colors].pixel);
     }
   
   printf("Done!                    \n");
   
   return 0;
}

/* Some globals */
int can_use_mitshm = 1;
int mitshm_major_code;
int mitshm_minor_code;

/* error handler to catch MITSHM problems */ 
int mitshm_handler(Display *d, XErrorEvent *ev)
{
   can_use_mitshm = 0;
   
   return 0;
}

/* The meaning of these are pretty much given */
#define COLORS	8
#define WIN_W	320
#define WIN_H	200

int main(void)
{
   Display *d;
   XImage *img=NULL;
   XEvent ev;
   Window win;
   XShmSegmentInfo shminfo;
   int screen, should_quit=0;
   GC gc;
   XVisualInfo vis, *vlist;
   int match;
   XColor rgb[COLORS];
   int i;
   struct rusage resource_utilization;
   int shared_pixmaps; /* dummy */
   
   d = XOpenDisplay(NULL);

   if(!d)
     fputs("Couldn't open display\n", stderr), exit(1);
   
   screen = DefaultScreen(d);
   gc = DefaultGC(d, screen);

   /* Find a visual */
   
   vis.screen = screen;
   vlist = XGetVisualInfo(d, VisualScreenMask, &vis, &match);
   
   if(!vlist) 
     fputs("No matched visuals\n", stderr), exit(1);
   
   vis = vlist[0]; 
   XFree(vlist);
   
   if(vis.colormap_size<COLORS)
     fputs("Colormap is too small.\n", stderr), exit(1);
   
   for(i=0; i<COLORS; i++) /* Set up the colors gradient */
     {
	rgb[i].red = 65535 * (1.0-1.0*i/COLORS);
	rgb[i].green = 65535 * (1.0-1.0*i/COLORS);;
	rgb[i].blue = 65535 * (1.0*i/COLORS);;
	rgb[i].flags = DoRed|DoGreen|DoBlue;
     }
   
   if(vis.class%2==1) /* The odd numbers can redefine colors */
     {
	unsigned long color[COLORS];
	
	/* Allocate cells */
	if(XAllocColorCells(d, DefaultColormap(d, screen), 1,
			    NULL, 0,
			    color, COLORS)==0)
	  fputs("Couldn't allocate enough colors cells.\n", 
		stderr), exit(1);

	/* Modify the colorcells */
	for(i=0; i<COLORS; i++) rgb[i].pixel = color[i];
	
	XStoreColors(d, DefaultColormap(d, screen), rgb, COLORS);
     }
   else if(vis.class == TrueColor)
     {
	/* This will lookup the color and sets the rgb[i].pixel value */
	for(i=0; i<COLORS; i++)
	  XAllocColor(d, DefaultColormap(d, screen), rgb + i);
     }
   else
     fprintf(stderr, "Not content with visual class %d.\n", 
	     vis.class), exit(1);
   
   /* Find out if MITSHM is supported and useable */   
   printf("MITSHM: ");
   
   if(XShmQueryVersion(d, &mitshm_major_code, 
		       &mitshm_minor_code, &shared_pixmaps))
     {
	int (*handler)(Display *, XErrorEvent *);
	img = XShmCreateImage(d, vis.visual,
                              vis.depth, XShmPixmapFormat(d),
                              NULL, &shminfo, WIN_W, WIN_H);
        shminfo.shmid = shmget(IPC_PRIVATE,
                               img->bytes_per_line*img->height,
                               IPC_CREAT|0777);
        shminfo.shmaddr = img->data = shmat(shminfo.shmid, 0, 0);
	
	handler = XSetErrorHandler(mitshm_handler);
        XShmAttach(d, &shminfo); /* Tell the server to attach */
	XSync(d, 0);
	XSetErrorHandler(handler);
	
	shmctl(shminfo.shmid, IPC_RMID, 0);
        /* Mark this shm segment for deletion at once. The segment will
         * automatically become released when both the server and this
         * client have detached from it.
         * (Process termination automagically detach shm segments) */
	
	if(!can_use_mitshm)
	  {
	     shmdt(shminfo.shmaddr);
	     img = NULL;
	  }
     }
   
   if(img==NULL) 
     {
	can_use_mitshm = 0;
        /* XInitImage(img); */
        img = XCreateImage(d, vis.visual, vis.depth, ZPixmap,
                           0, malloc(WIN_W*WIN_H), WIN_W, WIN_H, 
			   8, 0);
     }
   
   if(can_use_mitshm)
     printf("YES!\n");
   else
     printf("NO, using fallback instead.\n");
   
   DrawFractal(img, rgb, COLORS, WIN_W, WIN_H);
   
   win = XCreateSimpleWindow(d, DefaultRootWindow(d),
                             0, 0, WIN_W, WIN_H, 0,
                             WhitePixel(d, screen),
                             BlackPixel(d, screen));
   XSelectInput(d, win, ButtonPressMask|ExposureMask);
   XMapWindow(d, win);
   
   printf("Click in the fractal window to terminate the program.\n");
   
   while(!should_quit) {
      XNextEvent(d, &ev);
      switch(ev.type) {
       case ButtonPress:
	 should_quit=1;
	 break;
       case Expose:
	 if(can_use_mitshm)
           XShmPutImage(d, win, gc, img,
                        0,0,
                        0,0,
                        WIN_W,WIN_H,
                        True);
         else
           XPutImage(d, win, gc, img,
                     0,0,
                     0,0,
                     WIN_W,WIN_H);
         break;
       default:
      }
   }
   
   if(vis.class%2==1 || vis.class == TrueColor)
     {
	unsigned long color[COLORS];
	
	for(i=0; i<COLORS; i++) 
	  color[i] = rgb[i].pixel;
	XFreeColors(d, DefaultColormap(d, screen), color, COLORS, 0);
     } /* Allocated colors freed */
   
   if(can_use_mitshm)
     {
	XShmDetach(d, &shminfo); /* Server detached */
	XDestroyImage (img);	 /* Image struct freed */
	shmdt(shminfo.shmaddr);  /* We're detached */
     }
   else 
     XDestroyImage (img);	 /* Image struct freed */
   
   XDestroyWindow(d, win);  /* Window removed */
   XCloseDisplay(d);        /* Display disconnected */
   
   /* So you can see how your computer compares to your friend's */ 
   getrusage(RUSAGE_SELF, &resource_utilization);
   printf("CPU seconds consumed: %ds and %dµs\n",
	  (int)resource_utilization.ru_utime.tv_sec,
	  (int)resource_utilization.ru_utime.tv_usec);
   
   return 0;
}
