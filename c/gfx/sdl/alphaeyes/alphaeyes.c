
// #define DOS

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

#define regPixel(surface,x,y) ( (*(Uint32 *)(((Uint8 *)((surface)->pixels))+(y)*surface->pitch+x*surface->format->BytesPerPixel)) )
#define setPixel(surface,x,y,p) { regPixel(surface,x,y)=p; }
#define getPixel(surface,x,y) ( regPixel(surface,x,y) )

int main(int argc,char *argv[]) {

	SDL_Surface *input,*output;

	input=SDL_LoadBMP(argv[1]);

	output=SDL_CreateRGBSurface(0,input->w,input->h,32,0,0,0,0);
	{ int x,y;
		for (x=0;x<input->w;x++)
			for (y=0;y<input->h;y++) {
				Uint8 r,g,b,a;
				Uint32 pixel=getPixel(input,x,y);
				SDL_GetRGBA(pixel,input->format,&r,&g,&b,&a);
				a=(Uint32)((255-r)+(255-g)+(255-b))/3;
				pixel = SDL_MapRGBA(input->format,r,g,b,a);
				setPixel(output,x,y,pixel);
			}
	}
	SDL_SaveBMP(output,argv[2]);

	return 0;

}

