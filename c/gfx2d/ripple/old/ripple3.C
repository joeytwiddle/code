/**
 ** COLOROPS.C ---- test WRITE, XOR, OR, and AND draw modes
 **
 ** Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 ** [e-mail: csaba@vuse.vanderbilt.edu] See "doc/copying.cb" for details.
 **/

#include "grx20.h"
#include "drawing.h"
#include <math.h>

void main(void) {
	
	int x,y,i;
	float r,g,b;
	GrColor c[255];
	
	GrSetMode(GR_width_height_graphics,320,200);
	//GrSetMode(GR_default_graphics);
	//GrSetMode(GR_default_text);
	//printf("hello?");
	//GrSetRGBcolorMode();
	
	/*for (i=0; i<=255; i++) {
		GrAllocCell();
		GrSetColor(i,i,i,i);
	}*/
	for (i=0; i<=255; i++) {
		c[i]=GrAllocColor(i,i,i);
	}

//#   define XP(x)   (int)((((long)(x) * (long)xsize) / 100L) + xpos)
//#   define YP(y)   (int)((((long)(y) * (long)ysize) / 100L) + ypos)
	//GrLine(XP(10),YP(10),XP(40),YP(40),fg);

	/*while(!kbhit()) { }
	getch();*/

	i=0;
	for (x=0;x<=319;x++) {
	for (y=0;y<199;y++) {
		r=x/320;
		g=y/200;
		b=0;
		//i=63*r+255*64*g+65535*64*b;
		//c=x+y*320;
		//printf("%f",(float)((x-160)^2+(y-100)^2)));
		//while(!kbhit()) { }
		//getch();
		//r=sqrt((float)((x-160)^2+(y-100)^2));
		//c=r*65536/sqrt((float)(160*160+100*100));
		//printf("%l,",c[i]);
		i++;
		if (i>255)
			i=0;
		GrPlot(x,y,c[i]);
	}
	}

	while(!kbhit()) { }
	getch();

}


//#include "test.h"
//#include "rand.h"

//TESTFUNC(colorops)
//{
	
	
/*    GrFBoxColors bcolors,ocolors,icolors;
	GrColor bg,c;
	int x = GrSizeX();
	int y = GrSizeY();
	int ww = (x * 2) / 3;
	int wh = (y * 2) / 3;
	int ii,jj;
	int wdt = ww / 150;
	int bw = x / 16;
	int bh = y / 16;
	int bx,by;

	// This won't work very well under X11 in pseudocolor
	//** mode (256 colors or less) if not using a private
	//** color map. The missing colors break RGB mode      
	GrSetRGBcolorMode();

	bcolors.fbx_intcolor = GrAllocColor(160,100,30);
	bcolors.fbx_topcolor = GrAllocColor(240,150,45);
	bcolors.fbx_leftcolor = GrAllocColor(240,150,45);
	bcolors.fbx_rightcolor = GrAllocColor(80,50,15);
	bcolors.fbx_bottomcolor = GrAllocColor(80,50,15);

	ocolors.fbx_intcolor = GrAllocColor(0,120,100);
	ocolors.fbx_topcolor = GrAllocColor(0,180,150);
	ocolors.fbx_leftcolor = GrAllocColor(0,180,150);
	ocolors.fbx_rightcolor = GrAllocColor(0,90,60);
	ocolors.fbx_bottomcolor = GrAllocColor(0,90,60);

	icolors.fbx_intcolor = GrAllocColor(30,30,30);
	icolors.fbx_bottomcolor = GrAllocColor(0,180,150);
	icolors.fbx_rightcolor = GrAllocColor(0,180,150);
	icolors.fbx_leftcolor = GrAllocColor(0,90,60);
	icolors.fbx_topcolor = GrAllocColor(0,90,60);

	c  = GrAllocColor(250,250,0);
	bg = GrNOCOLOR;

	for(ii = 0,by = -(bh / 3); ii < 17; ii++) {
		for(jj = 0,bx = (-bw / 2); jj < 17; jj++) {
		GrFramedBox(bx+2*wdt,by+2*wdt,bx+bw-2*wdt-1,by+bh-2*wdt-1,2*wdt,&bcolors);
		bx += bw;
		}
		by += bh;
	}

	GrFramedBox(ww/4-5*wdt-1,wh/4-5*wdt-1,ww/4+5*wdt+ww+1,wh/4+5*wdt+wh+1,wdt,&ocolors);
	GrFramedBox(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);

	GrSetClipBox(ww/4,wh/4,ww/4+ww,wh/4+wh);

	drawing(ww/4,wh/4,ww,wh,c,bg);
	while(!kbhit()) {
		drawing(ww/4+(RND()%100),
		wh/4+(RND()%100),
		ww,
		wh,
		((RND() / 16) & (GrNumColors() - 1)),
		bg
		);
	}
	getch();
	GrFramedBox(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);
	drawing(ww/4,wh/4,ww,wh,c,bg);
	while(!kbhit()) {
		drawing(ww/4+(RND()%100),
		wh/4+(RND()%100),
		ww,
		wh,
		((RND() / 16) & (GrNumColors() - 1)) | GrXOR,
		bg
		);
	}
	getch();
	GrFramedBox(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);
	drawing(ww/4,wh/4,ww,wh,c,bg);
	while(!kbhit()) {
		drawing(ww/4+(RND()%100),
		wh/4+(RND()%100),
		ww,
		wh,
		((RND() / 16) & (GrNumColors() - 1)) | GrOR,
		bg
		);
	}
	getch();
	GrFramedBox(ww/4-1,wh/4-1,ww/4+ww+1,wh/4+wh+1,wdt,&icolors);
	drawing(ww/4,wh/4,ww,wh,c,bg);
	while(!kbhit()) {
		drawing(ww/4+(RND()%100),
		wh/4+(RND()%100),
		ww,
		wh,
		((RND() / 16) & (GrNumColors() - 1)) | GrAND,
		bg
		);
	}
	getch();*/

