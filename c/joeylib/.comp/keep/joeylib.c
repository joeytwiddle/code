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

#include <mem.c>

// For throwing errors
#include <error.c>

// Simple operations on strings (see also args.c and parser.c)
#include <strings.c>

// Not working yet
#ifdef USINGCURSES
  #include <jcurses.c>
#endif


// Dynamic (slow to create) static (fast to access) list (template)
#include <list.c>
// Binary ordered tree (template) for getting ordered data
#include <ordtree.c>
#include <list2.c>

#include <strings2.c>

// Lots of lovely maths functions
#include <maths.c>

// For timing frame rates
#include <timer.c>

// Reading and writing (batches of) files
#include <files.c>

// Creating and parsing HTML files
#include <htmler.c>

// RGB colour
#include <myrgb.c>

// My abstraction over Allegro
#ifdef ALLEGRO
#include <myalleg.c>
#endif

#include <vnd.c>
// 2d vectors etc
#include <v2d.c>
#include <line2d.c>
#include <polygon2d.c>
// 3d vectors, with many operations on them
#include <matrix0.c>
#include <v3d.c>
// 3d orientation (right,up,forward) with pitch,yaw,roll operations
#include <ori.c>
#include <v3dmore.c>
#include <matrix.c>
// #include <maths2.c>
#include <viewpnt.c>
#include <line3d.c>
#include <plane.c>
#include <rectangle3d.c>
// #include <plane3d.c> silly!
#include <polygon3d.c>

#include <writeable.c>

// Template maps (boxed grids).  2d allows filtering (for edges, derivatives),
// smoothing, (adaptive) thresholding, combination of maps...
#include <maps.c>
// 2d region, a boolean mask map or list of pixels
#include <region.c>
#include <map2d2.c>

// A uchar bitmap for writing to a 256 colour screen quickly
#include <jbmp.c>

// Perspective projection functions (using V3ds)
#include <pp.c>
#include <frustrum.c>

// Colour histogram
#include <rgbhist.c>
// Colour bitmap
#include <rgbmp.c>
#include <texturedrectangle3d.c>
#include <maths2.c>
// 2D line of best fit with outliers
#include <correlator2d.c>
// Bitmap read/write by Angus
#include <angus.c>

// My 2d fonts
#include <jfont.c>

// Quadtree (template)
#include <quadtree.c>

// A quadrilateral
#include <quad.c>

// Renders an unknown 3d volume where many 3d points must be inspected
#include <volrend.c>

// Octree (template) with may be clipped to a viewing frustrum
#include <octree.c>
#ifdef USINGJFONT
#include <renderable.c>
#include <renderer.c>
#endif

// For parsing command line arguments
#include <args.c>

#endif
