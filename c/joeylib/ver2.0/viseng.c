#define scrwid 320
#define scrhei 200

float fogdepth=8.0;

float turnability=0.03;
float forcevel=0.03;
float markerrange=3.0;
int taillen=60;

float partrad=0.02;

V3d pos=V3d(0,0,0);
Ori ori=Ori();

  V3d vel=V3d(0,0,0);
  V3d acc=V3d(0,0,0);
  float droll=0;
  float dyaw=0;
  float dpitch=0;

Octree<V3d> *octree=new Octree<V3d>(V3d(0,0,0),50);

JBmp jbmp=JBmp(scrwid,scrhei,0);

void plotsphere(V3d cen,int c) {
  int x,y;
  float p=PPgetunitnoadd(cen);
  if (p) {
    float rad=partrad*p;
    PPgetscrposnoadd(cen,&x,&y);
    jbmp.filledcircle(x,y,rad,c);

//    printf("> %i %i\n",x,y);

//    PPgetscrposnoadd(&cen,&PPrighteye,&x,&y);
//    right.filledcircle(x,y,rad,16*c);
    // b.filledcircle(x,y,,c);
    // plotpart2(part[i],-eyewid,sgn(c)*b))
    // plotpart2(part[i],eyewid,sgn(c)*(128+b)))
  }
}

void plotline(V3d a,V3d b,float i) {
  PPlinenoadd(&jbmp,a,b,15*i);
//  PPlinenoadd(&right,PPrighteye,a,b,(int)(15*i)*16);
}

V3d prepare(V3d v) {
  return (v-pos).disorientate(ori);
}

void line(V3d a,V3d b) {
  plotline(prepare(a),prepare(b),1);
}

void plotscene() {
  jbmp.clear(0);
  ori.quickorisetup();
  Frustrum f=Frustrum(pos,ori,0.01,fogdepth);
  OrderedList<V3d> particles=*octree->getorderedlist(f);
  for (int i=particles.len;i>=1;i--) {
    int x,y;
    V3d p=particles.num(i);

//    printf("%f %f %f\n",p.x,p.y,p.z);

    V3d cen=prepare(p);

//    printf("%f %f %f\n",cen.x,cen.y,cen.z);

//    int c=255;
    int c=255.0-chop(254.0*cen.z/fogdepth,0,254);
    plotsphere(cen,c);
  }
}    

void movement() {
    // Movement
    float angvel=turnability;
    float dir=( key[KEY_ALT] ? -1 : +1 );

    if (key[KEY_LCONTROL])
      vel=vel+dir*ori.z()*forcevel;

    if (key[KEY_UP])
      if (key[KEY_LSHIFT])
        vel=vel+ori.y*forcevel;
      else
        dpitch=dpitch-angvel;

    if (key[KEY_DOWN])
      if (key[KEY_LSHIFT])
        vel=vel-ori.y*forcevel;
      else
        dpitch=dpitch+angvel;

    if (key[KEY_LEFT])
      if (key[KEY_LSHIFT])
        vel=vel-ori.x*forcevel;
      else if (key[KEY_ALT])
        droll=droll-angvel;
      else
        dyaw=dyaw-angvel;

    if (key[KEY_RIGHT])
      if (key[KEY_LSHIFT])
        vel=vel+ori.x*forcevel;
      else if (key[KEY_ALT])
        droll=droll+angvel;
      else
        dyaw=dyaw+angvel;

    vel=hang(vel,V3d::o,0.92,0);
    pos=pos+vel;
    droll=hang(droll,0,0.9,0);
    dyaw=hang(dyaw,0,0.9,0);
    dpitch=hang(dpitch,0,0.9,0);
    ori.roll(droll/5.0);
    ori.yaw(dyaw/5.0);
    ori.pitch(dpitch/5.0);
}

void visengfly() {

//  octree=o;

  setuptriglookup();

  int frame=0;

  do {
    frame++;
    
    plotscene();

    jbmp.blit();

    movement();

  } while (!key[KEY_SPACE] && !key[KEY_ESC]);

}
