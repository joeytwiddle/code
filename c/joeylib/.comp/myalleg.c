#ifndef myalleg_C
  #define myalleg_C

  #include <myalleg.h>

int allegrostarted=0;

void allegrosetup(int scrwid,int scrhei,int cd) {
  #ifdef ALLEGRO
  // - Gets allegro up and working at specified screen size
  if (allegrostarted!=scrwid*scrhei*cd &&
    ( (scrwid==320 && scrhei==200) ||
      (scrwid==640 && scrhei==480) ||
      (scrwid==800 && scrhei==600) ||
      (scrwid==1024 && scrhei==768) )
    ) {

    allegrostarted=scrwid*scrhei*cd;

    set_color_depth(cd);
    allegro_init ();
    install_keyboard ();
    install_timer ();
    #ifdef NOGRAPHICS
      printf("Not starting Allegro graphics.\n");
    #else
      set_gfx_mode (GFX_AUTODETECT, scrwid, scrhei, 0, 0);
      set_pallete (desktop_palette);
    #endif
  }
  #ifndef NOGRAPHICS
  clear(screen);
  #endif
  #endif
}

void allegrosetup(int scrwid,int scrhei) {
  allegrosetup(scrwid,scrhei,8);
}

void allegroexit() {
  allegrostarted=0;
#ifdef ALLEGRO
#ifndef NOGRAPHICS
  allegro_exit();
#endif
#endif
}

#ifndef ALLEGRO
// Starts of class RGB


  // Variable declared in .h file
// End class 

#endif

RGB mynewRGB(float r,float g,float b) {
  RGB rgb;
  rgb.r=(uchar)(63*r);
  rgb.g=(uchar)(63*g);
  rgb.b=(uchar)(63*b);
  return rgb;
}

RGB greypalette(float f) {
  RGB r=mynewRGB(f,f,f);
  return r;
}

RGB redpalette(float f) {
  RGB r=mynewRGB(f,0,0);
  return r;
}

RGB greenpalette(float f) {
  RGB r=mynewRGB(0,f,0);
  return r;
}

RGB redtowhitepalette(float f) {
  if (f<0.5)
    return mynewRGB(2.0*f,0,0);
  else
  return mynewRGB(1,2.0*(f-.5),2.0*(f-.5));
}

RGB greentowhitepalette(float f) {
  if (f<0.5)
    return mynewRGB(0,2.0*f,0);
  else
  return mynewRGB(2.0*(f-.5),1,2.0*(f-.5));
}

void makepalette(RGB (*fn)(float)) {
  #ifdef ALLEGRO
  #ifdef NOGRAPHICS
  #else
  for (int i=0;i<=255;i++)
  set_color(i,&(fn((float)i/255)));
  #endif
  #endif
}

void makesplitpalette(RGB (*fna)(float),RGB (*fnb)(float)) {
#ifdef ALLEGRO
  for (int i=0;i<=127;i++)
  set_color(i,&(fna((float)i/127.0)));
  for (int i=128;i<=255;i++)
  set_color(i,&(fnb((float)(i-128)/127.0)));
  #endif
}

void makepaletterange(int a,int b,RGB c,RGB d) {
  RGB rgb;
  float t;
  for (int i=a;i<=b;i++) {
    t=(float)(i-a)/(b-a);
    rgb.r=(uchar)fromto(t,c.r,d.r);
    rgb.g=(uchar)fromto(t,c.g,d.g);
    rgb.b=(uchar)fromto(t,c.b,d.b);
    #ifdef ALLEGRO
    set_color(i,&rgb);
    #endif
  }
}

void makestereopalette() {
  makepaletterange(0,0,mynewRGB(0,0,0),mynewRGB(0,0,0));
  makepaletterange(1,127,mynewRGB(.3,0,0),mynewRGB(.9,0,0));
  makepaletterange(128,255,mynewRGB(0,.3,0),mynewRGB(0,.9,0));
}

void mypalette(int c,float r,float g,float b) {
  #ifdef NOGRAPHICS
  printf("Not making palette.\n");
  #else
  #ifdef ALLEGRO
  RGB rgb;
  rgb.r=(unsigned char)(63*r);
  rgb.g=(unsigned char)(63*g);
  rgb.b=(unsigned char)(63*b);
  set_color(c,&rgb);
  #endif
  #endif
}

void mypalette(int c,myRGB m) {
#ifdef ALLEGRO
#ifndef NOGRAPHICS
  RGB rgb;
  rgb.r=(unsigned char)(63*m.r/255);
  rgb.g=(unsigned char)(63*m.g/255);
  rgb.b=(unsigned char)(63*m.b/255);
  set_color(c,&rgb);
#endif
#endif
}

void makepalette(myRGB (*fn)(float)) {
  for (int i=0;i<=255;i++)
  mypalette(i,fn((float)i/255));
} 

// - eg. makepalette(&myRGB::hue);

void savescreen(String fname) {
#ifdef ALLEGRO
  PALETTE pal;
  get_palette(pal);
  save_bitmap(fname,screen,pal);
  #endif
}

void psychedelicpalette() {
  makepalette(&myRGB::hue);
}

RGB newallegRGB(float x,float y,float z) {
  RGB *n=new RGB;
  n->r=63*x;
  n->g=63*y;
  n->b=63*z;
  return *n;
}

RGB newallegRGB(myRGB r) {
  return newallegRGB(r.r/255.0,r.g/255.0,r.b/255.0);
}

RGB *makegreyallegpalette() {
  RGB *n=new RGB[256];
  for (int i=0;i<=255;i++) {
    n[i]=newallegRGB((float)i/255.0,(float)i/255.0,(float)i/255.0);
  }
  // printf("pal done\n");
  return n;
}

void waitforkeypress() {
#ifdef ALLEGRO
    allegro_init ();
    install_keyboard ();
  while (!keypressed()) {
  };
  #endif
//  while (keypressed()) {
//  };
//  readkey();
}

#endif
