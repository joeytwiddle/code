// todo: Any macros which use a provided expression N times
//   should be turned into functions if, in terms of time,
//   N*eval(expression) > 1*alloc(type)+1*eval(expression)+N*refer(type)
//   (This will be done like BR and TR I guess.)
//   Or does the C compiler do this automatically, assuming state of
//   expression does not change during macro.
// todo: Resolve palette==NULL at init and switch routing of code to be execed.
// 	 Or make it part of the SDLwrap model.
// 	 Or just ignore it because we will only stand for truecolour.
// todo: Both BR and TR should be given functions to perform on the pixel register
//   rather than x,y.  This will allow the algorithms to perform fast
//   reg++'s rather than the given function performing reg=big_calculation(x,y)
//   (Actually keep passing x,y just in case the plot function wants to use it.)
// process: Are we ever forced to blit on a 16bit surface?
//   If so, would it be efficient to use a hidden 32bit surface to do
//   read/writes for transparency, and then blit via the 16bit surface?
// process: Is there in general a better local surface we can use for lookups
//   before finally writing to the screen?
// process: When rendering two overlapping triangles, the result is three
//   polygons: two containing the pure colours, and one the merged colour.
//   We can avoid reading pixels from the map if we find the triangle
//   intersections first, and then render the resulting polygons with
//   the merged colours pre-calculated.
//   (In fact only one pixel read is needed, for the background.)
//   This requires 2D "CSG".
// todo: Of course, stop using full colour and start cheating.
//   eg. Use transparencies of one colour (essentially greyscale),
//   or OR otherwise limit bits as Julian suggests.
// todo: Store #define constants for a variety of video modes so that the
//   user may choose at runtime.

#include <sched.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>

#define SCRWID 320
#define SCRHEI 200
// #define SCRWID 640
// #define SCRHEI 400
#define SCRBPS 32
#define NUMTRIS 300
#define desiredFramesPerSecond 50

#include "define.c"
#define CONST
#include "incl.c"
#undef CONST

#define new(t,c) ((t *)malloc(sizeof(t)*c))
#define sgn(x) ((x)>0.0?1.0:-1.0)
/* 0.0 <= frand() < 1.0 */
#define frand() ((float)(rand()/2)/(float)(RAND_MAX/2+1))
// #define nearhalf() (frand())
#define nearhalf() (0.5+0.8*(frand()-0.5)*frand())
#define cap(x,b) ( (x) > (b) ? (b) : (x) )
#define square(x) ( (x) * (x) )
#define clip(x,a,b) ( (x)<(a) ? (a) : (x)>=(b) ? (b)-1 : (x) )

// #define paletteChoice(p,a,b) a
// #define paletteChoice(p,a,b) ( ( (p) == NULL ) ? (a) : (b) )
// #define paletteChoice(p,a,b) { if ( (p) == NULL ) { (a) } else { (b) } }
// #define paletteChoice(p,a,b) b
// This does seem to speed it up a little
// Macro/inline clones of SDL_MapRGB and SDL_GetRGB
// #define MapRGB SDL_MapRGB
// #define MapRGB(format,r,g,b) ( paletteChoice( (format)->palette , TrueMapRGB((format),(r),(g),(b)) , PalMapRGB((format),(r),(g),(b)) )
// #define PalMapRGB(format,r,g,b) ( SDL_FindColor((format)->palette, (r), (g), (b)) )
// #define TrueMapRGB(format,r,g,b) ( ((int)(r) >> (format)->Rloss) << (format)->Rshift | ((int)(g) >> (format)->Gloss) << (format)->Gshift | ((int)(b) >> (format)->Bloss) << (format)->Bshift | (format)->Amask )
// The reading functions offer even greater improvement (due to being heavy!)
// #define GetRGB SDL_GetRGB
// #define GetRGB(pixel,fmt,r,g,b) paletteChoice( (fmt)->palette , TrueGetRGB((pixel),(fmt),(r),(g),(b)) , PalGetRGB((pixel),(fmt),(r),(g),(b)) )
// #define PalGetRGB(pixel,fmt,red,green,blue) { *(red) = (fmt)->palette->colors[(pixel)].r; *(green) = (fmt)->palette->colors[(pixel)].g; *(blue) = (fmt)->palette->colors[(pixel)].b; }
/* void inline TrueGetRGB(Uint32 pixel, SDL_PixelFormat *fmt, Uint8 *r,Uint8 *g,Uint8 *b) {
                unsigned rv, gv, bv;
                rv = (pixel & fmt->Rmask) >> fmt->Rshift;
                *r = (rv << fmt->Rloss) + (rv >> (8 - fmt->Rloss));
                gv = (pixel & fmt->Gmask) >> fmt->Gshift;
                *g = (gv << fmt->Gloss) + (gv >> (8 - fmt->Gloss));
                bv = (pixel & fmt->Bmask) >> fmt->Bshift;
                *b = (bv << fmt->Bloss) + (bv >> (8 - fmt->Bloss));
} */


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

inline void merge(SDL_Surface *screen,int x,int y,Uint8 dr,Uint8 dg,Uint8 db) {
	Uint8 r,g,b;
	Uint16 fr,fg,fb;
	// Uint32 p=getPixel(screen,x,y);
	// Uint32 p=screen_getPixel(x,y);
	screen_pixelType p=screen_getPixel(x,y);
	// printf("%i\n",p);
	// SDL_GetRGB(p,(screen->format),&r,&g,&b);
	screen_GetRGB(p,&r,&g,&b);
	// printf("    %i %i %i\n",r,g,b);
	// printf("  + %i %i %i\n",dr,dg,db);
	fr=r+dr;
	fg=g+dg;
	fb=b+db;
	if (fr>255) fr=255;
	if (fg>255) fg=255;
	if (fb>255) fb=255;
	// p= SDL_MapRGB(screen->format,fr,fg,fb);
	p= screen_MapRGB(fr,fg,fb);
	screen_setPixel(x,y,p);
}

void plotLine(SDL_Surface *screen,int BR_x1,int BR_y1,int BR_x2,int BR_y2,Uint32 colour) {
	// #define BR_todo(x,y)  ((int *)screen->pixels)[y*screen_w+x]=colour
	// #define BR_todo(x,y)  setPixel(screen,x,y,colour);
	#define BR_todo(x,y)  screen_setPixel(x,y,colour);
	#include "bresenham.c"
	#undef BR_todo(x,y)
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

void plotTriRGB(SDL_Surface *screen,int TR_x1,int TR_y1,int TR_x2,int TR_y2,int TR_x3, int TR_y3, Uint8 r,Uint8 g,Uint8 b) {
	#define TR_todo(x,y) merge(screen,x,y,r,g,b);
	#include "tri.c"
	#undef TR_todo
	// plotLine(screen,TR_x1,TR_y1,TR_x2,TR_y2,screen_MapRGB(255,255,255));
	// plotLine(screen,TR_x1,TR_y1,TR_x3,TR_y3,screen_MapRGB(255,255,255));
	// plotLine(screen,TR_x2,TR_y2,TR_x3,TR_y3,screen_MapRGB(255,255,255));
}

int main(int argc,char *argv[]) {

	SDL_Surface *screen,*bgtexture;
	SDL_Event event;
	int keepLooping=1;
	int frames=0;
	clock_t starttime;
	FILE *fp;
    int cylcenx=SCRWID/2,cylceny=SCRHEI/2;

	setuptriglookup();
	// srand(time(NULL));

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr,"Couldn't initialise SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	screen=SDL_SetVideoMode(SCRWID,SCRHEI,SCRBPS, SDL_HWPALETTE /* | SDL_FULLSCREEN */ );
	if (screen==NULL) {
		fprintf(stderr,"Couldn't initialise video.\n");
		return 1;
	}
	
	bgtexture=SDL_LoadBMP("bgtexture.bmp");
	// SDL_LockSurface(bgtexture);

	fp=fopen("incl.c","wa");
	printSurfaceDetails(fp,"screen",screen);
	printSurfaceDetails(fp,"bgtexture",bgtexture);
	fclose(fp);

	starttime = clock()+CLOCKS_PER_SEC/desiredFramesPerSecond;

	while (keepLooping) {
		
		int releasetime = clock()+CLOCKS_PER_SEC/desiredFramesPerSecond;

		if ((frames % 20) == 0) {
			fprintf(stderr,"\rframes/second: %.2f   triangles/second: %.0f ",(float)CLOCKS_PER_SEC*(float)frames/(float)(releasetime-starttime),NUMTRIS*(float)CLOCKS_PER_SEC*(float)frames/(float)(releasetime-starttime));
		}
		
		SDL_Flip(screen);

        #define brown 0.01
        // cylcenx+=clip(brown*SCRWID*(-1.0+2.0*frand()),0,SCRWID);
        // cylceny+=clip(brown*SCRHEI*(-1.0+2.0*frand()),0,SCRHEI);
        // cylcenx+=brown*SCRWID*(-1.0+2.0*frand());
        // cylceny+=brown*SCRHEI*(-1.0+2.0*frand());
        cylcenx=SCRWID/2+SCRWID/2*sin(frames*0.017);
        cylceny=SCRHEI/2+SCRHEI/2*cos(frames*0.012);

        #define IMGSKIP (SCRWID/160)
		{
		Uint16 i,j;
		float qs=qsin((float)frames*3.141/45.0);
        Uint8 qsi=128+127*qs;
		float bounce=0.1+0.099*qcos(frames*M_PI/120.0);
		float bounceB=2.0+1.5*qsin(frames*M_PI/120.0);
		float bounceC=0.05-0.05*(pow((qcos(frames*M_PI/130.0)+1.0)/2.0,0.6)*2.0-1.0);
        Sint16 Doffset=100000-frames*1.0;
        Uint16 aoffset=2000+frames*2.0+sin(frames*0.03)*200.0;
		for (i=0;i<screen_w;i+=IMGSKIP) {
				Uint16 u,v;
                // Uint8 thruwid=i*256/screen_w;
			for (j=0;j<screen_h;j+=IMGSKIP) {
				Uint8 r,g,b,br;
				Uint32 p;
                // todo: Optimise this!
                Uint16 d=sqrt(square(i-cylcenx)+square(j-cylceny)); //+bounceB*sqrt(square(i-SCRWID/2)+square(j-SCRHEI/2));
                // Uint16 d=(abs(i-cylcenx)+abs(j-cylceny));
                Uint16 D=1000.0/pow(d,bounce)+Doffset;
                Uint16 a=(Uint16)((float)bgtexture_w/M_PI*(atan2(i-cylcenx,j-cylceny)+M_PI)+aoffset);
                u=a%bgtexture_w;
                v=D%bgtexture_h;
				// p=getPixel(bgtexture, u, v);
				p=bgtexture_getPixel( u, v);
				// setPixel(screen,i,j,p);
				// screen_setPixel(i,j,p);
				// Slow but necessary if the colorspaces are different
				bgtexture_GetRGB(p,&r,&g,&b);
                // br=clip(200.0/pow(0.01*d,8.0),0,256);
                br=clip(2000*(SCRWID*2-d)*bounceC/SCRWID,0,256);
                r=clip((int)r+(int)br,0,256);
                g=clip((int)g+(int)br,0,256);
                b=clip((int)b+(int)br,0,256);
				p=screen_MapRGB(r,g,b);
                { int I,J;
                    for (I=0;I<IMGSKIP;I++)
                        for (J=0;J<IMGSKIP;J++) {
                            screen_setPixel(i+I,j+J,p);
                        }
                }
				// SDL_GetRGB(p,bgtexture->format,&r,&g,&b);
				// SDL_MapRGB(screen->format,r,g,b);
				// float br=0.2*(2.0+2.0*qcos((double)j/12.0+3.0*qsin((double)frames*0.0039)))*qsin(10.0*qsin(4.0*qsin((double)frames/300.0)+(double)i/25.0))+(2.0+2.0*qcos((double)j/11.0+0.012*qsin((double)frames/17.0)))*qcos(2+(double)i*0.14+(double)j/7.0+4.0*qcos(((double)frames*0.29+j/15.0)/23.0));
				// if (br<0.0) br=0.0;
				// if (br>1.0) br=1.0;
				// ((int *)screen->pixels)[j*screen_w+i]=MapRGB(screen->format,br*i*256/screen_w,br*j*256/screen_h,128+127*qsin(clock()*3.141/5.0/(float)CLOCKS_PER_SEC));
				// ((int *)screen->pixels)[j*screen_w+i]=0;
				// ((int *)screen->pixels)[j*screen_w+i]=MapRGB(screen->format,i*256/screen_w,j*256/screen_h,128+127*qs);
				// screen_setPixel(i,j,screen_MapRGB(thruwid,j*256/screen_h,qsi));
			}
		}

		#define brightness 11
		for (i=0;i<NUMTRIS;i++) {
			// plotLine(screen,screen_w*frand(),screen_h*frand(),screen_w*frand(),screen_h*frand(),MapRGB(screen->format,256*frand(),256*frand(),256*frand()));
			// plotTri(screen,screen_w*frand(),screen_h*frand(),screen_w*frand(),screen_h*frand(),screen_w*frand(),screen_h*frand(),MapRGB(screen->format,256*frand(),256*frand(),256*frand()));
			plotTriRGB(screen,screen_w*nearhalf(),screen_h*nearhalf(),screen_w*nearhalf(),screen_h*nearhalf(),screen_w*nearhalf(),screen_h*nearhalf(),brightness*nearhalf(),brightness*nearhalf(),brightness*nearhalf());
		}
		}

		frames++;

		while (SDL_PollEvent(&event)) {
			if (event.type==SDL_KEYDOWN || event.type==SDL_QUIT || event.type==SDL_MOUSEBUTTONDOWN) {
				keepLooping=0;
			}
		}
		
        {
		    int timetogo;
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
    
    printf("\n");

	return 0;

}
