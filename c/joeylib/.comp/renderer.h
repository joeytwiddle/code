#ifndef renderer_H
  #define renderer_H

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
