/* All my source code is freely distributable under the GNU public licence.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 1999
   http://www.cs.bris.ac.uk/~pclark */


// Refraction
// with interpolation

#include <joeylib.c>

#define scrwid 320
#define scrhei 200
#define res 1
#define maxripples 20
#define pi 3.141593
#define big 65536
#define lx 1;
#define ly 1;
#define ex 0.8;
#define ey 0.6;
#define jcoeff 3;
#define scale 1.0/8.0
#define refract 5.0
#define jump 4

typedef struct {
  int st,x,y,w,v,n,front,back;
} ripple;

BITMAP *buffer;

ripple ripples[maxripples];
// GrColor colors[255];
int numripples,mytime;

void newripple(int i);

void main(void) {
  
  int xy,i,c,x,y,front,back,n,minc,maxc;
  float dzdx,dzdy,a,b,dot,norm,tmp;
  long p,q;
  RGB rgb;
  long starttime;
  int numframes;
  
  // srand(456789);
  srand((int)time(NULL));
  // printf("%d\n",(int)time(NULL));
  
  
  mytime=10;
  maxc=0; minc=255;
  
  
  // Set up ripples
  numripples=5;
  for (i=0; i<numripples; i++) {
    newripple(i);
  }

  Map2d<int> dzdxmap=Map2d<int>(scrwid,scrhei);
  Map2d<int> dzdymap=Map2d<int>(scrwid,scrhei);  
  
  allegro_init ();
  install_keyboard ();
  install_timer ();
  set_gfx_mode (GFX_AUTODETECT, scrwid, scrhei, 0, 0);
  set_pallete (desktop_palette);
  buffer = create_bitmap (scrwid, scrhei);
  clear (buffer);
  
  // Set up grayscale colours
  for (c=0;c<=255;c++) {
    i=0;
    rgb.r=c*63/255;
    rgb.g=0;
    rgb.b=0;
    set_color(c,&rgb);
    // colors[c]=GrAllocColor(c,i,i);
  }
  
  PALLETE my_pallete;
  BITMAP *scr_buffer;
  scr_buffer = load_bitmap("worm4.pcx",my_pallete);
  set_pallete (my_pallete);
  blit (scr_buffer, screen, 0,0,0,0,320,200);

  
  // circlefill (buffer, x, y, 3, 255);
  textout_centre (buffer, font, "Press SPACE!", 60, 220, 4);
  
  blit (buffer, screen, 0, 0, 0, 0, scrwid, scrhei);
  
  starttime=uclock();
  numframes=0;
  while(!key[KEY_ESC]) {
    for (x=0; x<scrwid; x=x+jump) {
      for (y=0; y<scrhei; y=y+jump) {
        dzdx=0;
        dzdy=0;
        for (i=0;i<numripples;i++) {
          ripples[i].front=ripples[i].v*(mytime-ripples[i].st);
          ripples[i].back=ripples[i].v*(mytime-ripples[i].st)-ripples[i].w;
          if (ripples[i].back>scrwid)
            newripple(i);
        }
        for (i=0;i<numripples;i++) {
          xy=(sqrt((x-ripples[i].x)*(x-ripples[i].x)+(y-ripples[i].y)*(y-ripples[i].y)));
          // printf("xy%i",xy);
          // printf("x%i",x);
          // printf("y%i",y);
          // printf("rx%i",ripples[i].x);
          // printf("ry%i",ripples[i].y);
          // printf("f%i\n",front);
          // printf("b%i\n",back);
          if ((xy>2)&&(xy<ripples[i].front)&&(xy>ripples[i].back)) {
            // printf("*********");
            a=pi/ripples[i].w*sin(2*pi*ripples[i].n/ripples[i].w*(xy-ripples[i].v*(mytime-ripples[i].st)));
            // printf("a%f",a);
            b=(float)10.0/mysquare(xy+1);
            // printf("b%f",b);
            // norm=1500/(mysquare(xy))*(exp(3*(xy-(ripples[i].v)*((mytime-ripples[i].st)))/ripples[i].w));
            norm=2.0*(xy-ripples[i].back)/(ripples[i].front-ripples[i].back);
            // printf("n%f",norm);
            dzdx=dzdx+a*b*(x-ripples[i].x)*ripples[i].w*norm;
            dzdy=dzdy+a*b*(y-ripples[i].y)*ripples[i].w*norm;
          }
        }

        // dot=dzdx*lx+dzdy*ly;
        // dot=dot*(dzdx*0.8+dzdy*0.6);
        // dot=dzdx*0.8+dzdy*0.6;
//        dot=dzdx * 1 * 0.8 + dzdy * 1 * 0.6;
//        float f=mymod(0.5+dot*0.5*scale);

        dzdxmap.pos[x][y]=refract*dzdx;
        dzdymap.pos[x][y]=refract*dzdy;

        if (x>0 && y>0) {
          int nwdx=dzdxmap.pos[x-jump][y-jump];
          int nedx=dzdxmap.pos[x][y-jump];
          int swdx=dzdxmap.pos[x-jump][y];
          int sedx=dzdxmap.pos[x][y];
          int nwdy=dzdymap.pos[x-jump][y-jump];
          int nedy=dzdymap.pos[x][y-jump];
          int swdy=dzdymap.pos[x-jump][y];
          int sedy=dzdymap.pos[x][y];
          for (int i=0;i<jump;i++)
            for (int j=0;j<jump;j++) {
              float a=(float)i/(float)jump;
              float d=(float)j/(float)jump;
              int idzdx=(1.0-a)*(1.0-d)*nwdx+(a)*(1.0-d)*nedx+(1.0-a)*(d)*swdx+(a)*(d)*sedx;
              int idzdy=(1.0-a)*(1.0-d)*nwdy+(a)*(1.0-d)*nedy+(1.0-a)*(d)*swdy+(a)*(d)*sedy;
              int rx=x-jump+i+idzdx;
              int ry=y-jump+j+idzdy;
              if (rx>=0 && ry>=0 && rx<scrwid && ry<scrhei)
                c=getpixel(scr_buffer,rx,ry);
              else
                c=0;
//              c=ucharchop(8*sqrt(idzdx*idzdx+idzdy*idzdy));
              putpixel(buffer,x-jump+i,y-jump+j,c);
            }
        
        }
        // GrFilledBox(x,y,x+jump-1,y+jump-1,colors[c]);
      }
    }
    /* for (i=0;i<numripples;i++) {
         mycircle(ripples[i].x,ripples[i].y,ripples[i].front,colors[0]);
         mycircle(ripples[i].x,ripples[i].y,ripples[i].back,colors[0]);
       }*/
    blit (buffer, screen, 0, 0, 0, 0, scrwid, scrhei);
    mytime=mytime+2;
    numframes++;
//    save_bitmap(getnextfilename("bmp"),buffer,my_pallete);
  }
  destroy_bitmap(buffer);
  allegro_exit();
  printf("%d frames per second.\n",UCLOCKS_PER_SEC*numframes/(uclock()-starttime));
  printf("max col %d\n",maxc);
  printf("min col %d\n",minc);
  
}

void newripple(int i) {
  ripples[i].st=mytime;
  ripples[i].x=myrnd()*scrwid;
  ripples[i].y=myrnd()*scrhei;
  ripples[i].w=50+myrnd()*150;
  ripples[i].v=1+myrnd()*4;
  ripples[i].n=2+myrnd()*7;
  // printf("%f\n",myrnd());
}
