/* Simple x client without error checking
 * drawing text and reading keyboard input 
 * 
 * gcc -o xlib-3 -Wall -O3 xlib-3.c -L/usr/X11R6/lib/ -lX11
 *
 * Written by cel98csn@mds.mdh.se, April 1999
 */
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h> 		/* for XLookupString */
#include <stdio.h>

int main(void)
{
   Display *d; int s; Window w; XEvent ev;
   int should_quit = 0;
   int width,height;
   XFontStruct *font;
   XCharStruct text_bounds;
   XGCValues values;
   unsigned long valuemask;
   GC gc;
   char outdata[256] = "Press me!";
   int dummy;

   d = XOpenDisplay(NULL);
   s = XDefaultScreen(d);

   w = XCreateSimpleWindow(d, XRootWindow(d, s), 0, 0,
                           width=200, height=200, 0,
                           XBlackPixel(d, s),
                           XWhitePixel(d, s));

   font = XLoadQueryFont(d, "variable");

   values.foreground = XBlackPixel(d, s);
   values.font = font->fid;
   valuemask = 0;
   valuemask |= GCForeground | GCFont;

   gc = XCreateGC(d, w, valuemask, &values);

   XTextExtents(font, outdata, strlen(outdata), &dummy, &dummy, &dummy,
                &text_bounds);

   XSelectInput(d, w, ButtonPressMask|ExposureMask|KeyPressMask);
   XMapWindow(d, w);

   while(!should_quit)
     {
        XNextEvent(d, &ev);
        switch(ev.type)
          {
           case Expose:
             XDrawString(d, w, gc, (width-text_bounds.width)/2,
                         (height-text_bounds.ascent-text_bounds.descent)/2,
                         outdata, strlen(outdata));
             break;
           case ButtonPress:
             sprintf(outdata, "That was button %d", ev.xbutton.button);
             XTextExtents(font, outdata, strlen(outdata), &dummy, &dummy,
                          &dummy, &text_bounds);
             /* This clears and makes sure we receive a new expose. */
             XClearArea(d, w, 0,0,0,0, 1);
             break;
           case KeyPress: {
              KeySym keysym;
              char buf[32];
              int n;
              n = XLookupString(&ev.xkey, buf, sizeof(buf),
                                &keysym, NULL);

              if(keysym== XK_Escape)
                should_quit = 1;
              else if(n)
                {
                   int i;
		   for(i=0; i<n; i++) 
		     fputc(buf[i], stdout);
                   fflush(stdout);
                }
           }
	     break;
           default:
          }
     }

   return 0;
}
