/* Simple x client without error checking
 * extended with color and graphic drawing 
 * 
 * gcc -o xlib-2 -Wall -O3 xlib-2.c -L/usr/X11R6/lib/ -lX11
 *
 * Written by cel98csn@mds.mdh.se, April 1999
 */
#include <X11/Xlib.h>

GC yin, yang;

/* Round to the nearest integer */
#define r(x)	(int)((x)+.5)
void yinyang(Display *d, Window win, int w, int h)
{
   XFillArc(d, win,  yin,    0, 0, w, h, 90*64,  180*64);
   XFillArc(d, win, yang,    0, 0, w, h, 90*64, -180*64);

   XFillArc(d, win,  yin, r(w*.25), r(   0), r(w*.5), r(h*.5), 90*64, -180*64);
   XFillArc(d, win, yang, r(w*.25), r(h*.5), r(w*.5), r(h*.5), 90*64, 180*64);

   XFillArc(d, win,  yin, r(w*.4), r(h*.65), r(w*.2), r(h*.2), 0, 360*64);
   XFillArc(d, win, yang, r(w*.4), r(h*.15), r(w*.2), r(h*.2), 0, 360*64);

   XDrawArc(d, win, yin, 0, 0, w, h, 0, 360*64);
   
   return;
}
#undef r

int main(void)
{
   Display *d; int s; Window w; XEvent ev;
   int should_quit = 0;
   int width,height;
   XGCValues values;
   unsigned long valuemask;
   XColor color, exact;
   unsigned long pixel[2];

   d = XOpenDisplay(NULL);
   s = XDefaultScreen(d);

   w = XCreateSimpleWindow(d, XRootWindow(d, s), 0, 0,
                           width=200, height=200, 0,
                           XBlackPixel(d, s),
                           XWhitePixel(d, s));

   if(XAllocNamedColor(d, DefaultColormap(d,s), "blue", &color, &exact))
     pixel[0] = color.pixel;
   else
     pixel[0] = XBlackPixel(d, s);

   if(XAllocNamedColor(d, DefaultColormap(d,s), "yellow", &color, &exact))
     pixel[1] = color.pixel;
   else
     pixel[1] = XWhitePixel(d, s);

   values.foreground = pixel[0];
   values.line_width = 2;
   values.fill_style = FillSolid;
   values.arc_mode = ArcPieSlice;
   valuemask = 0;
   valuemask |= GCForeground | GCLineWidth | GCFillStyle | GCArcMode;
   yin = XCreateGC(d, w, valuemask, &values);
   values.foreground = pixel[1];
   yang = XCreateGC(d, w, valuemask, &values);

   XSelectInput(d, w, ButtonPressMask|ExposureMask|StructureNotifyMask);
   XMapWindow(d, w);

   while(!should_quit)
     {
	XNextEvent(d, &ev);
	switch(ev.type)
	  {
	   case ConfigureNotify:
	     width = ev.xconfigure.width;
	     height = ev.xconfigure.height;
	     break;
	   case Expose:
	     if(ev.xexpose.count==0)
	       yinyang(d, w, width, height);
	     break;
	   case ButtonPress:
	     should_quit = 1;
	     break;
	   default:
	  }
     }
   
   return 0;
}
