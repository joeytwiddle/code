#ifndef bwlib_H
  #define bwlib_H

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

Display *BWConnectToServer(); // Method


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

BasicWin BWCreateNewWindow(Display *display, int argc, char **argv); // Method


/*======================================================================*/
/* Get graphics context for a graphical window
 * Used by open_new_window - users should not use this function
 */



#endif
