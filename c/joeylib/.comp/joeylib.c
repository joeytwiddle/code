#ifndef joeylib_C
  #define joeylib_C

  #include <joeylib.h>

// joeylib

/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */

// This library is in a constant state of (r)evolution and is a right old mess!

// Are we using the Allegro library for DJGPP (graphics under DOS)?
#ifdef DOS
 // #define ALLEGRO
#endif

#include <globals.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

void helloworld() {
	printf("Hello world! =)\n");
}

// #include <query.c>

#include <mem.c>

// For throwing errors
#include <error.c>

// Simple operations on strings (see also args.c and parser.c)
#include <strings.c>

// Not working yet
#ifdef USINGCURSES
  // #include <jcurses.c>
#endif

// Dynamic (slow to create) static (fast to access) list (template)
#include <list.c>
// Binary ordered tree (template) for getting ordered data
#include <ordtree.c>
#include <list2.c>

#include <strings2.c>

#include <progmon.c>

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
// #include <writeppm.c>
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

#include <fixers.c>

#endif
