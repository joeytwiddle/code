#ifndef renderer_C
  #define renderer_C

  #include <renderer.h>

// ImageType and ColourType define in jfont.c

// Starts of class Scene


  // Variable declared in .h file
   Scene::Scene() {
  }
  void Scene::add(Renderable *r) {
    o.add(r);
  }
// End class 


// Starts of class Renderer


   void Renderer::renderimage(ImageType *p,Scene *s,Viewpoint *v) {
  }
// End class 


// Starts of class WireframeRenderer


   WireframeRenderer::WireframeRenderer() {
  }
   void WireframeRenderer::renderimage(ImageType *p,Scene *s,Viewpoint *v) {
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
// End class 


#endif
