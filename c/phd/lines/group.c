class Group {
public:
  List<Pixel> ps;
  virtual void freedom() {
    ps.freedom();
  }
};

class OnePixel : public Group {
public:
  OnePixel(Pixel p) {
    ps.add(p);
 	}
 	Pixel pixel() {
 	  return ps.num(1);
 	}
};

class Grouped : public Group {
public:
  List<Group *> gs;
  Grouped() {
  }
  void update() {
    ps.clear();
    for (int i=1;i<=gs.len;i++)
      ps.add(gs.num(i)->ps);
	}
	virtual void freedom() {
	  gs.destroyall();
	  ps.freedom();
	}
};

void group(Map2d<bool> *bin) {
  List<Region *> rs=*bin->getrealregions();
  List<Group *> gs;
  for (int i=1;i<=rs.len;i++) {
 		Region *r=rs.num(i);
 		Grouped *g=new Grouped();
 		List<Pixel> *ps=r->getlist();
 		for (int j=1;j<=ps->len;j++) {
 		  g->gs.add(new OnePixel(ps->num(j)));
 	  }
 	  g->update();
 	  gs.add(g);
	}
	printf("Got %i groups\n",gs.len);
	bin->writefile(getnextfilename("bin","bmp"));
	rs.destroyall();
	rs.freedom();
	gs.destroyall();
	gs.freedom();
}