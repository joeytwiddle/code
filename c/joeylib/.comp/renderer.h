#ifndef renderer_H
  #define renderer_H

  #include <globals.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <stdarg.h>
  #include <math.h>
  #include <time.h>
  #include <mem.h>
  #include <error.h>
  #include <strings.h>
  #include <list.h>
  #include <ordtree.h>
  #include <list2.h>
  #include <strings2.h>
  #include <progmon.h>
  #include <maths.h>
  #include <timer.h>
  #include <files.h>
  #include <htmler.h>
  #include <myrgb.h>
  #include <myalleg.h>
  #include <vnd.h>
  #include <v2d.h>
  #include <line2d.h>
  #include <polygon2d.h>
  #include <matrix0.h>
  #include <v3d.h>
  #include <ori.h>
  #include <v3dmore.h>
  #include <matrix.h>
  #include <viewpnt.h>
  #include <line3d.h>
  #include <plane.h>
  #include <rectangle3d.h>
  #include <polygon3d.h>
  #include <writeable.h>
  #include <maps.h>
  #include <region.h>
  #include <map2d2.h>
  #include <jbmp.h>
  #include <pp.h>
  #include <frustrum.h>
  #include <rgbhist.h>
  #include <rgbmp.h>
  #include <texturedrectangle3d.h>
  #include <maths2.h>
  #include <correlator2d.h>
  #include <angus.h>
  #include <jfont.h>
  #include <quadtree.h>
  #include <quad.h>
  #include <volrend.h>
  #include <octree.h>
  #include <renderable.h>

// ImageType and ColourType define in jfont.c

class Scene {
public:
  Octree<Renderable *> o; // Exists
   Scene(); // Method

  void add(Renderable *r); // Method

};

class Renderer {
public:
  virtual void renderimage(ImageType *p,Scene *s,Viewpoint *v); // Method

};

class WireframeRenderer : public Renderer {
public:
   WireframeRenderer(); // Method

  virtual void renderimage(ImageType *p,Scene *s,Viewpoint *v); // Method

};

#endif
