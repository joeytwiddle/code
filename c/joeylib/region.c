// make() functions use memoing
// remake() will re-memo

class Region {
public:
  // Size of image, coordinates of window
  int width,height,left,right,top,bottom;
  // Can be represented in different forms:
  Map2d<bool> *map;
  List<Pixel> *list;
  Correlator *cor;
  List<Pixel> *boundary;
  void destroyallbut(void *n) {
  // void destroyallbut(Map2d<bool> *not) {
    if (n!=&map) {
    // if (not!=map) {
      destroy(map);
    }
//    printf("Thinking about list\n");
    if (n!=&list) //{
      destroy(list);
//    } else
//      printf("Nah!\n");
    if (n!=&cor) {
      destroy(cor);
    }
    if (n!=&boundary) {
      destroy(boundary);
    }
  }
  void destroyall() {
    destroyallbut(NULL);
  }
  void destroyallbutmap() {
    destroyallbut(&map);
  }
  void destroyallbutlist() {
    destroyallbut(&list);
  }
  Region() { // If you make a list out of this, and then want to get the region, you must give it a width and height!
    map=NULL;
    list=NULL;
    cor=NULL;
    boundary=NULL;
    clear();
    list=new List<Pixel>();
  }
  Region(int w,int h) {
    map=NULL;
    list=NULL;
    cor=NULL;
    boundary=NULL;
    clear();
    list=new List<Pixel>();
    width=w; height=h;
  }
  Region(Polygon2d p,int w,int h) {
    map=NULL;
    list=NULL;
    width=w; height=h;
    cor=NULL;
    boundary=NULL;
    clear();
    map=new Map2d<bool>(w,h,false);
    map->plotpoly(p,true);
  }
  Region(Map2d<bool> *m) {
    list=NULL;
    cor=NULL;
    boundary=NULL;
    clear();
    map=m;
    getsize();
  }
  Region(List<Pixel> *l) {
    map=NULL;
    cor=NULL;
    boundary=NULL;
    clear();
    list=l;
  }
  Region(List<V2d> l) {
    map=NULL;
    cor=NULL;
    boundary=NULL;
    clear();
    list=new List<Pixel>(l.len);
    for (int i=0;i<l.len;i++)
      list->add(Pixel(l.get(i)));
  }
  void clear() {
    width=height=left=right=top=bottom=-1;
//    destroy(map);
//    destroy(list);
  }
  float listleftmost() {
    if (left==-1) {
    makelist();
    float lm=list->num(1).x;
    for (int i=2;i<=list->len;i++) {
      if (list->num(i).x<lm)
        lm=list->num(i).x;
    }
    left=lm;
    }
    return left;
  }
  float listrightmost() {
    if (right==-1) {
    makelist();
    right=list->num(1).x;
    for (int i=2;i<=list->len;i++) {
      if (list->num(i).x>right)
        right=list->num(i).x;
    }
    }
    return right;
  }
  float listtopmost() {
    if (top==-1) {
    makelist();
    float tm=list->num(1).y;
    for (int i=2;i<=list->len;i++) {
      if (list->num(i).y<tm)
        tm=list->num(i).y;
    }
    top=tm;
    }
    return top;
  }
  float listbottommost() {
    if (bottom==-1) {
    makelist();
    float bm=list->num(1).y;
    for (int i=2;i<=list->len;i++) {
      if (list->num(i).y>bm)
        bm=list->num(i).y;
    }
    bottom=bm;
    }
    return bottom;
  }
  float leftmost() {
    makemap();
    for (int x=0;left==-1;x++)
    for (int y=0;y<height;y++)
      if (map->getpos(x,y)) {
        left=x;
        break;
      }
    return left;
  }
  float rightmost() {
    makemap();
    for (int x=width-1;right==-1;x--)
    for (int y=0;y<height;y++)
      if (map->getpos(x,y)) {
        right=x;
        break;
      }
    return right;
  }
  float topmost() {
    makemap();
    for (int y=0;top==-1;y++)
    for (int x=0;x<width;x++)
      if (map->getpos(x,y)) {
        top=y;
        break;
      }
    return top;
  }
  float bottommost() {
    makemap();
    for (int y=height-1;bottom==-1;y--)
    for (int x=0;x<width;x++)
      if (map->getpos(x,y)) {
        bottom=y;
        break;
      }
    return bottom;
  }
  static Region *listready() { // may as well use Region() instead
    Region *n=new Region();
    n->list=new List<Pixel>();
    return n;
  }
  List<Region *> *oldsplit() { // Try splitting up using mean centre
    makelist();
    float l=leftmost();
    float r=rightmost();
    float t=topmost();
    float b=bottommost();
    float mx=(l+r)/2.0;
    float my=(t+b)/2.0;
    List<Region *> *rl=new List<Region *>(4);
    for (int i=1;i<=4;i++)
      rl->add(new Region(width,height));
    for (int i=1;i<=list->len;i++) {
      int lr=( list->num(i).x<mx ? 0 : 1 );
      int tb=( list->num(i).y<my ? 0 : 2 );
      int j=1+lr+tb;
      // printf("%i ",j);
      // printf("%i\n",rl->num(j)->list->len);
      rl->num(j)->list->add(list->num(i));
      // printf("B\n");
    }
    return rl;
  }
  List<Region *> *split() {
    printf("a");
    bool splithor=(diff(leftmost(),rightmost())>diff(topmost(),bottommost()));
    printf("b");
    Pixel cen=centreofmass();
    printf("c");
    List<Region *> *rl=new List<Region *>(2);
    printf("d");
    for (int i=1;i<=2;i++)
      rl->add(new Region(width,height));
    for (int i=1;i<=list->len;i++) {
      printf(">");
      int w;
      if (splithor)
        w=( list->num(i).x<cen.x ? 1 : 2 );
      else
        w=( list->num(i).y<cen.y ? 1 : 2 );
      rl->num(w)->list->add(list->num(i));
    }
    printf("<");
    return rl;
  }
  void add(Pixel v) {
    // makelist();
    list->add(v);
  }
  void makelist() {
    if (list==NULL) {
      remakelist();
    }
//printf("Got list length %i\n",list->len);
  }
  void remakelist() {
    // destroy(list); ?
    list=map->getlist();
  }
  void remakemap(int width,int height) {
    if ((width<1) || (height<1))
      myerror(Sformat("Region::remakemap, width=%i height=%i",width,height));
//    printf("Making map (%i) %ix%i\n",list->len,width,height);
    map=new Map2d<bool>(width,height,false);
    for (int x=1;x<=list->len;x++) {
      int i=list->num(x).x;
      int j=list->num(x).y;
      if (map->inmap(i,j)) {
        map->pos[i][j]=true;
        // printf(".");
      }
    }
  }
  Map2d<bool> *getmap(int width,int height) {
    makemap(width,height);
    return map;
  }
  Map2d<bool> *getmap() {
    return getmap(width,height);
  }
  void makemap(int width,int height) {
    if (map==NULL)
      remakemap(width,height);
  }
  void makemap() {
    makemap(width,height);
  }
  List<Pixel> *getlist() {
    if (list==NULL)
      makelist();
    return list;
  }
  void freedom() {
    destroyall();
  }
  void getsize() {
    if (map!=NULL) {
      width=map->width;
      height=map->height;
    }
  }
        
  void scaleto(int w,int h) {
    getsize();
//    printf("%i %i\n",width,height);
    makemap();
    if (width!=w || height!=h) {
      Map2d<bool> *n=new Map2d<bool>(w,h);
      for (int i=0;i<n->width;i++)
      for (int j=0;j<n->height;j++) {
        int x=i*width/n->width;
        int y=j*height/n->height;
        n->pos[i][j]=map->pos[x][y];
      }
      clear();
      map=n;
      getsize();
    }
  }

  Pixel centreofmass() {
    makelist();
    Pixel cen=Pixel(0,0);
    for (int i=1;i<=list->len;i++)
      cen=cen+list->num(i);
    cen=cen/list->len;
    return cen;
  }

  void contract(int r) {
    Map2d<bool> *n=getmap()->contract(r);
    destroyall();
    map=n;
  }

  Region contracted(int r) {
    return Region(getmap()->contract(r)); // Should make like expanded...
  }

  Region expanded(int r) {
//    return Region(getmap()->expand(r));
    makemap();
    Map2d<bool> *n=new Map2d<bool>(width,height,false);
      for (int i=leftmost()-r;i<=rightmost()+r;i++)
      for (int j=topmost()-r;j<=bottommost()+r;j++) {
        if (map->pixelwithinrad(i,j,r))
          n->pos[i][j]=true;
        else
          n->pos[i][j]=false;
      }
    return Region(n);
  }

  void newlist() {
    destroy(list);
    list=new List<Pixel>();
  }

  void singleexpansion() {
    makemap();
    makelist();
    for (int i=1;i<=list->len;i++)
      for (int d=0;d<4;d++)
        map->setpos(list->num(i)+Pixel::indir(d),true);
    destroyallbutmap();
  }

  List<int> checkexpansion(Map2d<int> *regmap) {
    // Map version - speedy!
    makemap();
    makelist();
    List<int> l;
    for (int i=1;i<=list->len;i++)
      for (int d=0;d<4;d++) {
        Pixel p=list->num(i)+Pixel::indir(d);
        map->setpos(p,true);
        int i=regmap->getpos(p);
        if (i>0 && !l.contains(i))
          l.add(i);
      }
      destroy(list);
      makelist();
      destroy(map);
    return l;
  }

  List<int> checkexpansion(Map2d<int> *regmap,int col) {
    // Map version - speedy!
    makemap();
    makelist();
    List<int> l;
    for (int i=1;i<=list->len;i++)
      for (int d=0;d<4;d++) {
        Pixel p=list->num(i)+Pixel::indir(d);
        map->setpos(p,true);
        int i=regmap->getpos(p);
        if (i>0 && !l.contains(i))
          l.add(i);
        regmap->setpos(p,col);
      }
//printf("A %i %i\n",map->width,map->height);
      destroy(list);
//printf("B\n");
      makelist();
//printf("C\n");
      destroy(map);
//printf("D\n");
//    remakelist();
//    destroy(map);
    return l;

/*    // List version - mem efficient but fucking slow!
    makelist();
    List<int> l;
    for (int i=1;i<=list->len;i++)
      for (int d=0;d<4;d++) {
        Pixel p=list->num(i)+Pixel::indir(d);
        list->addifnot(p);
        int i=regmap->getpos(p);
        if (i>0 && !l.contains(i))
          l.add(i);
        regmap->setpos(p,col);
      }
    destroyallbutlist();
    return l;*/
  }

  int area() {
    return getlist()->len;
  }

  V2d centroid() {
  	makelist();
  	V2d centroid=V2d(0,0);
  	for (int i=1;i<=list->len;i++) {
  		centroid=centroid+V2d(list->num(i));
  	}
  	centroid=centroid/list->len;
  	return centroid;
  }

  float correlation() {
    makecorrelator();
    return cor->correlation();
  }
  void makecorrelator() {
    if (cor==NULL)
      remakecorrelator();
  }
  void remakecorrelator() {
    cor=new Correlator(getlist());
  }
  Correlator *getcor() {
    makecorrelator();
    return cor;
  }
  /*
  Line2d line() {
    makecorrelator();
    makelist();
    float c=cor->A();
    V2d v=V2d(1.0,c).norm()*cor->correlation()*(float)sqrt(list->len)*2.0;
    return Line2d(centroid()-v,centroid()+v);
//    return Line2d(centroid(),centroid()+V2d(1.0,c).norm());
  }
  */

  float error(float a) {
    V2d ctr=centroid();
    float error=0;
    makelist();
    for (int i=1;i<=list->len;i++) {
      V2d v=V2d(list->num(i))-ctr;
      error+=square(v.x*sin(a)+v.y*cos(a));
    }
    return error;
  }

  float angle() {
    makecorrelator();
    return cor->angle();

/*    if (corang==NULL) {
    float best=100000000000;
    float bestt=0;
    for (float t=0;t<pi;t=t+pi/20.0) {
      float e=error(t);
      if (e<best) {
        best=e;
        bestt=t;
      }
    }
      corang=new float[1];
      corang[0]=bestt;
    }
    return *corang;

    V2d ctr=centroid();
    float c=0;
    float b=0;
    makelist();
    for (int i=1;i<=list->len;i++) {
      V2d v=V2d(list->num(i))-ctr;
      c=c+v.x*v.y;
      b=b+v.x*v.x-v.y*v.y;
    }
    float a=-c;
    float ta=atan((-b+sqrt(b*b-4*a*c))/2.0/a);
    float tb=atan((-b-sqrt(b*b-4*a*c))/2.0/a);
    if (error(ta)<error(tb))
      return ta;
    else
      return tb;*/

  }

/*  float angle() {
    makelist();
    V2d c=centroid();
    float sumx=0;
    float sumy=0;
    float sum=0;
    for (int i=1;i<=list->len;i++) {
      Pixel p=list->num(i);
      sumx+=(float)p.x-c.x;
      sumy+=(float)p.y-c.y;
      sum+=((float)p.x-c.x)/((float)p.y-c.y);
    }
    printf("%f %f ",sumx,sumy);
//    return atanf(sum);
    return atanf((float)sumx/(float)sumy);
  }*/

  V2d ori() {
    float a=angle();
    return V2d(sin(a+pi/2.0),cos(a+pi/2.0)).norm();
  }

  float blength() {
    makecorrelator();
    return cor->length();
  }

  float bheight() {
    makecorrelator();
    return cor->fatness();
  }

  Line2d endtoend() { // finds ends from orientation
    return endtoend(ori());
  }

  Line2d endtoend(V2d ori) {
    makelist();
    makecorrelator();
    float lowest=-1.0;
    float highest=-1.0;
    V2d vlowest,vhighest;
    for (int i=1;i<=list->len;i++) {
      V2d v=V2d(list->num(i));
      float f=ori.dot(v);
      if (lowest==-1 || f<lowest) {
        lowest=f;
        vlowest=v;
      }
      if (highest==-1 || f>highest) {
        highest=f;
        vhighest=v;
      }
    }
    return Line2d(vlowest,vhighest);
  }

  Line2d endtoendvp(V2d vp) { // finds ends of line by distance from HVP
    // Assume VP far away
    V2d cen=centroid();
    V2d dir=(cen-vp).norm();
    return endtoend(dir);
  }

  Line2d line() { // uses centroid as center
    V2d v=ori()*getcor()->length();
    V2d c=centroid();
//    printf("Made line length %f List %i\n",v.mod(),list->len);
    return Line2d(c-v,c+v);
  }

  void makeboundary() {
    if (boundary==NULL)
      remakeboundary();
  }
  Pixel dir(int i) {
    float t=2.0*pi*(float)i/4.0;
    return Pixel(sin(t)*1.5,cos(t)*1.5);
  }
  void remakeboundary() {
//printf("A\n");
    makelist();
//printf("B\n");
    makemap();
//printf("C\n");
    boundary=new List<Pixel>();
    for (int k=1;k<=list->len;k++) {
      int i=list->num(k).x;
      int j=list->num(k).y;
      if (map->getpos(i,j))
        if (map->getpos(i-1,j)!=true || map->getpos(i+1,j)!=true || map->getpos(i,j-1)!=true || map->getpos(i,j+1)!=true)
          boundary->add(Pixel(i,j));
    }

    destroy(map); // For memory efficiency in my "lines" program

/*  Using map - inefficient  makemap();
    boundary=new List<Pixel>();
    for (int i=0;i<width;i++)
    for (int j=0;j<height;j++)
      if (map->getpos(i,j))
        if (map->getpos(i-1,j)!=true || map->getpos(i+1,j)!=true || map->getpos(i,j-1)!=true || map->getpos(i,j+1)!=true)
          boundary->add(Pixel(i,j));*/

    /* Little man searching the boundary - sometimes doesn't work
    Pixel *pp=NULL; // Find a start pixel
    for (int j=0;j<height && pp==NULL;j++)
    for (int i=0;i<width && pp==NULL;i++)
      if (map->getpos(i,j)==true)
        pp=new Pixel(i,j);
    Pixel p=*pp;
    Pixel t=p;
    int d=0;
    bool done=false;
    while (!done) {
      boundary->add(p);
      d--; // Start looking left
      while (map->getpos(p+dir(d))!=true) {
        d++;
      }
      Pixel newp=p+dir(d);
      if (newp==t)
        done=true;
      p=newp;
//      printf("Moved (%i,%i)\n",dir(d).x,dir(d).y);
    }
    boundary->remnum(boundary->len);
//        error("Region::remakeboundary - Ran out of directions to turn in!");
//    destroy(map);*/

//    printf("Found boundary length %i\n",boundary->len);
//    printf(".\n");

  }
  List<Pixel> *getboundary() {
    makeboundary();
    return boundary;
  }

};

  bool Polygon2d::overlaps(Polygon2d *p) { // works but rather inefficient!
    return overlaps(*p);

  //if (p->smallreg==NULL)
      p->smallreg=new Region(*p*0.25,200,150);
    Region *r=p->smallreg;
    //if (smallreg==NULL)
      smallreg=new Region(*this*0.25,200,150);
    Region *o=smallreg;
//    o.contract(2);
    List<Pixel> *ltmp=o->getlist();
    int cnto=ltmp->len;
//    destroy(ltmp);
    Map2d<bool> *tmp=Map2d<bool>::applyop(r->map,&booland,o->map);
    Region n=Region(tmp);
//    Region n=Region(Map2d<bool>::applyop(r->map,&booland,o->map));
    List<Pixel> *ltmp2=n.getlist();
    int cntn=ltmp2->len;
//    destroy(ltmp2);
    destroy(r);
    destroy(o);
    n.freedom();
    if (cntn>=0.99*cnto)
      return container;
    if (cntn>0)
      return true;
    return false;
  }





