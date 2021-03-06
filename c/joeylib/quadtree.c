#define Quadtreemaxobjs 10

template<class Object>
  class Inst {
  public:
    int x,y;
    Object *o;
    Inst(int a,int b,Object *p) {
      x=a; y=b; o=p;
    }
  };

template<class Object>
class Quadtree {
public:
  bool leaf;
  int cx,cy,size;
  List<Inst<Object>*> *num;
  Quadtree *root,nw,ne,sw,se;
  Quadtree(int a,int b,int s) {
    Quadtree(a,b,s,this);
  }
  Quadtree(int a,int b,int s,Quadtree *r) {
    cx=a; cy=b; size=s;
    num=new List<Inst<Object>*>();
    nw=ne=sw=se=NULL;
    root=r;
    leaf=true;
  }
  void add(int x,int y,Object *o) {
    add(new Inst<Object>(x,y,o));
  }
  void add(Inst<Object> *i) {
    if (!leaf) {
      if (i->x<cx) {
        if (i->y<cy)
          nw.add(i);
        else
        sw.add(i);
      } else {
        if (i->y<cy)
          ne.add(i);
        else
        se.add(i);
      }
    } else {
      num->add(i);
      if (num->len>Quadtreemaxobjs && size>4) {
        nw=Quadtree(cx-size/2,cy-size/2,size/2,root);
        sw=Quadtree(cx-size/2,cy+size/2,size/2,root);
        ne=Quadtree(cx+size/2,cy-size/2,size/2,root);
        se=Quadtree(cx+size/2,cy+size/2,size/2,root);
        leaf=true;
        add(num);
        num=NULL;
      }
    }
  }
  void add(List<Inst<Object>*> l) {
    for (int i=1;i<=l->len;i++) {
      Inst<Object> *in=l->num(i);
      add(in);
    }
  }
};

