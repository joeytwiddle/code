
#define Region Banana
#include <joeylib.h>
// #include <joeylib.c>
#define Region Region

#include <time.h>

#define COLORS	255
#define colors COLORS

#define WIN_W 320
#define WIN_H 200
#define scrwid WIN_W
#define scrhei WIN_H
// #define WIN_W	640
// #define WIN_H	480
// #define scrwid 160
// #define scrhei 100


#ifdef X11GFX
  // #include "xlib/xlib.h"
  #include <sys/ipc.h>
  #include <sys/shm.h>
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>    /* For the visual matching */
  #include <X11/extensions/XShm.h>
  #include <stdlib.h>
  #include <stdio.h>
  /* These are for the CPU time measurement. */
  #include <sys/time.h>
  #include <sys/resource.h>
  #include <unistd.h>
  #include <sched.h>

  /* Some globals */
  int can_use_mitshm = 1;
  int mitshm_major_code;
  int mitshm_minor_code;
  
  Display *d;
  XImage *ximg = NULL;
  GC gc;

  Window win;
  XColor xrgb[COLORS];
  XEvent ev;

  Colormap colormap;
	static int styleeTrueColor = 0 ;
	static int styleePrivate   = 1 ;
  unsigned long color[COLORS];


#endif


float desiredFPS=10.0;


int stylee;
int frameno;
JBmp img,img2,imgtmp;




// Memory leak is here
void wicked_blit() {

  #ifdef X11GFX
      if (can_use_mitshm)
				XShmPutImage (d, win, gc, ximg, 0, 0, 0, 0, WIN_W, WIN_H, True);
      else
				XPutImage (d, win, gc, ximg, 0, 0, 0, 0, WIN_W, WIN_H);
			XFlush(d);
	    XSync (d, 0); // this stopped it going mad
		  // wait(0.01);
		sched_yield();
			// printf(".\n");
  #endif
  #ifdef ALLEGRO
    img2.blit();
  #endif

}

#define pi 3.141593
#define uchar unsigned char

#define fs 12
#define palspeed 1
#define colspeed 1
#define framespermap 30

//r = .5 + 2 * t / pi + .5 * SIN(1.5 * t)

void perturb (int f);
//void mycircle(int cx,int cy,int r,int c);
void moremap();

// Maps used for background writing
#define amountmax 256
int amount[scrwid][scrhei][2][2][2];
int pix[scrwid][scrhei][2],piy[scrwid][scrhei][2];
//float *****amount;
//int ***pix,***piy;
int fon[fs];
float var[fs],op[fs],damp[fs],force[fs];
float toff=0;
int usingmap,makingmap,mmx,mmy,tmpmap,maploop;



void redocolors();




int huefor(float t,float o) {
  float ctmp;
  ctmp=myabs(t-o);
  if (ctmp>0.5)
    ctmp=(float)1-ctmp;
  // return 255*((float)0.5+0.5*cos(2*pi*ctmp));
  ctmp=ctmp*6;
  if (ctmp<1)
    return 255;
  if (ctmp>2)
    return 0;
  return 63*((float)2-ctmp);
}




	int f,k,c,ix,iy,displayloop;
  float thru,ctmp;



int init_candy(void) {

				frameno=0;
  register int i,j,x,y;
  

  srand((int)time(NULL));
  
  usingmap=0; makingmap=1; mmx=0; mmy=0;

  img=JBmp(scrwid,scrhei);
  img2=JBmp(scrwid,scrhei);

  for (y=0;y<scrhei;y++) {
    for (x=0;x<scrwid;x++) {
      // img.bmp[y][x]=256*y/scrhei;
      // img.bmp[y][x]=0;
      img.bmp[y][x]=255;
      if (x<scrwid-1 && y<scrhei-1) {
        pix[x][y][usingmap]=x;
        piy[x][y][usingmap]=y;
        for (i=0;i<=1;i++)
        for (j=0;j<=1;j++)
          amount[x][y][i][j][usingmap]=amountmax/4;
      }
    }
  }
  
  /* Originals from QB
     op[0] = 1; damp[0] = .999; force[0] = .005;
     op[1] = 1.02; damp[1] = .999; force[1] = .002;
     op[2] = 0; damp[2] = .999; force[2] = .002;
     op[3] = 1; damp[3] = .999; force[3] = .005;
     op[4] = 1; damp[4] = .999; force[4] = .005;
     op[5] = 0; damp[5] = .999; force[5] = .002;
  */
  
  /* From QB later
     name$(1) = "Velocity"
     op(1) = 1: damp(1) = .999: force(1) = .002
     name$(2) = "Rotation"
     op(2) = 0: damp(2) = .999: force(2) = .002
     name$(3) = "Drip"
     op(3) = 1: damp(3) = .999: force(3) = .005
     name$(4) = "Dribble"
     op(4) = 1: damp(4) = .999: force(4) = .005
     name$(5) = "Slide"
     op(5) = 0: damp(5) = .999: force(5) = .002
     name$(6) = "Accelerate"
     op(6) = 1: damp(6) = .999: force(6) = .005
     name$(7) = "xDisplace"
     op(7) = 0: damp(7) = .999: force(7) = .005
     name$(8) = "yDisplace"
     op(8) = 0: damp(8) = .999: force(8) = .005
     REM 9 and 10 are options for splitting displacements (no var)
     name$(9) = "2d/3d split"
     name$(10) = "Split"
  */
  
  // 0 Accelerate
  op[0] = 1; damp[0] = .9; force[0] = .01;
  // 1 Velocity
  op[1] = 1; damp[1] = .9; force[1] = .01;
  // 2 Rotation
  op[2] = 0; damp[2] = .9; force[2] = .02;
  // 5 x splurge
  op[5] = 0; damp[5] = .9; force[5] = .01;
  op[6]=2;damp[6]=.99;force[6]=.01;
  op[3] = 1; damp[3] = .9; force[3] = .01;
  op[4] = 1; damp[4] = .9; force[4] = .01;
  op[7]=1;damp[7]=.99;force[7]=.01;
  // Dribble
  op[8] = 1; damp[8] = .9; force[8] = .01;
  // Slide
  op[9] = 0; damp[9] = .9; force[9] = .01;
  // xDisplace
  op[10] = 0; damp[10] = .9; force[10] = .01;
  // yDisplace
  op[11] = 0; damp[11] = .9; force[11] = .01;
  
  for (f=0;f<fs;f++) {
    var[f] = op[f];
    fon[f]=1;
  }

  for (j=1;j<=10000;j++)
  for (i=0;i<fs;i++)
    perturb(i);

  for (i=0;i<=framespermap;i++) {
    moremap();
  }
  
  #ifdef ALLEGRO
    allegrosetup(scrwid,scrhei);
    // _farsetsel(screen->seg);
  #endif
  
  redocolors();

}
  





void plotsomeshapes() {

  for (int i=0;i<2;i++)
  	if (myrnd()<0.4) {
			V2d cen=V2d((float)scrwid*myrnd(),(float)scrhei*myrnd());
			V2d dir=V2d::random();
			V2d a=cen-dir*(scrwid);			
			V2d b=cen+dir*(scrwid);			
      // img.line(a.x,a.y,b.x,b.y,255);
      img.thickline(a.x,a.y,b.x,b.y,1,255);
			// float oth=sqrt(myrnd())*5.0;
			// V2d perp=oth*dir.perp();
			// img.filltri((a-perp).x,(a-perp).y,(a+perp).x,(a+perp).y,b.x,b.y,myrnd()*255);
			
			// img.line(a.x,a.y,b.x,b.y,255);
		}
			
}



long lasttime=0;
// uclock performs significantly worse!

void timesync() {
  if (lasttime==0)
    lasttime=clock();
  // printf("%f\n",(float)lasttime/(float)CLOCKS_PER_SEC);
  long watchtime;
  while ((watchtime=clock())<lasttime+CLOCKS_PER_SEC/desiredFPS)
		{
			// Sould release the processor
		}
  lasttime=lasttime+CLOCKS_PER_SEC/desiredFPS;
}
	


void iterate_candy() {

		int ka,kb,la,lb;
		int nw,ne,se,sw;
		int n,e,s,w;
		int mix;
		#define mixmax 256
  register int i,j,x,y;
				
    moremap();

		float xmapping=(float)(WIN_W)/(float)(scrwid);
		float ymapping=(float)(WIN_H)/(float)(scrhei);
    
    // Animate
    for (x=0; x<scrwid; x++) {
      for (y=0; y<scrhei; y++) {
        c=0;
        for (i=0;i<=1;i++) {
          for (j=0;j<=1;j++) {
            c=c+(amount[x][y][i][j][usingmap]*img.bmp[piy[x][y][usingmap]+j][pix[x][y][usingmap]+i]);
          }
        }
        c=(c >> 8)-colspeed;
        if (c<0)
          c=0;
        img2.bmp[y][x]=c;

				// if (x>0 && y>0) {
					// nw=img2.bmp[y-1][x-1];
					// ne=img2.bmp[y-1][x];
					// se=img2.bmp[y][x];
					// sw=img2.bmp[y][x-1];
			    // ka=x*xmapping; la=y*ymapping;
			    // kb=(x+1)*xmapping; lb=(y+1)*ymapping;
					// for (i=ka;i<kb;i++)
					// for (j=la;j<lb;j++) {
						// e=mixmax*(i-ka)/(kb-ka);
						// s=mixmax*(j-la)/(lb-la);
						// w=mixmax-e;
						// n=mixmax-s;
						// mix= (se*s*e+sw*s*w+nw*n*w+ne*n*e) /mixmax/mixmax;
						// // printf("%i\t%i\n",mix,nw);
						// mix=(mix+palspeed*frameno)%colors;
						// // mix=nw;
						// ximg->f.put_pixel(ximg,i,j,xrgb[mix].pixel);
					// }
				// }

				// Simple posterisation
				// for (i=ka;i<kb;i++)
				// for (j=la;j<lb;j++)
					// ximg->f.put_pixel(ximg,i,j,xrgb[c].pixel);

        #ifdef X11GFX
          // Fake palette switching
          c=(c+palspeed*frameno)%colors;
          ximg->f.put_pixel(ximg,x,y,xrgb[c].pixel);
        #endif
				
				// Doesn't work even in 256-col mode:
				// ximg->data[i+j*scrwid]=xrgb[c].pixel;
				
      }
    }

    #ifdef ALLEGRO
      redocolors(); // Not fake palette switching
    #endif

		// for (x=0;x<WIN_W;x++)
			// for (y=0;y<WIN_H;y++) {
				// i=x*scrwid/WIN_W;
				// j=y*scrhei/WIN_H;
				// c=(img2.bmp[j][i]+palspeed*frameno)%colors;
				// ximg->f.put_pixel(ximg,x,y,xrgb[c].pixel);
			// }
				
    wicked_blit();
		timesync();
  
    toff=toff-(float)palspeed/256;
    imgtmp=img;
    img=img2;
    img2=imgtmp;
    plotsomeshapes();
    framedone();
		frameno++;

		// redocolors();
  
}



void perturb (int f) {
  var[f] = (var[f] - op[f]) * damp[f] + force[f] * (myrnd() - .5) + op[f];
}



/*void mycircle(int cx,int cy,int r,int c) {
  int x,h,y,px,py;
  for (x=-r;x<=r;x++) {
    h=sqrt(mysquare(r)-mysquare(x));
    for (y=-h;y<=h;y++) {
      px=cx+x; py=cy+y;
      if (px>=0 && px<scrwid && py>=0 && py<scrhei)
        img[py][px]=myrnd()*256;
    }
  }
}*/



void moremap() {
  float rx,ry,nrx,nry,px,py;
  int f,i,j,k,c,x,y,ix,iy,displayloop;
  // Generate some more of the map
  for (maploop=1;maploop<scrwid*scrhei/framespermap;maploop++) {
    rx=2.0*(float)mmx/(float)scrwid-1.0;
    ry=2.0*(float)(mmy-scrhei/2)/(float)scrwid;
    
    if (fon[8]) {
      ry=(ry-1)/var[8]+1;
    }
    if (fon[9]) {
      rx=rx+var[9]*rx;
    }
    if (fon[10]) {
      rx=rx+var[10];
    }
    if (fon[11]) {
      ry=ry+var[11];
    }
    if (fon[0]) {
			rx = rx + (var[7]-1.0)*0.2 * sin((20.0+30.0*(var[8]-1.0))*ry);
			ry = ry - (var[7]-1.0)*0.2 * sin((20.0+30.0*(var[8]-1.0))*rx);
    }
    if (fon[1]) {
      rx = rx / var[1]; ry = ry / var[1];
    }
    if (fon[2]) {
      nrx = rx * cos(var[2]) + ry * sin(var[2]);
      nry = -rx * sin(var[2]) + ry * cos(var[2]);
      rx = nrx; ry=nry;
    }
    if (fon[3]) {
      // ry = ry + mysgn(ry) * sin(var[6]*pi*myabs(ry)) * var[3];
			rx = rx + (var[4]-1.0)*0.3 * sin((20.0+30.0*(var[3]-1.0))*ry);
    }
    if (fon[5]) {
      // rx = rx + mysgn(rx) * sin(var[6]*pi*myabs(rx)) * var[5];
			ry = ry + (var[3]-1.0)*0.2 * sin((20.0+30.0*(var[4]-1.0))*rx);
    }
/*    if (fon[4]) {
      ry = ((myabs(ry) - 1) / var[4] + 1) * mysgn(ry);
    }*/
    px=(rx+1)/2*scrwid;
    py=scrhei/2+(ry)/2*scrwid;
    ix=(int)px;
    iy=(int)py;
    if (ix<0 || ix>=scrwid-1 || iy<0 || iy>=scrhei-1) {
      ix=px; iy=py;
    }
    amount[mmx][mmy][0][0][makingmap]=amountmax*((float)ix+1-(float)px)*((float)(iy+1)-(float)py);
    amount[mmx][mmy][1][0][makingmap]=amountmax*((float)px-(float)ix)*((float)(iy+1)-(float)py);
    amount[mmx][mmy][0][1][makingmap]=amountmax*((float)ix+1-(float)px)*((float)py-(float)iy);
    amount[mmx][mmy][1][1][makingmap]=amountmax*((float)px-(float)ix)*((float)py-(float)iy);
    pix[mmx][mmy][makingmap]=ix;
    piy[mmx][mmy][makingmap]=iy;
    if (ix<0 || ix>=scrwid-1 || iy<0 || iy>=scrhei-1) {
/*      pix[mmx][mmy][makingmap]=scrwid/2;
      piy[mmx][mmy][makingmap]=scrhei/2;
      for (i=0;i<=1;i++) {
        for (j=0;j<=1;j++) {
          amount[mmx][mmy][i][j][makingmap]=0;
        }
      }*/
/*      pix[mmx][mmy][makingmap]=myrnd()*(scrwid-1);
      piy[mmx][mmy][makingmap]=myrnd()*(scrhei-1);
      for (i=0;i<=1;i++) {
        for (j=0;j<=1;j++) {
          amount[mmx][mmy][i][j][makingmap]=myrnd()/4;
        }
      }*/
      pix[mmx][mmy][makingmap]=( mmx<scrwid-1 ? mmx : mmx-1 );
      piy[mmx][mmy][makingmap]=( mmy<scrhei-1 ? mmy : mmy-1 );
			float splitall=myrnd();
			float splitleft=myrnd();
			float splitright=myrnd();
      amount[mmx][mmy][0][0][makingmap]=amountmax/4;
      amount[mmx][mmy][1][0][makingmap]=amountmax/4;
      amount[mmx][mmy][0][1][makingmap]=amountmax/4;
      amount[mmx][mmy][1][1][makingmap]=amountmax/4;
    }
    mmx++;
    if (mmx>=scrwid) {
      mmx=0;
      mmy++;
      if (mmy>=scrhei) {
        mmy=0;
        tmpmap=usingmap;
        usingmap=makingmap;
        makingmap=tmpmap;
        int rep=4;
        if (myrnd()<0.2)
          rep=200;
        for (int r=1;r<=rep;r++) {
        for (f=0;f<fs;f++) {
          perturb(f);
/*          if (myabs(var[f]-op[f])<force[f])
            fon[f]=( myrnd()<0.6 ? 1 : 0 );*/
        }
        }
      }
    }
  }
}










		
// (Probably inefficient) palette switching:
		
void redocolors() {
										
  int i;
  for (i = 0; i < COLORS; i++) {	/* Set up the colors gradient */
		// myRGB r=myRGB(thru,thru,thru);
		// myRGB r=( thru < 1 ? myRGB(0.0,thru*5.0,0.0)
						             // : myRGB(thru*2.0-1.0,1.0,thru*2.0-1.0)
						// );
    float thru=4.0*(float)((frameno*palspeed+i)%colors)/(float)COLORS;
		myRGB r;
		if (thru<1.0)
			r=myRGB(0.0,thru,0.0);
		else if (thru<2.0)
			r=myRGB(thru-1.0,1.0,thru-1.0);
		else if (thru<3.0)
			r=myRGB(1.0-(thru-2.0),1.0-(thru-2.0),1.0);
		else
			r=myRGB(0.0,0.0,1.0-(thru-3.0));

    #ifdef X11GFX
      xrgb[i].red = 65535 * r.r/255; // (1.0 - 1.0 * i / COLORS);
      xrgb[i].green = 65535 * r.g/255;
      xrgb[i].blue = 65535 * r.b/255;
      xrgb[i].flags = DoRed | DoGreen | DoBlue;
      xrgb[i].pixel=i;
    #endif

    #ifdef ALLEGRO
      RGB algrgb;
      algrgb.r=r.r/4;
      algrgb.g=r.g/4;
      algrgb.b=r.b/4;
      set_color(i,&algrgb);
    #endif

  }

    #ifdef X11GFX
			XAllocColorCells(d,colormap,1,0,0,color,colors);

			if (stylee == styleeTrueColor) {
        // XInstallColormap (d, colormap);
			  for (int i=0;i<colors;i++) {
				  if ( XAllocColor(d,colormap,&xrgb[i]) == 0 )
					  printf("Error allocating %i\n",i);
				  // works for true-color
			  }
			} else {
			  // for (int i=0;i<colors;i++) {
				  // xrgb[i].pixel=color[i];
			  // }
			  XStoreColors(d,colormap,xrgb,COLORS);
			  XSetWindowColormap(d,win,colormap);
        // fputs("Couldn't allocate enough colors cells.\n", 
        // stderr), exit(1);
			}

    #endif

}





void real_main() {

	init_candy();

  bool should_quit = false;
	
  starttimer();

  while (!should_quit) {

    iterate_candy();

    #ifdef X11GFX
    bool ev_occ=XCheckMaskEvent(d,ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask,
															&ev);
		if (ev_occ) {
			switch (ev.type) {
				case Expose: 			break;
				case ButtonPress: should_quit=true; break;
				case KeyPress: 		break;
				default: 					break;
			}
		}
    #endif

    #ifdef ALLEGRO
      if (key[KEY_ESC] || key[KEY_SPACE])
        should_quit=true;
    #endif
	
  }

  savetimer();

  #ifdef ALLEGRO
    allegro_exit();
  #endif

	displayframespersecond();

}

//  while(!key[KEY_ESC] && !key[KEY_SPACE]) {
 
// while(!keypressed()) {





#ifdef X11GFX

  /* error handler to catch MITSHM problems */
  int mitshm_handler (Display * d, XErrorEvent * ev)
  {
    can_use_mitshm = 0;
    return 0;
  }

  XShmSegmentInfo shminfo;
  int screen;
  XVisualInfo vis, *vlist;
  int match;
  struct rusage resource_utilization;
  int shared_pixmaps;		/* dummy */

  int get_xvisinfo_class(XVisualInfo x) {
					return x.c_class;
					  // return 1;
				  }
  	
  int main (void)
  {
    int i;
  
    int allocateOK;
  
    ximg = NULL;
  
  	
    d = XOpenDisplay (NULL);
  
    if (!d)
      fputs ("Couldn't open display\n", stderr), exit (1);
  
    screen = DefaultScreen (d);
    gc = DefaultGC (d, screen);
  
    /* Find a visual */
  
    vis.screen = screen;
    vlist = XGetVisualInfo (d, VisualScreenMask, &vis, &match);
  
    if (!vlist)
      fputs ("No matched visuals\n", stderr), exit (1);
  
    vis = vlist[0];
    XFree (vlist);
  
    if (vis.colormap_size < COLORS)
      fputs ("Colormap is too small.\n", stderr); // , exit (1);
  
  
    win = XCreateSimpleWindow (d, DefaultRootWindow (d),
			       0, 0, WIN_W, WIN_H, 0,
			       WhitePixel (d, screen), BlackPixel (d, screen));
  
	  int xclass=get_xvisinfo_class(vis);
		printf("class = %i\n",xclass);
	  stylee = ( vis.depth > 8 ? styleeTrueColor : styleePrivate );
	  printf("stylee=%i\n",stylee);
  
    if ( get_xvisinfo_class(vis) % 2 == 1) {	/* The odd numbers can redefine colors */
  
			  // printf("%i\n",get_xvisinfo_class(vis));
	  	
      colormap = DefaultColormap (d, screen);
		  Visual *defaultVisual=DefaultVisual(d,screen);
	  	
      /* Allocate cells */
      allocateOK = (XAllocColorCells (d, colormap, 1,
							      NULL, 0, color, COLORS) != 0);

			printf("Allocated OK? %i\n",allocateOK);
			
      if (allocateOK) {
  
			  printf("Allocated OK\n");
        // This doesn't work for installed colormap!
  
        /* Modify the colorcells */
        for (i = 0; i < COLORS; i++)
				  xrgb[i].pixel = color[i];
  
        XStoreColors (d, colormap, xrgb, COLORS);
  
		  } else {

			  colormap = XCreateColormap(d,win,defaultVisual,AllocNone);
		  	
    	  // redocolors();
					  	
      }
  
		  // black = XBlackPixel(d,screen);
		  // white = XWhitePixel(d,screen);
  
    } else if ( get_xvisinfo_class(vis) == TrueColor) {
					  printf("TrueColor %i = %i\n",xclass,TrueColor);
      /* This will lookup the color and sets the xrgb[i].pixel value */
      for (i = 0; i < COLORS; i++)
        XAllocColor (d, colormap, &xrgb[i]);
    } else
      fprintf (stderr, "Not content with visual class %d.\n",
	       get_xvisinfo_class(vis) ), exit (1);
  
    /* Find out if MITSHM is supported and useable */
    printf ("MITSHM: ");
  
    if (XShmQueryVersion (d, &mitshm_major_code,
			  &mitshm_minor_code, &shared_pixmaps)) {
      int (*handler) (Display *, XErrorEvent *);
      ximg = XShmCreateImage (d, vis.visual,
			     vis.depth, XShmPixmapFormat (d),
			     NULL, &shminfo, WIN_W, WIN_H);
      shminfo.shmid = shmget (IPC_PRIVATE,
			      ximg->bytes_per_line * ximg->height,
			      IPC_CREAT | 0777);
      shminfo.shmaddr = (char *)shmat (shminfo.shmid, 0, 0);
		  ximg->data = (char *)shminfo.shmaddr;
  
      handler = XSetErrorHandler (mitshm_handler);
      XShmAttach (d, &shminfo);	/* Tell the server to attach */
      XSync (d, 0);
      XSetErrorHandler (handler);
  
      shmctl (shminfo.shmid, IPC_RMID, 0);
      /* Mark this shm segment for deletion at once. The segment will
       * automatically become released when both the server and this
       * client have detached from it.
       * (Process termination automagically detach shm segments) */
  
      if (!can_use_mitshm) {
        shmdt (shminfo.shmaddr);
        ximg = NULL;
      }
    }
  
    if (ximg == NULL) {
      can_use_mitshm = 0;
      /* XInitImage(ximg); */
      ximg = XCreateImage (d, vis.visual, vis.depth, ZPixmap,
			  0, (char *)malloc (WIN_W * WIN_H), WIN_W, WIN_H, 8, 0);
    }
  
    if (can_use_mitshm)
      printf ("YES!\n");
    else
      printf ("NO, using fallback instead.\n");
  
    // DrawFractal (ximg,xrgb);
  
    XSelectInput (d, win, ButtonPressMask | ExposureMask);
    XMapWindow (d, win);
  
  
  
  
    real_main();


  
      // XNextEvent (d, &ev);
      // switch (ev.type) {
      // case ButtonPress:
        // should_quit = 1;
        // break;
      // case Expose:
        // if (can_use_mitshm)
				  // XShmPutImage (d, win, gc, img, 0, 0, 0, 0, WIN_W, WIN_H, True);
        // else
				  // XPutImage (d, win, gc, img, 0, 0, 0, 0, WIN_W, WIN_H);
        // break;
      // default:
			  // break;
      // }
  
  
  
    if (get_xvisinfo_class(vis) % 2 == 1 || get_xvisinfo_class(vis) == TrueColor) {
      unsigned long color[COLORS];
  
      if (allocateOK) {
        for (i = 0; i < COLORS; i++)
				  color[i] = xrgb[i].pixel;
        XFreeColors (d, colormap, color, COLORS, 0);
      }				/* Allocated colors freed */
    } else {
      XUninstallColormap (d, colormap);
    }
  
    if (can_use_mitshm) {
      XShmDetach (d, &shminfo);	/* Server detached */
      XDestroyImage (ximg);	/* Image struct freed */
      shmdt (shminfo.shmaddr);	/* We're detached */
    } else
      XDestroyImage (ximg);	/* Image struct freed */
  
    XDestroyWindow (d, win);	/* Window removed */
    XCloseDisplay (d);		/* Display disconnected */
  
    /* So you can see how your computer compares to your friend's */
    getrusage (RUSAGE_SELF, &resource_utilization);
		float seconds=(float)resource_utilization.ru_utime.tv_sec
						     +(float)resource_utilization.ru_utime.tv_usec*0.000000001;
		printf("CPU seconds per frame: %f\n",seconds/(float)frameno);
    // printf ("CPU seconds consumed: %ds and %dµs\n",
	    // (int) resource_utilization.ru_utime.tv_sec,
	    // (int) resource_utilization.ru_utime.tv_usec);
  
    return 0;
  }
  




#else


  void main() {
    real_main();
  }


#endif





