/**************************************************************************/
/*                                                                        */
/*             BasicWindow:  A Simple Window Graphics Library             */
/*            ------------------------------------------------            */
/*                Neil Pollard, University of Bristol 1995                */
/*                                                                        */
/*            Please read the file README for more information            */
/*                                                                        */
/**************************************************************************/

/* BWlib.h */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

/* definitions of event-type integers returned by BWCheckEvents
 * These values are subject to change */

#define EV_UNDEF 	0
#define EV_EXPOSED 	1
#define EV_BUTTON	2
#define EV_KEY		3

/* the BasicWin structure
 * This should be opaque to users, and is subject to change,
 * so do not manipulate this structure directly! */

typedef struct
	{
	Display *display;
	XEvent win_report;
	Window win;
	GC win_gc;
	XFontStruct *win_font_info;
	}
BasicWin;

/* Funtions available to users */

extern Display *	BWConnectToServer();
extern BasicWin 	BWCreateNewWindow(Display*, int, char**);
extern int 		BWCheckEvents(BasicWin);
extern void 		BWCloseWindow(BasicWin);
extern void 		BWPrintText(BasicWin, char*, int, int);
extern void 		BWDrawLine(BasicWin, int, int, int, int);
extern void 		BWDrawRectangle(BasicWin, int, int, int, int);
extern void 		BWDrawPoint(BasicWin, int, int);
extern void 		BWClearArea(BasicWin, int, int, int, int);
