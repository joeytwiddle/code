#ifndef bwlib_C
  #define bwlib_C

  #include <bwlib.h>

/**************************************************************************/
/*                                                                        */
/*             BasicWindow:  A Simple Window Graphics Library             */
/*            ------------------------------------------------            */
/*                Neil Pollard, University of Bristol 1995                */
/*                                                                        */
/*            Please read the file README for more information            */
/*                                                                        */
/**************************************************************************/

/* BWlib.c */

/* This section contains various functions for preparing a window for display.
 *
 * The next two functions have been based (broadly) on 
 * examples from The Xlib Programming Manual,
 * Copyright 1989, 1990, 1991 O'Reilly and Associates, Inc.
 * 
 * Please see the file Xlib.Copyright for full details
 *
 * They really need to be amended to take
 * notice of command line conventions, such as the
 * -display parameter */

#include <stdio.h>
#include <string.h>
//#include "BWicon"
//#include "BWlib.h"

/*=======================================================================*/
/* Connect the application to a server for window display.
 * The display should really be settable from the command line, but
 * this is not yet implemented
 *
 * Takes no arguments
 * Returns a pointer to the display - this must then be used
 * when opening windows */

Display *BWConnectToServer()
{

Display *display;
char *display_name = NULL;

/* connect to X server */
if ( (display = XOpenDisplay(display_name)) == NULL )
	{
	fprintf(stderr, "BasicWin: cannot connect to X server %s\n", XDisplayName(display_name));
	}
return display;
}

/*=======================================================================*/
/* BWCreateNewWindow
 * Opens a new window ready for text and graphics
 * The window size is fixed (subject to cooperation from the wm)
 *
 * Takes: a pointer to a display (returned from connect_to server)
 *        argv and argv, the command line parameters
 * 
 * Returns a BasicWin structure, which is used by the graphics 
 * routines.
 *       
 */

BasicWin BWCreateNewWindow(Display *display, int argc, char **argv)
{
	BasicWin this_window;
	unsigned int newwin_width, newwin_height;	/* window size */
	int newwin_x, newwin_y;				/* window position */
	unsigned int newwin_border_width = 4;
	char *newwin_window_name = "Output";
	char *newwin_icon_name = "Output";
	Pixmap icon_pixmap;
	XSizeHints newwin_size_hints;
	XIconSize *size_list;
	int screen_num;
	XEvent win_report;
	Window win;
	XFontStruct *win_font_info;
	GC win_gc;
	char *progname;

	/* The name of the executable */
	progname=argv[0];
	
	/* get screen size from display structure macro */
	screen_num = DefaultScreen(display);

	/* At the moment, the position of the window is not
	settable from the command line */
	newwin_x = 255;
	newwin_y = 255;

	/* size window */
	newwin_width = 255, newwin_height = 255;

	/* create opaque window */
	win = XCreateSimpleWindow(display, RootWindow(display,screen_num), 
			newwin_x, newwin_y, newwin_width, newwin_height, newwin_border_width, 
			BlackPixel(display,screen_num), 
			WhitePixel(display,screen_num));

	/* Create bitmap for the icon */
	icon_pixmap = XCreateBitmapFromData(display, win, icon_bits, 
			icon_width, icon_height);

	/* Set size hints for window manager. */

#ifdef X11R3
	newwin_size_hints.flags = PPosition | PSize | PMinSize | PMaxSize;
	newwin_size_hints.x = newwin_x;
	newwin_size_hints.y = newwin_y;
	newwin_size_hints.width = newwin_width;
	newwin_size_hints.height = newwin_height;
	newwin_size_hints.min_width = 255;
	newwin_size_hints.min_height = 255;

#else /* X11R4 or later */

	newwin_size_hints.flags = PPosition | PSize | PMinSize | PMaxSize;
	newwin_size_hints.min_width = 255;
	newwin_size_hints.min_height = 255;
#endif

#ifdef X11R3
	/* set Properties for window manager (always before mapping) */
	XSetStandardProperties(display, win, window_name, newwin_icon_name, 
			icon_pixmap, argv, argc, &newwin_size_hints);

#else /* X11R4 or later */
	{
	XWMHints wm_hints;
	XClassHint class_hints;

	/* format of the window name and icon name 
	 * arguments has changed in R4 */
	XTextProperty windowName, iconName;

	/* These calls store window_name and icon_name into
	 * XTextProperty structures and set their other 
	 * fields properly. */
	if (XStringListToTextProperty(&newwin_window_name, 1, &windowName) == 0) {
		(void) fprintf( stderr, "%s: structure allocation for windowName failed.\n",progname);
		exit(-1);
	}
		
	if (XStringListToTextProperty(&newwin_icon_name, 1, &iconName) == 0) {
		(void) fprintf( stderr, "%s: structure allocation for iconName 
failed.\n",progname);
		exit(-1);
	}

	wm_hints.initial_state = NormalState;
	wm_hints.input = True;
	wm_hints.icon_pixmap = icon_pixmap;
	wm_hints.flags = StateHint | IconPixmapHint | InputHint;

	class_hints.res_name = "basicwin";
	class_hints.res_class = "Graphical";

	XSetWMProperties(display, win, &windowName, &iconName, 
			argv, argc, &newwin_size_hints, &wm_hints, 
			&class_hints);
	}
#endif

	/* Select event types wanted */
	XSelectInput(display, win, ExposureMask | KeyPressMask |
		PointerMotionHintMask |
		ButtonPressMask	| StructureNotifyMask);

	load_font(&win_font_info, display);

	/* create GC for text and graphics */
	get_graphical_GC(win, &win_gc, win_font_info, screen_num, display);

	/* Display window */
	XMapWindow(display, win);

	this_window.display=display;
	this_window.win_report=win_report;
	this_window.win=win;
	this_window.win_gc=win_gc;
	this_window.win_font_info=win_font_info;

	return this_window;

}

/*======================================================================*/
/* Get graphics context for a graphical window
 * Used by open_new_window - users should not use this function
 */



#endif
