#ifndef joeylib_H
  #define joeylib_H

// joeylib

//


// This library is in a constant state of (r)evolution and is a right old mess!

// Which operating system are we under?

// #include <whichos.c>
// #include "/whatos"

// #define LINUX
// #define DOS
#define UNIX

// For SG machines, without time and difftime functions:
// #define STUPIDUNIX

// Anything special for this particular compile?
// Don't use the graphics library
// #define NOGRAPHICS
// Output debugging info
// #define DEBUG
// Don't print usual information about what's happening
// #define QUIET
// Loads a global JFont object
// #define USINGJFONT
// Force segmentation fault when error occurs - program should stop running
// #define FORCESEGFAULT
// My curses library (probably not working anyway)
// #define USINGCURSES

// Are we using the Allegro library for DJGPP (graphics under DOS)?
#ifdef DOS
 #define ALLEGRO
#endif

#ifdef UNIX
  #define POSIX
#endif
#ifdef LINUX
  #define POSIX
#endif

// I needed these for porting - maybe still do!
#ifdef LINUX
  #define static const static
#endif
#ifdef UNIX
  #define fillin 
#else
  #define fillin static
#endif
#define fillup static
#define fillin static
// Used to be static!

#define uchar unsigned char
#define bool uchar
#define true (bool)1
#define false (bool)0
#define boolean bool
#define destroy(i) if (i!=NULL) { (i)->freedom(); free(i); i=NULL; }
#define destroystruct(i) if (i!=NULL) { (i)->freestruct(); free(i); i=NULL; }

#ifdef ALLEGRO
  #include <allegro.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#ifdef DOS
  #include <dir.h>
#endif

void helloworld(); // Method


// #include <query.c>

#include <mem.h>

// For throwing errors
#include <error.h>

// Simple operations on strings (see also args.c and parser.c)
#include <strings.h>

// Not working yet
#ifdef USINGCURSES
  #include <jcurses.h>
#endif


// Dynamic (slow to create) static (fast to access) list (template)
#include <list.h>
// Binary ordered tree (template) for getting ordered data
#include <ordtree.h>
#include <list2.h>

#include <strings2.h>

// Lots of lovely maths functions
#include <maths.h>

// For timing frame rates
#include <timer.h>

// Reading and writing (batches of) files
#include <files.h>

// Creating and parsing HTML files
#include <htmler.h>

// RGB colour
#include <myrgb.h>

// My abstraction over Allegro
#ifdef ALLEGRO
#include <myalleg.h>
#endif

#include <vnd.h>
// 2d vectors etc
#include <v2d.h>
#include <line2d.h>
#include <polygon2d.h>
// 3d vectors, with many operations on them
#include <matrix0.h>
#include <v3d.h>
// 3d orientation (right,up,forward) with pitch,yaw,roll operations
#include <ori.h>
#include <v3dmore.h>
#include <matrix.h>
// #include <maths2.h>
#include <viewpnt.h>
#include <line3d.h>
#include <plane.h>
#include <rectangle3d.h>
// #include <plane3d.h> silly!
#include <polygon3d.h>

#include <writeable.h>

// Template maps (boxed grids).  2d allows filtering (for edges, derivatives),
// smoothing, (adaptive) thresholding, combination of maps...
#include <maps.h>
// 2d region, a boolean mask map or list of pixels
#include <region.h>
#include <map2d2.h>

// A uchar bitmap for writing to a 256 colour screen quickly
#include <jbmp.h>

// Perspective projection functions (using V3ds)
#include <pp.h>
#include <frustrum.h>

// Colour histogram
#include <rgbhist.h>
// Colour bitmap
class RGBmp;
#include <writeppm.h>
#include <rgbmp.h>
#include <texturedrectangle3d.h>
#include <maths2.h>
// 2D line of best fit with outliers
#include <correlator2d.h>
// Bitmap read/write by Angus
#include <angus.h>

// My 2d fonts
#include <jfont.h>

// Quadtree (template)
#include <quadtree.h>

// A quadrilateral
#include <quad.h>

// Renders an unknown 3d volume where many 3d points must be inspected
#include <volrend.h>

// Octree (template) with may be clipped to a viewing frustrum
#include <octree.h>
#ifdef USINGJFONT
#include <renderable.h>
#include <renderer.h>
#endif

// For parsing command line arguments
#include <args.h>

#endif
