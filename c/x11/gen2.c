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

#define COLORS	100
#define colors COLORS

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
#define WIN_W	scrwid
#define WIN_H	scrhei
#define scrwid 320
#define scrhei 200

  Display *d;
  XImage *ximg = NULL;
  GC gc;


void wicked_blit(Window win) {

      if (can_use_mitshm)
				XShmPutImage (d, win, gc, ximg, 0, 0, 0, 0, WIN_W, WIN_H, True);
      else
				XPutImage (d, win, gc, ximg, 0, 0, 0, 0, WIN_W, WIN_H);
			XFlush(d);
	    XSync (d, 0); // this stopped it going mad
		  // wait(0.01);
		sched_yield();
			printf(".\n");

}

#define pi 3.141593
#define uchar unsigned char

#define fs 12
#define palspeed 2
#define colspeed 2
#define framespermap 20

//r = .5 + 2 * t / pi + .5 * SIN(1.5 * t)

void perturb (int f);
//void mycircle(int cx,int cy,int r,int c);
void moremap();

// Maps used for background writing
JBmp img,img2,imgtmp;
float amount[scrwid][scrhei][2][2][2];
int pix[scrwid][scrhei][2],piy[scrwid][scrhei][2];
//float *****amount;
//int ***pix,***piy;
int fon[fs];
float var[fs],op[fs],damp[fs],force[fs];
float toff=0;
int usingmap,makingmap,mmx,mmy,tmpmap,maploop;

int huefor(float t,float o) {
  float ctmp;
  ctmp=myabs(t-o);
  if (ctmp>0.5)
    ctmp=(float)1-ctmp;
  // return 255*((float)0.5+0.5*cos(2*pi*ctmp));
  ctmp=ctmp*6;
  if (ctmp<1)
    return 255;
  if (ctmp>2)
    return 0;
  return 63*((float)2-ctmp);
}


  int f,i,j,k,c,x,y,ix,iy,displayloop;
  float thru,ctmp;
  myRGB rgb;

int init_candy(void) {
  

  srand((int)time(NULL));
  
  usingmap=0; makingmap=1; mmx=0; mmy=0;

  img=JBmp(scrwid,scrhei);
  img2=JBmp(scrwid,scrhei);

  for (y=0;y<scrhei;y++) {
    for (x=0;x<scrwid;x++) {
      img.bmp[y][x]=256*y/scrhei;
      if (x<scrwid-1 && y<scrhei-1) {
        pix[x][y][usingmap]=x;
        piy[x][y][usingmap]=y;
        for (i=0;i<=1;i++)
        for (j=0;j<=1;j++)
          amount[x][y][i][j][usingmap]=(float)1/4;
      }
    }
  }
  
  /* Originals from QB
     op[0] = 1; damp[0] = .999; force[0] = .005;
     op[1] = 1.02; damp[1] = .999; force[1] = .002;
     op[2] = 0; damp[2] = .999; force[2] = .002;
     op[3] = 1; damp[3] = .999; force[3] = .005;
     op[4] = 1; damp[4] = .999; force[4] = .005;
     op[5] = 0; damp[5] = .999; force[5] = .002;
  */
  
  /* From QB later
     name$(1) = "Velocity"
     op(1) = 1: damp(1) = .999: force(1) = .002
     name$(2) = "Rotation"
     op(2) = 0: damp(2) = .999: force(2) = .002
     name$(3) = "Drip"
     op(3) = 1: damp(3) = .999: force(3) = .005
     name$(4) = "Dribble"
     op(4) = 1: damp(4) = .999: force(4) = .005
     name$(5) = "Slide"
     op(5) = 0: damp(5) = .999: force(5) = .002
     name$(6) = "Accelerate"
     op(6) = 1: damp(6) = .999: force(6) = .005
     name$(7) = "xDisplace"
     op(7) = 0: damp(7) = .999: force(7) = .005
     name$(8) = "yDisplace"
     op(8) = 0: damp(8) = .999: force(8) = .005
     REM 9 and 10 are options for splitting displacements (no var)
     name$(9) = "2d/3d split"
     name$(10) = "Split"
  */
  
  // 0 Accelerate
  op[0] = 1; damp[0] = .9; force[0] = .01;
  // 1 Velocity
  op[1] = 1; damp[1] = .9; force[1] = .01;
  // 2 Rotation
  op[2] = 0; damp[2] = .9; force[2] = .02;
  // 3 y splurge
  op[3] = 0; damp[3] = .9; force[3] = .01;
  // 4 Dribble
  op[4] = 1; damp[4] = 0; force[4] = .01;
  // 5 x splurge
  op[5] = 0; damp[5] = .9; force[5] = .01;
  op[6]=2;damp[6]=.99;force[6]=.01;
  op[7]=1;damp[7]=.99;force[7]=.01;
  // Dribble
  op[8] = 1; damp[8] = .9; force[8] = .01;
  // Slide
  op[9] = 0; damp[9] = .9; force[9] = .01;
  // xDisplace
  op[10] = 0; damp[10] = .9; force[10] = .01;
  // yDisplace
  op[11] = 0; damp[11] = .9; force[11] = .01;
  
  for (f=0;f<fs;f++) {
    var[f] = op[f];
    fon[f]=1;
  }

  for (j=1;j<=10000;j++)
  for (i=0;i<fs;i++)
    perturb(i);

  for (i=0;i<=framespermap;i++) {
    moremap();
  }
  
  // allegrosetup(scrwid,scrhei);
  // _farsetsel(screen->seg);
  
  starttimer();

}
  
//  while(!key[KEY_ESC] && !key[KEY_SPACE]) {
 
// while(!keypressed()) {


void iterate_candy(Window win,XColor xrgb[]) {

    moremap();
    
    // Animate
    for (x=0; x<scrwid; x++) {
      for (y=0; y<scrhei; y++) {
        c=0;
        for (i=0;i<=1;i++) {
          for (j=0;j<=1;j++) {
            c=c+amount[x][y][i][j][usingmap]*img.bmp[piy[x][y][usingmap]+j][pix[x][y][usingmap]+i];
          }
        }
        c=c-colspeed;
        if (c<0)
          c=0;
        img2.bmp[y][x]=c;
				// printf("%i\n",c);

				ximg->f.put_pixel(ximg,x,y,xrgb[c%colors].pixel);
				
      }
    }
    /* for (y=0;y<scrhei;y++) {
         for (x=0;x<scrwid;x++) {
           _farpokeb(screen->seg, (unsigned long)screen->line[y]+x, img2[y][x]);
         }
    }*/
/*    for (y=0; y<scrhei; y++) {
      movedata(_my_ds(), img2[y], screen->seg, bmp_write_line(screen,y), scrwid);
    }*/
	wicked_blit(win);
  
    // img2.writetoscreen();
/*    for (f=0;f<fs;f++) {
      if (fon[f]) {
        hline(screen, scrwid/2, f*2, scrwid/2+(var[f] - op[f]) * scrwid * 4, 128);
      }
    }*/
    toff=toff-(float)palspeed/256;
    for (c=0;c<=255;c++) {
      thru=saw((float)c/255-toff);
      rgb.r=huefor(thru,(float)0);
      rgb.g=huefor(thru,(float)1/3);
      rgb.b=huefor(thru,(float)2/3);
      // set_color(c,&rgb);
    }
    imgtmp=img;
    img=img2;
    img2=imgtmp;
    for (i=1;i<=2;i++) {
//      img.filledcircle(myrnd()*scrwid,myrnd()*scrhei,2+myrnd()*8,myrnd()*255);
      img.filledpolar(&heartpolarfn,myrnd()*scrwid,myrnd()*scrhei,myrnd()*20,myrnd()*2*pi,saw(toff)*255);
      img.filledpolar(&flowerpolarfn,myrnd()*scrwid,myrnd()*scrhei,myrnd()*20,myrnd()*2*pi,saw(toff+(float)2/3)*255);
      img.filledpolar(&starpolarfn,myrnd()*scrwid,myrnd()*scrhei,myrnd()*20,myrnd()*2*pi,saw(toff+(float)1/6)*255);
    }
    framedone();
  
}

void perturb (int f) {
  var[f] = (var[f] - op[f]) * damp[f] + force[f] * (myrnd() - .5) + op[f];
}

/*void mycircle(int cx,int cy,int r,int c) {
  int x,h,y,px,py;
  for (x=-r;x<=r;x++) {
    h=sqrt(mysquare(r)-mysquare(x));
    for (y=-h;y<=h;y++) {
      px=cx+x; py=cy+y;
      if (px>=0 && px<scrwid && py>=0 && py<scrhei)
        img[py][px]=myrnd()*256;
    }
  }
}*/

void moremap() {
  float rx,ry,nrx,nry,px,py;
  int f,i,j,k,c,x,y,ix,iy,displayloop;
  // Generate some more of the map
  for (maploop=1;maploop<scrwid*scrhei/framespermap;maploop++) {
    rx=(float)mmx/scrwid*2-1;
    ry=(float)(mmy-scrhei/2)/scrwid*2;
    
    /* From QB later
       IF fon%(4) THEN
       y = (y - 1) * var(4) + 1
       END IF
       IF fon%(5) THEN
       x = x + var(5) * x
       END IF
       IF fon%(7) THEN
       x = x + var(7)
       END IF
       IF fon%(8) THEN
       y = y + var(8)
       END IF
       END SUB
    */
    if (fon[8]) {
      ry=(ry-1)/var[8]+1;
    }
    if (fon[9]) {
      rx=rx+var[9]*rx;
    }
    if (fon[10]) {
      rx=rx+var[10];
    }
    if (fon[11]) {
      ry=ry+var[11];
    }
    if (fon[0]) {
      rx = mysgn(rx)/var[7]*mypow(myabs(rx),var[0]);
      ry = mysgn(ry)/var[7]*mypow(myabs(ry),var[0]);
    }
    if (fon[1]) {
      rx = rx / var[1]; ry = ry / var[1];
    }
    if (fon[2]) {
      nrx = rx * cos(var[2]) + ry * sin(var[2]);
      nry = -rx * sin(var[2]) + ry * cos(var[2]);
      rx = nrx; ry=nry;
    }
    if (fon[3]) {
      ry = ry + mysgn(ry) * sin(var[6]*pi*myabs(ry)) * var[3];
    }
    if (fon[5]) {
      rx = rx + mysgn(rx) * sin(var[6]*pi*myabs(rx)) * var[5];
    }
/*    if (fon[4]) {
      ry = ((myabs(ry) - 1) / var[4] + 1) * mysgn(ry);
    }*/
    px=(rx+1)/2*scrwid;
    py=scrhei/2+(ry)/2*scrwid;
    ix=(int)px;
    iy=(int)py;
    if (ix<0 || ix>=scrwid-1 || iy<0 || iy>=scrhei-1) {
      ix=px; iy=py;
    }
    amount[mmx][mmy][0][0][makingmap]=((float)ix+1-(float)px)*((float)(iy+1)-(float)py);
    amount[mmx][mmy][1][0][makingmap]=((float)px-(float)ix)*((float)(iy+1)-(float)py);
    amount[mmx][mmy][0][1][makingmap]=((float)ix+1-(float)px)*((float)py-(float)iy);
    amount[mmx][mmy][1][1][makingmap]=((float)px-(float)ix)*((float)py-(float)iy);
    pix[mmx][mmy][makingmap]=ix;
    piy[mmx][mmy][makingmap]=iy;
    if (ix<0 || ix>=scrwid-1 || iy<0 || iy>=scrhei-1) {
/*      pix[mmx][mmy][makingmap]=scrwid/2;
      piy[mmx][mmy][makingmap]=scrhei/2;
      for (i=0;i<=1;i++) {
        for (j=0;j<=1;j++) {
          amount[mmx][mmy][i][j][makingmap]=0;
        }
      }*/
/*      pix[mmx][mmy][makingmap]=myrnd()*(scrwid-1);
      piy[mmx][mmy][makingmap]=myrnd()*(scrhei-1);
      for (i=0;i<=1;i++) {
        for (j=0;j<=1;j++) {
          amount[mmx][mmy][i][j][makingmap]=myrnd()/4;
        }
      }*/
      pix[mmx][mmy][makingmap]=( mmx<scrwid-1 ? mmx : mmx-1 );
      piy[mmx][mmy][makingmap]=( mmy<scrhei-1 ? mmy : mmy-1 );
      amount[mmx][mmy][0][0][makingmap]=0.25;
      amount[mmx][mmy][1][0][makingmap]=0.25;
      amount[mmx][mmy][0][1][makingmap]=0.25;
      amount[mmx][mmy][1][1][makingmap]=0.25;
    }
    mmx++;
    if (mmx>=scrwid) {
      mmx=0;
      mmy++;
      if (mmy>=scrhei) {
        mmy=0;
        tmpmap=usingmap;
        usingmap=makingmap;
        makingmap=tmpmap;
        int rep=4;
        if (myrnd()<0.2)
          rep=200;
        for (int r=1;r<=rep;r++) {
        for (f=0;f<fs;f++) {
          perturb(f);
/*          if (myabs(var[f]-op[f])<force[f])
            fon[f]=( myrnd()<0.6 ? 1 : 0 );*/
        }
        }
      }
    }
  }
}


















  XEvent ev;
  XShmSegmentInfo shminfo;
  int screen, should_quit = 0;
  XVisualInfo vis, *vlist;
  int match;
  struct rusage resource_utilization;
  int shared_pixmaps;		/* dummy */

int main (void)
{
  int i;

XColor xrgb[COLORS];
  int allocateOK;
  Colormap colormap;

  ximg = NULL;

	
	init_candy();
	

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
    xrgb[i].red = 65535 * (1.0 - 1.0 * i / COLORS);
    xrgb[i].green = 65535 * (1.0 - 1.0 * i / COLORS);;
    xrgb[i].blue = 65535 * (1.0 * i / COLORS);;
    xrgb[i].flags = DoRed | DoGreen | DoBlue;
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
				xrgb[i].pixel = color[i];

      // This doesn't work for installed colormap!

      XStoreColors (d, colormap, xrgb, COLORS);

    }

		// black = XBlackPixel(d,screen);
		// white = XWhitePixel(d,screen);

  } else if ( get_class(vis) == TrueColor) {
    /* This will lookup the color and sets the xrgb[i].pixel value */
    for (i = 0; i < COLORS; i++)
      XAllocColor (d, DefaultColormap (d, screen), xrgb + i);
  } else
    fprintf (stderr, "Not content with visual class %d.\n",
	     get_class(vis) ), exit (1);

  /* Find out if MITSHM is supported and useable */
  printf ("MITSHM: ");

  if (XShmQueryVersion (d, &mitshm_major_code,
			&mitshm_minor_code, &shared_pixmaps)) {
    int (*handler) (Display *, XErrorEvent *);
    ximg = XShmCreateImage (d, vis.visual,
			   vis.depth, XShmPixmapFormat (d),
			   NULL, &shminfo, WIN_W, WIN_H);
    shminfo.shmid = shmget (IPC_PRIVATE,
			    ximg->bytes_per_line * ximg->height,
			    IPC_CREAT | 0777);
    shminfo.shmaddr = (char *)shmat (shminfo.shmid, 0, 0);
		ximg->data = (char *)shminfo.shmaddr;

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
      ximg = NULL;
    }
  }

  if (ximg == NULL) {
    can_use_mitshm = 0;
    /* XInitImage(ximg); */
    ximg = XCreateImage (d, vis.visual, vis.depth, ZPixmap,
			0, (char *)malloc (WIN_W * WIN_H), WIN_W, WIN_H, 8, 0);
  }

  if (can_use_mitshm)
    printf ("YES!\n");
  else
    printf ("NO, using fallback instead.\n");

  // DrawFractal (ximg,xrgb);

  Window win = XCreateSimpleWindow (d, DefaultRootWindow (d),
			     0, 0, WIN_W, WIN_H, 0,
			     WhitePixel (d, screen), BlackPixel (d, screen));
  XSelectInput (d, win, ButtonPressMask | ExposureMask);
  XMapWindow (d, win);

  printf ("Click in the fractal window to terminate the program.\n");

  while (!should_quit) {

	iterate_candy(win,xrgb);
	
		// for (int i=0;i<flies;i++)
			// fly[i].move();
		// DrawFractal(ximg,xrgb);
      // if (can_use_mitshm)
				// XShmPutImage (d, win, gc, ximg, 0, 0, 0, 0, WIN_W, WIN_H, True);
      // else
				// XPutImage (d, win, gc, ximg, 0, 0, 0, 0, WIN_W, WIN_H);
			// XFlush(d);
	    // XSync (d, 0); // this stopped it going mad
		// // if (col>100) {
			// // printf(".\n");
		// }
	
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
				color[i] = xrgb[i].pixel;
      XFreeColors (d, colormap, color, COLORS, 0);
    }				/* Allocated colors freed */
  } else {
    XUninstallColormap (d, colormap);
  }

  if (can_use_mitshm) {
    XShmDetach (d, &shminfo);	/* Server detached */
    XDestroyImage (ximg);	/* Image struct freed */
    shmdt (shminfo.shmaddr);	/* We're detached */
  } else
    XDestroyImage (ximg);	/* Image struct freed */

  XDestroyWindow (d, win);	/* Window removed */
  XCloseDisplay (d);		/* Display disconnected */

  /* So you can see how your computer compares to your friend's */
  getrusage (RUSAGE_SELF, &resource_utilization);
  printf ("CPU seconds consumed: %ds and %dµs\n",
	  (int) resource_utilization.ru_utime.tv_sec,
	  (int) resource_utilization.ru_utime.tv_usec);

  return 0;
}
