#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>

#define sgn(x) (x>0?1:-1)

#define SCRWID 320
#define SCRHEI 200

#define desiredFramesPerSecond 10000

/* 0.0 <= frand() < 1.0 */
#define frand() ((float)(rand()/2)/(float)(RAND_MAX/2+1))

#define new(t,c) ((t *)malloc(sizeof(t)*c))

// Very slight increase?
#define lookupmax 65536
float coslookup[lookupmax];
// float *coslookup;
// float coslookup[lookupmax];
#define qcos(x) (coslookup[abs(((x)*(int)(lookupmax/2.0/M_PI)))%lookupmax])
// #define qcos(x) cos(x)
#define qsin(x) (qcos(x-M_PI/2.0))

// This does seem to speed it up a little
// Macro'd clone of SDL_MapRGB
#define MapRGB(format,r,g,b) ( (format)->palette == NULL ? TrueMapRGB(format,r,g,b) : SDL_FindColor(format->palette, r, g, b) )
// Use this one if you know you have truecolor
#define TrueMapRGB(format,r,g,b) ( ((int)(r) >> (format)->Rloss) << (format)->Rshift | ((int)(g) >> (format)->Gloss) << (format)->Gshift | ((int)(b) >> (format)->Bloss) << (format)->Bshift | (format)->Amask )
// #define TrueMapRGB(format,r,g,b) SDL_MapRGB(format,r,g,b)

void setuptriglookup() {
	int i;
	// coslookup=new(float,lookupmax);
	for (i=0;i<lookupmax;i++)
		coslookup[i]=cos((float)i*2.0*M_PI/(float)lookupmax);
}

// Bresenham's line plotting.  Would it be faster in assembly?
/*
void plotLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 colour )
    {
      int dx  = abs(x2 - x1),
          dy  = abs(y2 - y1),
		  ddx = sgn(x2-x1),
		  ddy = sgn(y2-y1),
		  x   = x1,
          y   = y1,
          eps = 0;
    
      for ( ; x != x2; x+=ddx )  {
        ((int *)surface->pixels)[y*SCRWID+x]=colour;
        eps += dy;
        if ( (eps << 1) >= dx )  {
          y+=ddy;  eps -= dx;
        }
      }
    }
*/

#define getPixel(x,y) (regPixel(x,y))
#define setPixel(x,y,c) (regPixel(x,y)=c)
#define regPixel(x,y) (((int *)surface->pixels)[y*SCRWID+x])

#define cap(x,b) ( (x) > (b) ? (b) : (x) )

inline void merge(SDL_Surface *surface,int x,int y,Uint8 dr,Uint8 dg,Uint8 db) {
	Uint8 r,g,b;
	int fr,fg,fb;
	SDL_GetRGB(getPixel(x,y),surface->format,&r,&g,&b);
	// printf("    %i %i %i\n",r,g,b);
	// printf("  + %i %i %i\n",dr,dg,db);
	fr=r+dr;
	fg=g+dg;
	fb=b+db;
	if (fr>255) fr=255;
	if (fg>255) fg=255;
	if (fb>255) fb=255;
	// printf(" -> %i %i %i\n",fr,fg,fb);
	setPixel(x,y,TrueMapRGB(surface->format,fr,fg,fb));
}

void plotLine(SDL_Surface *surface,int BR_x1,int BR_y1,int BR_x2,int BR_y2,Uint32 colour) {
	#define BR_todo(x,y)  ((int *)surface->pixels)[y*SCRWID+x]=colour
	#include "bresenham.c"
	#undef BR_todo(x,y)
}

void plotTri(SDL_Surface *surface,int TR_x1,int TR_y1,int TR_x2,int TR_y2,int TR_x3, int TR_y3, Uint32 colour) {
	#define TR_todo(x,y) {setPixel(x,y,colour);}
	// #define TR_todo(x,y) { if (x>=0 && x<SCRWID && y>=0 && y<SCRHEI) {setPixel(x,y,colour);} }
	// #define TR_todo(x,y) printf("Hello %i %i\n",x,y); setPixel(x,y,colour);
	#include "tri.c"
	#undef TR_todo
	plotLine(surface,TR_x1,TR_y1,TR_x2,TR_y2,TrueMapRGB(surface->format,255,255,255));
	plotLine(surface,TR_x1,TR_y1,TR_x3,TR_y3,TrueMapRGB(surface->format,255,255,255));
	plotLine(surface,TR_x2,TR_y2,TR_x3,TR_y3,TrueMapRGB(surface->format,255,255,255));
}

void plotTriRGB(SDL_Surface *surface,int TR_x1,int TR_y1,int TR_x2,int TR_y2,int TR_x3, int TR_y3, int r,int g,int b) {
	#define TR_todo(x,y) merge(surface,x,y,r,g,b);
	#include "tri.c"
	#undef TR_todo
	plotLine(surface,TR_x1,TR_y1,TR_x2,TR_y2,TrueMapRGB(surface->format,255,255,255));
	plotLine(surface,TR_x1,TR_y1,TR_x3,TR_y3,TrueMapRGB(surface->format,255,255,255));
	plotLine(surface,TR_x2,TR_y2,TR_x3,TR_y3,TrueMapRGB(surface->format,255,255,255));
}

int main(int argc,char *argv[]) {

	SDL_Surface* surface;
	SDL_Event event;
	int keepLooping=1;
	int frames=0;

	setuptriglookup();
	// srand(time(NULL));

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr,"Couldn't initialise SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	surface=SDL_SetVideoMode(SCRWID,SCRHEI,32,SDL_HWPALETTE /* | SDL_FULLSCREEN */ );
	if (surface==NULL) {
		fprintf(stderr,"Couldn't initialise video.\n");
		return 1;
	}

	while (keepLooping) {
		
		clock_t releasetime = clock()+CLOCKS_PER_SEC/desiredFramesPerSecond;

		int i,j;
		for (i=0;i<SCRWID;i++)
			for (j=0;j<SCRHEI;j++) {
				float br=0.2*(2.0+2.0*qcos((double)j/12.0+3.0*qsin((double)frames*0.0039)))*qsin(10.0*qsin(4.0*qsin((double)frames/300.0)+(double)i/25.0))+(2.0+2.0*qcos((double)j/11.0+0.012*qsin((double)frames/17.0)))*qcos(2+(double)i*0.14+(double)j/7.0+4.0*qcos(((double)frames*0.29+j/15.0)/23.0));
				if (br<0.0) br=0.0;
				if (br>1.0) br=1.0;
				// ((int *)surface->pixels)[j*SCRWID+i]=SDL_MapRGB(surface->format,br*i*256/SCRWID,br*j*256/SCRHEI,128+127*qsin(clock()*3.141/5.0/(float)CLOCKS_PER_SEC));
				((int *)surface->pixels)[j*SCRWID+i]=TrueMapRGB(surface->format,i*256/SCRWID,j*256/SCRHEI,128+127*qsin((float)frames*3.141/45.0));
				// ((int *)surface->pixels)[j*SCRWID+i]=0;
			}

		for (i=0;i<12;i++) {
			// plotLine(surface,SCRWID*frand(),SCRHEI*frand(),SCRWID*frand(),SCRHEI*frand(),TrueMapRGB(surface->format,256*frand(),256*frand(),256*frand()));
			// plotTri(surface,SCRWID*frand(),SCRHEI*frand(),SCRWID*frand(),SCRHEI*frand(),SCRWID*frand(),SCRHEI*frand(),TrueMapRGB(surface->format,256*frand(),256*frand(),256*frand()));
			plotTriRGB(surface,SCRWID*frand(),SCRHEI*frand(),SCRWID*frand(),SCRHEI*frand(),SCRWID*frand(),SCRHEI*frand(),64*frand(),64*frand(),64*frand());
		}

		frames++;

		while (SDL_PollEvent(&event)) {
			if (event.type==SDL_KEYDOWN || event.type==SDL_QUIT || event.type==SDL_MOUSEBUTTONDOWN) {
				keepLooping=0;
			}
		}
		
		{
			clock_t timetogo;
			while ( (timetogo=releasetime-clock()) > 0) {
				// Should release processor...
			}
		}
		
		SDL_Flip(surface);

	}

	return 0;

}
