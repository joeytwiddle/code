/* Written by Vaughn Betz at the University of Toronto, Department of       *
 * Electrical and Computer Engineering.  Graphics package  Version 1.3.     *
 * All rights reserved by U of T, etc.                                      *
 *                                                                          *
 * You may freely use this graphics interface for non-commercial purposes   *
 * as long as you leave the written by Vaughn Betz message in it -- who     *
 * knows, maybe someday an employer will see it and  give me a job or large *
 * sums of money :).                                                        *
 *                                                                          *
 * Revision History:                                                        *
 *                                                                          *
 * January 12, 1999: (Paul)                                                 *
 * - Fixed a bunch of stuff with the Win32 support (memory leaks, etc)      *
 * - Made the clipping function using the update rectangle for Win32        *
 *                                                                          *
 * January 9, 1999:  (Paul Leventis, leventi@eecg.utoronto.ca)              *
 * - Added Win32 support.  Should work under Windows98/95/NT 4.0/NT 5.0.    *
 * - Added a check to deselect_all to determine whether the screen needs to *
 * be updated or not.  Should elminate flicker from mouse clicks            *
 * - Added invalidate_screen() call to graphics.c that in turn calls        *
 *   update_screen, so this function was made non-static and added to the   *
 *   header file.  This is due to differences in the structure of Win32     *
 *   windowing apps.                                                        *
 * - Added setOKtoPaint(TRUE/FALSE) to explicitly tell the graphics engine  *
 *   that the data structures are o.k. to read for redrawing the screen.    *
 *   Used for Win32 since painting is a separate thread of execution.       *
 * - Win32 needs clipping (though done automatically, could be faster)      *
 *                                                                          *
 * Sept. 19, 1997:  Incorporated Zoom Fit code of Haneef Mohammed at        *
 * Cypress.  Makes it easy to zoom to a full view of the graphics.          *
 *                                                                          *
 * Sept. 11, 1997:  Added the create_and delete_button interface to  *
 * make it easy to add and destroy buttons from user code.  Removed the     *
 * bnum parameter to the button functions, since it wasn't really needed.   *
 *                                                                          *
 * June 28, 1997:  Added filled arc drawing primitive.  Minor modifications *
 * to PostScript driver to make the PostScript output slightly smaller.     *
 *                                                                          *
 * April 15, 1997:  Added code to init_graphics so it waits for a window    *
 * to be exposed before returning.  This ensures that users of non-         *
 * interactive graphics can never draw to a window before it is available.  *
 *                                                                          *
 * Feb. 24, 1997:  Added code so the package will allocate  a private       *
 * colormap if the default colormap doesn't have enough free colours.       *
 *                                                                          *
 * June 28, 1996:  Converted all internal functions in graphics.c to have   *
 * internal (static) linkage to avoid any conflicts with user routines in   *
 * the rest of the program.                                                 *
 *                                                                          *
 * June 12, 1996:  Added setfontsize and setlinewidth attributes.  Added    *
 * pre-clipping of objects for speed (and compactness of PS output) when    *
 * graphics are zoomed in.  Rewrote PostScript engine to shrink the output  *
 * and make it easier to read.  Made drawscreen a callback function passed  *
 * in rather than a global.  Graphics attribute calls are more efficient -- *
 * they check if they have to change anything before doing it.              * 
 *                                                                          *
 * October 27, 1995:  Added the message area, a callback function for       *
 * interacting with user button clicks, and implemented a workaround for a  *
 * Sun X Server bug that misdisplays extremely highly zoomed graphics.      *
 *                                                                          *
 * Jan. 13, 1995:  Modified to incorporate PostScript Support.              */

#if defined(X11) || defined(WIN32)

/*********************************
* Common Preprocessor Directives *
*********************************/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graphics.h"

/* Macros for translation from world to PostScript coordinates */
#define XPOST(worldx) (((worldx)-xleft)*ps_xmult + ps_left)
#define YPOST(worldy) (((worldy)-ybot)*ps_ymult + ps_bot)

/* Macros to convert from X Windows Internal Coordinates to my  *
 * World Coordinates.  (This macro is used only rarely, so      *
 * the divides don't hurt speed).                               */
#define XTOWORLD(x) (((float) x)*xdiv + xleft)
#define YTOWORLD(y) (((float) y)*ydiv + ytop)

#ifndef max
#define max(a,b) (((a) > (b))? (a) : (b))
#endif
#ifndef min
#define min(a,b) ((a) > (b)? (b) : (a))
#endif

#define MWIDTH			104    /* width of menu window */
#define T_AREA_HEIGHT	24  /* Height of text window */
#define MAX_FONT_SIZE	40  /* Largest point size of text */
#define PI				3.141592654

#define BUTTON_TEXT_LEN	20
#define BUFSIZE			1000

/*********************************************
* X-Windows Specific Preprocessor Directives *
*********************************************/
#ifdef X11

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

/* Uncomment the line below if your X11 header files don't define XPointer */
/* typedef char *XPointer;                                                 */

#define MAXPIXEL 15000   
#define MINPIXEL -15000 

#endif /* X11 Preprocessor Directives */


/*************************************************************
* Microsoft Windows (WIN32) Specific Preprocessor Directives *
*************************************************************/
#ifdef WIN32

#include <windows.h>

#define SELECT_ERROR() { printf("Error: Couldn't select graphics object on line %d of graphics.c\n", __LINE__); exit(-1); }
#define DELETE_ERROR() { printf("Error: Couldn't delete graphics object on line %d of graphics.c\n", __LINE__); exit(-1); }
#define CREATE_ERROR() { printf("Error: Couldn't create graphics object on line %d of graphics.c\n", __LINE__); exit(-1); }
#define DRAW_ERROR() { printf("Error with drawing API call on line %d of graphics.c\n", __LINE__); exit(-1); }

/* Due to funny clipping problems under windows that I suspect are caused by round-off thingys */
#define MAXPIXEL 2000
#define MINPIXEL -2000

#endif /* Win32 preprocessor Directives */



/**********************
* Common Declarations *
**********************/

static int xcoord (float worldx);
static int ycoord (float worldy);
static void force_setcolor(int cindex);
static void force_setlinestyle(int linestyle);
static void force_setlinewidth(int linewidth);
static void build_default_menu (void); 

typedef struct {
	int width; 
	int height; 
	int xleft; 
	int ytop;
	void (*fcn) (void (*drawscreen) (void));
#ifdef X11
	Window win; 
#else
	HWND hwnd;
#endif
	int istext; 
	char text[BUTTON_TEXT_LEN]; 
	int ispoly; 
	int poly[3][2]; 
	int ispressed;
} t_button;

static const int menu_font_size = 14;   /* Font for menus and dialog boxes. */

static t_button *button = NULL;                 /* [0..num_buttons-1] */
static int num_buttons = 0;                  /* Number of menu buttons */

static int disp_type;    /* Selects SCREEN or POSTSCRIPT */

static unsigned int display_width, display_height;  /* screen size */
static unsigned int top_width, top_height;      /* window size */
static float xleft, xright, ytop, ybot;         /* world coordinates */
static float saved_xleft, saved_xright, saved_ytop, saved_ybot; 

static float ps_left, ps_right, ps_top, ps_bot; /* Figure boundaries for *
                        * PostScript output, in PostScript coordinates.  */
static float ps_xmult, ps_ymult;     /* Transformation for PostScript. */
static float xmult, ymult;                  /* Transformation factors */
static float xdiv, ydiv;

static int currentcolor = BLACK;
static int currentlinestyle = SOLID;
static int currentlinewidth = 0;
static int currentfontsize = 10;
/* For PostScript output */
static  FILE *ps;

static char statusMessage[BUFSIZE] = ""; /* User message to display */

/* Function declarations for button responses */
static void translate_up (void (*drawscreen) (void)); 
static void translate_left (void (*drawscreen) (void)); 
static void translate_right (void (*drawscreen) (void)); 
static void translate_down (void (*drawscreen) (void)); 
static void zoom_in (void (*drawscreen) (void));
static void zoom_out (void (*drawscreen) (void));
static void zoom_fit (void (*drawscreen) (void));
static void adjustwin (void (*drawscreen) (void)); 
static void postscript (void (*drawscreen) (void));
static void proceed (void (*drawscreen) (void));
static void quit (void (*drawscreen) (void)); 
static void map_button (int bnum); 
static void unmap_button (int bnum); 

/*************************
* X-Windows Declarations *
*************************/

#ifdef X11

static Display *display;
static int screen_num;
static GC gc, gcxor, gc_menus;
static XFontStruct *font_info[MAX_FONT_SIZE+1]; /* Data for each size */
static int font_is_loaded[MAX_FONT_SIZE + 1];  /* 1: loaded, 0: not  */
static Window toplevel, menu, textarea;  /* various windows */
static Colormap private_cmap; /* "None" unless a private cmap was allocated. */

/* Graphics state.  Set start-up defaults here. */


/* Color indices passed back from X Windows. */
static int colors[NUM_COLOR];

/* MAXPIXEL and MINPIXEL are set to prevent what appears to be *
 * overflow with very large pixel values on the Sun X Server.  */

static void load_font(int pointsize);

static Bool test_if_exposed (Display *disp, XEvent *event_ptr, 
          XPointer dummy);
static void build_textarea (void);
static void drawbut (int bnum);
static int which_button (Window win);


#endif /* X11 Declarations */


/*****************************************
* Microsoft Windows (Win32) Declarations *
*****************************************/

#ifdef WIN32

static int win32_line_styles[2] = { PS_SOLID, PS_DOT };

static COLORREF win32_colors[NUM_COLOR] = { RGB(255, 255, 255),
	RGB(0, 0, 0), RGB(128, 128, 128), RGB(192, 192, 192), RGB(0, 0, 255),
	RGB(0, 255, 0), RGB(255, 255, 0), RGB(0, 255, 255), RGB(255, 0, 0), RGB(0, 128, 0),
	RGB(255, 0, 255) };

static TCHAR szAppName[256], 
	szGraphicsName[] = TEXT("VPR Graphics"), 
	szStatusName[] = TEXT("VPR Status"),
	szButtonsName[] = TEXT("VPR Buttons");
static HPEN hGraphicsPen = 0;
static HBRUSH hGraphicsBrush = 0, hGrayBrush = 0;
static HDC hGraphicsDC = 0;
static HWND hMainWnd, hGraphicsWnd, hButtonsWnd, hStatusWnd;

static LRESULT CALLBACK GraphicsWND(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK StatusWND(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK ButtonsWND(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK MainWND(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

static cxClient, cyClient;

static int windowAdjustFlag = 0, adjustButton = -1;
static RECT adjustRect, updateRect;

static HBITMAP buttonImages[4];

static void (*clb_click)(float x, float y);
static void (*drawscreen_ptr)(void);

static boolean ProceedPressed = FALSE;
static boolean InEventLoop = FALSE;

#endif /* Win32 Declarations */

/*********************
* Common Definitions *
*********************/

/* Translates from my internal coordinates to real-world coordinates   *
 * in the x direction.  Add 0.5 at end for extra half-pixel accuracy. */
static int 
xcoord (float worldx) 
{
	int winx;

	winx = (int) ((worldx-xleft)*xmult + 0.5);
 
	/* Avoids overflow in the  Window routines.  This will allow horizontal  *
	 * and vertical lines to be drawn correctly regardless of zooming, but   *
	 * will cause diagonal lines that go way off screen to change their      *
	 * slope as you zoom in.  The only way I can think of to completely fix  *
	 * this problem is to do all the clipping in advance in floating point,  *
	 * then convert to integers and call   Windows.  This is a lot of extra  *
	 * coding, and means that coordinates will be clipped twice, even though *
	 * this "Super Zoom" problem won't occur unless users zoom way in on     * 
	 * the graphics.                                                         */ 

	winx = max (winx, MINPIXEL);
	winx = min (winx, MAXPIXEL);

	return (winx);
}


/* Translates from my internal coordinates to real-world coordinates   *
 * in the y direction.  Add 0.5 at end for extra half-pixel accuracy. */
static int 
ycoord (float worldy) 
{
	int winy;
	
	winy = (int) ((worldy-ytop)*ymult + 0.5);
	
	/* Avoid overflow in the X/Win32 Window routines. */
	winy = max (winy, MINPIXEL);
	winy = min (winy, MAXPIXEL);
	
	return (winy);
}

/* Tells the graphics engine to redraw the graphics display since information
 * has changed */
void
invalidate_screen(void)
{
#ifdef X11
/*
	clearscreen();
	redraw_screen();
	*/
#else /* Win32 */
	if(!InvalidateRect(hGraphicsWnd, NULL, TRUE))
		DRAW_ERROR();
	if(!UpdateWindow(hGraphicsWnd))
		DRAW_ERROR();
#endif
}


/* Sets the current colour to cindex, regardless of its value */
static void 
force_setcolor (int cindex) 
{
	static char *ps_cnames[NUM_COLOR] = {"white", "black", "grey55", "grey75",
		"blue", "green", "yellow", "cyan", "red", "darkgreen", "magenta"};
	
	currentcolor = cindex; 
	if (disp_type == SCREEN) {
#ifdef X11
		XSetForeground (display, gc,colors[cindex]);
#else /* Win32 */
		if(!SelectObject(hGraphicsDC, GetStockObject(NULL_PEN))) 
			SELECT_ERROR();
		if(!DeleteObject(hGraphicsPen))
			DELETE_ERROR();
		hGraphicsPen = CreatePen(win32_line_styles[currentlinestyle], currentlinewidth, 
			win32_colors[currentcolor]);
		if(!hGraphicsPen)
			CREATE_ERROR();

		if(!SelectObject(hGraphicsDC, GetStockObject(NULL_BRUSH)))
			SELECT_ERROR();
		if(!DeleteObject(hGraphicsBrush)) 
			DELETE_ERROR();
		hGraphicsBrush = CreateSolidBrush(win32_colors[currentcolor]);
		if(!hGraphicsBrush)
			CREATE_ERROR();
#endif
	}
	else {
		fprintf (ps,"%s\n", ps_cnames[cindex]);
	}
}


/* Sets the current colour to cindex if necessary */
void 
setcolor (int cindex) 
{
	if (currentcolor != cindex) 
		force_setcolor (cindex);
}


/* Sets the current linestyle to linestyle.
 * Note SOLID is 0 and DASHED is 1 for linestyle. */
static void 
force_setlinestyle (int linestyle) 
{
	/* PostScript, X commands needed, respectively. */
	static char *ps_text[2] = {"linesolid", "linedashed"};  
#ifdef X11
	static int x_vals[2] = {LineSolid, LineOnOffDash};
#endif

	currentlinestyle = linestyle;

	if (disp_type == SCREEN) {
#ifdef X11
		XSetLineAttributes (display, gc, currentlinewidth, x_vals[linestyle],
        CapButt, JoinMiter);
#else /* Win32 */
		if(!SelectObject(hGraphicsDC, GetStockObject(NULL_PEN)))
			SELECT_ERROR();
		if(!DeleteObject(hGraphicsPen))
			DELETE_ERROR();
		hGraphicsPen = CreatePen(win32_line_styles[currentlinestyle], currentlinewidth, 
			win32_colors[currentcolor]);
		if(!hGraphicsPen)
			CREATE_ERROR();
#endif
	}
	else {
		fprintf(ps,"%s\n", ps_text[linestyle]);
	}
}


void 
setlinestyle (int linestyle) 
{
	if (linestyle != currentlinestyle) 
		force_setlinestyle (linestyle);
}


/* Sets current linewidth 
 * linewidth should be greater than or equal to 0 to make any sense. */
static void 
force_setlinewidth (int linewidth) 
{
#ifdef X11
	static int x_vals[2] = {LineSolid, LineOnOffDash};
#endif

	currentlinewidth = linewidth;
	
	if (disp_type == SCREEN) {
#ifdef X11
		XSetLineAttributes (display, gc, linewidth, x_vals[currentlinestyle],
			CapButt, JoinMiter);
#else /* Win32 */
		if(!SelectObject(hGraphicsDC, GetStockObject(NULL_PEN)))
			SELECT_ERROR();
		if(!DeleteObject(hGraphicsPen)) 
			DELETE_ERROR();
		hGraphicsPen = CreatePen(win32_line_styles[currentlinestyle], currentlinewidth, 
			win32_colors[currentcolor]);
		if(!hGraphicsPen)
			CREATE_ERROR();
#endif
	}
	else {
		fprintf(ps,"%d setlinewidth\n", linewidth);
	}
}


void 
setlinewidth (int linewidth) 
{
	if (linewidth != currentlinewidth)
		force_setlinewidth (linewidth);
}


static void 
force_setfontsize (int pointsize) 
{
	/* Valid point sizes are between 1 and MAX_FONT_SIZE */

	if (pointsize < 1) 
		pointsize = 1;
	else if (pointsize > MAX_FONT_SIZE) 
		pointsize = MAX_FONT_SIZE;

	currentfontsize = pointsize;

	if (disp_type == SCREEN) {
#if X11
		if (!font_is_loaded[pointsize]) {
			load_font (pointsize);
			font_is_loaded[pointsize] = 1;
		}
		XSetFont(display, gc, font_info[pointsize]->fid); 
#else /* Win32 */
		/* Does nothing for now */
#endif
	}
 
	else {
		/* PostScript:  set up font and centering function */
		fprintf(ps,"%d setfontsize\n",pointsize);
	} 
}


/* For efficiency, this routine doesn't do anything if no change is *
 * implied.  If you want to force the graphics context or PS file   *
 * to have font info set, call force_setfontsize (this is necessary *
 * in initialization and X11 / Postscript switches).                */
void 
setfontsize (int pointsize) 
{

 if (pointsize != currentfontsize) 
    force_setfontsize (pointsize);
}


/* Puts a triangle in the poly array for button[bnum] */
static void 
setpoly (int bnum, int xc, int yc, int r, float theta) 
{
	int i;

	button[bnum].istext = 0;
	button[bnum].ispoly = 1;
	for (i=0;i<3;i++) {
		button[bnum].poly[i][0] = (int) (xc + r*cos(theta) + 0.5);
		button[bnum].poly[i][1] = (int) (yc + r*sin(theta) + 0.5);
		theta += 2*PI/3;
	}
}


/* Maps a button onto the screen and set it up for input, etc.        */
static void 
map_button (int bnum) 
{
#ifdef X11
	button[bnum].win = XCreateSimpleWindow(display,menu,
		button[bnum].xleft, button[bnum].ytop, button[bnum].width, 
		button[bnum].height, 0, colors[WHITE], colors[LIGHTGREY]); 
	XMapWindow (display, button[bnum].win);
	XSelectInput (display, button[bnum].win, ButtonPressMask);
	button[bnum].ispressed = 1;
#else
	if(1 || button[bnum].istext) {
		button[bnum].ispressed = 1;
		button[bnum].hwnd = CreateWindow( TEXT("button"), TEXT(button[bnum].text),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, button[bnum].xleft, button[bnum].ytop,
			button[bnum].width, button[bnum].height, hButtonsWnd, 200+bnum, 
			(HINSTANCE) GetWindowLong(hMainWnd, GWL_HINSTANCE), NULL);
	} else {
		if(!button[bnum].ispoly)
			DRAW_ERROR();
		button[bnum].hwnd = CreateWindow( TEXT("button"), NULL,
			WS_CHILD | WS_VISIBLE | BS_BITMAP, button[bnum].xleft, button[bnum].ytop,
			button[bnum].width, button[bnum].height, hButtonsWnd, 200+bnum, 
			(HINSTANCE) GetWindowLong(hMainWnd, GWL_HINSTANCE), NULL);
/*
		SendMessage(button[bnum].hwnd, BM_SETIMAGE, IMAGE_BITMAP, hBitmap);

		hdc = GetDC(button[bnum].hwnd);
		hdcMem = CreateCompatibleDC(hdc);
		Create
			CreateWindow
*/
	}
	if(!InvalidateRect(hButtonsWnd, NULL, TRUE))
		DRAW_ERROR();
	if(!UpdateWindow(hButtonsWnd))
		DRAW_ERROR();
#endif
}


static void 
unmap_button (int bnum) 
{
	/* Unmaps a button from the screen.        */
#ifdef X11
	XUnmapWindow (display, button[bnum].win);
#else
	if(!DestroyWindow(button[bnum].hwnd))
		DRAW_ERROR();
	if(!InvalidateRect(hButtonsWnd, NULL, TRUE))
		DRAW_ERROR();
	if(!UpdateWindow(hButtonsWnd))
		DRAW_ERROR();
#endif
}


/* Creates a new button below the button containing prev_button_text.       *
 * The text and button function are set according to button_text and        *
 * button_func, respectively.                                               */
void create_button (char *prev_button_text , char *button_text, 
       void (*button_func) (void (*drawscreen) (void))) 
{
	int i, bnum, space;
	
	space = 8;
	
	/* Only allow new buttons that are text (not poly) types.                   */
	
	bnum = -1;
	for (i=4;i<num_buttons;i++) {
		if (button[i].istext == 1 && 
			strcmp (button[i].text, prev_button_text) == 0) {
			bnum = i + 1;
			break;
		}
	}
	
	if (bnum == -1) {
		printf ("Error in create_button:  button with text %s not found.\n",
			prev_button_text);
		exit (1);
	}
	
	button = (t_button *) my_realloc (button, (num_buttons+1) * sizeof (t_button));
	/* NB:  Requirement that you specify the button that this button goes under *
	* guarantees that button[num_buttons-2] exists and is a text button.       */
	
	button[num_buttons].xleft = button[num_buttons-1].xleft;
	button[num_buttons].ytop = button[num_buttons-1].ytop + 
		button[num_buttons-1].height + space;
	button[num_buttons].height = button[num_buttons-1].height;
	button[num_buttons].width = button[num_buttons-1].width;
	button[num_buttons].text[0] = '\0';

	map_button (num_buttons);
	
	for (i=num_buttons;i>bnum;i--) {
		button[i].ispoly = button[i-1].ispoly;
		/* No poly copy for now, as I'm only providing the ability to create text *
		* buttons.                                                               */
		
		button[i].istext = button[i-1].istext;
		strcpy (button[i].text, button[i-1].text);
		button[i].fcn = button[i-1].fcn;
		button[i].ispressed = button[i-1].ispressed;
#ifdef WIN32
		SetWindowText(button[i].hwnd, TEXT(button[i].text));
#endif
	}
	
	button[bnum].istext = 1;
	button[bnum].ispoly = 0;
	strncpy (button[bnum].text, button_text, BUTTON_TEXT_LEN);
	button[bnum].fcn = button_func;
	button[bnum].ispressed = 1;
#ifdef WIN32
		SetWindowText(button[bnum].hwnd, TEXT(button[i].text));
#endif

	num_buttons++;
}


/* Destroys the button with text button_text. */
void
destroy_button (char *button_text) 
{
	int i, bnum;
	
	bnum = -1;
	for (i=4;i<num_buttons;i++) {
		if (button[i].istext == 1 && 
			strcmp (button[i].text, button_text) == 0) {
			bnum = i;
			break;
		}
	}
	
	if (bnum == -1) {
		printf ("Error in destroy_button:  button with text %s not found.\n",
			button_text);
		exit (1);
	}
	
	for (i=bnum+1;i<num_buttons;i++) {
		button[i-1].ispoly = button[i].ispoly;
		/* No poly copy for now, as I'm only providing the ability to create text *
		* buttons.                                                               */
		
		button[i-1].istext = button[i].istext;
		strcpy (button[i-1].text, button[i].text);
		button[i-1].fcn = button[i].fcn;
		button[i-1].ispressed = button[i].ispressed;
	}
	
	unmap_button (num_buttons-1);
	button = (t_button *) my_realloc (button, (num_buttons-1) * sizeof (t_button));
	num_buttons--;
}


/* Open the toplevel window, get the colors, 2 graphics         *
 * contexts, load a font, and set up the toplevel window        *
 * Calls build_default_menu to set up the default menu.         */
void 
init_graphics (char *window_name) 
{
#if X11
	char *display_name = NULL;
	int x, y;                                   /* window position */
	unsigned int border_width = 2;  /* ignored by OpenWindows */
	XTextProperty windowName;
	
	/* X Windows' names for my colours. */
	char *cnames[NUM_COLOR] = {"white", "black", "grey55", "grey75", "blue", 
		"green", "yellow", "cyan", "red", "RGBi:0.0/0.5/0.0", "magenta" };
	
	XColor exact_def;
	Colormap cmap;
	int i;
	unsigned long valuemask = 0; /* ignore XGCvalues and use defaults */
	XGCValues values;
	XEvent event;
	
	
	disp_type = SCREEN;         /* Graphics go to screen, not ps */
	
	for (i=0;i<=MAX_FONT_SIZE;i++) 
		font_is_loaded[i] = 0;     /* No fonts loaded yet. */
	
	/* connect to X server */
	/* connect to X server */
	if ( (display=XOpenDisplay(display_name)) == NULL )
	{
		fprintf( stderr, "Cannot connect to X server %s\n",
			XDisplayName(display_name));
		exit( -1 );
	}
	
	/* get screen size from display structure macro */
	screen_num = DefaultScreen(display);
	display_width = DisplayWidth(display, screen_num);
	display_height = DisplayHeight(display, screen_num);
	
	x = y = 0;
	
	top_width = 2*display_width/3;
	top_height = 4*display_height/5;
	
	cmap = DefaultColormap(display, screen_num);
	private_cmap = None;
	
	for (i=0;i<NUM_COLOR;i++) {
		if (!XParseColor(display,cmap,cnames[i],&exact_def)) {
			fprintf(stderr, "Color name %s not in database", cnames[i]);
			exit(-1);
		}
		if (!XAllocColor(display, cmap, &exact_def)) {
			fprintf(stderr, "Couldn't allocate color %s.\n",cnames[i]); 
			
			if (private_cmap == None) {
				fprintf(stderr, "Will try to allocate a private colourmap.\n");
				fprintf(stderr, "Colours will only display correctly when your "
					"cursor is in the graphics window.\n"
					"Exit other colour applications and rerun this "
					"program if you don't like that.\n\n");
				
				private_cmap = XCopyColormapAndFree (display, cmap);
				cmap = private_cmap;
				if (!XAllocColor (display, cmap, &exact_def)) {
					fprintf (stderr, "Couldn't allocate color %s as private.\n",
						cnames[i]);
					exit (1);
				}
			}
			
			else {
				fprintf (stderr, "Couldn't allocate color %s as private.\n",
					cnames[i]);
				exit (1);
			}
		}
		colors[i] = exact_def.pixel;
	}
	
	toplevel = XCreateSimpleWindow(display,RootWindow(display,screen_num),
		x, y, top_width, top_height, border_width, colors[BLACK],
		colors[WHITE]);  
	
	if (private_cmap != None) 
		XSetWindowColormap (display, toplevel, private_cmap);
	
	/* hints stuff deleted. */
	
	XSelectInput (display, toplevel, ExposureMask | StructureNotifyMask |
		ButtonPressMask);
	
	
	/* Create default Graphics Contexts.  valuemask = 0 -> use defaults. */
	gc = XCreateGC(display, toplevel, valuemask, &values);
	gc_menus = XCreateGC(display, toplevel, valuemask, &values);
	
	/* Create XOR graphics context for Rubber Banding */
	values.function = GXxor;   
	values.foreground = colors[BLACK];
	gcxor = XCreateGC(display, toplevel, (GCFunction | GCForeground),
		&values);
	
	/* specify font for menus.  */
	load_font(menu_font_size);
	font_is_loaded[menu_font_size] = 1;
	XSetFont(display, gc_menus, font_info[menu_font_size]->fid);
	
	/* Set drawing defaults for user-drawable area.  Use whatever the *
	* initial values of the current stuff was set to.                */
	force_setfontsize(currentfontsize);
	force_setcolor (currentcolor);
	force_setlinestyle (currentlinestyle);
	force_setlinewidth (currentlinewidth);
	
	XStringListToTextProperty(&window_name, 1, &windowName);
	XSetWMName (display, toplevel, &windowName);
	/* XSetWMIconName (display, toplevel, &windowName); */
	
	/* XStringListToTextProperty copies the window_name string into            *
	* windowName.value.  Free this memory now.                                */
	
	free (windowName.value);  
	
	XMapWindow (display, toplevel);
	build_textarea ();
	build_default_menu ();
	
	/* The following is completely unnecessary if the user is using the       *
	* interactive (event_loop) graphics.  It waits for the first Expose      *
	* event before returning so that I can tell the window manager has got   *
	* the top-level window up and running.  Thus the user can start drawing  *
	* into this window immediately, and there's no danger of the window not  *
	* being ready and output being lost.                                     */
	
	XPeekIfEvent (display, &event, test_if_exposed, NULL); 

#else /* WIN32 */

	WNDCLASS wndclass;
	HANDLE hInstance = GetModuleHandle(NULL);

	disp_type = SCREEN;

	/* Grab the Application name */
	wsprintf(szAppName, TEXT(window_name));

	hGraphicsPen = CreatePen(win32_line_styles[SOLID], 1, win32_colors[BLACK]);
	if(!hGraphicsPen)
		CREATE_ERROR();
	hGraphicsBrush = CreateSolidBrush(win32_colors[DARKGREY]); 
	if(!hGraphicsBrush)
		CREATE_ERROR();
	hGrayBrush = CreateSolidBrush(win32_colors[LIGHTGREY]);
	if(!hGrayBrush)
		CREATE_ERROR();


	/* Register the Main Window class */
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.lpfnWndProc = MainWND;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass)) {
		MessageBox(NULL, TEXT("Initialization of Windows graphics (init_graphics) failed."),
			szAppName, MB_ICONERROR);
		exit(-1);
	}

	/* Register the Graphics Window class */
	wndclass.lpfnWndProc = GraphicsWND;
	wndclass.hIcon = NULL;
	wndclass.lpszClassName = szGraphicsName;

	if(!RegisterClass(&wndclass))
		DRAW_ERROR();

	/* Register the Status Window class */
	wndclass.lpfnWndProc = StatusWND;
	wndclass.hIcon = NULL;
	wndclass.lpszClassName = szStatusName;
	wndclass.hbrBackground = hGrayBrush;

	if(!RegisterClass(&wndclass))
		DRAW_ERROR();

	/* Register the Buttons Window class */
	wndclass.lpfnWndProc = ButtonsWND;
	wndclass.hIcon = NULL;
	wndclass.lpszClassName = szButtonsName;
	wndclass.hbrBackground = hGrayBrush;

	if(!RegisterClass(&wndclass))
		DRAW_ERROR();

	hMainWnd = CreateWindow(szAppName, TEXT("VPR: Versatile Place and Route for FPGAs"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if(!hMainWnd)
		DRAW_ERROR();

	/* Graphics go to screen, not ps */
	disp_type = SCREEN;  

	/* Set drawing defaults for user-drawable area.  Use whatever the *
  	 * initial values of the current stuff was set to.                */

	if(ShowWindow(hMainWnd, SW_SHOWNORMAL))
		DRAW_ERROR();
	build_default_menu();
	if(!UpdateWindow(hMainWnd))
		DRAW_ERROR();
#endif
}




static void 
update_transform (void) 
{
	/* Set up the factors for transforming from the user world to X Windows *
	 * coordinates.                                                         */
	
	float mult, y1, y2, x1, x2;
	/* X Window coordinates go from (0,0) to (width-1,height-1) */
	xmult = ((float) top_width - 1. - MWIDTH) / (xright - xleft);
	ymult = ((float) top_height - 1. - T_AREA_HEIGHT)/ (ybot - ytop);
	/* Need to use same scaling factor to preserve aspect ratio */
	if (fabs(xmult) <= fabs(ymult)) {
		mult = fabs(ymult/xmult);
		y1 = ytop - (ybot-ytop)*(mult-1.)/2.;
		y2 = ybot + (ybot-ytop)*(mult-1.)/2.;
		ytop = y1;
		ybot = y2;
	}
	else {
		mult = fabs(xmult/ymult);
		x1 = xleft - (xright-xleft)*(mult-1.)/2.;
		x2 = xright + (xright-xleft)*(mult-1.)/2.;
		xleft = x1;
		xright = x2;
	}
	xmult = ((float) top_width - 1. - MWIDTH) / (xright - xleft);
	xdiv = 1/xmult;
	ymult = ((float) top_height - 1. - T_AREA_HEIGHT)/ (ybot - ytop);
	ydiv = 1/ymult;
}


static void 
update_ps_transform (void) 
{
	
/* Postscript coordinates start at (0,0) for the lower left hand corner *
* of the page and increase upwards and to the right.  For 8.5 x 11     *
* sheet, coordinates go from (0,0) to (612,792).  Spacing is 1/72 inch.*
* I'm leaving a minimum of half an inch (36 units) of border around    *
	* each edge.                                                           */
	
	float ps_width, ps_height;
	
	ps_width = 540.;    /* 72 * 7.5 */
	ps_height = 720.;   /* 72 * 10  */
	
	ps_xmult = ps_width / (xright - xleft);
	ps_ymult = ps_height / (ytop - ybot);
	/* Need to use same scaling factor to preserve aspect ratio.   *
	* I show exactly as much on paper as the screen window shows, *
	* or the user specifies.                                      */
	if (fabs(ps_xmult) <= fabs(ps_ymult)) {  
		ps_left = 36.;
		ps_right = 36. + ps_width;
		ps_bot = 396. - fabs(ps_xmult * (ytop - ybot))/2.;
		ps_top = 396. + fabs(ps_xmult * (ytop - ybot))/2.;
		/* Maintain aspect ratio but watch signs */
		ps_ymult = (ps_xmult*ps_ymult < 0) ? -ps_xmult : ps_xmult;
	}
	else {  
		ps_bot = 36.;
		ps_top = 36. + ps_height;
		ps_left = 306. - fabs(ps_ymult * (xright - xleft))/2.;
		ps_right = 306. + fabs(ps_ymult * (xright - xleft))/2.;
		/* Maintain aspect ratio but watch signs */
		ps_xmult = (ps_xmult*ps_ymult < 0) ? -ps_ymult : ps_ymult;
	}
}


/* The program's main event loop.  Must be passed a user routine        *
 * drawscreen which redraws the screen.  It handles all window resizing *
 * zooming etc. itself.  If the user clicks a button in the graphics    *
 * (toplevel) area, the act_on_button routine passed in is called.      */
void 
event_loop (void (*act_on_button)(float x, float y), void (*drawscreen) (void)) 
{
#ifdef X11
	XEvent report;
	int bnum;
	float x, y;
	
#define OFF 1
#define ON 0
	
	turn_on_off (ON);
	while (1) {
		XNextEvent (display, &report);
		switch (report.type) {  
		case Expose:
#ifdef VERBOSE 
			printf("Got an expose event.\n");
			printf("Count is: %d.\n",report.xexpose.count);
			printf("Window ID is: %d.\n",report.xexpose.window);
#endif
			if (report.xexpose.count != 0)
				break;
			if (report.xexpose.window == menu)
				drawmenu(); 
			else if (report.xexpose.window == toplevel)
				drawscreen();
			else if (report.xexpose.window == textarea)
				draw_message();
			break;
		case ConfigureNotify:
			top_width = report.xconfigure.width;
			top_height = report.xconfigure.height;
			update_transform();
#ifdef VERBOSE 
			printf("Got a ConfigureNotify.\n");
			printf("New width: %d  New height: %d.\n",top_width,top_height);
#endif
			break; 
		case ButtonPress:
#ifdef VERBOSE 
			printf("Got a buttonpress.\n");
			printf("Window ID is: %d.\n",report.xbutton.window);
#endif
			if (report.xbutton.window == toplevel) {
				x = XTOWORLD(report.xbutton.x);
				y = YTOWORLD(report.xbutton.y); 
				act_on_button (x, y);
			} 
			else {  /* A menu button was pressed. */
				bnum = which_button(report.xbutton.window);
#ifdef VERBOSE 
				printf("Button number is %d\n",bnum);
#endif
				button[bnum].ispressed = 1;
				drawbut(bnum);
				XFlush(display);  /* Flash the button */
				button[bnum].fcn (drawscreen);
				button[bnum].ispressed = 0;
				drawbut(bnum);
				if (button[bnum].fcn == proceed) {
					turn_on_off(OFF);
					flushinput ();
					return;  /* Rather clumsy way of returning *
					* control to the simulator       */
				}
			}
			break;
		}
	}
#else /* Win32 */
	MSG msg;

	clb_click = act_on_button;
	drawscreen_ptr = drawscreen;
	ProceedPressed = FALSE;
	InEventLoop = TRUE;

	invalidate_screen();
	while(!ProceedPressed && GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	InEventLoop = FALSE;
	if(msg.message == WM_QUIT) {
		exit(-1);
	}
#endif
}

void 
clearscreen (void) 
{
#ifdef X11
	int savecolor;
	
	if (disp_type == SCREEN) {
		XClearWindow (display, toplevel);
	}
	else {
	/* erases current page.  Don't use erasepage, since this will erase *
	* everything, (even stuff outside the clipping path) causing       *
		* problems if this picture is incorporated into a larger document. */
		savecolor = currentcolor;
		setcolor (WHITE);
		fprintf(ps,"clippath fill\n\n");
		setcolor (savecolor);
	}
#else /* Win32 */
	setcolor (WHITE);
	fillrect (xleft, ytop, xright, ybot);
	/* Obsolete */
#endif
}

/* Return 1 if I can quarantee no part of this rectangle will         *
 * lie within the user drawing area.  Otherwise return 0.             *
 * Note:  this routine is only used to help speed (and to shrink ps   *
 * files) -- it will be highly effective when the graphics are zoomed *
 * in and lots are off-screen.  I don't have to pre-clip for          *
 * correctness.                                                       */
static int 
rect_off_screen (float x1, float y1, float x2, float y2) 
{

 float xmin, xmax, ymin, ymax;

#ifdef WIN32
 if(disp_type==SCREEN) {
	 if(x1 > x2) {
		 float t = x1;
		 x1 = x2;
		 x2 = t;
	 }
	 if(x2 < XTOWORLD(updateRect.left) || x1 > XTOWORLD(updateRect.right))
		 return 1;
	 if(y1 > y2) {
		 float t = y1;
		 y1 = y2;
		 y2 = t;
	 }
	 if(y1 > YTOWORLD(updateRect.top) || y2 < YTOWORLD(updateRect.bottom))
		 return 1;
	 return 0;
 }
#endif
 xmin = min (xleft, xright);
 if (x1 < xmin && x2 < xmin) 
    return (1);

 xmax = max (xleft, xright);
 if (x1 > xmax && x2 > xmax)
    return (1);

 ymin = min (ytop, ybot);
 if (y1 < ymin && y2 < ymin)
    return (1);

 ymax = max (ytop, ybot);
 if (y1 > ymax && y2 > ymax)
    return (1);

 return (0);
}

void 
drawline (float x1, float y1, float x2, float y2) 
{
	/* Draw a line from (x1,y1) to (x2,y2) in the user-drawable area. *
 	 * Coordinates are in world (user) space.                         */
	
	if (rect_off_screen(x1,y1,x2,y2))
		return;
	
	if (disp_type == SCREEN) {
#ifdef X11
		/* Xlib.h prototype has x2 and y1 mixed up. */ 
		XDrawLine(display, toplevel, gc, xcoord(x1), ycoord(y1), 
			xcoord(x2), ycoord(y2));
#else /* Win32 */
		if(!SelectObject(hGraphicsDC, hGraphicsPen))
			SELECT_ERROR();

		if(!MoveToEx (hGraphicsDC, xcoord(x1), ycoord(y1), NULL))
			DRAW_ERROR();
		if(!LineTo (hGraphicsDC, xcoord(x2), ycoord(y2)))
			DRAW_ERROR();
#endif
	}
	else {
		fprintf(ps,"%.2f %.2f %.2f %.2f drawline\n",XPOST(x1),YPOST(y1),
			XPOST(x2),YPOST(y2));
	}
}

/* (x1,y1) and (x2,y2) are diagonally opposed corners, in world coords. */
void 
drawrect (float x1, float y1, float x2, float y2) 
{
	unsigned int width, height;
	int xw1, yw1, xw2, yw2, xl, yt;
	
	if (rect_off_screen(x1,y1,x2,y2))
		return;
	
	if (disp_type == SCREEN) { 
		/* translate to X Windows calling convention. */
		xw1 = xcoord(x1);
		xw2 = xcoord(x2);
		yw1 = ycoord(y1);
		yw2 = ycoord(y2); 
#ifdef X11
		xl = min(xw1,xw2);
		yt = min(yw1,yw2);
		width = abs (xw1-xw2);
		height = abs (yw1-yw2);
		XDrawRectangle(display, toplevel, gc, xl, yt, width, height);
#else /* Win32 */
		if(xw1 > xw2) {
			int temp = xw1;
			xw1 = xw2;
			xw2 = temp;
		}
		if(yw1 > yw2) {
			int temp = yw1;
			yw1 = yw2;
			yw2 = temp;
		}

		if(!SelectObject(hGraphicsDC, hGraphicsPen))
			SELECT_ERROR();

		if(!SelectObject(hGraphicsDC, GetStockObject(NULL_BRUSH)))
			SELECT_ERROR();

		if(!Rectangle(hGraphicsDC, xw1, yw1, xw2, yw2))
			DRAW_ERROR();
#endif

	}
	else {
		fprintf(ps,"%.2f %.2f %.2f %.2f drawrect\n",XPOST(x1),YPOST(y1),
			XPOST(x2),YPOST(y2));
	}
}


/* (x1,y1) and (x2,y2) are diagonally opposed corners in world coords. */
void 
fillrect (float x1, float y1, float x2, float y2) 
{
 unsigned int width, height;
 int xw1, yw1, xw2, yw2, xl, yt;

 if (rect_off_screen(x1,y1,x2,y2))
    return;

 if (disp_type == SCREEN) {
/* translate to X Windows calling convention. */
    xw1 = xcoord(x1);
    xw2 = xcoord(x2);
    yw1 = ycoord(y1);
    yw2 = ycoord(y2); 
#ifdef X11
    xl = min(xw1,xw2);
    yt = min(yw1,yw2);
    width = abs (xw1-xw2);
    height = abs (yw1-yw2);
    XFillRectangle(display, toplevel, gc, xl, yt, width, height);
#else /* Win32 */
 	if(xw1 > xw2) {
		int temp = xw1;
		xw1 = xw2;
		xw2 = temp;
	}
	if(yw1 > yw2) {
		int temp = yw1;
		yw1 = yw2;
		yw2 = temp;
	}

	if(!SelectObject(hGraphicsDC, hGraphicsPen))
		SELECT_ERROR();

	if(!SelectObject(hGraphicsDC, hGraphicsBrush))
		SELECT_ERROR();

	if(!Rectangle(hGraphicsDC, xw1, yw1, xw2, yw2))
		DRAW_ERROR();

#endif
 }
 else {
    fprintf(ps,"%.2f %.2f %.2f %.2f fillrect\n",XPOST(x1),YPOST(y1),
          XPOST(x2),YPOST(y2));
 }
}


/* Normalizes an angle to be between 0 and 360 degrees. */
static float 
angnorm (float ang) 
{
	int scale;
	
	if (ang < 0) {
		scale = (int) (ang / 360. - 1);
	}
	else {
		scale = (int) (ang / 360.);
	}
	ang = ang - scale * 360.;
	return (ang);
}


/* Draws a circular arc.  X11 can do elliptical arcs quite simply, and *
 * PostScript could do them by scaling the coordinate axes.  Too much  *
 * work for now, and probably too complex an object for users to draw  *
 * much, so I'm just doing circular arcs.  Startang is relative to the *
 * Window's positive x direction.  Angles in degrees.                  */
void 
drawarc (float xc, float yc, float rad, float startang, 
   float angextent) 
{
	int xl, yt;
	unsigned int width, height;
	
	/* Conservative (but fast) clip test -- check containing rectangle of *
	* a circle.                                                          */
	
	if (rect_off_screen (xc-rad,yc-rad,xc+rad,yc+rad))
		return;
	
	/* X Windows has trouble with very large angles. (Over 360).    *
	* Do following to prevent its inaccurate (overflow?) problems. */
	if (fabs(angextent) > 360.) 
		angextent = 360.;
	
	startang = angnorm (startang);
	
	if (disp_type == SCREEN) {
		xl = (int) (xcoord(xc) - fabs(xmult*rad));
		yt = (int) (ycoord(yc) - fabs(ymult*rad));
		width = (unsigned int) (2*fabs(xmult*rad));
		height = width;
#ifdef X11
		XDrawArc (display, toplevel, gc, xl, yt, width, height,
			(int) (startang*64), (int) (angextent*64));
#else
		if(!Arc(hGraphicsDC, xl, yt, xl+width, yt+height, xl, yt, xl, yt))
			DRAW_ERROR();
#endif
	}
	else {
		fprintf(ps,"%.2f %.2f %.2f %.2f %.2f %s stroke\n", XPOST(xc), 
			YPOST(yc), fabs(rad*ps_xmult), startang, startang+angextent, 
			(angextent < 0) ? "drawarcn" : "drawarc") ;
	}
}


/* Fills a circular arc.  Startang is relative to the Window's positive x   *
 * direction.  Angles in degrees.                                           */
void 
fillarc (float xc, float yc, float rad, float startang, 
      float angextent) 
{
	int xl, yt;
	unsigned int width, height;
	
	/* Conservative (but fast) clip test -- check containing rectangle of *
	* a circle.                                                          */
	
	if (rect_off_screen (xc-rad,yc-rad,xc+rad,yc+rad))
		return;
	
		/* X Windows has trouble with very large angles. (Over 360).    *
	* Do following to prevent its inaccurate (overflow?) problems. */
	
	if (fabs(angextent) > 360.) 
		angextent = 360.;
	
	startang = angnorm (startang);
	
	if (disp_type == SCREEN) {
#ifdef X11
		xl = (int) (xcoord(xc) - fabs(xmult*rad));
		yt = (int) (ycoord(yc) - fabs(ymult*rad));
		width = (unsigned int) (2*fabs(xmult*rad));
		height = width;
		XFillArc (display, toplevel, gc, xl, yt, width, height,
			(int) (startang*64), (int) (angextent*64));
#else
		MessageBox(NULL, TEXT("Sorry, drawarc not supported yet."), NULL, MB_OK);
		exit(-1);
#endif
	}
	else {
		fprintf(ps,"%.2f %.2f %.2f %.2f %.2f %s\n", fabs(rad*ps_xmult), 
			startang, startang+angextent, XPOST(xc), YPOST(yc),
			(angextent < 0) ? "fillarcn" : "fillarc") ;
	}
}


void 
fillpoly (t_point *points, int npoints) 
{
#ifdef X11
 XPoint transpoints[MAXPTS];
#else
 POINT transpoints[MAXPTS];
#endif
 int i;
 float xmin, ymin, xmax, ymax;

 if (npoints > MAXPTS) {
    printf("Error in fillpoly:  Only %d points allowed per polygon.\n",
       MAXPTS);
    printf("%d points were requested.  Polygon is not drawn.\n",npoints);
    return;
 }

/* Conservative (but fast) clip test -- check containing rectangle of *
 * polygon.                                                           */

  xmin = xmax = points[0].x;
  ymin = ymax = points[0].y;

  for (i=1;i<npoints;i++) {
     xmin = min (xmin,points[i].x);
     xmax = max (xmax,points[i].x);
     ymin = min (ymin,points[i].y);
     ymax = max (ymax,points[i].y);
  }

  if (rect_off_screen(xmin,ymin,xmax,ymax))
     return;

 if (disp_type == SCREEN) {
    for (i=0;i<npoints;i++) {
		 transpoints[i].x = (short) xcoord (points[i].x);
		 transpoints[i].y = (short) ycoord (points[i].y);
    }
#ifdef X11
    XFillPolygon(display, toplevel, gc, transpoints, npoints, Complex,
		 CoordModeOrigin);
#else
	 if(!SelectObject(hGraphicsDC, hGraphicsPen))
		 SELECT_ERROR();
	 if(!SelectObject(hGraphicsDC, hGraphicsBrush))
 		 SELECT_ERROR();
	 if(!Polygon (hGraphicsDC, transpoints, npoints))
		 DRAW_ERROR();
#endif
 }
 else {
    fprintf(ps,"\n");

    for (i=npoints-1;i>=0;i--) 
       fprintf (ps, "%.2f %.2f\n", XPOST(points[i].x), YPOST(points[i].y));
    
    fprintf (ps, "%d fillpoly\n", npoints);
 }
}

/* Draws text centered on xc,yc if it fits in boundx */
void 
drawtext (float xc, float yc, char *text, float boundx) 
{
#ifdef X11
	int len, width, xw_off, yw_off; 
	
	len = strlen(text);
	width = XTextWidth(font_info[currentfontsize], text, len);
	if (width > fabs(boundx*xmult)) return; /* Don't draw if it won't fit */
	
	xw_off = width/(2.*xmult);      /* NB:  sign doesn't matter. */
	
	/* NB:  2 * descent makes this slightly conservative but simplifies code. */
	yw_off = (font_info[currentfontsize]->ascent + 
		2 * font_info[currentfontsize]->descent)/(2.*ymult); 
	
	/* Note:  text can be clipped when a little bit of it would be visible *
	* right now.  Perhaps X doesn't return extremely accurate width and   *
	* ascent values, etc?  Could remove this completely by multiplying    *
	* xw_off and yw_off by, 1.2 or 1.5.                                   */
	if (rect_off_screen(xc-xw_off, yc-yw_off, xc+xw_off, yc+yw_off))
		return;
#endif	
	
	if (disp_type == SCREEN) {
#ifdef X11
		XDrawString(display, toplevel, gc, xcoord(xc)-width/2, ycoord(yc) + 
			(font_info[currentfontsize]->ascent - font_info[currentfontsize]->descent)/2,
			text, len);
#else
		/* Obsolete? */
#endif
	}
	else {
/*
		fprintf(ps,"(%s) %.2f %.2f censhow\n",text,XPOST(xc),YPOST(yc));
*/
	}
}


void 
flushinput (void) 
{
	if (disp_type != SCREEN) 
		return;
#ifdef X11
	XFlush(display);
#endif
}


void 
init_world (float x1, float y1, float x2, float y2) 
{
	/* Sets the coordinate system the user wants to draw into.          */
	
	xleft = x1;
	xright = x2;
	ytop = y1;
	ybot = y2;
	
	saved_xleft = xleft;     /* Save initial world coordinates to allow full */
	saved_xright = xright;   /* view button to zoom all the way out.         */
	saved_ytop = ytop;
	saved_ybot = ybot;
	
	if (disp_type == SCREEN) {
		update_transform();
	}
	else {
		update_ps_transform();
	}
}


/* Draw the current message in the text area at the screen bottom. */
void 
draw_message (void) 
{
 int len, width, savefontsize, savecolor;
 float ylow;

 if (disp_type == SCREEN) {
#ifdef X11
    XClearWindow (display, textarea);
    len = strlen (statusMessage);
    width = XTextWidth(font_info[menu_font_size], statusMessage, len);

    XSetForeground(display, gc_menus,colors[BLACK]);
    XDrawString(display, textarea, gc_menus, 
       (top_width - MWIDTH - width)/2, 
       (T_AREA_HEIGHT - 4)/2 + (font_info[menu_font_size]->ascent - 
        font_info[menu_font_size]->descent)/2, statusMessage, len);
#else
	 if(!InvalidateRect(hStatusWnd, NULL, TRUE))
		 DRAW_ERROR();
	 if(!UpdateWindow(hStatusWnd))
		 DRAW_ERROR();
#endif
 }

 else {
/* Draw the message in the bottom margin.  Printer's generally can't  *
 * print on the bottom 1/4" (area with y < 18 in PostScript coords.)  */

    savecolor = currentcolor;
    setcolor (BLACK);
    savefontsize = currentfontsize;
    setfontsize (menu_font_size - 2);  /* Smaller OK on paper */
    ylow = ps_bot - 8.; 
    fprintf(ps,"(%s) %.2f %.2f censhow\n",statusMessage,(ps_left+ps_right)/2.,
        ylow);
    setcolor (savecolor);
    setfontsize (savefontsize);
 }
}


/* Changes the message to be displayed on screen.   */
void 
update_message (char *msg) 
{
	strncpy (statusMessage, msg, BUFSIZE);
	draw_message ();
}


/* Zooms in by a factor of 1.666. */
static void 
zoom_in (void (*drawscreen) (void)) 
{
	float xdiff, ydiff;
	
	xdiff = xright - xleft; 
	ydiff = ybot - ytop;
	xleft += xdiff/5.;
	xright -= xdiff/5.;
	ytop += ydiff/5.;
	ybot -= ydiff/5.;
	
	update_transform ();
	drawscreen();
}


/* Zooms out by a factor of 1.666. */
static void 
zoom_out (void (*drawscreen) (void)) 
{
	float xdiff, ydiff;
	
	xdiff = xright - xleft; 
	ydiff = ybot - ytop;
	xleft -= xdiff/3.;
	xright += xdiff/3.;
	ytop -= ydiff/3.;
	ybot += ydiff/3.;
	
	update_transform ();
	drawscreen();
}


/* Sets the view back to the initial view set by init_world (i.e. a full     *
 * view) of all the graphics.                                                */
static void 
zoom_fit (void (*drawscreen) (void)) 
{
	xleft = saved_xleft;
	xright = saved_xright;
	ytop = saved_ytop;
	ybot = saved_ybot;
	
	update_transform ();
	drawscreen();
}


/* Moves view 1/2 screen up. */
static void 
translate_up (void (*drawscreen) (void)) 
{
	float ystep;
	
	ystep = (ybot - ytop)/2.;
	ytop -= ystep;
	ybot -= ystep;
	update_transform();         
	drawscreen();
}


/* Moves view 1/2 screen down. */
static void 
translate_down (void (*drawscreen) (void)) 
{
	float ystep;
	
	ystep = (ybot - ytop)/2.;
	ytop += ystep;
	ybot += ystep;
	update_transform();         
	drawscreen();
}


/* Moves view 1/2 screen left. */
static void 
translate_left (void (*drawscreen) (void)) 
{
	
	float xstep;
	
	xstep = (xright - xleft)/2.;
	xleft -= xstep;
	xright -= xstep; 
	update_transform();         
	drawscreen();
}


/* Moves view 1/2 screen right. */
static void 
translate_right (void (*drawscreen) (void)) 
{
	float xstep;
	
	xstep = (xright - xleft)/2.;
	xleft += xstep;
	xright += xstep; 
	update_transform();         
	drawscreen();
}


static void 
update_win (int x[2], int y[2], void (*drawscreen)(void)) 
{
	float x1, x2, y1, y2;
	
	x[0] = min(x[0],top_width-MWIDTH);  /* Can't go under menu */
	x[1] = min(x[1],top_width-MWIDTH);
	y[0] = min(y[0],top_height-T_AREA_HEIGHT); /* Can't go under text area */
	y[1] = min(y[1],top_height-T_AREA_HEIGHT);
	
	if ((x[0] == x[1]) || (y[0] == y[1])) {
		printf("Illegal (zero area) window.  Window unchanged.\n");
		return;
	}
	x1 = XTOWORLD(min(x[0],x[1]));
	x2 = XTOWORLD(max(x[0],x[1]));
	y1 = YTOWORLD(min(y[0],y[1]));
	y2 = YTOWORLD(max(y[0],y[1]));
	xleft = x1;
	xright = x2;
	ytop = y1;
	ybot = y2;
	update_transform();
	drawscreen();
}


/* The window button was pressed.  Let the user click on the two *
 * diagonally opposed corners, and zoom in on this area.         */
static void 
adjustwin (void (*drawscreen) (void)) 
{
#ifdef X11	

 XEvent report;
 int corner, xold, yold, x[2], y[2];

 corner = 0;
 xold = -1;
 yold = -1;    /* Don't need to init yold, but stops compiler warning. */
 
 while (corner<2) {
    XNextEvent (display, &report);
    switch (report.type) {
    case Expose:
#ifdef VERBOSE 
       printf("Got an expose event.\n");
       printf("Count is: %d.\n",report.xexpose.count);
       printf("Window ID is: %d.\n",report.xexpose.window);
#endif
       if (report.xexpose.count != 0)
           break;
       if (report.xexpose.window == menu)
          drawmenu(); 
       else if (report.xexpose.window == toplevel) {
          drawscreen();
          xold = -1;   /* No rubber band on screen */
       }
       else if (report.xexpose.window == textarea)
          draw_message();
       break;
    case ConfigureNotify:
       top_width = report.xconfigure.width;
       top_height = report.xconfigure.height;
       update_transform();
#ifdef VERBOSE 
       printf("Got a ConfigureNotify.\n");
       printf("New width: %d  New height: %d.\n",top_width,top_height);
#endif
       break;
    case ButtonPress:
#ifdef VERBOSE 
       printf("Got a buttonpress.\n");
       printf("Window ID is: %d.\n",report.xbutton.window);
       printf("Location (%d, %d).\n", report.xbutton.x,
          report.xbutton.y);
#endif
       if (report.xbutton.window != toplevel) break;
       x[corner] = report.xbutton.x;
       y[corner] = report.xbutton.y; 
       if (corner == 0) {
       XSelectInput (display, toplevel, ExposureMask | 
         StructureNotifyMask | ButtonPressMask | PointerMotionMask);
       }
       else {
          update_win(x,y,drawscreen);
       }
       corner++;
       break;
    case MotionNotify:
#ifdef VERBOSE 
       printf("Got a MotionNotify Event.\n");
       printf("x: %d    y: %d\n",report.xmotion.x,report.xmotion.y);
#endif
       if (xold >= 0) {  /* xold set -ve before we draw first box */
          XDrawRectangle(display,toplevel,gcxor,min(x[0],xold),
             min(y[0],yold),abs(x[0]-xold),abs(y[0]-yold));
       }
       /* Don't allow user to window under menu region */
       xold = min(report.xmotion.x,top_width-1-MWIDTH); 
       yold = report.xmotion.y;
       XDrawRectangle(display,toplevel,gcxor,min(x[0],xold),
          min(y[0],yold),abs(x[0]-xold),abs(y[0]-yold));
       break;
    }
 }
 XSelectInput (display, toplevel, ExposureMask | StructureNotifyMask
                | ButtonPressMask); 
#else /* Win32 */
	/* Implemented as WM_LB... events */

   /* Begin window adjust */
	if (!windowAdjustFlag) {
		windowAdjustFlag = 1;
	} 
#endif
}


static void 
postscript (void (*drawscreen) (void)) 
{
	/* Takes a snapshot of the screen and stores it in pic?.ps.  The *
	* first picture goes in pic1.ps, the second in pic2.ps, etc.    */
	
	static int piccount = 1;
	int success;
	char fname[20];
	char msg[200];
	char oldmsg[200];

	sprintf(fname,"pic%d.ps",piccount);
	sprintf(msg, "Writing output to file %s", fname);
	strcpy(oldmsg, statusMessage);
	update_message(msg);
	success = init_postscript (fname);
	
	if (success) {

		/* Hmmm.... Couldn't get around doing this.  I guess it's bad... Oh well :) */
#ifdef X11
		drawscreen();
#else
		drawscreen();
#endif
		close_postscript ();
		piccount++;
	} else {
#ifdef WIN32
		MessageBox(hMainWnd, "Error initializing postscript output.", NULL, MB_OK);
#endif
	}
	update_message(oldmsg);
}


static void 
proceed (void (*drawscreen) (void)) 
{
	ProceedPressed = TRUE;
}


static void 
quit (void (*drawscreen) (void)) 
{
	close_graphics();
	exit(0);
}


	/* Release all my drawing structures (through the X server) and *
 	 * close down the connection.                                   */
void 
close_graphics (void) 
{
	int i;
#ifdef X11
	
	for (i=1;i<=MAX_FONT_SIZE;i++) {
		if (font_is_loaded[i])
			XFreeFont(display,font_info[i]);
	}
	
	XFreeGC(display,gc);
	XFreeGC(display,gcxor);
	XFreeGC(display,gc_menus);
	
	if (private_cmap != None) 
		XFreeColormap (display, private_cmap);
	
	XCloseDisplay(display);
#else /* Win32 */		
	if(!DestroyWindow(hMainWnd))
		DRAW_ERROR();
#endif
	free(button);
}


/* Opens a file for PostScript output.  The header information,  *
 * clipping path, etc. are all dumped out.  If the file could    *
 * not be opened, the routine returns 0; otherwise it returns 1. */
int 
init_postscript (char *fname) 
{
	
	ps = fopen (fname,"w");
	if (ps == NULL) {
		printf("Error: could not open %s for PostScript output.\n",fname);
		printf("Drawing to screen instead.\n");
		return (0);
	}
	disp_type = POSTSCRIPT;  /* Graphics go to postscript file now. */
	
	/* Header for minimal conformance with the Adobe structuring convention */
	fprintf(ps,"%%!PS-Adobe-1.0\n");
	fprintf(ps,"%%%%DocumentFonts: Helvetica\n");
	fprintf(ps,"%%%%Pages: 1\n");
	/* Set up postscript transformation macros and page boundaries */
	update_ps_transform();
	/* Bottom margin is at ps_bot - 15. to leave room for the on-screen message. */
	fprintf(ps,"%%%%BoundingBox: %.2f %.2f %.2f %.2f\n",
      ps_left, ps_bot - 15., ps_right, ps_top); 
	fprintf(ps,"%%%%EndComments\n");
	
	fprintf(ps,"/censhow   %%draw a centered string\n");
	fprintf(ps," { moveto               %% move to proper spot\n");
	fprintf(ps,"   dup stringwidth pop  %% get x length of string\n");
	fprintf(ps,"   -2 div               %% Proper left start\n");
	fprintf(ps,"   yoff rmoveto         %% Move left that much and down half font height\n");
	fprintf(ps,"   show newpath } def   %% show the string\n\n"); 
	
	fprintf(ps,"/setfontsize     %% set font to desired size and compute "
		"centering yoff\n");
	fprintf(ps," { /Helvetica findfont\n");
	fprintf(ps,"   exch scalefont\n");
	fprintf(ps,"   setfont         %% Font size set ...\n\n");
	fprintf(ps,"   0 0 moveto      %% Get vertical centering offset\n");
	fprintf(ps,"   (Xg) true charpath\n");
	fprintf(ps,"   flattenpath pathbbox\n");
	fprintf(ps,"   /ascent exch def pop -1 mul /descent exch def pop\n");
	fprintf(ps,"   newpath\n");
	fprintf(ps,"   descent ascent sub 2 div /yoff exch def } def\n\n");
	
	fprintf(ps,"%% Next two lines for debugging only.\n");
	fprintf(ps,"/str 20 string def\n");
	fprintf(ps,"/pnum {str cvs print (  ) print} def\n");
	
	fprintf(ps,"/drawline      %% draw a line from (x2,y2) to (x1,y1)\n");
	fprintf(ps," { moveto lineto stroke } def\n\n");
	
	fprintf(ps,"/rect          %% outline a rectangle \n");
	fprintf(ps," { /y2 exch def /x2 exch def /y1 exch def /x1 exch def\n");
	fprintf(ps,"   x1 y1 moveto\n");
	fprintf(ps,"   x2 y1 lineto\n");
	fprintf(ps,"   x2 y2 lineto\n");
	fprintf(ps,"   x1 y2 lineto\n");
	fprintf(ps,"   closepath } def\n\n");
	
	fprintf(ps,"/drawrect      %% draw outline of a rectanagle\n");
	fprintf(ps," { rect stroke } def\n\n");
	
	fprintf(ps,"/fillrect      %% fill in a rectanagle\n");
	fprintf(ps," { rect fill } def\n\n");
	
	fprintf (ps,"/drawarc { arc stroke } def           %% draw an arc\n");
	fprintf (ps,"/drawarcn { arcn stroke } def "
		"        %% draw an arc in the opposite direction\n\n");
	
	fprintf (ps,"%%Fill a counterclockwise or clockwise arc sector, "
		"respectively.\n");
	fprintf (ps,"/fillarc { moveto currentpoint 5 2 roll arc closepath fill } "
		"def\n");
	fprintf (ps,"/fillarcn { moveto currentpoint 5 2 roll arcn closepath fill } "
		"def\n\n");
	
	fprintf (ps,"/fillpoly { 3 1 roll moveto         %% move to first point\n"
		"   2 exch 1 exch {pop lineto} for   %% line to all other points\n"
		"   closepath fill } def\n\n");
	
	
	fprintf(ps,"%%Color Definitions:\n");
	fprintf(ps,"/white { 1 setgray } def\n");
	fprintf(ps,"/black { 0 setgray } def\n");
	fprintf(ps,"/grey55 { .55 setgray } def\n");
	fprintf(ps,"/grey75 { .75 setgray } def\n");
	fprintf(ps,"/blue { 0 0 1 setrgbcolor } def\n");
	fprintf(ps,"/green { 0 1 0 setrgbcolor } def\n");
	fprintf(ps,"/yellow { 1 1 0 setrgbcolor } def\n");
	fprintf(ps,"/cyan { 0 1 1 setrgbcolor } def\n");
	fprintf(ps,"/red { 1 0 0 setrgbcolor } def\n");
	fprintf(ps,"/darkgreen { 0 0.5 0 setrgbcolor } def\n");
	fprintf(ps,"/magenta { 1 0 1 setrgbcolor } def\n");
	
	fprintf(ps,"\n%%Solid and dashed line definitions:\n");
	fprintf(ps,"/linesolid {[] 0 setdash} def\n");
	fprintf(ps,"/linedashed {[3 3] 0 setdash} def\n");
	
	fprintf(ps,"\n%%%%EndProlog\n");
	fprintf(ps,"%%%%Page: 1 1\n\n");
	
	/* Set up PostScript graphics state to match current one. */
	force_setcolor (currentcolor);
	force_setlinestyle (currentlinestyle);
	force_setlinewidth (currentlinewidth);
	force_setfontsize (currentfontsize); 
	
	/* Draw this in the bottom margin -- must do before the clippath is set */
	draw_message ();
	
	/* Set clipping on page. */
	fprintf(ps,"%.2f %.2f %.2f %.2f rect ",ps_left, ps_bot,ps_right,ps_top);
	fprintf(ps,"clip newpath\n\n");
	
	return (1);
}


void 
close_postscript (void) 
{

	/* Properly ends postscript output and redirects output to screen. */
	
	fprintf(ps,"showpage\n");
	fprintf(ps,"\n%%%%Trailer\n");
	fclose (ps);
	disp_type = SCREEN;
	update_transform();   /* Ensure screen world reflects any changes      *
	* made while printing.                          */
	
	/* Need to make sure that we really set up the graphics context --  *
	* don't want the change requested to match the current setting and *
	* do nothing -> force the changes.                                 */
/*	
	force_setcolor (currentcolor);
	force_setlinestyle (currentlinestyle);
	force_setlinewidth (currentlinewidth);
	force_setfontsize (currentfontsize); 
*/
}


/* Sets up the default menu buttons on the right hand side of the window. */
static void 
build_default_menu (void) 
{
	unsigned long valuemask;
	int i, xcen, x1, y1, bwid, bheight, space;

#ifdef X11
	XSetWindowAttributes menu_attributes;
	
	menu = XCreateSimpleWindow(display,toplevel,
		top_width-MWIDTH, 0, MWIDTH-4, display_height, 2,
		colors[BLACK], colors[LIGHTGREY]); 
	menu_attributes.event_mask = ExposureMask;
   /* Ignore button presses on the menu background. */
	menu_attributes.do_not_propagate_mask = ButtonPressMask;
   /* Keep menu on top right */
	menu_attributes.win_gravity = NorthEastGravity; 
	valuemask = CWWinGravity | CWEventMask | CWDontPropagate;
	XChangeWindowAttributes(display, menu, valuemask, &menu_attributes);
	XMapWindow (display, menu);
#endif

	button = (t_button *) my_malloc (11 * sizeof (t_button));
	
	/* Now do the arrow buttons */
	bwid = 28;
	space = 3;
	y1 = 10;
	xcen = 51;
	x1 = xcen - bwid/2; 
	button[0].xleft = x1;
	button[0].ytop = y1;
#ifdef X11
	setpoly (0, bwid/2, bwid/2, bwid/3, -PI/2.); /* Up */
#else
	button[0].istext = 1;
	button[0].ispoly = 0;
	strcpy(button[0].text, "U");
#endif
	button[0].fcn = translate_up;
	
	y1 += bwid + space;
	x1 = xcen - 3*bwid/2 - space;
	button[1].xleft = x1;
	button[1].ytop = y1;
#if X11
	setpoly (1, bwid/2, bwid/2, bwid/3, PI);  /* Left */
#else
	button[1].istext = 1;
	button[1].ispoly = 0;
	strcpy(button[1].text, "L");
#endif
	button[1].fcn = translate_left;
	
	x1 = xcen + bwid/2 + space;
	button[2].xleft = x1;
	button[2].ytop = y1;
#if X11
	setpoly (2, bwid/2, bwid/2, bwid/3, 0);  /* Right */
#else
	button[2].istext = 1;
	button[2].ispoly = 0;
	strcpy(button[2].text, "R");
#endif
	button[2].fcn = translate_right;
	
	y1 += bwid + space;
	x1 = xcen - bwid/2;
	button[3].xleft = x1;
	button[3].ytop = y1;
#if X11
	setpoly (3, bwid/2, bwid/2, bwid/3, +PI/2.);  /* Down */
#else
	button[3].istext = 1;
	button[3].ispoly = 0;
	strcpy(button[3].text, "D");
#endif
	button[3].fcn = translate_down;
	
	for (i=0;i<4;i++) {
		button[i].width = bwid;
		button[i].height = bwid;
	} 
	
	/* Rectangular buttons */
	
	y1 += bwid + space + 6;
	space = 8;
	bwid = 90;
	bheight = 26;
	x1 = xcen - bwid/2;
	for (i=4;i<11;i++) {
		button[i].xleft = x1;
		button[i].ytop = y1;
		y1 += bheight + space;
		button[i].istext = 1;
		button[i].ispoly = 0;
		button[i].width = bwid;
		button[i].height = bheight;
	}
	
	strcpy (button[4].text,"Zoom In");
	strcpy (button[5].text,"Zoom Out");
	strcpy (button[6].text,"Zoom Fit");
	strcpy (button[7].text,"Window");
	strcpy (button[8].text,"PostScript");
	strcpy (button[9].text,"Proceed");
	strcpy (button[10].text,"Exit");
	
	button[4].fcn = zoom_in;
	button[5].fcn = zoom_out;
	button[6].fcn = zoom_fit;
	button[7].fcn = adjustwin;
	button[8].fcn = postscript;
	button[9].fcn = proceed;
	button[10].fcn = quit;
	
	for (i=0;i<11;i++) 
		map_button (i);
	num_buttons = 11;
}


/**********************************
 * X-Windows Specific Definitions *
 *********************************/
#ifdef X11

/* Makes sure the font of the specified size is loaded.  Point_size   *
 * MUST be between 1 and MAX_FONT_SIZE -- no check is performed here. */
/* Use proper point-size medium-weight upright helvetica font */
static void 
load_font(int pointsize) 
{
	
	char fontname[44];
	
	sprintf(fontname,"-*-helvetica-medium-r-*--*-%d0-*-*-*-*-*-*",
		pointsize); 
	
#ifdef VERBOSE
	printf ("Loading font: point size: %d, fontname: %s\n",pointsize, fontname);
#endif
	
	/* Load font and get font information structure. */
	if ((font_info[pointsize] = XLoadQueryFont(display,fontname)) == NULL) {
		fprintf( stderr, "Cannot open desired font\n");
		exit( -1 );
	}
}

/* Creates a small window at the top of the graphics area for text messages */
static void 
build_textarea (void) 
{
	XSetWindowAttributes menu_attributes;
	unsigned long valuemask;
	
	textarea = XCreateSimpleWindow(display,toplevel,
		0, top_height-T_AREA_HEIGHT, display_width, T_AREA_HEIGHT-4, 2,
		colors[BLACK], colors[LIGHTGREY]); 
	menu_attributes.event_mask = ExposureMask;
   /* ButtonPresses in this area are ignored. */
	menu_attributes.do_not_propagate_mask = ButtonPressMask;
   /* Keep text area on bottom left */
	menu_attributes.win_gravity = SouthWestGravity; 
	valuemask = CWWinGravity | CWEventMask | CWDontPropagate;
	XChangeWindowAttributes(display, textarea, valuemask, &menu_attributes);
	XMapWindow (display, textarea);
}



/* Returns True if the event passed in is an exposure event.   Note that *
 * the bool type returned by this function is defined in Xlib.h.         */
static Bool 
test_if_exposed (Display *disp, XEvent *event_ptr, XPointer dummy) 
{


 if (event_ptr->type == Expose) {
    return (True);
 }

 return (False);
}

static void 
menutext(Window win, int xc, int yc, char *text) 
{

/* draws text center at xc, yc -- used only by menu drawing stuff */

 int len, width; 
 
 len = strlen(text);
 width = XTextWidth(font_info[menu_font_size], text, len);
 XDrawString(display, win, gc_menus, xc-width/2, yc + 
     (font_info[menu_font_size]->ascent - font_info[menu_font_size]->descent)/2,
     text, len);
}


static void 
drawbut (int bnum) 
{

/* Draws button bnum in either its pressed or unpressed state.    */

 int width, height, thick, i, ispressed;
 XPoint mypoly[6];
 
 ispressed = button[bnum].ispressed;
 thick = 2;
 width = button[bnum].width;
 height = button[bnum].height;
/* Draw top and left edges of 3D box. */
 if (ispressed) {
    XSetForeground(display, gc_menus,colors[BLACK]);
 }
 else {
    XSetForeground(display, gc_menus,colors[WHITE]);
 }

/* Note:  X Windows doesn't appear to draw the bottom pixel of *
 * a polygon with XFillPolygon, so I make this 1 pixel thicker *
 * to compensate.                                              */
 mypoly[0].x = 0;
 mypoly[0].y = height;
 mypoly[1].x = 0;
 mypoly[1].y = 0;
 mypoly[2].x = width;
 mypoly[2].y = 0;
 mypoly[3].x = width-thick;
 mypoly[3].y = thick;
 mypoly[4].x = thick;
 mypoly[4].y = thick;
 mypoly[5].x = thick;
 mypoly[5].y = height-thick;
 XFillPolygon(display,button[bnum].win,gc_menus,mypoly,6,Convex,
     CoordModeOrigin);

/* Draw bottom and right edges of 3D box. */
 if (ispressed) {
    XSetForeground(display, gc_menus,colors[WHITE]);
 }
 else {
    XSetForeground(display, gc_menus,colors[BLACK]);
 } 
 mypoly[0].x = 0;
 mypoly[0].y = height;
 mypoly[1].x = width;
 mypoly[1].y = height;
 mypoly[2].x = width;
 mypoly[2].y = 0;
 mypoly[3].x = width-thick;
 mypoly[3].y = thick;
 mypoly[4].x = width-thick;
 mypoly[4].y = height-thick;
 mypoly[5].x = thick;
 mypoly[5].y = height-thick;
 XFillPolygon(display,button[bnum].win,gc_menus,mypoly,6,Convex,
     CoordModeOrigin);

/* Draw background */
 if (ispressed) {
    XSetForeground(display, gc_menus,colors[DARKGREY]);
 }
 else {
    XSetForeground(display, gc_menus,colors[LIGHTGREY]);
 }

/* Give x,y of top corner and width and height */
 XFillRectangle (display,button[bnum].win,gc_menus,thick,thick,
     width-2*thick, height-2*thick);
 
/* Draw polygon, if there is one */
 if (button[bnum].ispoly) {
    for (i=0;i<3;i++) {
       mypoly[i].x = button[bnum].poly[i][0];
       mypoly[i].y = button[bnum].poly[i][1];
    }
    XSetForeground(display, gc_menus,colors[BLACK]);
    XFillPolygon(display,button[bnum].win,gc_menus,mypoly,3,Convex,
          CoordModeOrigin);
 }
 
/* Draw text, if there is any */
 if (button[bnum].istext) {
    XSetForeground(display, gc_menus,colors[BLACK]);
    menutext(button[bnum].win,button[bnum].width/2,
      button[bnum].height/2,button[bnum].text);
 }
}


static int 
which_button (Window win) 
{
	int i;
	
	for (i=0;i<num_buttons;i++) {
		if (button[i].win == win)
			return(i);
	}
	printf("Error:  Unknown button ID in which_button.\n");
	return(0);
}


/* Shows when the menu is active or inactive by colouring the *
 * buttons.                                                   */
static void 
turn_on_off (int pressed) 
{
	int i;
	
	for (i=0;i<num_buttons;i++) {
		button[i].ispressed = pressed;
		drawbut(i);
	}
}

static void 
drawmenu(void) 
{
	int i;
	
	for (i=0;i<num_buttons;i++)  {
		drawbut(i);
	}
}






#endif /* X-Windows Specific Definitions */



/*************************************************
* Microsoft Windows (WIN32) Specific Definitions *
*************************************************/
#ifdef WIN32

static LRESULT CALLBACK 
MainWND(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WNDCLASS wndclass;

	switch(message)
	{
	case WM_CREATE:
		hStatusWnd = CreateWindow(szStatusName, NULL, WS_CHILDWINDOW | WS_VISIBLE,
			0, 0, 0, 0, hwnd, (HMENU) 102, (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
		hButtonsWnd = CreateWindow(szButtonsName, NULL, WS_CHILDWINDOW | WS_VISIBLE,
			0, 0, 0, 0, hwnd, (HMENU) 103, (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
		hGraphicsWnd = CreateWindow(szGraphicsName, NULL, WS_CHILDWINDOW | WS_VISIBLE,
			0, 0, 0, 0, hwnd, (HMENU) 101, (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
		return 0;

	case WM_SIZE:
		/* Window has been resized.  Save the new client dimensions */
		top_width = LOWORD (lParam);
		top_height = HIWORD (lParam);

		/* Resize the children windows */
		if(!MoveWindow(hGraphicsWnd, 1, 1, top_width - MWIDTH - 1, top_height - T_AREA_HEIGHT - 1, TRUE))
			DRAW_ERROR();
		if(!MoveWindow(hStatusWnd, 0, top_height - T_AREA_HEIGHT, top_width - MWIDTH, T_AREA_HEIGHT, TRUE))
			DRAW_ERROR();
		if(!MoveWindow(hButtonsWnd, top_width - MWIDTH, 0, MWIDTH, top_height, TRUE))
			DRAW_ERROR();

		return 0;

	case WM_DESTROY:
		if(!DeleteObject(hGrayBrush))
			DELETE_ERROR();
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}


static LRESULT CALLBACK 
GraphicsWND(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TEXTMETRIC tm;

	PAINTSTRUCT ps;
	RECT rect;
	static RECT oldAdjustRect;
	static HPEN hDotPen = 0;
	
	switch(message)
	{
	case WM_CREATE:
		/* Get the text metrics once upon creation (system font cannot change) */
		hGraphicsDC = GetDC (hwnd);
		if(!hGraphicsDC)
			DRAW_ERROR();

		if(!GetTextMetrics (hGraphicsDC, &tm))
			DRAW_ERROR();
		if(!SetBkMode(hGraphicsDC, TRANSPARENT))
			DRAW_ERROR();


		/* Setup the pens, etc */
		currentlinestyle = SOLID;
		currentcolor = BLACK;
		currentlinewidth = 1;

/*
		if(!ReleaseDC (hwnd, hGraphicsDC))
			DRAW_ERROR();

		hGraphicsDC = 0;
*/		currentfontsize = 12;
		return 0;

	case WM_PAINT:
		/*hGraphicsDC =*/ BeginPaint(hwnd, &ps);
		if(!hGraphicsDC)
			DRAW_ERROR();

		if (InEventLoop) {
			if(!GetUpdateRect(hwnd, &updateRect, FALSE)) {
				updateRect.left = 0;
				updateRect.right = top_width;
				updateRect.top = 0;
				updateRect.bottom = top_height;
			}

			hDotPen = CreatePen(PS_DOT, 1, win32_colors[DARKGREEN]);
			if(!hDotPen)
				CREATE_ERROR();
			/* Yes, this is terribly slow.  Should use XOR mode -- don't remember how. (did it in RTrack version) */
			if(windowAdjustFlag > 1) {
				if(!SelectObject(hGraphicsDC, GetStockObject(NULL_BRUSH)))
					SELECT_ERROR();
				if(!SelectObject(hGraphicsDC, GetStockObject(WHITE_PEN)))
					SELECT_ERROR();
				if(!Rectangle(hGraphicsDC, oldAdjustRect.left, oldAdjustRect.top, 
					oldAdjustRect.right, oldAdjustRect.bottom))
					DRAW_ERROR();
			}

			drawscreen_ptr();
			if(windowAdjustFlag > 1) {
				if(!SelectObject(hGraphicsDC, GetStockObject(NULL_BRUSH)))
					SELECT_ERROR();
				if(!SelectObject(hGraphicsDC, hDotPen))
					SELECT_ERROR();
				if(!Rectangle(hGraphicsDC, adjustRect.left, adjustRect.top, adjustRect.right,
					adjustRect.bottom))
					DRAW_ERROR();
				oldAdjustRect = adjustRect;
			}
			if(!SelectObject(hGraphicsDC, GetStockObject(NULL_PEN)))
				SELECT_ERROR();
			if(!DeleteObject(hDotPen))
				DELETE_ERROR();
		}
		EndPaint(hwnd, &ps);

		/* Crash hard if called at wrong time */
/*		hGraphicsDC = NULL;*/
		return 0;

	case WM_SIZE:
		/* Window has been resized.  Save the new client dimensions */
		cxClient = LOWORD (lParam);
		cyClient = HIWORD (lParam);
		update_transform();
		return 0;

	case WM_DESTROY:
		if(!DeleteObject(hGraphicsPen))
			DELETE_ERROR();
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:
		if (!windowAdjustFlag) {
			clb_click(XTOWORLD(LOWORD(lParam)), YTOWORLD(HIWORD(lParam)));
		} else {
			if(windowAdjustFlag == 1) {
				windowAdjustFlag ++;
				adjustRect.left = adjustRect.right = LOWORD(lParam);
				adjustRect.top = adjustRect.bottom = HIWORD(lParam);
				oldAdjustRect = adjustRect;
			} else {
				int i;
				int adjustx[2], adjusty[2];

				windowAdjustFlag = 0;
				button[adjustButton].ispressed = 1;
				SendMessage(button[adjustButton].hwnd, BM_SETSTATE, 0, 0);

				adjustRect.right = LOWORD(lParam);
				adjustRect.bottom = HIWORD(lParam);

				for (i=0; i<num_buttons; i++) {
					if (i!=adjustButton) {
						if(!EnableWindow (button[i].hwnd, TRUE))
							DRAW_ERROR();
					}
				}
				adjustx[0] = adjustRect.left;
				adjustx[1] = adjustRect.right;
				adjusty[0] = adjustRect.top;
				adjusty[1] = adjustRect.bottom;

				update_win(adjustx, adjusty, invalidate_screen);
			}
		}
		return 0;
	case WM_MOUSEMOVE:
		if(windowAdjustFlag < 2) {
			return 0;
		} else {
			adjustRect.right = LOWORD(lParam);
			adjustRect.bottom = HIWORD(lParam);
			if(!InvalidateRect(hGraphicsWnd, &oldAdjustRect, FALSE))
				DRAW_ERROR();
			if(!InvalidateRect(hGraphicsWnd, &adjustRect, FALSE))
				DRAW_ERROR();
			if(!UpdateWindow(hGraphicsWnd))
				DRAW_ERROR();

			return 0;
		}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

static LRESULT CALLBACK 
StatusWND(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch(message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);
		if(!hdc)
			DRAW_ERROR();
		if(!SetBkMode(hdc, TRANSPARENT))
			DRAW_ERROR();
		if(!ReleaseDC(hwnd, hdc))
			DRAW_ERROR();
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if(!hdc)
			DRAW_ERROR();

		if(!GetClientRect(hwnd, &rect))
			DRAW_ERROR();

		if(!SelectObject(hdc, GetStockObject(BLACK_PEN)))
			SELECT_ERROR();
		if(!SelectObject(hdc, GetStockObject(NULL_BRUSH)))
			SELECT_ERROR();
		if(!Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom))
			DRAW_ERROR();
		if(!DrawText(hdc, TEXT(statusMessage), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE))
			DRAW_ERROR();

		if(!EndPaint(hwnd, &ps))
			DRAW_ERROR();
		return 0;

	case WM_SIZE:
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

static LRESULT CALLBACK 
ButtonsWND(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	static HBRUSH hBrush;
	int i;
	TCHAR buffer[200];

	switch(message)
	{
	case WM_COMMAND:
		if (!windowAdjustFlag) {
			button[LOWORD(wParam) - 200].fcn(invalidate_screen);
			if (windowAdjustFlag) {
				adjustButton = LOWORD(wParam) - 200;
				button[adjustButton].ispressed = 0;
				SendMessage(button[adjustButton].hwnd, BM_SETSTATE, 1, 0);
				for (i=0; i<num_buttons; i++) {
					if(i!=adjustButton) {
						if(EnableWindow(button[i].hwnd, FALSE))
							DRAW_ERROR();
					}
				}
			}
		}
		return 0;

	case WM_CREATE:
		hdc = GetDC(hwnd);
		if(!hdc)
			DRAW_ERROR();
		hBrush = CreateSolidBrush(win32_colors[LIGHTGREY]);
		if(!hBrush)
			CREATE_ERROR();
		if(!SelectObject(hdc, hBrush))
			SELECT_ERROR();
		if(!SetBkMode(hdc, TRANSPARENT))
			DRAW_ERROR();
		if(!ReleaseDC(hwnd, hdc))
			DRAW_ERROR();

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		if(!hdc)
			DRAW_ERROR();

		if(!GetClientRect(hwnd, &rect))
			DRAW_ERROR();
		
		if(!SelectObject(hdc, GetStockObject(BLACK_PEN)))
			SELECT_ERROR();
		if(!SelectObject(hdc, GetStockObject(NULL_BRUSH)))
			SELECT_ERROR();
		if(!Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom))
			DRAW_ERROR();

		for (i=0; i<num_buttons; i++) {
			if(button[i].ispressed) {
				SendMessage(button[i].hwnd, BM_SETSTATE, 0, 0);
			} else {
				SendMessage(button[i].hwnd, BM_SETSTATE, 1, 0);
			}
		}
		if(!EndPaint(hwnd, &ps))
			DRAW_ERROR();
		return 0;

	case WM_DESTROY:
		for (i=0; i<num_buttons; i++) {
		}
		if(!DeleteObject(hBrush))
			DELETE_ERROR();
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
#endif /* Win32 Specific Definitions */

#else /* Any graphics at all? */

#include "graphics.h"

void event_loop (void (*act_on_button) (float x, float y),
                 void (*drawscreen) (void)) { }

void init_graphics (char *window_name) { }
void close_graphics (void) { }
void update_message (char *msg) { }
void draw_message (void) { }
void init_world (float xl, float yt, float xr, float yb) { }
void flushinput (void) { }
void setcolor (int cindex) { }
void setlinestyle (int linestyle) { }
void setlinewidth (int linewidth) { }
void setfontsize (int pointsize) { }
void drawline (float x1, float y1, float x2, float y2) { }
void drawrect (float x1, float y1, float x2, float y2) { }
void fillrect (float x1, float y1, float x2, float y2) { }
void fillpoly (t_point *points, int npoints) { }
void drawarc (float xcen, float ycen, float rad, float startang,
  float angextent) { }

void fillarc (float xcen, float ycen, float rad, float startang,
  float angextent) { }

void drawtext (float xc, float yc, char *text, float boundx) { }
void clearscreen (void) { }

void create_button (char *prev_button_text , char *button_text,
       void (*button_func) (void (*drawscreen) (void))) { }

void destroy_button (char *button_text) { }

int init_postscript (char *fname) { 
     return (1);
}

void close_postscript (void) { }

void invalidate_screen() { }

void setOKtoPaint(int val) { }

void setWaitForProceed(int val) { }


#endif