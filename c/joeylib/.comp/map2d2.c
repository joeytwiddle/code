#ifndef map2d2_C
  #define map2d2_C

  #include <map2d2.h>

template <class Object>
void Map2d<Object>::realsearch(Map2d<bool> *togo,Region *r,List<Pixel> *s) {
  // myerror("No method defined for Map2d<Object>::search()\n");
  while (s->len>0) {
    Pixel v=s->num(1);
    int i=v.x;
    int j=v.y;
    s->removenum(1);
    if (togo->inmap(i,j))
      if (togo->pos[i][j]) {
      togo->pos[i][j]=false;
      r->add(Pixel(i,j));
      s->add(Pixel(i-1,j));
      s->add(Pixel(i+1,j));
      s->add(Pixel(i,j-1));
      s->add(Pixel(i,j+1));
    }
  }
}

template <class Object>
List<Region *> *Map2d<Object>::getrealregions(bool oftype) {
  List<Region *> *rs=new List<Region *>();
  Map2d<bool> togo=*this->threshold(0.5);
  if (oftype==false)
  	togo.invert();
  for (int i=0;i<width;i++)
  for (int j=0;j<height;j++) {
    if (togo.pos[i][j]) {
      Region *n=new Region(width,height);
      List<Pixel> s=List<Pixel>();
      s.add(Pixel(i,j));
      realsearch(&togo,n,&s);
      rs->add(n);
//      printf("Region created (%i).\n",n->list->len);
    }
  }
  togo.freedom();
  return rs;
  // myerror("No method defined for Map2d<Object>::getregions()\n");
}

template <class Object>
List<Region *> *Map2d<Object>::getrealregions() {
	return getrealregions(true);
}

/* Inefficient
class RErec {
public:
  int col;
  List<int> equiv;
  RErec(int c) {
    col=c;
  }
  bool inlist(int x) {
    for (int i=1;i<=equiv.len;i++)
      if (equiv.num(i)==x)
        return true;
    return false;
  }
  int findequiv(List<Pixel> *e,int f) {
    for (int i=1;i<=e->len;i++) {
      Pixel p=e->num(i);
      if (p.x==f && !inlist(p.y)) {
        e->removenum(i);
        return p.y;
      }
      if (p.y==f && !inlist(p.x)) {
        e->removenum(i);
        return p.x;
      }
    }
    return -1;
  }
  void collect(List<Pixel> *e,List<int> *todo) {
    bool doneall=false;
    while (!doneall) {
      doneall=true;
      for (int i=1;(i<=equiv.len) && doneall;i++) {
        int c=equiv.num(i);
        // Could this class be equivalent?
        int t=findequiv(e,c);
        if (t>0) {
          doneall=false;
          equiv.add(t);
          todo->remove(t);
        }
      }
    }
  }
};

template <class Object>
Map2d<int> Map2d<Object>::getregionmap(Object oftype) {
  Map2d<int> n=Map2d<int>(width,height,0);
  List<Pixel> equiv; // Spread out equivalence
  int made=0;
//printf("A\n");
  printf("First scan\n");
  // Find all regions and make connection between joining regions
  for (int i=0;i<width;i++)
  for (int j=0;j<height;j++) {
    if (getpos(i,j)==oftype) {
      int l=n.getpos(i-1,j);
      int t=n.getpos(i,j-1);
      if (l>0 || t>0) {
        if (l>0 && t>0) {
          n.setpos(i,j,l);
          equiv.add(Pixel(t,l));
        } else if (l>0) {
          n.setpos(i,j,l);
        } else if (t>0) {
          n.setpos(i,j,t);
        }
      } else {
        made++;
        n.setpos(i,j,made);
      }
    }
  }
//printf("B\n");
  // Combine joined regions into one
  printf("Find equivalence\n");
  List<int> todo;
  for (int i=1;i<=made;i++)
    todo.add(i);
  List<RErec> realequiv;
  while (todo.len>0) {
    RErec r=RErec(realequiv.len+1);
    r.equiv.add(todo.pop(todo.len));
    r.collect(&equiv,&todo);
//    if (myrnd()<0.5)
    realequiv.add(r);
  }
  printf("Second scan\n");
  // Redraw map
  for (int i=0;i<width;i++)
  for (int j=0;j<height;j++) {
    int c=n.getpos(i,j);
    if (c>0) {
      // Which class is it?
      int found=-1;
      for (int k=1;(k<=realequiv.len) && found==-1;k++) {
        if (realequiv.num(k).inlist(c)) {
          found=k;
        }
      }
//      if (found>0)
      n.setpos(i,j,realequiv.num(found).col);
      if (found!=realequiv.num(found).col)
        error(Sformat("%i /= %i !\n",found,realequiv.num(found).col));
    }
  }
  return n;
}
*/



// Efficient
// Starts of class RErec


  // Variable declared in .h file
  // Variable declared in .h file
   RErec::RErec(int c) {
    col=c;
  }
  bool RErec::inlist(int x) {
    for (int i=1;i<=equiv.len;i++)
      if (equiv.num(i)==x)
        return true;
    return false;
  }
  int RErec::findequiv(List<List<int> > *e,int f) {
    List<int> *ls=e->p2num(f);
    if (ls->len>0) {
      return ls->pop();
    }
    return -1;
  }
  void RErec::collect(List<List<int> > *e,List<int> *todo) {
    bool doneall=false;
    while (!doneall) {
      doneall=true;
      for (int i=1;(i<=equiv.len) && doneall;i++) {
        int c=equiv.num(i);
        // Could this class be equivalent?
        int t=findequiv(e,c);
        if (t>0) {
          doneall=false;
          equiv.addifnot(t);
          todo->remove(t);
        }
      }
    }
  }
  void RErec::freedom() {
    equiv.freedom();
  }
// End class 


template <class Object>
Map2d<int> Map2d<Object>::getregionmap(Object oftype) {
  Map2d<int> n=Map2d<int>(width,height,0);
  List<List<int> > equiv;
  int made=0;

  printf("First scan\n");
  // Find all regions and make connection between joining regions
  for (int i=0;i<width;i++)
  for (int j=0;j<height;j++) {
    if (getpos(i,j)==oftype) {
      int l=n.getpos(i-1,j);
      int t=n.getpos(i,j-1);
      if (l>0 || t>0) {
        if (l>0 && t>0) {
          n.setpos(i,j,l);
          equiv.p2num(l)->addifnot(t);
          equiv.p2num(t)->addifnot(l);
        } else if (l>0) {
          n.setpos(i,j,l);
        } else if (t>0) {
          n.setpos(i,j,t);
        }
      } else {
        made++;
        n.setpos(i,j,made);
        equiv.add(List<int>());
      }
    }
  }
//printf("B\n");

  // Combine joined regions into one
  printf("Find equivalence between %i\n",made);
  List<int> todo;
  for (int i=1;i<=made;i++)
    todo.add(i);
  List<RErec> realequiv;
  while (todo.len>0) {
    RErec r=RErec(realequiv.len+1);
    r.equiv.add(todo.pop());
    r.collect(&equiv,&todo);
    realequiv.add(r);
  }

  printf("Point original into %i\n",realequiv.len);
  List<int> changeto;
  for (int i=1;i<=made;i++)
    changeto.add(i);
  for (int i=1;i<=realequiv.len;i++) {
    List<int> *e=&realequiv.num(i).equiv;
    for (int j=1;j<=e->len;j++)
      changeto.setnum(e->num(j),i);
  }

  printf("Second scan\n");
  // Redraw map
  for (int i=0;i<width;i++)
  for (int j=0;j<height;j++) {
    int c=n.getpos(i,j);
    if (c>0) {
      // Which class is it?
/*      int found=-1;
      for (int k=1;(k<=realequiv.len) && found==-1;k++) {
        if (realequiv.num(k).inlist(c)) {
          found=k;
        }
      }
//      if (found>0)*/
      int found=changeto.num(c);
//      printf("%i -> %i\n",c,found);
      n.setpos(i,j,realequiv.num(found).col);
      if (found!=realequiv.num(found).col)
        error(Sformat("%i /= %i !\n",found,realequiv.num(found).col));
    }
  }
  equiv.liberateall();
  equiv.freedom();
  realequiv.liberateall();
  realequiv.freedom();
  todo.freedom();
  changeto.freedom();
  return n;
}

template <class Object>
List<Region> Map2d<Object>::collectregions() {
  int numrs=largest();
  List<Region> rs=List<Region>(numrs);
  for (int i=1;i<=numrs;i++)
    rs.add(Region(width,height));
  for (int i=0;i<width;i++)
  for (int j=0;j<height;j++)
    if (getpos(i,j)>0)
      rs.p2num(getpos(i,j))->list->add(Pixel(i,j));
  return rs;
}
#endif
