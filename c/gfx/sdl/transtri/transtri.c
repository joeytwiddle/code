// todo: Interpolate BG
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
#define IMGSKIP (SCRWID/160)
// #define IMGSKIP 1
// #define DOUBLEMERGEBG

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
int sqrtlookup[sqrtlookupmax];
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
// One of those funky visuals - notice the curvature
// #define qsqrt(x) ( sqrtlookup[(Sint16)x] )
// #define qsqrt(x) sqrt(x)

void setuplookups() {
    setuptriglookup();
    setupsqrtlookup();
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
    // note: Sum of A B will exceed 255 if from top bit down either both bits are set or one bit is set, check next (worst case 00000001+11111111)
    // process: In asm, don't we get a register bit set if there was an overflow?
	if (fr>255) fr=255;
	if (fg>255) fg=255;
	if (fb>255) fb=255;
	// p= SDL_MapRGB(screen->format,fr,fg,fb);
	p= screen_MapRGB(fr,fg,fb);
	screen_setPixel(x,y,p);
}

#define halfClipLine(XA,YA,XB,YB,LX,HX,LY,HY) { if (XA<LX) { YA = YA + (YB-YA)*(LX-XA)/(XB-XA); XA = LX; } if (XA>=HX) { YA = YA + (YB-YA)*(HX-XA)/(XB-XA); XA = HX-1; } if (YA<LY) { XA = XA + (XB-XA)*(LY-YA)/(YB-YA); YA = LY; } if (YA>=HY) { XA = XA + (XB-XA)*(HY-YA)/(YB-YA); YA = HY-1; } }
// #define halfClipLine(XA,YA,XB,YB,LX,HX,LY,HY) { if ((*(XA))<LX) { (*(YA)) = (*(YA)) + ((*(YB))-(*(YA)))*(LX-(*(XA)))/((*(XB))-(*(XA))); (*(XA)) = LX; } if ((*(XA))>=HX) { (*(YA)) = (*(YA)) + ((*(YB))-(*(YA)))*(HX-(*(XA)))/((*(XB))-(*(XA))); (*(XA)) = HX-1; } if ((*(YA))<LY) { (*(XA)) = (*(XA)) + ((*(XB))-(*(XA)))*(LY-(*(YA)))/((*(YB))-(*(YA))); (*(YA)) = LY; } if ((*(YA))>=HY) { (*(XA)) = (*(XA)) + ((*(XB))-(*(XA)))*(HY-(*(YA)))/((*(YB))-(*(YA))); (*(YA)) = HY-1; } }
// #define clipLine(XA,YA,XB,YB,LX,HX,LY,HY) { halfClipLine(XA,YA,XB,YB,LX,HX,LY,HY); halfClipLine(XB,YB,XA,YA,LX,HX,LY,HY); }
#define clipLine(XA,YA,XB,YB,LX,HX,LY,HY) { halfClipLine(XA,YA,XB,YB,LX,HX,LY,HY); halfClipLine(XB,YB,XA,YA,LX,HX,LY,HY); }

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
	clock_t starttime;
	FILE *fp;
    int cylcenx=SCRWID/2,cylceny=SCRHEI/2;

	setuplookups();
	// srand(time(NULL));

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr,"Couldn't initialise SDL: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
    SDL_WM_SetCaption("transience",NULL);
	screen=SDL_SetVideoMode(SCRWID,SCRHEI,SCRBPS, SDL_HWPALETTE /* | SDL_FULLSCREEN */ );
	if (screen==NULL) {
		fprintf(stderr,"Couldn't initialise video.\n");
		return 1;
	}

	bgtexture=SDL_LoadBMP("bgtexture.bmp");
	bgtexture2=SDL_LoadBMP("bgtexture2.bmp");
	// SDL_LockSurface(bgtexture);

	fp=fopen("incl.c","wa");
	printSurfaceDetails(fp,"screen",screen);
	printSurfaceDetails(fp,"bgtexture",bgtexture);
	printSurfaceDetails(fp,"bgtexture2",bgtexture2);
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

		{
		Uint16 i,j;
		// float qs=qsin((float)frames*3.141/45.0);
        // Uint8 qsi=128+127*qs;
		// float bounce=0.1+0.099*qcos(frames*M_PI/120.0);
		// float bounceB=2.0+1.5*qsin(frames*M_PI/120.0);
		Uint16 bounceC=2000*(0.05-0.05*(pow((qcos(frames*M_PI/115.0-2.5)+1.0)/2.0,0.6)*2.0-1.0));
        Sint16 Doffset=100000+frames*10.0-600.0*qcos(frames*M_PI/120.0-1.0);
        Uint16 aoffset=2000+frames*2.0+sin(frames*0.03-0.1)*200.0;
        Uint32 overd=20000*(0.5+0.5*qcos(frames*M_PI/120.0-1.0));
        // Uint32 tmp=((Uint16)bgtexture_w << 8)/M_PI;
        // Sint16 D2offset=Doffset+400.0*qsin(frames*M_PI/140.0+123.6);
        // Sint16 a2offset=aoffset+100.0*qsin(frames*M_PI/140.0+123.6);
        #ifdef DOUBLEMERGEBG
            Sint16 D2offset=1234+Doffset+frames*4.0+200.0*sin(frames*M_PI/123.0);
            Sint16 a2offset=4321+aoffset+0.4*frames+20.0*sin(frames*M_PI/453.0+1.2);
        #endif
        SDL_Rect dstrect;
        dstrect.w=IMGSKIP;
        dstrect.h=IMGSKIP;
        for (i=0;i<screen_w;i+=IMGSKIP) {
            // Uint8 thruwid=i*256/screen_w;
            Uint32 xpart=(Uint32)square(i-cylcenx);
            dstrect.x=i;
            dstrect.y=0;
            for (j=0;j<screen_h;j+=IMGSKIP, dstrect.y+=IMGSKIP) {
                Uint8 r,g,b,br;
                Uint32 p;
                // todo: Optimise this!
                Uint16 d=qsqrt(xpart+square(j-cylceny)); //+bounceB*sqrt(square(i-SCRWID/2)+square(j-SCRHEI/2));
                // Uint16 d=(abs(i-cylcenx)+abs(j-cylceny));
                // Uint16 D=1000.0/pow(d,bounce)+Doffset;
                // Uint16 D=(float)overd/d+Doffset;
                // Uint16 D=( (d & 65534) == 0 ? 0 : overd/d+Doffset);
                Uint16 D=(d<2 ? 0 : overd/d);
                // Uint16 a=(Uint16)((((Uint32)(tmp*(atan2(i-cylcenx,j-cylceny))) >> 8)+M_PI)+aoffset);
                Uint16 a=(Uint16)(bgtexture_w*(atan2(i-cylcenx,j-cylceny)/M_PI+M_PI));
                // dstrect.y=j;
                // p=getPixel(bgtexture, u, v);
                p=bgtexture_getPixel( ((Uint16)(a+aoffset)%bgtexture_w), ((Uint16)(D+Doffset)%bgtexture_h) );
                bgtexture_GetRGB(p,&r,&g,&b);
                // br=clip((Uint16)D/3.0,0,255);
                br=clip((SCRWID*2-d)*bounceC/SCRWID,0,256);
#ifdef DOUBLEMERGEBG
                {
                    Uint8 r2,g2,b2;
                    Uint32 p2;
                    p2=bgtexture2_getPixel( ((Uint16)(a+a2offset)%bgtexture2_w), ((Uint16)(D+D2offset)%bgtexture2_h) );
                    bgtexture2_GetRGB(p2,&r2,&g2,&b2);
                    // setPixel(screen,i,j,p);
                    // // screen_setPixel(i,j,p);
                    // Slow but necessary if the colorspaces are different
                    // br=clip(200.0/pow(0.01*d,8.0),0,256);
                    r=clip((int)r/2+(int)r2/2+(int)br,0,256);
                    g=clip((int)g/2+(int)g2/2+(int)br,0,256);
                    b=clip((int)b/2+(int)b2/2+(int)br,0,256);
                }
#else
                r=clip((int)r+(int)br,0,256);
                g=clip((int)g+(int)br,0,256);
                b=clip((int)b+(int)br,0,256);
#endif
                // r=max(r,br);
                // g=max(g,br);
                // b=max(b,br);
                p=screen_MapRGB(r,g,b);
                SDL_FillRect(screen,&dstrect,p);
                // if (i>0 && j>0) {
                //     // Interpolate last square to topleft
                // }
                // { register int I,J;
                // for (I=0;I<IMGSKIP;I++)
                // for (J=0;J<IMGSKIP;J++) {
                // screen_setPixel(i+I,j+J,p);
                // }
                // }
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

        {

            // float camX=sin(frames*0.013);
            // float camY=cos(frames*0.013);
            // float camZ=-1; // sin(frames*0.013+1.235);

            #define brightness 120
            // #define STARY (SCRHEI/2)
            #define SCALE ( SCRWID*2 )

            // #define STARX (2*SCRWID/3)
            float STARX=SCRWID/2+SCRWID/6*sin(frames*0.02);
            float STARY=SCRHEI/2+SCRWID/6*cos(frames*0.02)*sin(frames*0.001);
            #define starPlotTri(xa,ya,xb,yb,xc,yc,bright) plotTriRGB(screen,xa,ya,xb,yb,xc,yc,bright*brightness,bright*brightness/2.0,bright*brightness)
            #define starRad(a,b) ( (9.0)*(2.0+sin((a)*2.1+frames*0.063)+sin((b)*2.6+frames*0.05)) )
            // #define starRad(a,b) ( 20 )
            #define starRes 40
            #include "starsurface.c"
            #undef starRad
            #undef starRes
            #undef starPlotTri
            // #undef STARX

            STARX=SCRWID/2-SCRWID/6*sin(frames*0.02);
            STARY=SCRHEI/2-SCRWID/6*cos(frames*0.02)*sin(frames*0.001);
            // #define STARX (SCRWID/3)
            #define starPlotTri(xa,ya,xb,yb,xc,yc,bright) plotTriRGB(screen,xa,ya,xb,yb,xc,yc,bright*brightness/2.0,bright*brightness/2,0)
            #define starRad(a,b) ( (7.0)*(3.0+sinsharp((-a)*2.5+frames*0.044)+sinsharp((b)*3.5+frames*0.031)) )
            #define starRes 50
            #include "starsurface.c"
            #undef starRad
            #undef starRes
            #undef starPlotTri
        
        }

            for (i=0;i<NUMTRIS;i++) {
                // plotTriRGB(screen,screen_w*nearhalf(),screen_h*nearhalf(),screen_w*nearhalf(),screen_h*nearhalf(),screen_w*nearhalf(),screen_h*nearhalf(),brightness*nearhalf(),brightness*nearhalf(),brightness*nearhalf());
                // #define starRad(vx,vy,vz) ( 40.0*qsin(vx+frames*0.024)+qsin(vy+frames*0.016)+qsin(vz+frames*0.0048) )
            }

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
    
    printf("\n");

	return 0;

}
