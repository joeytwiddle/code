/*
done: Efficient interpolation (asm?) of BG (interpolate D,a space rather than color space?)
      (Interpolation done but not in asm)
      A better description is to say we are interpolating in the image plane
      using bgtexture position, rather than colour
      It means we can do less work or plot pixels more correctly for the same amount of work.
done: Interpolate inter-pixel on the bgtexture plane. Slow.
process: Any macros which use a provided expression N times
  should be turned into functions if, in terms of time,
  N*eval(expression) > 1*alloc(type)+1*eval(expression)+N*refer(type)
  (This will be done like BR and TR I guess.)
  Or does the C compiler do this automatically, assuming state of
  expression does not change during macro.
  Benchmark test?
todo: Resolve palette==NULL at init and switch routing of code to be execed.
  Or make it part of the SDLwrap model.
  Or just ignore it because we will only stand for truecolour.
halfdone: Both BR and TR should be given functions to perform on the pixel register
  rather than x,y.  This will allow the algorithms to perform fast
  reg++'s rather than the given function performing reg=big_calculation(x,y)
  (Actually keep passing x,y just in case the plot function wants to use it.)
  [ 19/3/02 Attempted for TR ]
process: Are we ever forced to blit on a 16bit surface?
  If so, would it be efficient to use a hidden 32bit surface to do
  read/writes for transparency, and then blit via the 16bit surface?
process: Is there in general a better local surface we can use for lookups
  before finally writing to the screen?
process: When rendering two overlapping triangles, the result is three
  polygons: two containing the pure colours, and one the merged colour.
  We can avoid reading all the pixels from the map if we find the triangle
  intersections first, and then render the resulting polygons with
  the merged colours pre-calculated.
  (In fact only one pixel read is needed, for the background.)
  This requires 2D "CSG".
todo: Of course, stop using full colour and start cheating.
  eg. Use transparencies of one colour (essentially greyscale),
  or OR otherwise limit bits as Julian suggests.
todo: Store #define constants for a variety of video modes so that the
  user may choose at runtime.  (erm what? how?!)
*/

// #define DOS
// #define DO_FULLSCREEN
// #define SCRWID 320
// #define SCRHEI 200
// #define NUMTRIS 400
// #define brightness 8
#define SCRWID 640
#define SCRHEI 480
#define NUMTRIS 100
#define brightness 30
#define SCRBPS 32
#define desiredFramesPerSecond 50
#define IMGSKIP (SCRHEI/40)
// #define IMGSKIP 4
#define DO_TRIANGLES
// #define DO_STARS
#define DO_WHITEOUT
#define BGQUANT 8
// #define INTERPOLATE_BG_FULL
#define ROTSYM180
// #define POSTERISE
#define TRISINGOFFSET 16

#ifdef DOS
#define M_PI 3.14159265358979323846
#include "SDL.h"
#define DEBUGTOFILE
#else
#include <SDL.h>
// #include <sched.h>
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>

#include "define.c"
#define CONST
#include "incl.c"
#undef CONST

#ifdef DEBUGTOFILE
FILE *output;
#else
#define output stderr
#endif

#define new(t,c) ((t *)malloc(sizeof(t)*c))
#define sgn(x) ((x)>0.0?1.0:-1.0)
/* 0.0 <= frand() < 1.0 */
#define frand() ((float)(rand()/2)/(float)(RAND_MAX/2+1))
// #define nearhalf() (frand())
#define nearhalf() (0.5+0.8*(frand()-0.5)*frand())
#define cap(x,b) ( (x) > (b) ? (b) : (x) )
#define square(x) ( (x) * (x) )
#define clip(x,a,b) ( (x)<(a) ? (a) : (x)>=(b) ? (b)-1 : (x) )
#define sinpow(x,y) ( 2.0*(pow((qsin(x)+1.0)/2.0,y)-0.5) )
#define sinsharp(x) ( 1.0-2.0*fabs((sin(x))) )

// Warning: only supports truecolour palettes.
#define SDLwrap_MapRGB(Rloss,Rshift,Gloss,Gshift,Bloss,Bshift,Amask,r,g,b) ( ((Uint8)(r) >> Rloss) << Rshift | ((Uint8)(g) >> Gloss) << Gshift | ((Uint8)(b) >> Bloss) << Bshift | Amask )
#define SDLwrap_GetRGB(Rmask,Rshift,Rloss,Gmask,Gshift,Gloss,Bmask,Bshift,Bloss,pixel,r,g,b) { unsigned rv, gv, bv; rv = ((pixel) & Rmask) >> Rshift; *(r) = (rv << Rloss) + (rv >> (8 - Rloss)); gv = ((pixel) & Gmask) >> Gshift; *(g) = (gv << Gloss) + (gv >> (8 - Gloss)); bv = ((pixel) & Bmask) >> Bshift; *(b) = (bv << Bloss) + (bv >> (8 - Bloss)); }

// attempt: Presumably we needn't even bother doing the masks, shifts etc.
//   for RGB mapping in 32 bit modes.
//   Maybe by bypassing these and just casting Uint8's we can get more speed!
// It seems to work for GetRGB.  32bit has now overtaken 16bit at 320x200 or below.
#define q32_GetRGB(pixel,r,g,b) { *(r)=(Uint8)((pixel)>>16); *(g)=(Uint8)((pixel)>>8); *(b)=(Uint8)((pixel)); }
// #define q32_GetRGB(pixel,r,g,b) { *(r)=(Uint8)(*((&(pixel))+1)); *(g)=(Uint8)(*((&(pixel))+2)); *(b)=(Uint8)(*((&(pixel)))+3); }
// But I couldn't get a faster version of SDLwrap above
// #define q32_MapRGB(r,g,b) ( ((Uint8)(r)<<16) | ((Uint8)(g)<<8) | (Uint8)(b) )
// #define q32_MapRGB(r,g,b) ( (((unsigned)(r) & 0xff)<<16) | (((unsigned)(g) & 0xff)<<8) | ((unsigned)(b) & 0xff) )

#define SDLwrap_getPixel(sdl_surface,pixelType,pitch,bytesperpixel,x,y) (SDLwrap_regPixel((sdl_surface),pixelType,(pitch),(bytesperpixel),(x),(y)))
#define SDLwrap_setPixel(sdl_surface,pixelType,pitch,bytesperpixel,x,y,c); {SDLwrap_regPixel((sdl_surface),pixelType,(pitch),(bytesperpixel),(x),(y))=(c);}
#define SDLwrap_regPixel(sdl_surface,pixelType,pitch,bytesperpixel,x,y) (*(pixelType *)((Uint8 *)((sdl_surface)->pixels)+(y)*(pitch)+(x)*(bytesperpixel)))
// #define SDLwrap_regPixel(sdl_surface,pixelType,pitch,bytesperpixel,x,y) (*(Uint32 *)((Uint8 *)((sdl_surface)->pixels)+(y)*(pitch)+(x)*(bytesperpixel)))

#define triglookupmax 65536
float coslookup[triglookupmax];
#define qcos(x) (coslookup[abs(((x)*(int)(triglookupmax/2.0/M_PI)))%triglookupmax])
// #define qcos(x) cos(x)
#define qsin(x) (qcos(x-M_PI/2.0))
void setuptriglookup() {
	int i;
	// coslookup=new(float,triglookupmax);
	for (i=0;i<triglookupmax;i++)
		coslookup[i]=cos((float)i*2.0*M_PI/(float)triglookupmax);
}

#define sqrtlookupmax 65536
#define sqrtlookupquant 8
Uint32 sqrtlookup[sqrtlookupmax];
void setupsqrtlookup() {
	int i;
	for (i=0;i<sqrtlookupmax;i++)
		sqrtlookup[i]=sqrt(i*sqrtlookupquant);
}
// Shows you if your max/quant are too small
// #define qsqrt(x) ( (x)<0 ? 0 : (x)>=sqrtlookupmax*sqrtlookupquant ? sqrtlookup[sqrtlookupmax-1] : sqrtlookup[(int)(x)/sqrtlookupquant] )
// Safe version
// #define qsqrt(x) ( (x)<0 ? 0 : (x)>=sqrtlookupmax*sqrtlookupquant ? sqrt(x) : sqrtlookup[(int)(x)/sqrtlookupquant] )
// Unsafe version
#define qsqrt(x) ( sqrtlookup[(x)/sqrtlookupquant] )
// One of those funky accidents - notice the curvature
// #define qsqrt(x) ( sqrtlookup[(Sint16)x] )
// #define qsqrt(x) sqrt(x)

#define hypotlookupmax 65536
Uint32 hypotlookup[hypotlookupmax+1];
void setuphypotlookup() {
	int i;
	for (i=0;i<hypotlookupmax+1;i++) {
		float fakex=i;
		float fakey=hypotlookupmax+1;
		hypotlookup[i]=sqrt(square(fakex)+square(fakey));
		//fprintf(output,"%i\n",hypotlookup[i]);
	}				 						
}
#define qhypot(x,y) ( (x) < 2 ? y : (y) < 2 ? x : (x) > (y) ? ((Uint32)(x)*hypotlookup[(Uint32)(y)*hypotlookupmax/(x)]) >> 16 : ((Uint32)(y)*hypotlookup[(Uint32)(x)*hypotlookupmax/(y)]) >> 16 )

void setuplookups() {
	setuptriglookup();
	setupsqrtlookup();
	setuphypotlookup();
}

/*
	void plotTri(SDL_Surface *screen,int TR_x1,int TR_y1,int TR_x2,int TR_y2,int TR_x3, int TR_y3, Uint32 colour) {
#define TR_todo(x,y) {setPixel(screen,x,y,colour);}
// #define TR_todo(x,y) { if (x>=0 && x<screen_w && y>=0 && y<screen_h) {setPixel(screen,x,y,colour);} }
// #define TR_todo(x,y) printf("Hello %i %i\n",x,y); setPixel(screen,x,y,colour);
#include "tri.c"
#undef TR_todo
plotLine(screen,TR_x1,TR_y1,TR_x2,TR_y2,MapRGB(screen->format,255,255,255));
plotLine(screen,TR_x1,TR_y1,TR_x3,TR_y3,MapRGB(screen->format,255,255,255));
plotLine(screen,TR_x2,TR_y2,TR_x3,TR_y3,MapRGB(screen->format,255,255,255));
}
*/

// Slight improvement using this fn
	inline void merge_reg(Uint8 *pix,Uint8 dr,Uint8 dg,Uint8 db) {
#define redreg (pix[0])
#define greenreg (pix[1])
#define bluereg (pix[2])
		redreg=(redreg<255-dr?redreg+dr:255);
		greenreg=(greenreg<255-dg?greenreg+dg:255);
		bluereg=(bluereg<255-db?bluereg+db:255);
	}

/* inline screen_pixelType merge_pixel(screen_pixelType p,Uint8 dr,Uint8 dg,Uint8 db) {
Uint8 r,g,b;
Uint16 fr,fg,fb;
screen_GetRGB(p,&r,&g,&b);
// printf("    %i %i %i\n",r,g,b);
// printf("  + %i %i %i\n",dr,dg,db);
fr=r+dr;
fg=g+dg;
fb=b+db;
// note: Sum of A B will exceed 255 if from top bit down either both bits are set or one bit is set, check next (worst case 00000001+11111111)
// process: In asm, don't we get a register bit set if there was an overflow?
// p= SDL_MapRGB(screen->format,fr,fg,fb);
return screen_MapRGB(fr>255?255:fr,fg>255?255:fg,fb>255?255:fb);
}

inline void merge(SDL_Surface *screen,int x,int y,Uint8 dr,Uint8 dg,Uint8 db) {
// Uint32 p=getPixel(screen,x,y);
// Uint32 p=screen_getPixel(x,y);
screen_pixelType p=screen_getPixel(x,y);
// printf("%i\n",p);
// SDL_GetRGB(p,(screen->format),&r,&g,&b);
screen_setPixel(x,y,merge_pixel(p,dr,dg,db));
}
*/

#define halfClipLine(XA,YA,XB,YB,LX,HX,LY,HY) { if (XA<LX) { YA = YA + (YB-YA)*(LX-XA)/(XB-XA); XA = LX; } if (XA>=HX) { YA = YA + (YB-YA)*(HX-XA)/(XB-XA); XA = HX-1; } if (YA<LY) { XA = XA + (XB-XA)*(LY-YA)/(YB-YA); YA = LY; } if (YA>=HY) { XA = XA + (XB-XA)*(HY-YA)/(YB-YA); YA = HY-1; } }
// #define halfClipLine(XA,YA,XB,YB,LX,HX,LY,HY) { if ((*(XA))<LX) { (*(YA)) = (*(YA)) + ((*(YB))-(*(YA)))*(LX-(*(XA)))/((*(XB))-(*(XA))); (*(XA)) = LX; } if ((*(XA))>=HX) { (*(YA)) = (*(YA)) + ((*(YB))-(*(YA)))*(HX-(*(XA)))/((*(XB))-(*(XA))); (*(XA)) = HX-1; } if ((*(YA))<LY) { (*(XA)) = (*(XA)) + ((*(XB))-(*(XA)))*(LY-(*(YA)))/((*(YB))-(*(YA))); (*(YA)) = LY; } if ((*(YA))>=HY) { (*(XA)) = (*(XA)) + ((*(XB))-(*(XA)))*(HY-(*(YA)))/((*(YB))-(*(YA))); (*(YA)) = HY-1; } }
// #define clipLine(XA,YA,XB,YB,LX,HX,LY,HY) { halfClipLine(XA,YA,XB,YB,LX,HX,LY,HY); halfClipLine(XB,YB,XA,YA,LX,HX,LY,HY); }
#define clipLine(XA,YA,XB,YB,LX,HX,LY,HY) { halfClipLine(XA,YA,XB,YB,LX,HX,LY,HY); halfClipLine(XB,YB,XA,YA,LX,HX,LY,HY); }

// #define minmaxinto(x,y,v,w) { if ((x)<(y)) { v=(x); w=(y); } else { v=(y); w=(x); } }
#define minmaxinto(x,y,v,w) { typeof(v) xa=(x), ya=(y); if ((xa)<(ya)) { v=(xa); w=(ya); } else { v=(ya); w=(xa); } }

void plotTriRGB(SDL_Surface *screen,int TR_x1,int TR_y1,int TR_x2,int TR_y2,int TR_x3, int TR_y3, Uint8 r,Uint8 g,Uint8 b) {
	// Segfaults!  Bad interpolation causes overflow?
	// clipLine(TR_x1,TR_y1,TR_x2,TR_y2,0,SCRWID,0,SCRHEI);
	// clipLine(TR_x3,TR_y3,TR_x2,TR_y2,0,SCRWID,0,SCRHEI);
	// clipLine(TR_x1,TR_y1,TR_x3,TR_y3,0,SCRWID,0,SCRHEI);
	if (
			TR_x1>=0 && TR_x1<SCRWID &&
			TR_x2>=0 && TR_x2<SCRWID &&
			TR_x3>=0 && TR_x3<SCRWID &&
			TR_y1>=0 && TR_y1<SCRHEI &&
			TR_y2>=0 && TR_y2<SCRHEI &&
			TR_y3>=0 && TR_y3<SCRHEI
		) {
		// #define forminmax(xa,xb,TR_todo) { Uint32 *reg,*endreg; minmaxinto(&SDLwrap_regPixel(screen,screen_pixelType,screen_pitch,screen_BytesPerPixel,xb,y),&SDLwrap_regPixel(screen,screen_pixelType,screen_pitch,screen_BytesPerPixel,xa,y),reg,endreg);  for (; reg <= endreg; reg += 1) { *reg=merge_pixel(*reg,r,g,b); } }
		// #define forminmax(xa,xb,TR_todo) { Uint32 *reg,*endreg; minmaxinto(&SDLwrap_regPixel(screen,screen_pixelType,screen_pitch,screen_BytesPerPixel,xb,y),&SDLwrap_regPixel(screen,screen_pixelType,screen_pitch,screen_BytesPerPixel,xa,y),reg,endreg);  for (; reg < endreg; reg += 1) { *reg=merge_pixel(*reg,r,g,b); } }
#define forminmax(xa,xb,TR_todo) { Uint32 *reg,*endreg; minmaxinto(&SDLwrap_regPixel(screen,screen_pixelType,screen_pitch,screen_BytesPerPixel,xb,y),&SDLwrap_regPixel(screen,screen_pixelType,screen_pitch,screen_BytesPerPixel,xa,y),reg,endreg);  for (; reg < endreg; reg += 1) { merge_reg((Uint8 *)reg,r,g,b); } }
#define TR_todo(x,y) merge(screen,x,y,r,g,b);
#include "tri.c"
#undef TR_todo
	}
	// plotLine(screen,TR_x1,TR_y1,TR_x2,TR_y2,screen_MapRGB(255,255,255));
	// plotLine(screen,TR_x1,TR_y1,TR_x3,TR_y3,screen_MapRGB(255,255,255));
	// plotLine(screen,TR_x2,TR_y2,TR_x3,TR_y3,screen_MapRGB(255,255,255));
}

void plotLine(SDL_Surface *screen,int BR_x1,int BR_y1,int BR_x2,int BR_y2,Uint32 colour) {
	// #define BR_todo(x,y)  ((int *)screen->pixels)[y*screen_w+x]=colour
	// #define BR_todo(x,y)  setPixel(screen,x,y,colour);
	// printf("(%i,%i) - (%i,%i)\n",BR_x1,BR_y1,BR_x2,BR_y2);
	clipLine(BR_x1,BR_y1,BR_x2,BR_y2,0,SCRWID,0,SCRHEI);
	// printf(" (%i,%i) - (%i,%i)\n",BR_x1,BR_y1,BR_x2,BR_y2);
	if (
			BR_x1>=0 && BR_x1<SCRWID &&
			BR_x2>=0 && BR_x2<SCRWID &&
			BR_y1>=0 && BR_y1<SCRHEI &&
			BR_y2>=0 && BR_y2<SCRHEI
		) {
#define BR_todo(x,y)  screen_setPixel(x,y,colour);
#include "bresenham.c"
#undef BR_todo(x,y)
	}
}

int main(int argc,char *argv[]) {

	SDL_Surface *screen,*bgtexture,*bgtexture2;
	SDL_Event event;
	int keepLooping=1;
	int frames=0;
	float framebits;
	clock_t starttime;
	FILE *fp;
	Uint16 cylcenx=SCRWID/2,cylceny=SCRHEI/2;
	Uint32 *bgpixels;
	int start_clock;

#ifdef DEBUGTOFILE
	output=fopen("debug.txt","wa");
#endif

	setuplookups();
	// srand(time(NULL));

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr,"Couldn't initialise SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	SDL_WM_SetCaption("transience",NULL);
#ifdef DO_FULLSCREEN
#define XTRASDLVMBITS | SDL_FULLSCREEN
#else
#define XTRASDLVMBITS
#endif
	screen=SDL_SetVideoMode(SCRWID,SCRHEI,SCRBPS, SDL_HWPALETTE XTRASDLVMBITS );
	if (screen==NULL) {
		fprintf(stderr,"Couldn't initialise video.\n");
		return 1;
	}

	bgtexture=SDL_LoadBMP("bgtexture.bmp");
	bgtexture2=bgtexture; // SDL_LoadBMP("bgtexture2.bmp");
	// SDL_LockSurface(bgtexture);

	fp=fopen("incl.c","wa");
	printSurfaceDetails(fp,"screen",screen);
	printSurfaceDetails(fp,"bgtexture",bgtexture);
	printSurfaceDetails(fp,"bgtexture2",bgtexture2);
	fclose(fp);

	{ int i,j;
		bgpixels=new(Uint32,bgtexture_w*bgtexture_h);
		for (i=0;i<bgtexture_w;i++)     
			for (j=0;j<bgtexture_h;j++) {
				Uint8 r,g,b;
				Uint32 p;
				p=bgtexture_getPixel( i,j);
				bgtexture_GetRGB(p,&r,&g,&b);
				p=screen_MapRGB(r,g,b);
				bgpixels[j*bgtexture_w+i]=p;
			}
	}


	starttime = clock()+CLOCKS_PER_SEC/desiredFramesPerSecond;

	start_clock = clock();

	while (keepLooping) {

		int releasetime = clock()+CLOCKS_PER_SEC/desiredFramesPerSecond;
		framebits=((float)clock()-start_clock)*20.0/CLOCKS_PER_SEC;

		// printf("%i %i %li\n",frames,framebits,CLOCKS_PER_SEC);

		if ((frames % 20) == 0) {
			fprintf(output,"\rframes/second: %.2f   triangles/second: %.0f ",(float)CLOCKS_PER_SEC*(float)frames/(float)(releasetime-starttime),NUMTRIS*(float)CLOCKS_PER_SEC*(float)frames/(float)(releasetime-starttime));
		}

		// SDL_Flip(screen);
		SDL_UpdateRect(screen,0,0,SCRWID,SCRHEI);

		cylcenx=SCRWID/2+SCRWID/2*sin(framebits*0.017);
		cylceny=SCRHEI/2+SCRHEI/2*cos(framebits*0.012);

		{
			Uint16 i,j;
			// float qs=qsin((float)framebits*3.141/45.0);
			// Uint8 qsi=128+127*qs;
			// float bounce=0.1+0.099*qcos(framebits*M_PI/120.0);
			// float bounceB=2.0+1.5*qsin(framebits*M_PI/120.0);
			Uint16 bounceC=2000*(0.05-0.05*(pow((qcos(framebits*M_PI/115.0-2.5)+1.0)/2.0,0.6)*2.0-1.0));
			Sint16 Doffset=BGQUANT*(100000+framebits*10.0-600.0*qcos(framebits*M_PI/120.0-1.0));
			Uint16 aoffset=BGQUANT*(2000+framebits*2.0+sin(framebits*0.03-0.1)*200.0);
			Uint32 overd=BGQUANT*20000*(0.5+0.5*qcos(framebits*M_PI/120.0-1.0));
			// Uint32 tmp=((Uint16)bgtexture_w << 8)/M_PI;
			// Sint16 D2offset=Doffset+400.0*qsin(framebits*M_PI/140.0+123.6);
			// Sint16 a2offset=aoffset+100.0*qsin(framebits*M_PI/140.0+123.6);
			SDL_Rect dstrect;
			dstrect.w=IMGSKIP;
			dstrect.h=IMGSKIP;
			for (i=0;i<=screen_w;i+=IMGSKIP) {
				// Uint8 thruwid=i*256/screen_w;
				Uint16 xpart0=abs(i-cylcenx);
				// Uint32 xpart=(Uint32)square(i-cylcenx);
				Uint8 br,brmax;
				Uint16 d;
				screen_pixelType p;
				Uint8 r,g,b;
				Uint16 Ds[screen_h+IMGSKIP]; // Note these are IMGSKIP times bigger than they need to be
				Uint16 as[screen_h+IMGSKIP]; // and accessed as such throughout.
				Uint16 Dnow,anow;
				Uint16 Dlast,alast;
				dstrect.x=i;
				dstrect.y=0;
				for (j=0;j<=screen_h;j+=IMGSKIP, dstrect.y+=IMGSKIP) {
					// todo: Optimise this!
					// Uint16 d=qsqrt(xpart+square(j-cylceny)); //+bounceB*sqrt(square(i-SCRWID/2)+square(j-SCRHEI/2));
					// fprintf(output,"Getting and printing d\n");
					d=qhypot(xpart0,abs(j-cylceny));
					// fprintf(output,"%i\n",d);
					// d=qsqrt(xpart+square(j-cylceny));
					// Uint16 d=(abs(i-cylcenx)+abs(j-cylceny));
					// Uint16 D=1000.0/pow(d,bounce)+Doffset;
					// Uint16 D=(float)overd/d+Doffset;
					// Uint16 D=( (d & 65534) == 0 ? 0 : overd/d+Doffset);
#define D ( d<2 ? 0 : overd/d )
					// fprintf(output,"  %i\n",D);
					// Uint16 a=(Uint16)((((Uint32)(tmp*(atan2(i-cylcenx,j-cylceny))) >> 8)+M_PI)+aoffset);
#ifdef ROTSYM180
					#define BGrotscale 1
#else
					#define BGrotscale 0.5
#endif
#define a ( BGQUANT * (bgtexture_w * BGrotscale * ( atan2(i-cylcenx,j-cylceny ) / M_PI + M_PI ) ) )
					// dstrect.y=j;

#ifdef DO_WHITEOUT
					br=clip((SCRWID*2-d)*bounceC/SCRWID,0,256);
					brmax=255-br;
#endif

					anow=((Uint16)(a+aoffset))%(bgtexture_w*BGQUANT);
					Dnow=((Uint16)(D+Doffset));

#ifdef POSTERISE
					p=bgpixels[((anow/BGQUANT)%bgtexture_w)+((Dnow/BGQUANT)%bgtexture_h)*bgtexture_w];
					if (i<screen_w && j<screen_h) {
					SDL_FillRect(screen,&dstrect,p);
					}
#else
					if (j>0) {
						if (i>0) {
							Uint16 TLx=as[j-IMGSKIP];
							Uint16 TLy=Ds[j-IMGSKIP];
							Uint16 BLx=as[j];
							Uint16 BLy=Ds[j];
							Uint16 TRx=alast;
							Uint16 TRy=Dlast;
							Uint16 BRx=anow;
							Uint16 BRy=Dnow;
							// Hack around problem interpolating over TLa=359 TRa=001
							// Doesn't smegging work
							// Ooh hurrah it smegging does now
							if ( TRx>BGQUANT*bgtexture_w*3/4 || BRx>BGQUANT*bgtexture_w*3/4 ||
									TLx>BGQUANT*bgtexture_w*3/4 || BLx>BGQUANT*bgtexture_w*3/4 ) {
								if (TLx<BGQUANT*bgtexture_w/4)
									TLx+=BGQUANT*bgtexture_w;
								if (TRx<BGQUANT*bgtexture_w/4)
									TRx+=BGQUANT*bgtexture_w;
								if (BLx<BGQUANT*bgtexture_w/4)
									BLx+=BGQUANT*bgtexture_w;
								if (BRx<BGQUANT*bgtexture_w/4)
									BRx+=BGQUANT*bgtexture_w;
							} 
							// This doesn't appear to prevent the odd glitch, so I've taken it out
							// The glitches remain :-(
							// Oh they have hopefully gone now, I found a mistake.
							// It would appear not.
							if ( TRy>BGQUANT*bgtexture_h*3/4 || BRy>BGQUANT*bgtexture_h*3/4 ||
									TLy>BGQUANT*bgtexture_h*3/4 || BLy>BGQUANT*bgtexture_h*3/4 ) {
								if (TLy<BGQUANT*bgtexture_h/4)
									TLy+=BGQUANT*bgtexture_h;
								if (TRy<BGQUANT*bgtexture_h/4)
									TRy+=BGQUANT*bgtexture_h;
								if (BLy<BGQUANT*bgtexture_h/4)
									BLy+=BGQUANT*bgtexture_h;
								if (BRy<BGQUANT*bgtexture_h/4)
									BRy+=BGQUANT*bgtexture_h;
							} 
							// Worked when BGQUANT=1, but not sure why bgtexture_w*2
							// if (i-cylcenx>=0 && i-cylcenx<IMGSKIP && j-cylceny<0) { Does not apply if BGQUANT!=1, besides no speed gain!
							// if (TRx-TLx>BGQUANT*bgtexture_w/2 || BRx-TLx>BGQUANT*bgtexture_w/2) {
							// TLx+=BGQUANT*bgtexture_w*2;
							// // }
							// // if (BRx-BLx>BGQUANT*bgtexture_w/2 || TRx-BLx>BGQUANT*bgtexture_w/2) {
							// BLx+=BGQUANT*bgtexture_w*2;
							// }
							// }
							{
								register Uint8 I,J;
								register screen_pixelType *reg;
								register screen_pixelType *startreg=&SDLwrap_regPixel(screen,screen_pixelType,screen_pitch,screen_BytesPerPixel,i-IMGSKIP,j-IMGSKIP);
								for (J=0;J<IMGSKIP;J++) {
									reg=startreg;
									for (I=0;I<IMGSKIP;I++) {
										Uint16 ahere,Dhere;
										// #define e ((I))
										// #define s ((J))
										// #define w ((IMGSKIP-I))
										// #define n ((IMGSKIP-J))
#define e ((I))
#define s ((J))
										register Uint8 w=((IMGSKIP-I));
										register Uint8 n=((IMGSKIP-J));
										ahere=((TLx*n*w+TRx*n*e+BLx*s*w+BRx*s*e)/IMGSKIP/IMGSKIP)%(bgtexture_w*BGQUANT);
										Dhere=((TLy*n*w+TRy*n*e+BLy*s*w+BRy*s*e)/IMGSKIP/IMGSKIP)%(bgtexture_h*BGQUANT);
#ifdef INTERPOLATE_BG_FULL
										{
											Uint16 iahere=((ahere/BGQUANT)%bgtexture_w);
											Uint16 iDhere=((Dhere/BGQUANT)%bgtexture_h);
											Uint16 ianext=(iahere+1)%bgtexture_w;
											Uint16 iDnext=(iDhere+1)%bgtexture_h;
#define NWp (iahere+bgtexture_w*iDhere)
#define NEp (ianext+bgtexture_w*iDhere)
#define SWp (iahere+bgtexture_w*iDnext)
#define SEp (ianext+bgtexture_w*iDnext)
#define east (ahere-iahere*BGQUANT)
#define south (Dhere-iDhere*BGQUANT)
#define west (BGQUANT-east)
#define north (BGQUANT-south)
											((Uint8 *)&p)[0]=((Uint32)((Uint8 *)&bgpixels[NWp])[0]*north*west+((Uint8 *)&bgpixels[NEp])[0]*north*east+((Uint8 *)&bgpixels[SWp])[0]*south*west+((Uint8 *)&bgpixels[SEp])[0]*south*east)/BGQUANT/BGQUANT;
											((Uint8 *)&p)[1]=((Uint32)((Uint8 *)&bgpixels[NWp])[1]*north*west+((Uint8 *)&bgpixels[NEp])[1]*north*east+((Uint8 *)&bgpixels[SWp])[1]*south*west+((Uint8 *)&bgpixels[SEp])[1]*south*east)/BGQUANT/BGQUANT;
											((Uint8 *)&p)[2]=((Uint32)((Uint8 *)&bgpixels[NWp])[2]*north*west+((Uint8 *)&bgpixels[NEp])[2]*north*east+((Uint8 *)&bgpixels[SWp])[2]*south*west+((Uint8 *)&bgpixels[SEp])[2]*south*east)/BGQUANT/BGQUANT;
										}
#else
										p=bgpixels[((ahere/BGQUANT)%bgtexture_w)+((Dhere/BGQUANT)%bgtexture_h)*bgtexture_w];
#endif

										*reg=p;
#ifdef DO_WHITEOUT
										// Apply whiteout
										// ((Uint8 *)reg)[0] = ( ((Uint8 *)reg)[0] < brmax ? ((Uint8 *)reg)[0]+br : 255 );
										// ((Uint8 *)reg)[1] = ( ((Uint8 *)reg)[1] < brmax ? ((Uint8 *)reg)[1]+br : 255 );
										// ((Uint8 *)reg)[2] = ( ((Uint8 *)reg)[2] < brmax ? ((Uint8 *)reg)[2]+br : 255 );
										r=((Uint8 *)&p)[0];
										g=((Uint8 *)&p)[1];
										b=((Uint8 *)&p)[2];
										((Uint8 *)reg)[0] = ( r < brmax ? r+br : 255 );
										((Uint8 *)reg)[1] = ( g < brmax ? g+br : 255 );
										((Uint8 *)reg)[2] = ( b < brmax ? b+br : 255 );
										// screen_GetRGB(p,&r,&g,&b);
										// r=clip((int)r+(int)br,0,256);
										// g=clip((int)g+(int)br,0,256);
										// b=clip((int)b+(int)br,0,256);
										// p=screen_MapRGB(r,g,b);
#endif
										reg++;

								// SDL_GetRGB(p,bgtexture->format,&r,&g,&b);
								// SDL_MapRGB(screen->format,r,g,b);
								// float br=0.2*(2.0+2.0*qcos((double)j/12.0+3.0*qsin((double)framebits*0.0039)))*qsin(10.0*qsin(4.0*qsin((double)framebits/300.0)+(double)i/25.0))+(2.0+2.0*qcos((double)j/11.0+0.012*qsin((double)framebits/17.0)))*qcos(2+(double)i*0.14+(double)j/7.0+4.0*qcos(((double)framebits*0.29+j/15.0)/23.0));
								// if (br<0.0) br=0.0;
								// if (br>1.0) br=1.0;
								// ((int *)screen->pixels)[j*screen_w+i]=MapRGB(screen->format,br*i*256/screen_w,br*j*256/screen_h,128+127*qsin(clock()*3.141/5.0/(float)CLOCKS_PER_SEC));
								// ((int *)screen->pixels)[j*screen_w+i]=0;
								// ((int *)screen->pixels)[j*screen_w+i]=MapRGB(screen->format,i*256/screen_w,j*256/screen_h,128+127*qs);
								// screen_setPixel(i,j,screen_MapRGB(thruwid,j*256/screen_h,qsi));
								// p=screen_MapRGB(r,g,b);

									}
									startreg+=screen_w;
								}
							}
						}
						// j>0 but not neccessarily i
						Ds[j-IMGSKIP]=Dlast;
						as[j-IMGSKIP]=alast;
						if (j==screen_h) {
							Ds[j]=Dnow;
							as[j]=anow;
						}
					}
					Dlast=Dnow;
					alast=anow;
#endif
				}
			}

#ifdef DO_TRIANGLES

			for (i=0;i<NUMTRIS;i++) {
				// plotLine(screen,screen_w*frand(),screen_h*frand(),scr een_w*frand(),screen_h*frand(),MapRGB(screen->format,256*frand(),256*frand(),256*frand()));
				// plotTri(screen,screen_w*frand(),screen_h*frand(),screen_w*frand(),screen_h*frand(),screen_w*frand(),screen_h*frand(),MapRGB(screen->format,256*frand(),256*frand(),256*frand()));
				plotTriRGB(screen,-cylcenx/TRISINGOFFSET+screen_w*nearhalf(),-cylceny/TRISINGOFFSET+screen_h*nearhalf(),-cylcenx/TRISINGOFFSET+screen_w*nearhalf(),-cylceny/TRISINGOFFSET+screen_h*nearhalf(),-cylcenx/TRISINGOFFSET+screen_w*nearhalf(),-cylceny/TRISINGOFFSET+screen_h*nearhalf(),brightness*nearhalf(),brightness*nearhalf(),brightness*nearhalf());
			}

#endif

#ifdef DO_STARS

			{



				// float camX=sin(framebits*0.013);
				// float camY=cos(framebits*0.013);
				// float camZ=-1; // sin(framebits*0.013+1.235);

#define brightness 120
				// #define STARY (SCRHEI/2)
#define SCALE ( SCRWID*2 )

				// #define STARX (2*SCRWID/3)
				float STARX=SCRWID/2+SCRWID/6*sin(framebits*0.02);
				float STARY=SCRHEI/2+SCRWID/6*cos(framebits*0.02)*sin(framebits*0.001);
#define starPlotTri(xa,ya,xb,yb,xc,yc,bright) plotTriRGB(screen,xa,ya,xb,yb,xc,yc,bright*brightness,bright*brightness/2.0,bright*brightness)
#define starRad(a,b) ( (9.0)*(2.0+sin((a)*2.1+framebits*0.063)+sin((b)*2.6+framebits*0.05)) )
				// #define starRad(a,b) ( 20 )
#define starRes 30
#include "starsurface.c"
#undef starRad
#undef starRes
#undef starPlotTri
				// #undef STARX

				STARX=SCRWID/2-SCRWID/6*sin(framebits*0.02);
				STARY=SCRHEI/2-SCRWID/6*cos(framebits*0.02)*sin(framebits*0.001);
				// #define STARX (SCRWID/3)
#define starPlotTri(xa,ya,xb,yb,xc,yc,bright) plotTriRGB(screen,xa,ya,xb,yb,xc,yc,bright*brightness/2.0,bright*brightness/2,0)
#define starRad(a,b) ( (7.0)*(3.0+sinsharp((-a)*2.5+framebits*0.044)+sinsharp((b)*3.5+framebits*0.031)) )
#define starRes 40
#include "starsurface.c"
#undef starRad
#undef starRes
#undef starPlotTri



			}

#endif

		}

		frames++;

		while (SDL_PollEvent(&event)) {
			if (event.type==SDL_KEYDOWN || event.type==SDL_QUIT || event.type==SDL_MOUSEBUTTONDOWN) {
				keepLooping=0;
			}
		}

		{
			// int timetogo;
			// sched_yield();
			// printf("%i %i\n",clock(),releasetime);
			// timetogo=(int)releasetime-(int)clock();
			// while ( timetogo > 0) {
			// // printf(". %i\n",(int)(releasetime-clock()));
			// // Should release processor...
			// // int i=timetogo;
			// // if (i<20)
			// // i=20;
			// // SDL_Delay(12);
			// sched_yield();
			// timetogo=(int)releasetime-(int)clock();
			// }
		}

	}

	fprintf(output,"\n");
#ifdef DEBUGTOFILE
	fclose(output);
#endif
	return 0;

}

