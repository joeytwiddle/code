#include <joeylib.c>

void displaypic(RGBmp *pic);

int main(int argc, char **argv) {

  // Parse command line parameters
  if (argc==1) {
    printf("jshow : rgbmp reader and displayer (Joey Feb 99)\n");
    exit(0);
  }

//  String infile=argv[1];

//  RGBmp *pic=new RGBmp("i\\tmp\\x_big10.bmp");
  RGBmp *pic=new RGBmp("i\\04.bmp");

  int scrwid=640,scrhei=480;

  pic->display(scrwid,scrhei);

  install_mouse();
  install_timer();
  show_mouse(screen);

  float ax=-.3,ay=-.2,bx=.3,by=-.2,cx=.3,cy=.2,dx=-.3,dy=.2;

  while (!(mouse_b & 1)) { }
  ax=pic->width/2+(mouse_x-scrwid/2)*pic->height/scrhei;
  ay=pic->height/2+(mouse_y-scrhei/2)*pic->height/scrhei;

  while ((mouse_b & 1)) { }

  while (!(mouse_b & 1)) { }
  bx=pic->width/2+(mouse_x-scrwid/2)*pic->height/scrhei;
  by=pic->height/2+(mouse_y-scrhei/2)*pic->height/scrhei;

  while ((mouse_b & 1)) { }

  while (!(mouse_b & 1)) { }
  cx=pic->width/2+(mouse_x-scrwid/2)*pic->height/scrhei;
  cy=pic->height/2+(mouse_y-scrhei/2)*pic->height/scrhei;

  while ((mouse_b & 1)) { }

  while (!(mouse_b & 1)) { }
  dx=pic->width/2+(mouse_x-scrwid/2)*pic->height/scrhei;
  dy=pic->height/2+(mouse_y-scrhei/2)*pic->height/scrhei;

  float eta=((cy-ay)*(bx-ax)+ax-cx)/((dy-ay)*(bx-ax)-dx+ax);
  float mu=(-eta*(dx-ax)+cx-ax)/(bx-ax);
  float lambda=mu+eta-1;
  float nu=1;

  V3d va=lambda*V3d(ax,ay,1);
  V3d vb=mu*V3d(bx,by,1);
  V3d vc=nu*V3d(cx,cy,1);
  V3d vd=eta*V3d(dx,dy,1);

  V3d top=vb-va;
  V3d left=vd-va;

  float aspect=top.mod()/left.mod();
  float check=(vc-vd).mod()/(vc-vb).mod();

  float topx=bx-ax;
  float topy=by-ay;
  float botx=cx-dx;
  float boty=cy-dy;
  float leftx=dx-ax;
  float lefty=dy-ay;
  float rightx=cx-bx;
  float righty=cy-by;

  aspect=(V2d::dist(V2d(ax,ay),V2d(bx,by))+V2d::dist(V2d(cx,cy),V2d(dx,dy)))/(V2d::dist(V2d(ax,ay),V2d(dx,dy))+V2d::dist(V2d(cx,cy),V2d(bx,by)));
  RGBmp *out=new RGBmp(600,600/aspect);
  for (int i=0;i<out->width;i++)
  for (int j=0;j<out->height;j++) {
    float across=(float)i/(float)out->width;
    float down=(float)j/(float)out->height;
    float nacross=1.0-across;
    float ndown=1.0-down;
//    float acrossx=topx+down*(botx-topx);
//    float acrossy=topy+down*(boty-topy);
    float acrossx=topx;
    float acrossy=topy;
    float downx=leftx+across*(rightx-leftx);
    float downy=lefty+across*(righty-lefty);
//    float downx=leftx;
//    float downy=lefty;
//    int px=ax+across*acrossx+down*downx;
//    int py=ay+across*acrossy+down*downy;
    int px=ax*nacross*ndown+bx*across*ndown+cx*across*down+dx*nacross*down;
    int py=ay*nacross*ndown+by*across*ndown+cy*across*down+dy*nacross*down;
    if (pic->inimage(px,py))
      out->bmp[i][j]=pic->bmp[px][py];
  }
  
  out->display();
  out->writefile("bodgeout.bmp");

  exit(0);

  //while (!key[KEY_ESC]) { }

  allegro_exit();

//  printf("%f %f\n",aspect,check);

  printf("%f %f %f %f\n",lambda,mu,nu,eta);

}
