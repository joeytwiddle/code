/* Simple x client without any error checking 
 * 
 * gcc -o xlib-1 -Wall -O3 xlib-1.c -L/usr/X11R6/lib/ -lX11
 * 
 * Written by cel98csn@mds.mdh.se, April 1999
 */
#include <X11/Xlib.h>


int main(void)
{
  Display *d; int s; Window w; XEvent ev;
  int should_quit = 0;

  d = XOpenDisplay(NULL);
  s = XDefaultScreen(d);

  w = XCreateSimpleWindow(d, XRootWindow(d, s), 0, 0,
                          200, 200, 0,
                          XBlackPixel(d, s),
                          XWhitePixel(d, s));

  XSelectInput(d, w, ButtonPressMask);
  XMapWindow(d, w);

  while(!should_quit)
    {
    XNextEvent(d, &ev);
    switch(ev.type)
        {
      case ButtonPress:
        should_quit = 1;
        break;
      default:
        }
    }

  return 0;
}
