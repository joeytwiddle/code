#include <joeylib.c>

int scrwid=640;
int scrhei=480;

/*
class World {
public:
  Scene *s;
  Renderer *r;
  Avatar *av;
  World() { }
};
*/

class Avatar {
public:

  Viewpoint v;

  float turnability=0.15;
  float forcevel=0.03;

  V3d vel;
  float droll=0;
  float dyaw=0;
  float dpitch=0;

  Avatar() {
    v=Viewpoint(V3d(0,0,-4),V3d(0,1.5,0),V3d::j);
  }

  void move() {
    // Movement
    float angvel=turnability;
    if (key[KEY_LCONTROL])
      vel=vel+v.ori.z()*forcevel;
    else
      angvel=turnability*2.0;
    if (key[KEY_UP])
      dpitch=dpitch-angvel;
    if (key[KEY_DOWN])
      dpitch=dpitch+angvel;
    if (key[KEY_LEFT])
      if (key[KEY_ALT])
        droll=droll-angvel;
      else
        dyaw=dyaw-angvel;
    if (key[KEY_RIGHT])
      if (key[KEY_ALT])
        droll=droll+angvel;
      else
        dyaw=dyaw+angvel;
    vel=hang(vel,V3d::o,0.92,0);
    v.pos=v.pos+vel;
    droll=hang(droll,0,0.8,0);
    dyaw=hang(dyaw,0,0.8,0);
    dpitch=hang(dpitch,0,0.8,0);
    v.ori.roll(droll/5.0);
    v.ori.yaw(dyaw/5.0);
    v.ori.pitch(dpitch/5.0);
  }

};
    

float (*ordering)(FileRec *);

void adddirtoscene(Scene *s,DirRec *dr,V3d pos,V3d up) {
//  printf("Adding directory %s\n",dr->fname);
  s->add(new Cylinder(pos+0.0*up,pos+1.0*up,0.05*up.mod()));
  pos=pos+up;
//  s->add(new TextRend(dr->fname,10,pos,0.3));
  OrderedList<FileRec> ol;
  for (int i=1;i<=dr->fs.len;i++) {
    FileRec *r=dr->fs.num(i);
    ol.add(r,ordering(r));
  }
  V3d x=V3d::normcross(up,V3d::k)*up.mod()*0.8;  // Actually, want y=closest to real z
  V3d y=V3d::normcross(up,x)*up.mod()*0.8;
  for (int i=1;i<=ol.len;i++) {
    FileRec *r=ol.p2num(i);
//    printf("  %s\n",r->fname);
    float ang=((i%2)==0?1:-1)*(pi*(float)(i-1)/(float)(ol.len-1));
    V3d d=up+x*mysin(ang)+y*mycos(ang); // Bad, should do rotation instead.
    if (Seq(r->type(),"file")) {
      //s->add(new Cylinder(pos+0.0*d,pos+0.2*d,0.05*up.mod()));
      s->add(new Sphere(pos+0.1*d,0.02*up.mod()));
//      s->add(new TextRend(r->fname,10,pos+d,0.1));
    } else {
      DirRec *dr2=(DirRec *)r;
      adddirtoscene(s,dr2,pos,d.norm()*up.mod()*0.9);
    }
  }
}

float orderbysize(FileRec *r) {
  return r->size;
}

float orderbydate(FileRec *r) {
  return myrnd();
}

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  String dir=a.argor("directory","../../");
  ordering=( a.argexists("-s") ? &orderbysize :
             a.argexists("-d") ? &orderbydate :
             &orderbysize );
  a.done();
  
  DirRec dr=DirRec(dir);

  Scene *s=new Scene();
  Renderer *r=new WireframeRenderer();
  Avatar *av=new Avatar();

  adddirtoscene(s,&dr,V3d::origin,V3d::j);
  
  ImageType *m=new ImageType(scrwid,scrhei,0);

  while (!key[KEY_ESC]) {

    r->renderimage(m,s,&(av->v));

    makepalette(&greypalette);

    m->writetoscreen();

    av->move();

  }

//  m->writefile("rendered.bmp");
  
}
