#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>

#define SCRWID 320
#define SCRHEI 200

#define desiredFramesPerSecond 10000

#define new(t,c) ((t *)malloc(sizeof(t)*c))
#define sgn(x) ((x)>0.0?1.0:-1.0)
/* 0.0 <= frand() < 1.0 */
#define frand() ((float)(rand()/2)/(float)(RAND_MAX/2+1))
#define nearhalf() (0.5+(frand()-0.5)*frand())
#define cap(x,b) ( (x) > (b) ? (b) : (x) )

// todo: resolve the p==NULL at init and switch routing of code to be execed
#define paletteChoice(p,a,b) ( ( (p) == NULL ) ? (a) : (b) )
// #define paletteChoice(p,a,b) { if ( (p) == NULL ) { (a) } else { (b) } }
// #define paletteChoice(p,a,b) a
// #define paletteChoice(p,a,b) b
// This does seem to speed it up a little
// Macro/inline clones of SDL_MapRGB and SDL_GetRGB
// Prefix True may be used for extra speed if you know format->palette==NULL (truecolor?)
// Actually preferably switch #define paletteChoice above
// #define MapRGB SDL_MapRGB
#define MapRGB(format,r,g,b) ( paletteChoice( (format)->palette , TrueMapRGB((format),(r),(g),(b)) , SDL_FindColor((format)->palette, (r), (g), (b)) ) )
#define TrueMapRGB(format,r,g,b) ( ((int)(r) >> (format)->Rloss) << (format)->Rshift | ((int)(g) >> (format)->Gloss) << (format)->Gshift | ((int)(b) >> (format)->Bloss) << (format)->Bshift | (format)->Amask )
// The original for comparison
// The reading functions offer even greater improvement (due to being heavy!)
// #define GetRGB SDL_GetRGB
#define GetRGB(pixel,fmt,r,g,b) paletteChoice( (fmt)->palette , TrueGetRGB((pixel),(fmt),(r),(g),(b)) , PalGetRGB((pixel),(fmt),(r),(g),(b)) )
#define PalGetRGB(pixel,fmt,red,green,blue) { *(red) = (fmt)->palette->colors[(pixel)].r; *(green) = (fmt)->palette->colors[(pixel)].g; *(blue) = (fmt)->palette->colors[(pixel)].b; }
void inline TrueGetRGB(Uint32 pixel, SDL_PixelFormat *fmt, Uint8 *r,Uint8 *g,Uint8 *b) {
                unsigned rv, gv, bv;
                rv = (pixel & fmt->Rmask) >> fmt->Rshift;
                *r = (rv << fmt->Rloss) + (rv >> (8 - fmt->Rloss));
                gv = (pixel & fmt->Gmask) >> fmt->Gshift;
                *g = (gv << fmt->Gloss) + (gv >> (8 - fmt->Gloss));
                bv = (pixel & fmt->Bmask) >> fmt->Bshift;
                *b = (bv << fmt->Bloss) + (bv >> (8 - fmt->Bloss));
}

#define getPixel(sdl_surface,x,y) (regPixel((sdl_surface),(x),(y)))
#define setPixel(sdl_surface,x,y,c); {regPixel((sdl_surface),(x),(y))=(c);}
// #define regPixel(sdl_surface,x,y) (((int *)(sdl_surface)->pixels)[(y)*SCRWID+(x)])
#define regPixel(sdl_surface,x,y) (((int *)(sdl_surface)->pixels)[((y)*((sdl_surface)->w)+(x))])
// #define regPixel(sdl_surface,x,y) *(((unsigned char *)(sdl_surface)->pixels)+((y)*((sdl_surface)->h)+(x))*((sdl_surface)->format->BytesPerPixel))

// Very slight increase?
#define lookupmax 65536
float coslookup[lookupmax];
// float *coslookup;
// float coslookup[lookupmax];
#define qcos(x) (coslookup[abs(((x)*(int)(lookupmax/2.0/M_PI)))%lookupmax])
// #define qcos(x) cos(x)
#define qsin(x) (qcos(x-M_PI/2.0))
void setuptriglookup() {
	int i;
	// coslookup=new(float,lookupmax);
	for (i=0;i<lookupmax;i++)
		coslookup[i]=cos((float)i*2.0*M_PI/(float)lookupmax);
}

inline void merge(SDL_Surface *surface,int x,int y,Uint8 dr,Uint8 dg,Uint8 db) {
	Uint8 r,g,b;
	int fr,fg,fb;
	GetRGB(getPixel(surface,x,y),(surface->format),&r,&g,&b);
	// printf("    %i %i %i\n",r,g,b);
	// printf("  + %i %i %i\n",dr,dg,db);
	fr=r+dr;
	fg=g+dg;
	fb=b+db;
	if (fr>255) fr=255;
	if (fg>255) fg=255;
	if (fb>255) fb=255;
	// printf(" -> %i %i %i\n",fr,fg,fb);
	setPixel(surface,x,y,MapRGB(surface->format,fr,fg,fb));
}

void plotLine(SDL_Surface *surface,int BR_x1,int BR_y1,int BR_x2,int BR_y2,Uint32 colour) {
	// #define BR_todo(x,y)  ((int *)surface->pixels)[y*SCRWID+x]=colour
	#define BR_todo(x,y)  setPixel(surface,x,y,colour);
	#include "bresenham.c"
	#undef BR_todo(x,y)
}

void plotTri(SDL_Surface *surface,int TR_x1,int TR_y1,int TR_x2,int TR_y2,int TR_x3, int TR_y3, Uint32 colour) {
	#define TR_todo(x,y) {setPixel(surface,x,y,colour);}
	// #define TR_todo(x,y) { if (x>=0 && x<SCRWID && y>=0 && y<SCRHEI) {setPixel(surface,x,y,colour);} }
	// #define TR_todo(x,y) printf("Hello %i %i\n",x,y); setPixel(surface,x,y,colour);
	#include "tri.c"
	#undef TR_todo
	plotLine(surface,TR_x1,TR_y1,TR_x2,TR_y2,MapRGB(surface->format,255,255,255));
	plotLine(surface,TR_x1,TR_y1,TR_x3,TR_y3,MapRGB(surface->format,255,255,255));
	plotLine(surface,TR_x2,TR_y2,TR_x3,TR_y3,MapRGB(surface->format,255,255,255));
}

void plotTriRGB(SDL_Surface *surface,int TR_x1,int TR_y1,int TR_x2,int TR_y2,int TR_x3, int TR_y3, int r,int g,int b) {
	#define TR_todo(x,y) merge(surface,x,y,r,g,b);
	#include "tri.c"
	#undef TR_todo
	plotLine(surface,TR_x1,TR_y1,TR_x2,TR_y2,MapRGB(surface->format,255,255,255));
	plotLine(surface,TR_x1,TR_y1,TR_x3,TR_y3,MapRGB(surface->format,255,255,255));
	plotLine(surface,TR_x2,TR_y2,TR_x3,TR_y3,MapRGB(surface->format,255,255,255));
}

int main(int argc,char *argv[]) {

	SDL_Surface *surface,*bgtexture;
	SDL_Event event;
	int keepLooping=1;
	int frames=0;
	clock_t starttime;

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
	{
		printf("Interestingly enough:\n");
		printf("Rmask = %i\n",surface->format->Rmask);
		printf("Rloss = %i\n",surface->format->Rloss);
		printf("Rshift = %i\n",surface->format->Rshift);
		printf("Gmask = %i\n",surface->format->Gmask);
		printf("Gloss = %i\n",surface->format->Gloss);
		printf("Gshift = %i\n",surface->format->Gshift);
		printf("Bmask = %i\n",surface->format->Bmask);
		printf("Bloss = %i\n",surface->format->Bloss);
		printf("Bshift = %i\n",surface->format->Bshift);
	}

	bgtexture=SDL_LoadBMP("bgtexture.bmp");
	SDL_LockSurface(bgtexture);

	starttime = clock()+CLOCKS_PER_SEC/desiredFramesPerSecond;

	while (keepLooping) {
		
		clock_t releasetime = clock()+CLOCKS_PER_SEC/desiredFramesPerSecond;

		if ((frames % 50) == 0) {
			fprintf(stderr,"\rFPS: %f",(float)CLOCKS_PER_SEC*(float)frames/(float)(releasetime-starttime));
		}
		
		SDL_Flip(surface);

		{
		int i,j;
		float qs=qsin((float)frames*3.141/45.0);
		float bounce=1.0+0.9*qsin(frames*M_PI/20.0);
		qs=(bounce)/2.0;
		for (i=0;i<SCRWID;i++)
			for (j=0;j<SCRHEI;j++) {
				float x=(float)(i-SCRWID/2)/(float)(SCRWID/2);
				float y=(float)(j-SCRHEI/2)/(float)(SCRHEI/2);
				unsigned int p;
				unsigned int u,v;
				unsigned char r,g,b;
				((int *)surface->pixels)[j*SCRWID+i]=MapRGB(surface->format,i*256/SCRWID,j*256/SCRHEI,128+127*qs);
				// fprintf(stderr,"%i %i : %f %f -> \n", i,j, x, y);
				x=sgn(x)*pow(fabs(x),bounce);
				y=sgn(y)*pow(fabs(y),bounce);
				u=(int)((bgtexture->w-5)*(0.5*(x+1.0))) % 196;
				v=(int)((bgtexture->h-5)*(0.5*(y+1.0))) % 196;
				// fprintf(stderr,"%f %f : %i %i\n", x, y, u,v);
				// fprintf(stderr,"%i\n",&regPixel(bgtexture,u,v));
				// fprintf(stderr,"%i %i\n",bgtexture->w,bgtexture->h);
				// p=getPixel(bgtexture, u, v);
				// p=((unsigned char *)(bgtexture->pixels))[bgtexture->h*v+u];
				p=((unsigned char *)(bgtexture->pixels))[(bgtexture->h*v+u)*bgtexture->format->BytesPerPixel];
				// p=((unsigned int *)(bgtexture->pixels))[bgtexture->h*v+u];
				SDL_GetRGB(p,bgtexture->format,&r,&g,&b);
				// fprintf(stderr,"%i\n",p);
				setPixel(surface,i,j,SDL_MapRGB(bgtexture->format,r,g,b));
				// ((int *)surface->pixels)[j*surface->h+i]=SDL_MapRGB(surface->format,r,g,b);
				// float br=0.2*(2.0+2.0*qcos((double)j/12.0+3.0*qsin((double)frames*0.0039)))*qsin(10.0*qsin(4.0*qsin((double)frames/300.0)+(double)i/25.0))+(2.0+2.0*qcos((double)j/11.0+0.012*qsin((double)frames/17.0)))*qcos(2+(double)i*0.14+(double)j/7.0+4.0*qcos(((double)frames*0.29+j/15.0)/23.0));
				// if (br<0.0) br=0.0;
				// if (br>1.0) br=1.0;
				// ((int *)surface->pixels)[j*SCRWID+i]=MapRGB(surface->format,br*i*256/SCRWID,br*j*256/SCRHEI,128+127*qsin(clock()*3.141/5.0/(float)CLOCKS_PER_SEC));
				// ((int *)surface->pixels)[j*SCRWID+i]=0;
			}

		#define brightness 20
		for (i=0;i<50;i++) {
			// plotLine(surface,SCRWID*frand(),SCRHEI*frand(),SCRWID*frand(),SCRHEI*frand(),MapRGB(surface->format,256*frand(),256*frand(),256*frand()));
			// plotTri(surface,SCRWID*frand(),SCRHEI*frand(),SCRWID*frand(),SCRHEI*frand(),SCRWID*frand(),SCRHEI*frand(),MapRGB(surface->format,256*frand(),256*frand(),256*frand()));
			plotTriRGB(surface,SCRWID*nearhalf(),SCRHEI*nearhalf(),SCRWID*nearhalf(),SCRHEI*nearhalf(),SCRWID*nearhalf(),SCRHEI*nearhalf(),brightness*nearhalf(),brightness*nearhalf(),brightness*nearhalf());
		}
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

	}

	return 0;

}
