
// #define DOS
// #define DO_FULLSCREEN
#define SCRWID 800
#define SCRHEI 300
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

#define TXTWID (10*7)
#define TXTHEI 23
char text[TXTHEI][TXTWID] =
{
	"  0      0      O      0      0      O      O      0      0     O     ",
	"  0      0      O      0      0      O      O      0      0     O     ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"0   O  00000  0   0  000      0    0      O   O  00000  00000         ",
	"0   O  0      0   0  0  O    O O   0      O   O    0    0             ",
	"0O  0  0      0   0  0   0  0   0  0      O   O    0    0             ",
	"00  0  0      0   0  0  O   0   0  0      O   O    0    0             ",
	"0 O 0  000    0   0  OOO    00000  0       O O     0    000           ",
	"0  00  0      0   0  0  O   0   0  0        O      0    0             ",
	"0  O0  0      0   0  0   0  0   0  0        O      0    0             ",
	"0   0  0      0   0  0   0  0   0  0        O      0    0             ",
	"0   0  00000   000   0   0  0   0  00000    O      0    00000         ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"  0      0      O      0      0      O      O      0      0     O     ",
	"  0      0      O      0      0      O      O      0      0     O     "
};

#define SCALECONST ((float)SCRWID/32.0/82.0)
#define PADDING 2
#define cr (SCRHEI/(TXTHEI+PADDING*2))

SDL_Surface *screen;
int frames=0;

SDL_Rect screenrect;
SDL_Rect dstrect;
int blackPixel;
int whitePixel;

float freq[TXTHEI];
float off[TXTHEI];
float mag[TXTHEI];
float cen[TXTHEI];
float speed[TXTHEI];
float space[TXTHEI];

void setSpeed(int i) {
	float mess=(1.0+cos((float)frames*0.02))/2.0;
	// speed[i]=9.0+2.0*sin(freq[i]*M_PI*frames);
	speed[i]=0.2*SCALECONST*square((cen[i]+mess*mag[i]*sin(off[i]+freq[i]*M_PI*(float)frames))/3.0);
	space[i]=speed[i]*(float)cr/6.0;
}

void init() {
	int i;
	// srand(time(NULL));
	for (i=0;i<TXTHEI;i++) {
		// speed[i]=12+(i%2);
		// speed[i]=7.0+3.0*frand();
		// space[i]=speed[i]*(float)cr/8.0;
		// freq[i]=0.01*frand();
		// freq[i]=(float)i/(float)TXTHEI/100.0;
		off[i]=2.0*M_PI*frand();
		freq[i]=0.02*frand();
		mag[i]=25.0*frand();
		// mag[i]=5.0+20.0*sin(M_PI*(float)i/(float)TXTHEI);
		// cen[i]=12.0+0.5*frand();
		cen[i]=30.0; // +2.0*frand();
		setSpeed(i);
	}
	// dstrect.w=cr/2;
	// dstrect.h=cr/2;
	blackPixel=SDL_MapRGB(screen->format,0,0,0);
	whitePixel=SDL_MapRGB(screen->format,255,60,40);
	screenrect.x=0;
	screenrect.y=0;
	screenrect.w=SCRWID;
	screenrect.h=SCRHEI;
	frames=0;
}

void plotBlob(int x,int y,int w,int h,int c) {
#define RECTSHRINK 2
	dstrect.x=x+RECTSHRINK;
	dstrect.w=(w<RECTSHRINK*2+1?1:w-RECTSHRINK*2);
	dstrect.y=y+RECTSHRINK;
	dstrect.h=h-RECTSHRINK*2;
	// dstrect.x=x;
	// dstrect.x=x+w/4;
	// dstrect.w=(w<4?1:w/2);
	// dstrect.y=y+h/4;
	// dstrect.h=h/2;
	// dstrect.x=x;
	// dstrect.w=w+1;
	// dstrect.y=y;
	// dstrect.h=h;
	SDL_FillRect(screen,&dstrect,c);
}

void doframe() {
	int row;
	SDL_FillRect(screen,&screenrect,blackPixel);
	for (row=0;row<TXTHEI;row++) {
		int cy=(PADDING+row)*cr;
		int leftat=speed[row]*frames-SCRWID/2;
		int i0=leftat/space[row];
		int diff=space[row]-(leftat-i0*space[row]);
		int col;
		// i0--; diff-=space[row];
		// printf("%i %i\n",i0,diff);
		for (col=0;col<2+SCRWID/space[row];col++) {
			int i=(i0+col)%TXTWID;
			// int c=( text[row][i] == ' ' ? blackPixel : whitePixel );
			if (text[row][i]!=' ')
			plotBlob(diff+col*space[row],cy,space[row],cr,whitePixel);
			// plotBlob(diff+col*space[row],cy,cr,cr,whitePixel);
			// printf("%s\n",text[row]);
		}
		setSpeed(row);
		// speed[row]+=0.2*(frand()-0.5);
		// space[row]=speed[row]*(float)cr/8.0;
	}
	plotBlob(SCRWID/2-cr,2*SCRHEI/8-cr,2*cr,2*cr,whitePixel);
	plotBlob(SCRWID/2-cr,6*SCRHEI/8-cr,2*cr,2*cr,whitePixel);
	// plotBlob(SCRWID/2-cr/4,0,cr/2,SCRHEI,whitePixel);
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

