/* main.c */

/* Just a short test program to open two windows and close them again 
 * when a mouse button is pressed.
 * I might write something useful soon.
 */

#include "BWlib.h"

void main(argc, argv)
int argc;
char **argv;
{
	Display *display;
	BasicWin window1;
	BasicWin window2;
	int events;

	/* Connect to the server for display */
	display = BWConnectToServer();

	/* Open the windows */
	window1 = BWCreateNewWindow(display, argc, argv);
	window2 = BWCreateNewWindow(display, argc, argv);

	/* Start Program */
	while(1)
	{
	/* Check window events */
	events = BWCheckEvents(window1);
	
	switch (events)
		{
		/* Window exposed - redraw */
		case EV_EXPOSED:
			{
			/* Write "Hello" in the first window */
			BWPrintText(window1,"Hello X Windows\0",20,20);

			/* Draw a simple shape in the second window */
			BWDrawLine(window2, 0,0, 255,255);
			BWClearArea(window2, 128, 128, 20, 20);
			BWDrawRectangle(window2, 128, 128, 20, 20);

			break;
			}

		/* Mouse button pressed - close both windows and exit*/
		case EV_BUTTON:
			{
			BWCloseWindow(window1);
			BWCloseWindow(window2);
			exit(0);
			}

		default:
			break;
			
		}
	}	

}
/**************************************************************************/
