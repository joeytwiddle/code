// ImageType and ColourType define in jfont.c

class Scene {
public:
  Octree<Renderable *> o;
  Scene() {
  }
  void add(Renderable *r) {
    o.add(r);
  }
};

class Renderer {
public:
  virtual void renderimage(ImageType *p,Scene *s,Viewpoint *v) {
  }
};

class WireframeRenderer : public Renderer {
public:
  WireframeRenderer() {
  }
  virtual void renderimage(ImageType *p,Scene *s,Viewpoint *v) {
    p->clear(BackgroundColour);
    PPsetup(p->width,p->height,2.0);
    #ifdef DEBUG
      printf("Getting list...\n");
    #endif
    OrderedList<Renderable *> *rs=s->o.getorderedlist(Frustrum(v->pos,v->ori,1.0,20));
    #ifdef DEBUG
      printf("Rendering...\n");
    #endif
    for (int i=1;i<=rs->len;i++) {
      #ifdef DEBUG
        printf("%i/%i\n",i,rs->len);
      #endif
      Renderable *r=rs->num(i);
      r->renderwireframe(p,v);
    }
//    destroy(rs);
  }
};
