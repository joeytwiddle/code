/* 
 *     XaoS, a fast portable realtime fractal zoomer 
 *                  Copyright (C) 1996,1997 by
 *
 *      Jan Hubicka          (hubicka@paru.cas.cz)
 *      Thomas Marsh         (tmarsh@austin.ibm.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 * Shamelessly ripped for use in xsynaesthesia
 */

#include "xvvis.c"

// #include "config.h"
/* Define if the X Window System is missing or not being used.  */
#undef X_DISPLAY_MISSING
/* Do we have the Simple Directmedia Layer library */
#undef HAVE_SDL
/* Define if you have the esd library (-lesd).  */
#undef HAVE_LIBESD
/* Define if you have the vga library (-lvga).  */
#undef HAVE_LIBVGA
/* Name of package */
#undef PACKAGE
/* Version number of package */
#undef VERSION

#ifndef X_DISPLAY_MISSING

#define X11_DRIVER
//#define MITSHM

#ifdef X11_DRIVER
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __FreeBSD__
#include <malloc.h>
#else
#include <stdlib.h>
#endif
#include "xlib.h"
#ifdef AMIGA
#define XFlush(x) while(0)
#endif

#undef PIXMAP

#define chkalloc(n) if (!n) fprintf(stderr, "out of memory\n"), exit(-1)

int xupdate_size (xdisplay * d)
{
  int tmp;
  Window wtmp;
  int width = d->width, height = d->height;
  XGetGeometry (d->display, d->window, &wtmp, &tmp, &tmp, &d->width,
		&d->height, (unsigned int *) &tmp, (unsigned int *) &tmp);
  if (d->width != width || d->height != height)
    return 1;
  return 0;
}

void xflip_buffers (xdisplay * d)
{
  d->back = d->vbuffs[d->current];
  d->current ^= 1;
  d->vbuff = d->vbuffs[d->current];
}

void draw_screen (xdisplay * d)
{
  switch (d->image[0]->bits_per_pixel) {
  case 16:{
      unsigned short *de;
      unsigned char *s;
      unsigned char *e;
      for (s = (unsigned char *) d->vbuffs[d->current],
	   e =
	   (unsigned char *) d->vbuffs[d->current] +
	   (d->linewidth * d->height), de =
	   (unsigned short *) d->data[d->current]; s < e; s += 8, de += 8)
	*de = d->pixels[*s], *(de + 1) = d->pixels[*(s + 1)], *(de + 2) =
	  d->pixels[*(s + 2)], *(de + 3) = d->pixels[*(s + 3)], *(de + 4) =
	  d->pixels[*(s + 4)], *(de + 5) = d->pixels[*(s + 5)], *(de + 6) =
	  d->pixels[*(s + 6)], *(de + 7) = d->pixels[*(s + 7)];
      s -= 8;
      de -= 8;
      for (; s < e; s++, de++)
	*de = d->pixels[*s];
      break;
    }
  case 24:{
      unsigned char *de;
      unsigned char *s;
      unsigned char *e;
      for (s = (unsigned char *) d->vbuffs[d->current],
	   e =
	   (unsigned char *) d->vbuffs[d->current] +
	   (d->linewidth * d->height), de =
	   (unsigned char *) d->data[d->current]; s < e; s++, de += 3)
	de[0] = d->pixels[*s], de[1] = d->pixels[*s] >> 8, de[2] =
	  d->pixels[*s] >> 16;

      break;
    }
  case 32:{
      unsigned long *de;
      unsigned char *s;
      unsigned char *e;
      for (s = (unsigned char *) d->vbuffs[d->current],
	   e =
	   (unsigned char *) d->vbuffs[d->current] +
	   (d->linewidth * d->height), de =
	   (unsigned long *) d->data[d->current]; s < e; s += 8, de += 8)
	*de = d->pixels[*s], *(de + 1) = d->pixels[*(s + 1)], *(de + 2) =
	  d->pixels[*(s + 2)], *(de + 3) = d->pixels[*(s + 3)], *(de + 4) =
	  d->pixels[*(s + 4)], *(de + 5) = d->pixels[*(s + 5)], *(de + 6) =
	  d->pixels[*(s + 6)], *(de + 7) = d->pixels[*(s + 7)];
      s -= 8;
      de -= 8;
      for (; s < e; s++, de++)
	*de = d->pixels[*s];
      break;
    }
  }
#ifdef MITSHM
  if (d->SharedMemFlag) {
    XShmPutImage (d->display, d->window, d->gc, d->image[d->current], 0, 0, 0,
		  0, d->width, d->height, True);
    XFlush (d->display);
  } else
#endif
  {
    XPutImage (d->display, d->window, d->gc, d->image[d->current], 0, 0, 0, 0,
	       d->width, d->height);
    XFlush (d->display);
  }
  d->screen_changed = 0;
}

#ifdef MITSHM
int alloc_shm_image (xdisplay * xdisp)
{
  register char *ptr;
  int temp, size = 0, i;
  ptr = DisplayString (xdisp->display);
  if (!ptr || (*ptr == ':') || !strncmp (ptr, "localhost:", 10) ||
      !strncmp (ptr, "unix:", 5) || !strncmp (ptr, "local:", 6)) {
    xdisp->SharedMemOption =
      XQueryExtension (xdisp->display, "MIT-SHM", &temp, &temp, &temp);
  } else {
    xdisp->SharedMemOption = False;
    return 0;
  }
  xdisp->SharedMemFlag = False;
#if 0
  xdisp->SharedMemOption = True;
  xdisp->SharedMemFlag = False;
#endif

  if (xdisp->SharedMemFlag) {
    XShmDetach (xdisp->display, &xdisp->xshminfo[0]);
    XShmDetach (xdisp->display, &xdisp->xshminfo[1]);
    xdisp->image[0]->data = (char *) NULL;
    xdisp->image[1]->data = (char *) NULL;
    shmdt (xdisp->xshminfo[0].shmaddr);
    shmdt (xdisp->xshminfo[1].shmaddr);
  }
  for (i = 0; i < 2; i++) {
    if (xdisp->SharedMemOption) {
      int mul;
      if (xdisp->depth == 8)
	mul = 1;
      else if (xdisp->depth <= 24)
	mul = 2;
      else
	mul = 4;
      xdisp->SharedMemFlag = False;
      xdisp->image[i] =
	XShmCreateImage (xdisp->display, xdisp->visual, xdisp->depth, ZPixmap,
			 NULL, &xdisp->xshminfo[i], xdisp->width,
			 xdisp->height * mul);
      if (xdisp->image[i]) {
	temp = xdisp->image[i]->bytes_per_line * xdisp->image[i]->height;
	xdisp->linewidth =
	  xdisp->image[i]->bytes_per_line * 8 /
	  xdisp->image[i]->bits_per_pixel;
	if (temp > size)
	  size = temp;
	xdisp->xshminfo[i].shmid =
	  shmget (IPC_PRIVATE, size, IPC_CREAT | 0777);
	if (xdisp->xshminfo[i].shmid != -1) {
	  xdisp->xshminfo[i].shmaddr =
	    (char *) shmat (xdisp->xshminfo[i].shmid, 0, 0);
	  if (xdisp->xshminfo[i].shmaddr != (char *) -1) {
	    xdisp->image[i]->data = xdisp->xshminfo[i].shmaddr;
	    xdisp->data[i] = xdisp->vbuffs[i] =
	      (char *) xdisp->image[i]->data;
	    xdisp->xshminfo[i].readOnly = True;

	    xdisp->SharedMemFlag =
	      XShmAttach (xdisp->display, &xdisp->xshminfo[i]);
	    XSync (xdisp->display, False);
	    if (!xdisp->SharedMemFlag) {
	      XDestroyImage (xdisp->image[i]);
	      xdisp->image[i] = (XImage *) NULL;
	      xdisp->SharedMemFlag = 0;
	      return 0;
	    }
	  }
	  /* Always Destroy Shared Memory Ident */
	  shmctl (xdisp->xshminfo[i].shmid, IPC_RMID, 0);
	}
	if (!xdisp->SharedMemFlag) {
	  XDestroyImage (xdisp->image[i]);
	  xdisp->image[i] = (XImage *) NULL;
	  xdisp->SharedMemFlag = 0;
	  return 0;
	}
      } else {
	xdisp->SharedMemFlag = 0;
	return 0;
      }
    } else {
      xdisp->SharedMemFlag = 0;
      return 0;
    }
  }
  xdisp->current = 0;
  xflip_buffers (xdisp);
  return 1;
}

void free_shm_image (xdisplay * d)
{
  if (d->SharedMemFlag) {
    XDestroyImage (d->image[0]);
    XDestroyImage (d->image[1]);
    XShmDetach (d->display, &d->xshminfo[0]);
    XShmDetach (d->display, &d->xshminfo[1]);
    shmdt (d->xshminfo[0].shmaddr);
    shmdt (d->xshminfo[1].shmaddr);
  }
}

#endif

int alloc_image (xdisplay * d)
{
  int i;
#ifdef MITSHM
  if (!d->params->nomitshm && alloc_shm_image (d)) {
    if (d->depth != 8) {
      for (i = 0; i < 2; i++)
	d->vbuffs[i] = malloc (d->linewidth * d->height);
    }
    return 1;
  }
#endif
  for (i = 0; i < 2; i++) {
    d->image[i] = XCreateImage (d->display, d->visual, d->depth, ZPixmap, 0,
				NULL, d->width, d->height, 8, 0);
    if (d->image[i] == NULL) {
      printf ("Out of memory for image..exiting\n");
      exit (-1);
    }
    //Add a little extra memory to catch overruns when dumping image to buffer in draw_screen
    d->image[i]->data =
      (char *) malloc (d->image[i]->bytes_per_line * d->height + 32);
    memset (d->image[i]->data, 0, d->image[i]->bytes_per_line * d->height);

    if (d->image[i]->data == NULL) {
      printf ("Out of memory for image buffers..exiting\n");
      exit (-1);
    }
    d->data[i] = d->vbuffs[i] = (char *) d->image[i]->data;
    d->linewidth =
      d->image[i]->bytes_per_line * 8 / d->image[i]->bits_per_pixel;
  }
  if (d->depth != 8) {
    for (i = 0; i < 2; i++) {
      //Add a little extra memory to catch overruns 
      //when dumping image to buffer in draw_screen
      d->vbuffs[i] = (char *) malloc (d->linewidth * d->height + 32);
      memset (d->vbuffs[i], 0, d->linewidth * d->height);

      if (d->vbuffs[i] == NULL) {
	printf ("Out of memory for image buffers2..exiting\n");
	exit (-1);
      }
    }
  }
  xflip_buffers (d);
  return 1;
}

void free_image (xdisplay * d)
{
  if (d->depth != 8)
    free (d->vbuffs[0]), free (d->vbuffs[1]);
#ifdef MITSHM
  if (d->SharedMemFlag) {
    free_shm_image (d);
    return;
  }
#endif
  XDestroyImage (d->image[0]);
  XDestroyImage (d->image[1]);
}

#define MAX(x,y) ((x)>(y)?(x):(y))

xdisplay *xalloc_display (char *s, int xHint, int yHint, int x, int y,
			  xlibparam * params)
{
  xdisplay *xdisp;
  Visual *defaultvisual;
  XVisualInfo vis;

  xdisp = (xdisplay *) calloc (sizeof (xdisplay), 1);
  chkalloc (xdisp);
  xdisp->display = XOpenDisplay ((char *) NULL);
  if (!xdisp->display) {
    free ((void *) xdisp);
    return NULL;
  }
  xdisp->screen = DefaultScreen (xdisp->display);
  xdisp->attributes = (XSetWindowAttributes *)
    malloc (sizeof (XSetWindowAttributes));
  chkalloc (xdisp->attributes);
  xdisp->attributes->background_pixel = BlackPixel (xdisp->display,
						    xdisp->screen);
  xdisp->attributes->border_pixel =
    BlackPixel (xdisp->display, xdisp->screen);
  xdisp->attributes->event_mask =
    ButtonPressMask | StructureNotifyMask | ButtonReleaseMask |
    ButtonMotionMask | KeyPressMask | ExposureMask | KeyReleaseMask;
  xdisp->attributes->override_redirect = False;
  xdisp->attr_mask = CWBackPixel | CWBorderPixel | CWEventMask;
  xdisp->visclassX = InputOutput;
  xdisp->xcolor.n = 0;
  xdisp->parent_window = RootWindow (xdisp->display, xdisp->screen);
  defaultvisual = DefaultVisual (xdisp->display, xdisp->screen);
  xdisp->params = params;
  if (!params->usedefault) {
    if (get_visual_class (defaultvisual) != PseudoColor
	||
	(!XMatchVisualInfo
	 (xdisp->display, xdisp->screen, 8, PseudoColor, &vis)
	 && vis.colormap_size > 128)) {
      xdisp->fixedcolormap = 1;
      if (!XMatchVisualInfo
	  (xdisp->display, xdisp->screen, 15, TrueColor, &vis)) {
	if (!XMatchVisualInfo
	    (xdisp->display, xdisp->screen, 16, TrueColor, &vis)) {
	  if (!XMatchVisualInfo
	      (xdisp->display, xdisp->screen, 32, TrueColor, &vis)
	      && !XMatchVisualInfo (xdisp->display, xdisp->screen, 24,
				    TrueColor, &vis)) {
	    if (!XMatchVisualInfo
		(xdisp->display, xdisp->screen, 8, PseudoColor, &vis)
		&& !XMatchVisualInfo (xdisp->display, xdisp->screen, 7,
				      PseudoColor, &vis)) {
	      if (!XMatchVisualInfo
		  (xdisp->display, xdisp->screen, 8, TrueColor, &vis)
		  && !XMatchVisualInfo (xdisp->display, xdisp->screen, 8,
					StaticColor, &vis)
		  && !XMatchVisualInfo (xdisp->display, xdisp->screen, 8,
					StaticGray, &vis)) {
		printf
		  ("Display does not support PseudoColor depth 7,8,StaticColor depth 8, StaticGray depth 8, Truecolor depth 8,15,16,24 nor 32!\n");
		return NULL;
	      } else
		xdisp->truecolor = 1;
	    } else
	      xdisp->fixedcolormap = 0, xdisp->truecolor = 0;
	  } else
	    xdisp->truecolor = 1;
	} else
	  xdisp->truecolor = 1;
      } else
	xdisp->truecolor = 1;
    } else {
      xdisp->truecolor = 0;
    }
    xdisp->depth = vis.depth;
    xdisp->visual = vis.visual;
  } else {			/*usedefault */
    vis.depth = xdisp->depth = DefaultDepth (xdisp->display, xdisp->screen);
    xdisp->visual = defaultvisual;
    switch (get_visual_class (defaultvisual)) {
    case PseudoColor:
      if (xdisp->depth <= 8) {
	xdisp->depth = 8;
	xdisp->truecolor = 0;
	xdisp->fixedcolormap = 0;
      } else {
	printf ("Pseudocolor visual on unsuported depth\n");
	return NULL;
      }
      break;
    case TrueColor:
    case StaticColor:
    case StaticGray:
      xdisp->truecolor = 1;
      xdisp->fixedcolormap = 1;
      if (xdisp->depth <= 8)
	xdisp->depth = 8;
      else if (xdisp->depth <= 16)
	xdisp->depth = 16;
      else if (xdisp->depth <= 32)
	xdisp->depth = 32;
      else {
	printf ("Truecolor visual on unsuported depth\n");
	return NULL;
      }
      break;
    default:
      printf ("Unusuported visual\n");
      break;
    }
  }
  /*xdisp->visual->map_entries = 256; */
  xdisp->colormap = xdisp->defaultcolormap =
    DefaultColormap (xdisp->display, xdisp->screen);

  xdisp->window_name = s;
  xdisp->height = y;
  xdisp->width = x;
  xdisp->border_width = 2;
  xdisp->lastx = 0;
  xdisp->lasty = 0;
  xdisp->font_struct = (XFontStruct *) NULL;

  xdisp->window =
    XCreateWindow (xdisp->display, xdisp->parent_window, xHint, yHint,
		   xdisp->width, xdisp->height, xdisp->border_width,
		   vis.depth, xdisp->visclassX, xdisp->visual,
		   xdisp->attr_mask, xdisp->attributes);
  if (!xdisp->fixedcolormap && params->privatecolormap) {
    unsigned long pixels[256];
    int i;
    xdisp->colormap =
      XCreateColormap (xdisp->display, xdisp->window, xdisp->visual,
		       AllocNone);
    XAllocColorCells (xdisp->display, xdisp->colormap, 1, 0, 0, pixels
		      MAX (xdisp->visual->map_entries, 256));
    for (i = 0; i < 16; i++) {
      xdisp->xcolor.c[i].pixel = pixels[i];
    }
    XQueryColors (xdisp->display, xdisp->defaultcolormap, xdisp->xcolor.c,
		  16);
    XStoreColors (xdisp->display, xdisp->colormap, xdisp->xcolor.c, 16);
    xdisp->privatecolormap = 1;
  }
  if (!xdisp->fixedcolormap)
    XSetWindowColormap (xdisp->display, xdisp->window, xdisp->colormap);
  xdisp->gc =
    XCreateGC (xdisp->display, xdisp->window, 0L, &(xdisp->xgcvalues));
  XSetBackground (xdisp->display, xdisp->gc,
		  BlackPixel (xdisp->display, xdisp->screen));
  XSetForeground (xdisp->display, xdisp->gc,
		  WhitePixel (xdisp->display, xdisp->screen));
  XStoreName (xdisp->display, xdisp->window, xdisp->window_name);
  XMapWindow (xdisp->display, xdisp->window);
#if 1
  XSelectInput (xdisp->display, xdisp->window,
		//ExposureMask | 
		KeyPress |
		//KeyRelease | 
		//ConfigureRequest | 
		//FocusChangeMask | 
		StructureNotifyMask | ButtonPressMask | ButtonReleaseMask);
#endif
#ifdef PIXAMP
  xdisp->pixmap = XCreatePixmap (xdisp->display, xdisp->window, xdisp->width,
				 xdisp->height, xdisp->depth);
#endif

  {
    XColor c;
    Pixmap p = XCreatePixmap (xdisp->display, xdisp->window, 1, 1, 1);
    memset (&c, 0, sizeof (c));
    xdisp->cursor = XCreatePixmapCursor (xdisp->display, p, p, &c, &c, 0, 0);
    XDefineCursor (xdisp->display, xdisp->window, xdisp->cursor);
    XFreePixmap (xdisp->display, p);
  }

  return (xdisp);
}

void xsetcolor (xdisplay * d, int col)
{
  switch (col) {
  case 0:
    XSetForeground (d->display, d->gc, BlackPixel (d->display, d->screen));
    break;
  case 1:
    XSetForeground (d->display, d->gc, WhitePixel (d->display, d->screen));
    break;
  default:
    if ((col - 2) > d->xcolor.n) {
      fprintf (stderr, "color error\n");
      exit (-1);
    }
    XSetForeground (d->display, d->gc, d->xcolor.c[col - 2].pixel);
    break;
  }
}
void xrotate_palette (xdisplay * d, int direction, unsigned char co[3][256],
		      int ncolors)
{
  int i, p;

  if (d->privatecolormap) {
    for (i = 0; i < d->xcolor.n; i++) {
      p = d->xcolor.c[i].pixel;
      d->xcolor.c[i].red = (int) co[0][p] * 256;
      d->xcolor.c[i].green = (int) co[1][p] * 256;
      d->xcolor.c[i].blue = (int) co[2][p] * 256;
    }
    XStoreColors (d->display, d->colormap, d->xcolor.c, d->xcolor.n);
  }
  if (d->truecolor) {
    unsigned long oldpixels[256];
    memcpy (oldpixels, d->pixels, sizeof (oldpixels));
    p = (ncolors - 1 + direction) % (ncolors - 1) + 1;
    for (i = 1; i < ncolors; i++) {	/*this is ugly..I know */
      d->pixels[i] = oldpixels[p];
      p++;
      if (p >= ncolors)
	p = 1;
    }
    draw_screen (d);
  }
}
int xalloc_color (xdisplay * d, int r, int g, int b, int readwrite)
{

				printf("!\n");
  d->xcolor.n++;
  d->xcolor.c[d->xcolor.n - 1].flags = DoRed | DoGreen | DoBlue;
  d->xcolor.c[d->xcolor.n - 1].red = r;
  d->xcolor.c[d->xcolor.n - 1].green = g;
  d->xcolor.c[d->xcolor.n - 1].blue = b;
  d->xcolor.c[d->xcolor.n - 1].pixel = d->xcolor.n - 1;
  if ((readwrite && !d->fixedcolormap) || d->privatecolormap) {
					printf("A\n");
    unsigned long cell;
    if (d->privatecolormap) {
      cell = d->xcolor.c[d->xcolor.n - 1].pixel += 16;
      if (d->xcolor.c[d->xcolor.n - 1].pixel >= d->visual->map_entries) {
	d->xcolor.n--;
	return (-1);
      }
    } else {
      if (!XAllocColorCells (d->display, d->colormap, 0, 0, 0, &cell, 1)) {
	d->xcolor.n--;
					printf("B\n");
	if (d->xcolor.n <= 32)
	  printf
	    ("Colormap is too full! close some colorfull aplications or use -private\n");
	return (-1);
      }
      d->xcolor.c[d->xcolor.n - 1].pixel = cell;
    }
    XStoreColor (d->display, d->colormap, &(d->xcolor.c[d->xcolor.n - 1]));
    return (cell);
  }
  if (!XAllocColor (d->display, d->colormap, &(d->xcolor.c[d->xcolor.n - 1]))) {
    d->xcolor.n--;
					printf("C\n");
    if (d->xcolor.n <= 32)
      printf
	("Colormap is too full! close some colorfull aplications or use -private\n");
    return (-1);
  }
  d->pixels[d->xcolor.n - 1] = d->xcolor.c[d->xcolor.n - 1].pixel;
  return (d->depth !=
	  8 ? d->xcolor.n - 1 : d->xcolor.c[d->xcolor.n - 1].pixel);
}

void xfree_colors (xdisplay * d)
{
  unsigned long pixels[256];
  int i;
  for (i = 0; i < d->xcolor.n; i++)
    pixels[i] = d->xcolor.c[i].pixel;
  if (!d->privatecolormap)
    XFreeColors (d->display, d->colormap, pixels, d->xcolor.n, 0);
  d->xcolor.n = 0;
}

void xfree_display (xdisplay * d)
{
  XSync (d->display, 0);
  if (d->font_struct != (XFontStruct *) NULL) {
    XFreeFont (d->display, d->font_struct);
  }
  XUnmapWindow (d->display, d->window);
#ifdef PIXMAP
  XFreePixmap (d->display, d->pixmap);
#endif
  XDestroyWindow (d->display, d->window);
  XFreeCursor (d->display, d->cursor);
  XCloseDisplay (d->display);
  free ((void *) d->attributes);
  free ((void *) d);
}

#ifdef PIXMAP
void xline (xdisplay * d, int x1, int y1, int x2, int y2)
{
  XDrawLine (d->display, d->pixmap, d->gc, x1, y1, x2, y2);
  d->lastx = x2, d->lasty = y2;
  d->screen_changed = 1;
} void xlineto (xdisplay * d, int x, int y)
{

  XDrawLine (d->display, d->pixmap, d->gc, d->lastx, d->lasty, x, y);
  d->lastx = x, d->lasty = y;
  d->screen_changed = 1;
} void xrect (xdisplay * d, int x1, int y1, int x2, int y2)
{

  XDrawRectangle (d->display, d->pixmap, d->gc, x1, y1, (x2 - x1), (y2 - y1));
  d->lastx = x2, d->lasty = y2;
  d->screen_changed = 1;
} void xfillrect (xdisplay * d, int x1, int y1, int x2, int y2)
{

  XFillRectangle (d->display, d->pixmap, d->gc, x1, y1, (x2 - x1), (y2 - y1));
  d->lastx = x2, d->lasty = y2;
  d->screen_changed = 1;
} void xpoint (xdisplay * d, int x, int y)
{

  XDrawPoint (d->display, d->pixmap, d->gc, x, y);
  d->lastx = x, d->lasty = y;
  d->screen_changed = 1;
} void xflush (xdisplay * d)
{

  draw_screen (d);
  XFlush (d->display);
}

void xclear_screen (xdisplay * d)
{
  xfillrect (d, 0, 0, d->width, d->height);
  d->screen_changed = 1;
}

#endif
void xmoveto (xdisplay * d, int x, int y)
{
  d->lastx = x, d->lasty = y;
} int xsetfont (xdisplay * d, char *font_name)
{

  if (d->font_struct != (XFontStruct *) NULL) {
    XFreeFont (d->display, d->font_struct);
  }
  d->font_struct = XLoadQueryFont (d->display, font_name);
  if (!d->font_struct) {
    fprintf (stderr, "could not load font: %s\n", font_name);
    exit (-1);
  }
  return (d->font_struct->max_bounds.ascent +
	  d->font_struct->max_bounds.descent);
}

void xouttext (xdisplay * d, char *string)
{
  int sz;

  sz = strlen (string);
  XDrawImageString (d->display, d->window, d->gc, d->lastx, d->lasty,
		    string, sz);
#if 0
  d->lastx += XTextWidth (d->font_struct, string, sz);
  d->screen_changed = 1;
#endif
} void xresize (xdisplay * d, XEvent * ev)
{

#ifdef PIXMAP
  XFreePixmap (d->display, d->pixmap);
#endif
  d->width = ev->xconfigure.width;
  d->height = ev->xconfigure.height;
#ifdef PIXMAP
  d->pixmap = XCreatePixmap (d->display, d->window, d->width,
			     d->height, d->depth);
#endif
}

#ifdef PIXMAP
void xarc (xdisplay * d, int x, int y, unsigned int w,
	   unsigned int h, int a1, int a2)
{
  XDrawArc (d->display, d->pixmap, d->gc, x, y, w, h, a1, a2);
} void xfillarc (xdisplay * d, int x, int y, unsigned int w,
		 unsigned int h, int a1, int a2)
{
  XFillArc (d->display, d->pixmap, d->gc, x, y, w, h, a1, a2);
}
#endif

void xsize_set (xdisplay * d, int width, int height)
{
  XResizeWindow (d->display, d->window, width, height);
}

int xmouse_x (xdisplay * d)
{

  return d->mouse_x;
}

int xmouse_y (xdisplay * d)
{
  return d->mouse_y;
}

void xmouse_update (xdisplay * d)
{
  Window rootreturn, childreturn;
  int rootx = 0, rooty = 0, buttons = 0;

  XEvent event;

  if (XCheckMaskEvent
      (d->display, ButtonPressMask | ButtonReleaseMask, &event)) {
    if (event.type == ButtonPress)
      d->mouse_buttons |= 1 << ((XButtonEvent *) (&event))->button;
    else
      d->mouse_buttons &= ~(1 << ((XButtonEvent *) (&event))->button);
  }

  XQueryPointer (d->display, d->window, &rootreturn, &childreturn,
		 &rootx, &rooty, &(d->mouse_x), &(d->mouse_y),
		 (unsigned int *) &buttons);
}

char xkeyboard_query (xdisplay * d)
{
  XEvent event;

  if (XCheckMaskEvent (d->display, KeyPressMask | KeyReleaseMask, &event)) {
    char *str =
      XKeysymToString (XLookupKeysym ((XKeyPressedEvent *) (&event), 0));

    if (((XKeyPressedEvent *) (&event))->state &
	(ControlMask | Mod1Mask | Mod2Mask | Mod3Mask | Mod4Mask | Mod5Mask))
      return 0;

    if (str) {
      char key;

      if (strlen (str) == 1)
	key = str[0];
      else if (strcmp (str, "equal") == 0)
	key = '=';
      else if (strcmp (str, "minus") == 0)
	key = '-';
      else if (strcmp (str, "bracketleft") == 0)
	key = '[';
      else if (strcmp (str, "bracketright") == 0)
	key = ']';
      else if (strcmp (str, "comma") == 0)
	key = ',';
      else if (strcmp (str, "period") == 0)
	key = '.';
      else if (strcmp (str, "slash") == 0)
	key = '/';
      else
	return 0;

      if (((XKeyPressedEvent *) (&event))->state & ShiftMask)
	switch (key) {
	case '=':
	  key = '+';
	  break;
	case '[':
	  key = '{';
	  break;
	case ']':
	  key = '}';
	  break;
	case ',':
	  key = '<';
	  break;
	case '/':
	  key = '?';
	  break;
	default:
	  if (key >= 'a' && key <= 'z')
	    key = key + 'A' - 'a';
	  break;
	}
      return key;
    }
  }

  return 0;
}

int xsize_update (xdisplay * d, int *width, int *height)
{
  XEvent event;

  if (XCheckMaskEvent (d->display, StructureNotifyMask, &event)) {
    if (event.type == ConfigureNotify) {
      xupdate_size (d);
      free_image (d);
      alloc_image (d);
      *width = d->linewidth;
      *height = d->height;
      return 1;
    }
  }

  return 0;
}

unsigned int xmouse_buttons (xdisplay * d)
{
  return d->mouse_buttons;
}
#endif

#endif
