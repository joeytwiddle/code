#include <joeylib.c>

#define scrwid 320
#define scrhei 200
#define numps 500

void dispfn(V3d p) {
  p.print();
}

void printfloat(float f) {
  printf("%f",f);
}

int main() {
  List<V3d> l;
  List<float> m;
  int x,y;

/*
  float v[2][2];

V3d axis=V3d(0,1,0);
float ang=45;

     v[0][0]=V3d::oldrotate(V3d(1,0,0),axis,ang).x;
     v[1][0]=V3d::oldrotate(V3d(0,1,0),axis,ang).x;
     v[2][0]=V3d::oldrotate(V3d(0,0,1),axis,ang).x;
     v[0][1]=V3d::oldrotate(V3d(1,0,0),axis,ang).y;
     v[1][1]=V3d::oldrotate(V3d(0,1,0),axis,ang).y;
     v[2][1]=V3d::oldrotate(V3d(0,0,1),axis,ang).y;
     v[0][2]=V3d::oldrotate(V3d(1,0,0),axis,ang).z;
     v[1][2]=V3d::oldrotate(V3d(0,1,0),axis,ang).z;
     v[2][2]=V3d::oldrotate(V3d(0,0,1),axis,ang).z;
  for (int i=0;i<=2;i++) {
    for (int j=0;j<=2;j++)
      printf("%f ",v[j][i]);
    printf("\n");
  }


   float c=cos(ang);
   float s=sin(ang);
   float cc=1-c;
   axis.normalise();
   v[0][0]=(cc*axis.x*axis.x)+c;
   v[0][1]=(cc*axis.x*axis.y)+(axis.z*s);
   v[0][2]=(cc*axis.x*axis.z)-(axis.y*s);
   v[1][0]=(cc*axis.x*axis.y)-(axis.z*s);
   v[1][1]=(cc*axis.y*axis.y)+c;
   v[1][2]=(cc*axis.z*axis.y)+(axis.x*s);
   v[2][0]=(cc*axis.x*axis.z)+(axis.y*s);
   v[2][1]=(cc*axis.y*axis.z)-(axis.x*s);
   v[2][2]=(cc*axis.z*axis.z)+c;

  for (int i=0;i<=2;i++) {
    for (int j=0;j<=2;j++)
      printf("%f ",v[i][j]);
    printf("\n");
  }
exit(0);
  */
  allegrosetup(scrwid,scrhei);
  makepalette(&greypalette);
  PPsetup(scrwid,scrhei,5,0);

//  List<V3d> l(5);
  for (int i=1;i<=numps;i++) {
    V3d r=sqrt(myrnd())*(V3d::random());
    l+r;
    m+r.mod();
  }

  JBmp j=JBmp(scrwid,scrhei);
  int time=0;
  starttimer();
  while (!key[KEY_ESC]) {
    time++;
    V3d rotaxis=V3d::rotatea(V3d(0,1,0),V3d(1,0,0),time*pi/700);
//    rotaxis.print();
    V3d::rotatelista(l,rotaxis,pi/40);
    j.clear();
    for (int i=1;i<=l.length();i++) {
//      V3d t=(V3d)(l.num(i));
//      t.changeto(V3d::rotate(t,rotaxis,pi/40));
//      l.replace(i,t);
      V3d *t=(l.p2num(i));
      PPgetscrpos(*t,&x,&y);
      j.setpixel(x,y,127*(1-t->z));
//      j.setpixel(0,scrhei/2*(1+t.z),127*(1-t.x));
    }
    j.writetoscreen();
    framedone();
  }
  allegro_exit();
  displayframespersecond();
}

