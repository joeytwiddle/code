#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>

#define SCRWID 320
#define SCRHEI 200

/***************/

/***************/

#define SDL_w(asurface) (asurface->w)
#define SDL_h(asurface) (asurface->h)
#define SDL_pitch(asurface) (asurface->pitch)
#define SDL_bytesperpixel(asurface) (asurface->format->BytesPerPixel)
#define SDL_Rmask(asurface) (asurface->Rmask)
#define SDL_Rloss(asurface) (asurface->Rloss)
#define SDL_Rshift(asurface) (asurface->Rshift)
#define SDL_Gmask(asurface) (asurface->Gmask)
#define SDL_Gloss(asurface) (asurface->Gloss)
#define SDL_Gshift(asurface) (asurface->Gshift)
#define SDL_Bmask(asurface) (asurface->Bmask)
#define SDL_Bloss(asurface) (asurface->Bloss)
#define SDL_Bshift(asurface) (asurface->Bshift)

#define desiredFramesPerSecond 10000

#define new(t,c) ((t *)malloc(sizeof(t)*c))
#define sgn(x) ((x)>0.0?1.0:-1.0)
/* 0.0 <= frand() < 1.0 */
#define frand() ((float)(rand()/2)/(float)(RAND_MAX/2+1))
#define nearhalf() (0.5+(frand()-0.5)*frand())
#define cap(x,b) ( (x) > (b) ? (b) : (x) )

// todo: resolve the p==NULL at init and switch routing of code to be execed
#define testNull(p) ( (p) == NULL )
#define paletteChoice(p,a,b) ( ( testNull(p) ) ? (a) : (b) )
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
// #define regPixel(sdl_surface,x,y) (*(Uint32 *)((Uint8 *)((sdl_surface)->pixels)+(y)*((sdl_surface)->pitch)+(x)*(sdl_surface)->format->BytesPerPixel))
#define regPixel(sdl_surface,x,y) (*(Uint32 *)((Uint8 *)((sdl_surface)->pixels)+(y)*(SDL_pitch(sdl_surface))+(x)*SDL_bytesperpixel(sdl_surface)))

#define SDLwrap_getPixel(sdl_surface,pixelType,pitch,bytesperpixel,x,y) (SDLwrap_regPixel((sdl_surface),(pixelType),(pitch),(bytesperpixel),(x),(y)))
#define SDLwrap_setPixel(sdl_surface,pixelType,pitch,bytesperpixel,x,y,c); {SDLwrap_regPixel((sdl_surface),(pixelType),(pitch),(bytesperpixel),(x),(y))=(c);}
#define SDLwrap_regPixel(sdl_surface,pixelType,pitch,bytesperpixelx,y) (*(pixelType *)((Uint8 *)((sdl_surface)->pixels)+(y)*(pitch)+(x)*(bytesperpixel)))

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
	Uint32 p=getPixel(surface,x,y);
	// printf("%i\n",p);
	SDL_GetRGB(p,(surface->format),&r,&g,&b);
	// printf("    %i %i %i\n",r,g,b);
	// printf("  + %i %i %i\n",dr,dg,db);
	fr=r+dr;
	fg=g+dg;
	fb=b+db;
	if (fr>255) fr=255;
	if (fg>255) fg=255;
	if (fb>255) fb=255;
	// printf("%i %i %i\n",fr,fg,fb);
	// printf(" -> %i %i %i\n",fr,fg,fb);
	p= SDL_MapRGB(surface->format,fr,fg,fb);
	setPixel(surface,x,y,p);
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

	void printSurfaceDefaults(char *name) {
		printf("// Volatile definitions for %s:\n",name);
		// printf("#define SDLwrap_%s_w %s->w\n",name,name);
		// printf("#define SDLwrap_%s_h %s->h\n",name,name);
		printf("#define SDLwrap_%s_pixelType Uint32 // guess\n",name);
		printf("#define SDLwrap_%s_pitch %s->pitch\n",name,name);
		printf("#define SDLwrap_%s_format_BytesPerPixel %s->format->BytesPerPixel\n",name,name);
		printf("#define SDLwrap_%s_Rmask %s->Rmask\n",name,name);
		printf("#define SDLwrap_%s_Rloss %s->Rloss\n",name,name);
		printf("#define SDLwrap_%s_Rshift %s->Rshift\n",name,name);
		printf("#define SDLwrap_%s_Gmask %s->Gmask\n",name,name);
		printf("#define SDLwrap_%s_Gloss %s->Gloss\n",name,name);
		printf("#define SDLwrap_%s_Gshift %s->Gshift\n",name,name);
		printf("#define SDLwrap_%s_Bmask %s->Bmask\n",name,name);
		printf("#define SDLwrap_%s_Bloss %s->Bloss\n",name,name);
		printf("#define SDLwrap_%s_Bshift %s->Bshift\n",name,name);
	}

	void printSurfaceConstants(char *name, SDL_Surface *surface) {
		printf("// Constant definitions for %s:\n",name);
		printf("#define SDLwrap_%s_w (%i)\n",name,surface->w);
		printf("#define SDLwrap_%s_h (%i)\n",name,surface->h);
		printf("#define SDLwrap_%s_pixelType Uint%i\n",name,surface->format->BitsPerPixel);
		printf("#define SDLwrap_%s_pitch (%i)\n",name,surface->pitch);
		printf("#define SDLwrap_%s_format_BytesPerPixel (%i)\n",name,surface->format->BytesPerPixel);
		printf("#define SDLwrap_%s_Rmask (%i)\n",name,surface->format->Rmask);
		printf("#define SDLwrap_%s_Rloss (%i)\n",name,surface->format->Rloss);
		printf("#define SDLwrap_%s_Rshift (%i)\n",name,surface->format->Rshift);
		printf("#define SDLwrap_%s_Gmask (%i)\n",name,surface->format->Gmask);
		printf("#define SDLwrap_%s_Gloss (%i)\n",name,surface->format->Gloss);
		printf("#define SDLwrap_%s_Gshift (%i)\n",name,surface->format->Gshift);
		printf("#define SDLwrap_%s_Bmask (%i)\n",name,surface->format->Bmask);
		printf("#define SDLwrap_%s_Bloss (%i)\n",name,surface->format->Bloss);
		printf("#define SDLwrap_%s_Bshift (%i)\n",name,surface->format->Bshift);
	}

	void printSurfaceInclude(char *name) {
		printf("// Includes for %s:\n",name);
		// printf("#define %s_w %s->w\n",name,name);
		// printf("#define %s_h %s->h\n",name,name);
		printf("#define %s_getPixel(x,y) SDLwrap_getPixel(SDLwrap_%s,SDLwrap_%s_pixelType,SDLwrap_%s_pitch,SDLwrap_%s_bytesperpixel,(x),(y))\n",name,name,name,name,name);
		printf("#define %s_setPixel(x,y) SDLwrap_setPixel(SDLwrap_%s,SDLwrap_%s_pixelType,SDLwrap_%s_pitch,SDLwrap_%s_bytesperpixel,(x),(y))\n",name,name,name,name,name);
	}

void printSurfaceDetails(char *name,SDL_Surface *surface) {
	printSurfaceDefaults(name);
	printSurfaceConstants(name,surface);
	printSurfaceInclude(name);
	printf("\n");
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
	surface=SDL_SetVideoMode(SCRWID,SCRHEI,32, 0 /* SDL_HWPALETTE */ /* | SDL_FULLSCREEN */ );
	if (surface==NULL) {
		fprintf(stderr,"Couldn't initialise video.\n");
		return 1;
	}
	printSurfaceDetails("surface",surface);
	
	bgtexture=SDL_LoadBMP("bgtexture.bmp");
	// SDL_LockSurface(bgtexture);
	printSurfaceDetails("bgtexture",surface);

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
		float bounce=1.4+0.4*qsin(frames*M_PI/10.0);
		for (i=0;i<SCRWID;i++)
			for (j=0;j<SCRHEI;j++) {
				float x=(float)(i-SCRWID/2)/(float)(SCRWID/2);
				float y=(float)(j-SCRHEI/2)/(float)(SCRHEI/2);
				Uint32 p;
				unsigned int u,v;
				unsigned char r,g,b;
				// fprintf(stderr,"%i %i : %f %f -> \n", i,j, x, y);
				x=sgn(x)*pow(fabs(x),bounce);
				y=sgn(y)*pow(fabs(y),bounce);
				u=(int)((SDL_w(bgtexture)-5)*(0.5*(x+1.0)));
				v=(int)((SDL_h(bgtexture)-5)*(0.5*(y+1.0)));
				// fprintf(stderr,"%f %f : %i %i\n", x, y, u,v);
				// fprintf(stderr,"%i\n",&regPixel(bgtexture,u,v));
				// fprintf(stderr,"%i %i\n",bgtexture->w,bgtexture->h);
				// p=((unsigned char *)(bgtexture->pixels))[bgtexture->h*v+u];
				// p=((Uint8 *)(bgtexture->pixels))[(bgtexture->h*v+u)*bgtexture->format->BytesPerPixel];
				// p=((unsigned int *)(bgtexture->pixels))[bgtexture->h*v+u];
				// fprintf(stderr,"%i\n",p);
				p=getPixel(bgtexture, u, v);
				// SDL_GetRGB(p,bgtexture->format,&r,&g,&b);
				// setPixel(surface,i,j,SDL_MapRGB(bgtexture->format,r,g,b));
				setPixel(surface,i,j,p);
				// ((int *)surface->pixels)[j*surface->h+i]=SDL_MapRGB(surface->format,r,g,b);
				// float br=0.2*(2.0+2.0*qcos((double)j/12.0+3.0*qsin((double)frames*0.0039)))*qsin(10.0*qsin(4.0*qsin((double)frames/300.0)+(double)i/25.0))+(2.0+2.0*qcos((double)j/11.0+0.012*qsin((double)frames/17.0)))*qcos(2+(double)i*0.14+(double)j/7.0+4.0*qcos(((double)frames*0.29+j/15.0)/23.0));
				// if (br<0.0) br=0.0;
				// if (br>1.0) br=1.0;
				// ((int *)surface->pixels)[j*SCRWID+i]=MapRGB(surface->format,br*i*256/SCRWID,br*j*256/SCRHEI,128+127*qsin(clock()*3.141/5.0/(float)CLOCKS_PER_SEC));
				// ((int *)surface->pixels)[j*SCRWID+i]=0;
				// ((int *)surface->pixels)[j*SCRWID+i]=MapRGB(surface->format,i*256/SCRWID,j*256/SCRHEI,128+127*qs);
			}

		#define brightness 80
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
