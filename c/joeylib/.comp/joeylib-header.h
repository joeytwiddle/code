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

void helloworld(); // Method


// #include <query.c>

#include <mem.h>

// For throwing errors
#include <error.h>

// Simple operations on strings (see also args.c and parser.c)
#include <strings.h>

// Not working yet
#ifdef USINGCURSES
  // #include <jcurses.c>
#endif

// Dynamic (slow to create) static (fast to access) list (template)
#include <list.h>
// Binary ordered tree (template) for getting ordered data
#include <ordtree.h>
#include <list2.h>

#include <strings2.h>

#include <progmon.h>

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
// #include <maths2.c>
#include <viewpnt.h>
#include <line3d.h>
#include <plane.h>
#include <rectangle3d.h>
// #include <plane3d.c> silly!
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
// #include <writeppm.c>
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

#include <fixers.h>
