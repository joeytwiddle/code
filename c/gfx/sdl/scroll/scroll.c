
// #define DOS
// #define DO_FULLSCREEN
#define SCRWID 640
#define SCRHEI 480
#define SCRBPS 32
#define desiredFramesPerSecond 50

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
// #define qsqrt(x) sqrt(x)

void setuplookups() {
	setuptriglookup();
	setupsqrtlookup();
}

// #define minmaxinto(x,y,v,w) { if ((x)<(y)) { v=(x); w=(y); } else { v=(y); w=(x); } }
#define minmaxinto(x,y,v,w) { typeof(v) xa=(x), ya=(y); if ((xa)<(ya)) { v=(xa); w=(ya); } else { v=(ya); w=(xa); } }

SDL_Surface *screen;
int frames=0;

#define TXTWID (10*4)
#define TXTHEI 5
unsigned char text[TXTHEI][TXTWID] =
{
	"00  000 0 0 00   0  0   O O 000 000    ",
	"0 0 0   0 0 0 0 0 0 0   O O  0  0      ",
	"0 0 00  0 0 00  000 0   OOO  0  00     ",
	"0 0 0   0 0 0 0 0 0 0    O   0  0      ",
	"0 0 000 000 0 0 0 0 000  O   0  000    "
};

Uint16 speed[TXTHEI];
Uint16 space[TXTHEI];

#define SCALECONST SCRWID/32
#define cr (SCRHEI/(TXTHEI+2))

SDL_Rect screenrect;
SDL_Rect dstrect;
int blackPixel;
int whitePixel;

void init() {
	int i;
	srand(time(NULL));
	for (i=0;i<TXTHEI;i++) {
		speed[i]=8+(i%4);
		space[i]=speed[i]*cr/4;
	}
	// dstrect.w=cr/2;
	// dstrect.h=cr/2;
	blackPixel=SDL_MapRGB(screen->format,0,0,0);
	whitePixel=SDL_MapRGB(screen->format,255,255,255);
	screenrect.x=0;
	screenrect.y=0;
	screenrect.w=SCRWID;
	screenrect.h=SCRHEI;
}

void plotBlob(int x,int y,int w,int h,int c) {
	dstrect.x=x;
	dstrect.y=y;
	dstrect.w=cr;
	dstrect.h=h;
	SDL_FillRect(screen,&dstrect,c);
}

void doframe() {
	int row;
	SDL_FillRect(screen,&screenrect,blackPixel);
	for (row=0;row<TXTHEI;row++) {
		int cy=(1+row)*cr;
		int leftat=speed[row]*frames-SCRWID/2;
		int i0=leftat/space[row];
		int diff=space[row]-(leftat-i0*space[row]);
		int col;
		i0--; diff-=space[row];
		// printf("%i %i\n",i0,diff);
		for (col=0;col<2+SCRWID/space[row];col++) {
			int i=(i0+col)%TXTWID;
			// int c=( text[row][i] == ' ' ? blackPixel : whitePixel );
			if (text[row][i]!=' ')
			plotBlob(diff+col*space[row],cy,space[row],cr,whitePixel);
		}
	}
}

int main(int argc,char *argv[]) {

	SDL_Event event;
	int keepLooping=1;
	float framebits;
	clock_t starttime;
	FILE *fp;
	int start_clock;

#ifdef DEBUGTOFILE
	output=fopen("debug.txt","wa");
	if (output==NULL)
		printf("Failed to open file debug.txt\n");
#endif

	setuplookups();

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

	fp=fopen("incl.c","wa");
	if (fp==NULL)
		printf("Failed to open file incl.c\n");
	printSurfaceDetails(fp,"screen",screen);
	fclose(fp);

	init();

	starttime = clock()+CLOCKS_PER_SEC/desiredFramesPerSecond;

	start_clock = clock();

	while (keepLooping) {

		int releasetime = clock()+CLOCKS_PER_SEC/desiredFramesPerSecond;
		framebits=((float)clock()-start_clock)*20.0/CLOCKS_PER_SEC;
		if ((frames % 20) == 0) {
			fprintf(output,"\rframes/second: %.2f",(float)CLOCKS_PER_SEC*(float)frames/(float)(releasetime-starttime));
		}

		// SDL_Flip(screen);
		SDL_UpdateRect(screen,0,0,SCRWID,SCRHEI);

		doframe();

		frames++;

		while (SDL_PollEvent(&event)) {
			if (event.type==SDL_KEYDOWN || event.type==SDL_QUIT || event.type==SDL_MOUSEBUTTONDOWN) {
				keepLooping=0;
			}
		}

	}

	fprintf(output,"\n");
#ifdef DEBUGTOFILE
	fclose(output);
#endif
	return 0;

}

