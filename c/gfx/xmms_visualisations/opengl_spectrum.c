/*  XMMS - Cross-platform multimedia player
 *  Copyright (C) 1998-2000  Peter Alm, Mikael Alm, Olle Hallnas, Thomas Nilsson and 4Front Technologies
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 *  Wed May 24 10:49:37 CDT 2000
 *  Fixes to threading/context creation for the nVidia X4 drivers by 
 *  Christian Zander <phoenix@minion.de>
 */

//// A makeprg to build and test in one go, and use clist/win.  We must retain
//// stdout of original make but detach output of xmms.
// :set makeprg=(killall\ xmms\ ;\ make\ &&\ make\ install\ &&\ (\ killall\ xmms\ ;\ sleep\ 2\ )\ &&\ ((nice\ -n\ 12\ xmms\ -p\ &)\ >/dev/null\ 2>&1)\ )



#include "config.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <gtk/gtk.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <pthread.h>
#ifdef HAVE_SCHED_SETSCHEDULER
#include <sched.h>
#endif
#include <stdlib.h>

#include "xmms/plugin.h"
#include "libxmms/xmmsctrl.h"
#include "libxmms/configfile.h"
#include "libxmms/util.h"
#include "opengl_spectrum.h"
#include "xmms/i18n.h"

/* NUM_BANDS should be either 16 or 32.  We do have a slow hack for larger values. */
// #define NUM_BANDS 16
// #define NUM_BANDS 32
// #define NUM_BANDS 48
#define NUM_BANDS 64
// #define NUM_BANDS 128
// #define NUM_BANDS 256

/* The original LENGTH was 16 */
#define LENGTH 64
// #define LENGTH 200
// #define LENGTH 160
// #define LENGTH 120
// #define LENGTH 80
// BUG TOOD: Any higher than 120 and we seem to suddenly lose the far ones anyway!

#if LENGTH < 32
	#define SCALEBACK (16.0/LENGTH)
#else
	// After 32 we give up normalising the size, and we let the trail lengthen rather than compress:
	// Increase the number to make things move faster!
	#define SCALEBACK (40.0/LENGTH)
#endif

#define WIDTH NUM_BANDS

#define PREVENT_HIDING_GLOW
// #define ANGLED_SURFACES_IN_X
// #define LIGHTER_VERSION
// #define CHECK_FOR_KICK
// #define SHOW_PEAKS_ONLY
// #define BUILD_UP_PEAK_ENERGY
// #define HIDE_TROUGHS

#ifdef LIGHTER_VERSION
	// For slower PCs with small screens.
	#define WINWIDTH 560
	// #define WINHEIGHT 320
	#define WINHEIGHT 420
	#undef LENGTH
	// Good framerate at 16, some framerate at 32
	// 48 poor framerate and poor depth
	// 64+ poor framerate but at least some history!
	#define LENGTH 16
	#if LENGTH <= 32
		// With too few history samples, fast fading makes things invisible too soon!
		#define SIMPLE_LIGHTING
	#endif
/*
	// #define WINWIDTH 320
	// #define WINHEIGHT 180
*/
#else
	#define WINWIDTH 640
	// #define WINHEIGHT 360
	#define WINHEIGHT 480
#endif

OGLSpectrumConfig oglspectrum_cfg;

static Display *dpy = NULL;
static Colormap colormap = 0;
static GLXContext glxcontext = NULL;
static Window window = 0;
static GLfloat y_angle = -15.0, y_speed = 0.5; // 0.5;
static GLfloat x_angle = 20.0, x_speed = 0.0;
static GLfloat z_angle = 0.0, z_speed = 0.0;
static GLfloat heights[LENGTH][WIDTH], scale;
static gboolean going = FALSE, grabbed_pointer = FALSE;
static Atom wm_delete_window_atom;
static pthread_t draw_thread;
static unsigned char modeCycle = 0;
#ifdef CHECK_FOR_KICK
static GLfloat recentPeak = 1.0;
#endif

static void oglspectrum_init(void);
static void oglspectrum_cleanup(void);
static void oglspectrum_playback_start(void);
static void oglspectrum_playback_stop(void);
static void oglspectrum_render_freq(gint16 data[2][256]);

VisPlugin oglspectrum_vp =
{
	NULL,
	NULL,
	0,
	NULL, /* Description */
	0,
	1,		
	oglspectrum_init, /* init */
	oglspectrum_cleanup, /* cleanup */
	NULL, /* about */
	oglspectrum_configure, /* configure */
	NULL, /* disable_plugin */
	oglspectrum_playback_start, /* playback_start */
	oglspectrum_playback_stop, /* playback_stop */
	NULL, /* render_pcm */
	oglspectrum_render_freq  /* render_freq */
};

static void setup_camera(void)
{
	// x_speed = 0.4; y_speed = -0.2; x_angle = -15.0;  // 0.5   // right/left
	// x_speed = 0.0; y_speed = 0.1; x_angle = 30.0;   // x=up/down, y=right/left
	x_speed = 0.0; y_speed = 0.0; y_angle = 35.0; x_angle = 30.0;   // x=up/down, y=right/left
	z_speed = 0.0;
	// y_angle = 55.0;
	// y_angle = -15.0;
	z_angle = 0.0;
	modeCycle = 6;
}

static Window create_window(int width, int height)
{
	int attr_list[] = {
		GLX_RGBA,
		GLX_DEPTH_SIZE, 16,
		GLX_DOUBLEBUFFER,
		None
	};
	int scrnum;
	XSetWindowAttributes attr;
	unsigned long mask;
	Window root, win;
	XVisualInfo *visinfo;
	Atom wm_protocols[1];

	if ((dpy = XOpenDisplay(NULL)) == NULL)
		return 0;

	scrnum = DefaultScreen(dpy);
	root = RootWindow(dpy, scrnum);

	if ((visinfo = glXChooseVisual(dpy, scrnum, attr_list)) == NULL)
		return 0;

	attr.background_pixel = 0;
	attr.border_pixel = 0;
	attr.colormap = colormap = XCreateColormap(dpy, root,
						   visinfo->visual, AllocNone);
	attr.event_mask = StructureNotifyMask | KeyPressMask;
	mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	win = XCreateWindow(dpy, root, 0, 0, width, height,
			    0, visinfo->depth, InputOutput,
			    visinfo->visual, mask, &attr);
	XmbSetWMProperties(dpy, win, _("OpenGL Spectrum analyzer"),
			   _("OpenGL Spectrum analyzer"), NULL, 0, NULL, NULL,
			   NULL);
	wm_delete_window_atom = wm_protocols[0] =
		XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, win, wm_protocols, 1);

	glxcontext = glXCreateContext(dpy, visinfo, NULL, True);

	XFree(visinfo);

	glXMakeCurrent(dpy, win, glxcontext);

	return win;
}

VisPlugin *get_vplugin_info(void)
{
	oglspectrum_vp.description =
		g_strdup_printf(_("OpenGL Spectrum analyzer %s"), VERSION);
	return &oglspectrum_vp;
}

void oglspectrum_read_config(void)
{
	ConfigFile *cfg;	
	gchar *filename;

	oglspectrum_cfg.tdfx_mode = FALSE;
	
	filename = g_strconcat(g_get_home_dir(), "/.xmms/config", NULL);
	cfg = xmms_cfg_open_file(filename);
	
	if (cfg)
	{
		xmms_cfg_read_boolean(cfg, "OpenGL Spectrum", "tdfx_fullscreen", &oglspectrum_cfg.tdfx_mode);		
		xmms_cfg_free(cfg);
	}
	g_free(filename);
}

static void draw_triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glVertex3f(x3, y3, z3);
}

static void draw_rectangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{
	if(y1 == y2) // z goes 112, 221
	{
	
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z1);
		glVertex3f(x2, y2, z2);
		
		glVertex3f(x2, y2, z2);
		glVertex3f(x1, y2, z2);
		glVertex3f(x1, y1, z1);
	}
	else // z goes 122, 211
	{
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y1, z2);
		glVertex3f(x2, y2, z2);
		
		glVertex3f(x2, y2, z2);
		glVertex3f(x1, y2, z1);
		glVertex3f(x1, y1, z1);
	}
}

// Doing our own hsl->rgb conversion is faster than making Chrome do it!
static void convertHSLtoRGB(GLfloat h, GLfloat s, GLfloat l, GLfloat *redPtr, GLfloat *greenPtr, GLfloat *bluePtr) {
	// All input and output values are in range 0.0 to 1.0
	if (s > 1) s=1;
	if (s < 0) s=0;
	if (l > 1) l=1;
	if (l < 0) l=0;
	GLfloat red, green, blue;
	if(s == 0) {
		red = green = blue = l;
	} else {
		GLfloat temp2 = ( l < 0.5 ? l*(1.0+s) : l+s - l*s );
		GLfloat temp1 = 2.0*l - temp2;
		GLfloat temp3;

		// Red
		temp3 = h + 1.0/3.0;
		if (temp3<0)
			temp3 += 1.0;
		if (temp3>1)
			temp3 -= 1.0;
		if (temp3 < 1.0/6.0)
			red = temp1 + (temp2-temp1)*6*temp3;
		else if (temp3 < 1.0/2.0)
			red = temp2;
		else if (temp3 < 2.0/3.0)
			red = temp1 + (temp2-temp1)*(2.0/3.0-temp3)*6;
		else
			red = temp1;

		// Green
		temp3 = h;
		if (temp3<0)
			temp3 += 1.0;
		if (temp3>1.0)
			temp3 -= 1.0;
		if (temp3 < 1.0/6.0)
			green = temp1 + (temp2-temp1)*6*temp3;
		else if (temp3 < 1.0/2.0)
			green = temp2;
		else if (temp3 < 2.0/3.0)
			green = temp1 + (temp2-temp1)*(2.0/3.0-temp3)*6.0;
		else
			green = temp1;

		// Blue
		temp3 = h - 1.0/3.0;
		if (temp3<0)
			temp3 += 1.0;
		if (temp3>1)
			temp3 -= 1.0;
		if (temp3 < 1.0/6.0)
			blue = temp1 + (temp2-temp1)*6.0*temp3;
		else if (temp3 < 1.0/2.0)
			blue = temp2;
		else if (temp3 < 2.0/3.0)
			blue = temp1 + (temp2-temp1)*(2.0/3.0-temp3)*6.0;
		else
			blue = temp1;

		// red   = (red   * 255) | 0;
		// green = (green * 255) | 0;
		// blue  = (blue  * 255) | 0;
	}
	glColor3f(red,green,blue);
	(*redPtr) = red;
	(*greenPtr) = green;
	(*bluePtr) = blue;
	// return "rgba("+red+","+green+","+blue+","+a/100+")";
}

static void draw_quadrilateral(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat x4, GLfloat y4, GLfloat z4)
{
	draw_triangle(x1,y1,z1, x2,y2,z2, x3,y3,z3);
	draw_triangle(x1,y1,z1, x4,y4,z4, x3,y3,z3);
}

static void draw_bar(GLfloat x_offset, GLfloat z_offset, GLfloat height, GLfloat lastYheight, GLfloat lastXheight, GLfloat lastXYheight, GLfloat hue, GLfloat sat, GLfloat lightness, GLfloat width, GLfloat length )
{

	// float whiteness = red*red;
	// float notwhiteness = 1.0 - whiteness;

	GLfloat red, green, blue;

	#define setHSL(hue, sat, lightness) convertHSLtoRGB(hue, sat, lightness, &red, &green, &blue); glColor3f(red,green,blue)

	//  red = 1.0*whiteness +  red*notwhiteness;
	// green = 1.0*whiteness + green*notwhiteness;
	// blue = 1.0*whiteness +  blue*notwhiteness;

	// float W = red*red;
	// float W = lightness;
	// float NW = 1.0 - W;

	#define glColor3f_with_scale_then_whiteness(r,g,b,s) glColor3f(W+r*s*NW,W+g*s*NW,W+b*s*NW)

	// If we imagine the spectrum going right across the screen, and history going in to the picture.

	// Flat (horizontal) facing up/down
	// glColor3f(red,green,blue);
	// glColor3f_with_scale_then_whiteness(red,green,blue,1.0);
	setHSL(hue, sat*0.8, lightness*0.8);
	// draw_rectangle(x_offset, height, z_offset, x_offset + width, lastYheight, z_offset + length);
	// draw_triangle(x_offset, lastYheight, z_offset, x_offset, height, z_offset + length, x_offset + width, height, z_offset + length);
	// draw_triangle(x_offset, lastYheight, z_offset, x_offset + width, lastYheight, z_offset, x_offset + width, height, z_offset + length);
	draw_quadrilateral(x_offset, lastXYheight, z_offset, x_offset, lastXheight, z_offset + length, x_offset + width, height, z_offset + length, x_offset + width, lastYheight, z_offset);
	draw_rectangle(x_offset,      0, z_offset, x_offset + width,           0, z_offset + length);

	// In width plane (across spectrum), facing front/back
	// glColor3f(0.5 * red, 0.5 * green, 0.5 * blue);
	// glColor3f_with_scale_then_whiteness(red,green,blue,0.5);
	setHSL(hue, sat, lightness);
	// draw_rectangle(x_offset, 0.0, z_offset + length, x_offset + width, height, z_offset + length);
	// draw_rectangle(x_offset, 0.0, z_offset         , x_offset + width, lastYheight, z_offset         );
	draw_quadrilateral(x_offset, 0.0, z_offset + length, x_offset+width, 0.0, z_offset+length, x_offset + width, height, z_offset + length, x_offset, lastXheight, z_offset+length);
	draw_quadrilateral(x_offset, 0.0, z_offset         , x_offset+width, 0.0, z_offset       , x_offset + width, lastYheight, z_offset, x_offset, lastXYheight, z_offset);

	// In depth plane (into history) facing left/right
	// glColor3f(0.25 * red, 0.25 * green, 0.25 * blue);
	// glColor3f_with_scale_then_whiteness(red,green,blue,0.25);
	// glColor3f(red,green,blue);
	setHSL(hue, sat*0.6, lightness*0.6);
	// draw_rectangle(x_offset        , 0.0, z_offset , x_offset        , height, z_offset + length);	
	draw_triangle(x_offset, 0.0, z_offset, x_offset, 0.0, z_offset + length, x_offset, lastXheight, z_offset + length);	
	draw_triangle(x_offset, 0.0, z_offset, x_offset, lastXYheight, z_offset, x_offset, lastXheight, z_offset + length);	
	// draw_quadrilateral(x_offset, 0.0, z_offset, x_offset, 0.0, z_offset + length, x_offset, height, z_offset + length, x_offset, lastYheight, z_offset);
	// draw_rectangle(x_offset + width, 0.0, z_offset , x_offset + width, height, z_offset + length);
	// draw_triangle(x_offset + width, 0.0, z_offset, x_offset + width, 0.0, z_offset + length, x_offset + width, height, z_offset + length);	
	// draw_triangle(x_offset + width, 0.0, z_offset, x_offset + width, lastYheight, z_offset, x_offset + width, height, z_offset + length);	
	draw_quadrilateral(x_offset + width, 0.0, z_offset, x_offset + width, 0.0, z_offset + length, x_offset + width, height, z_offset + length, x_offset + width, lastYheight, z_offset);

}

static void draw_bars(void)
{
	gint x,y;
	GLfloat x_offset, z_offset, r_base, b_base;

	GLfloat peakEnergy[WIDTH];
	GLfloat peakHeight[WIDTH];
	GLfloat localAverage[WIDTH];
	GLfloat localNoise[WIDTH];
	GLfloat lastYwhiteness[WIDTH];

	#define compensateForCurveX(x) (0.6+0.4*(float)x/(float)WIDTH)
	#define compensateForCurve compensateForCurveX(x)

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0,-0.2,-4.2);        // originally 0.0,-0.5,-5.0
	glRotatef(x_angle,1.0,0.0,0.0);
	glRotatef(y_angle,0.0,1.0,0.0);
	glRotatef(z_angle,0.0,0.0,1.0);

	for(x = 0; x < WIDTH; x++)
	{
		peakEnergy[x] = 0.0;
		peakHeight[x] = 0.0;
		localAverage[x] = 0.0;
		localNoise[x] = 0.0;
	}

	glBegin(GL_TRIANGLES);
	for(y = LENGTH-1; y >=0; y--)
	{
		// z_offset = -1.6 + ((LENGTH - y)*SCALEBACK * 0.2);
		z_offset = +1.6 - ((y)*SCALEBACK * 0.2);

		// b_base = y * (1.0 / (LENGTH-1));
		b_base = (float)y / (float)(LENGTH-1);
		r_base = 1.0 - b_base;

		float whiteness;
		float w_base = 2.0 * (r_base - 0.5);
		// float w_base = 4.0 * (r_base - 0.75);
		// float w_base = (r_base - 0.95)/0.05;
		if (w_base < 0.0)
			w_base = 0.0;
		// GLfloat breakingEdge = w_base * w_base;

		GLfloat fadeOff = (1.0 - (float)y/(float)(LENGTH-1));
		// fadeOff = fadeOff * fadeOff;
		fadeOff = pow(fadeOff,1.5);

		#ifdef ANGLED_SURFACES_IN_X
			GLfloat lastXheight = 0;
			GLfloat lastXYheight = 0;
			GLfloat lastXwhiteness = 0;
		#else
			// lastXYheight and lastYheight create angled surfaces across the spectrum
			// If we want the old behaviour (angled surfaces only into history)
			// then we can override them with the old values:
			#define lastXheight barHeight
			#define lastXYheight lastYheight
		#endif

		for(x = 0; x < WIDTH; x++)
		{
			GLfloat saturation = 1.0;
			// GLfloat hue = x*1.0/WIDTH;
			GLfloat hue = 2.0/3.0 + x*5.0/6.0/WIDTH; // blue to cyan
			// GLfloat hue = 0.0 + x*5.0/6.0/WIDTH; // red to purple
			// GLfloat hue = -1.0/6.0 + x*5.0/6.0/WIDTH; // purple to blue

			x_offset = -1.6 + 3.2*x/WIDTH;

			// #define barHeight heights[y][x]
			// #define barHeight (heights[y][x] + peakEnergy[x]) / 2.0
			// #define barHeight peakHeight[x]
			#define barHeight compensateForCurve*peakHeight[x]

			// PROBLEM: peakHeight[x] may be valid now, but peakHeight[x-1] was overwritten on the last iteration!
			// GLfloat lastXYheight = compensateForCurve*peakHeight[x-1];

			GLfloat lastYheight = barHeight; // evaluate it now, before we update peakHeight for the current cell

			// GLfloat energySlowFade = 2.0 * fmin(heights[y][x]-0.1,0.5) * r_base;
			// GLfloat energySlowFade = compensateForCurve*heights[y][x] * r_base;
			// breakingEdge = breakingEdge * energySlowFade * 4.0;
			// energySlowFade = energySlowFade*energySlowFade; // fast fade!
			// whiteness = fmin(1.0,fmax(energySlowFade, breakingEdge));

// #define energyDampening 0.5
// #define heightDampening 0.25
			// TODO: This || is probably invalid!
#ifdef SHOW_PEAKS_ONLY || HIDE_TROUGHS
#define energyDampening 0.5
#define heightDampening 0.7
#else
// #define energyDampening 0.5
// #define heightDampening 0.15
#define energyDampening 0.00
#define heightDampening 0.00
#endif

			peakEnergy[x] *= energyDampening;
			// GLfloat energyHere = 1.5 * heights[y][x];
			GLfloat energyHere = heights[y][x] + 0.5*fmin(0,heights[y][x] - heights[y+1][x]);
			energyHere *= compensateForCurve * 1.25;
			if (energyHere > peakEnergy[x]) {
				peakEnergy[x] = energyHere;
			}

			peakHeight[x] *= heightDampening;
			if (heights[y][x] > peakHeight[x]) {
				peakHeight[x] = heights[y][x];
			}
			// Sinking ship:
			peakHeight[x] -= 0.05 * y/LENGTH;
			peakHeight[x] = fmax(0, peakHeight[x]);

#ifdef SIMPLE_LIGHTING

			//// Fade out slowly from white to color to black:
			whiteness = 2.0 * peakEnergy[x] * fadeOff;

#else

			localAverage[x] *= 0.99;
			localAverage[x] += 0.01 * peakHeight[x];

			localNoise[x] *= 0.99;
			localNoise[x] += 0.01 * pow(heights[y][x] - heights[y+1][x],2);

			// whiteness = fmax(energyHere, breakingEdge);
			// whiteness = peakEnergy[x];
			// whiteness = 1.3 * peakEnergy[x] * fadeOff;
			//// Colored amplitudes fade to white when they drop:
			// whiteness = 0.8 - 1.1*peakEnergy[x];

			//// Power fade the energy, so the difference between high and low appears stronger in the distance.
			whiteness = 5.0 * peakEnergy[x];
			// whiteness = 5.0 * peakHeight[x];
			/* whiteness += 0.0001 - 0.1 * (float)y/(float)LENGTH;
			if (whiteness<0)
				whiteness=0; */

			whiteness = pow(whiteness, 1.0 + 2.0*(float)y/(float)LENGTH);
			whiteness /= 2.5;

			//// Reduce energy of all bars in a busy channel
			whiteness -= 0.5 * localAverage[x]; // * y/LENGTH;
			whiteness -= 0.5 * localNoise[x]; // * y/LENGTH;
			//// We should also consider horizontal energy (which we have not yet calculated).

			// Sinking ship:
			whiteness -= 0.05 * y/LENGTH;

			if (whiteness < 0)
				whiteness = 0;

			whiteness *= fadeOff;

			#ifdef PREVENT_HIDING_GLOW
				whiteness *= fadeOff;
			#endif

			//// Subjective leading glow
			// whiteness += 1.5 * energyHere * pow(breakingEdge,4);
			// whiteness += 0.5 * peakHeight[x] * breakingEdge * compensateForCurve;

			//// Free leading glow
			// whiteness += 0.3 * pow(breakingEdge,16);

			// Ambient:
			// whiteness += 0.05 * fadeOff;

			// whiteness *= pow(fadeOff, 1.0);

			// whiteness += 0.02;

			// whiteness = fmin(1.0,fmax(0.0,whiteness));

			// default spacing in both directions is 0.2 before scaling
			// original widths and lengths were 0.1 before scaling

#endif

			#define FLATTEN_TOPS_IN_Z ((modeCycle/12/6) & 2)
			if (FLATTEN_TOPS_IN_Z) {
				lastYheight = barHeight;
			}

			#define FLATTEN_TOPS_IN_X ((modeCycle/12/6) & 1)
#ifdef ANGLED_SURFACES_IN_X
			if (FLATTEN_TOPS_IN_X) {
				lastXheight = barHeight;
				lastXYheight = lastYheight;
			}
#endif

// DONE: For trailing edge issue: store last whiteness and use it if it exceeds current whiteness
#ifdef PREVENT_HIDING_GLOW

			if (y == LENGTH-1) {
				lastYwhiteness[x] = 0;
			}

			GLfloat useWhiteness;

			// TODO: TEXTURE and SYMMETRY could be #defined - texture is undesirable for low res

			////// Always merge with old (looks a bit textureless and darkens the lead)
			// useWhiteness = 0.5 * lastYwhiteness[x] + 0.5*whiteness;
			////// Use peak (symmetrical - that's what you wanted right?)
			useWhiteness = ( lastYwhiteness[x] > whiteness ? lastYwhiteness[x] : whiteness );
			if (!FLATTEN_TOPS_IN_X && !(modeCycle%4 == 0)) {
				#ifdef ANGLED_SURFACES_IN_X
					useWhiteness = ( lastXwhiteness > useWhiteness ? lastXwhiteness : useWhiteness );   // x-based
				#endif
			} else {
				//// Re-add texture (darkens lead but symmetrical)
				// useWhiteness = 0.5 * useWhiteness + 0.25*whiteness + 0.25*lastYwhiteness[x];
				//// Re-add texture - central glow, slightly darkened fadeoff.  Favourite!
				// useWhiteness = 0.5 * useWhiteness + 0.5*whiteness;
			}

	#define whiteness useWhiteness
#else
			// Just use whiteness normally :)
#endif

			if (barHeight >= 0.01 || useWhiteness >= 0.01) {

				#define SCALE_width 16.0/WIDTH
				#define SCALE_length SCALEBACK

				GLfloat shortSide,longSide;
				if (modeCycle%12 < 4) {
					/*
					// Isolated dots - dropped
					longSide  = 0.12;
					shortSide = 0.02;
					*/
					// Thin ribbons
					longSide  = 0.20;
					shortSide = 0.02;
				} else if (modeCycle%12 < 8) {
					// Medium ribbons
					longSide  = 0.20;
					shortSide = 0.10;
				} else {
					// Thick ribbons
					longSide  = 0.20;
					shortSide = 0.20;
				}

				// #define scaleBars (0.3 + 1.2*whiteness)
				// #define scaleBars (0.1 + 2.4*heights[y][x]*compensateForCurve*fadeOff)
				// #define scaleBars (0.1 + 2.4*peakEnergy[x]*fadeOff)
				#define scaleBars 2.5*(0.2 + 2.3*peakEnergy[x]*fadeOff*fadeOff*fadeOff*fadeOff)

				/*
				if ((modeCycle/12)%2 == 1) {
					shortSide *= scaleBars;
				}
				if ((modeCycle/12/2)%2 == 1) {
					longSide  *= scaleBars;
				}
				*/

				// TODO: If we are modifying bars in this way, we might want to modify centres also (at least in x axis)
				if ((modeCycle/12)%6 == 1) {
					shortSide *= scaleBars;
				}
				// Intelligent
				if ((modeCycle/12)%6 == 2 && y==0) {
					longSide  *= scaleBars;
				}
				// Not intelligent
				if ((modeCycle/12)%6 == 3) {
					longSide  *= scaleBars;
				}
				if ((modeCycle/12)%6 == 4) {
					shortSide *= scaleBars;
					if (y == 0) {
						longSide  *= scaleBars;
					}
				}
				// Not intelligent
				if ((modeCycle/12)%6 == 5) {
					shortSide *= scaleBars;
					longSide  *= scaleBars;
				}

				// Nicely spaced lines and rows of bars in both axes:
				if (modeCycle%2 > 0)
					draw_bar(x_offset, z_offset + 0.15*SCALE_length, barHeight, lastYheight, lastXheight, lastXYheight, hue, saturation, whiteness, longSide*SCALE_width, shortSide*SCALE_length);

				if (modeCycle%4 > 1)
					draw_bar(x_offset + 0.15*SCALE_width, z_offset, barHeight, lastYheight, lastXheight, lastXYheight, hue, saturation, whiteness, shortSide*SCALE_width, longSide*SCALE_length);

				// Crosses:
				/*
				if (modeCycle%2 > 0)
					draw_bar(x_offset - longSide*SCALE_width/2, z_offset - shortSide*SCALE_length/2, barHeight, barHeight, lastXheight, lastXYheight, hue, saturation, whiteness, longSide*SCALE_width, shortSide*SCALE_length);

				if (modeCycle%4 > 1)
					draw_bar(x_offset - shortSide*SCALE_width/2, z_offset - longSide*SCALE_length/2, barHeight, barHeight, lastXheight, lastXYheight, hue, saturation, whiteness, shortSide*SCALE_width, longSide*SCALE_length);
				*/

				if (modeCycle%4 == 0) {
					// Basic evenly-proportioned bars
					draw_bar(x_offset, z_offset, barHeight, barHeight, barHeight, barHeight, hue, saturation, whiteness, shortSide*SCALE_width, shortSide*SCALE_length);
					/*
					// Since neither of the above will fire, we just plot simple bars
					// We can't use longSide for both 4 and 8 because they are identical.
					if (modeCycle%12 == 0) {
						// Plot overlapping bars.  x&1 is a chequered offset to avoid flickering of intersecting surfaces.
						#define miniGap 0.001*((x+y)&1)
						draw_bar(x_offset + miniGap - 0.1*scaleBars*SCALE_width/2.0, z_offset + miniGap, barHeight, lastYheight, lastXheight, lastXYheight, hue, saturation, whiteness, scaleBars*0.25*SCALE_width, scaleBars*0.25*SCALE_length);
					} else {
						shortSide = ( modeCycle%12 == 4 ? 0.15 : 0.2 );
						draw_bar(x_offset, z_offset + 0.15*SCALE_length, barHeight, lastYheight, lastXheight, lastXYheight, hue, saturation, whiteness, shortSide*SCALE_width, shortSide*SCALE_length);
					}
					*/
				}

			}

			#ifdef lastXheight
				#undef lastXheight
				#undef lastXYheight
			#endif
			#ifdef ANGLED_SURFACES_IN_X
				lastXheight = barHeight;
				lastXYheight = lastYheight;
			#endif
			#undef barHeight

			#ifdef whiteness
				#undef whiteness
			#endif
			#ifdef ANGLED_SURFACES_IN_X
				lastXwhiteness = whiteness;
			#endif
			lastYwhiteness[x] = whiteness;

		}
	}
	glEnd();

	glPopMatrix();

	glXSwapBuffers(dpy,window);

#ifdef CHECK_FOR_KICK

	GLfloat energyOfCurrent = 0.0;

	for(x = 0; x < WIDTH; x++)
	{
		// Ignore mid-range, look for bass drum and snare.
		// We only use 5/8s of them spectrum, and we assume WIDTH=32!
		// TODO: We should probably really be ignoring if xscale[x] is in range 4-128
		if (x < WIDTH/8 || x>WIDTH*2/3) {
			// We were doing *compensateForCurve here but we shouldn't.
			// The bass covers only a few wavelengths/readings whereas the treble cover a lot.
			// Their amplitudes are naturally skewed to represent this.
			// Instead of compensateForCurve we use 1.5 instead!
			energyOfCurrent += heights[0][x] * 1.5 / ((float)WIDTH*5.0/8.0);
		}
	}

	if (energyOfCurrent > 0.2 && energyOfCurrent>recentPeak) {
		modeCycle++;
		recentPeak = 4.0 * energyOfCurrent;
	}

	// If it wasn't a beat, it may have been "noise".  Look for a leading edge!
	if (energyOfCurrent > recentPeak) {
		recentPeak = 1.5 * energyOfCurrent;
	}

	recentPeak *= 0.95;

#endif

}

static gint disable_func(gpointer data)
{
	oglspectrum_vp.disable_plugin(&oglspectrum_vp);
	return FALSE;
}


void *draw_thread_func(void *arg)
{
	Bool configured = FALSE;

	if ((window = create_window(WINWIDTH, WINHEIGHT)) == 0)
	{
		g_log(NULL, G_LOG_LEVEL_CRITICAL, __FILE__ ": unable to create window");
		pthread_exit(NULL);
	}
	
	XMapWindow(dpy, window);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1.5, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

#ifdef HAVE_SCHED_SETSCHEDULER
	if(xmms_check_realtime_priority())
	{
		struct sched_param sparam;
		sparam.sched_priority = sched_get_priority_max(SCHED_OTHER);
		pthread_setschedparam(pthread_self(), SCHED_OTHER, &sparam);		
	}
#endif
	
	while(going)
	{
		while(XPending(dpy))
		{
			XEvent event;
			KeySym keysym;
			char buf[16];
			
			XNextEvent(dpy, &event);
			switch(event.type)
			{
			case ConfigureNotify:
				glViewport(0,0,event.xconfigure.width, event.xconfigure.height);
				if(oglspectrum_cfg.tdfx_mode && !grabbed_pointer)
				{
					
					XGrabPointer(dpy, window,
						     True, ButtonPressMask,
						     GrabModeAsync,
						     GrabModeAsync,
						     window, None, CurrentTime);
					grabbed_pointer = TRUE;
				}
				configured = TRUE;
				break;
			case KeyPress:

				
				XLookupString (&event.xkey, buf, 16, &keysym, NULL);
				switch(keysym)
				{
				case XK_Escape:
					
					/* Ugly hack to get the disable_plugin call in the main thread. */
					GDK_THREADS_ENTER();
					gtk_idle_add(disable_func, NULL);
					GDK_THREADS_LEAVE();
					break;
				case XK_z:
					xmms_remote_playlist_prev(oglspectrum_vp.xmms_session);
					break;
				case XK_x:
					xmms_remote_play(oglspectrum_vp.xmms_session);
					break;
				case XK_c:
					xmms_remote_pause(oglspectrum_vp.xmms_session);
					break;
				case XK_v:
					xmms_remote_stop(oglspectrum_vp.xmms_session);
					break;
				case XK_b:
					xmms_remote_playlist_next(oglspectrum_vp.xmms_session);
					break;
				case XK_Up:					
					x_speed -= 0.1;
					// if(x_speed < -3.0)
						// x_speed = -3.0;
					break;
				case XK_Down:					
					x_speed += 0.1;
					// if(x_speed > 3.0)
						// x_speed = 3.0;
					break;
				case XK_Left:
					y_speed -= 0.1;
					// if(y_speed < -3.0)
						// y_speed = -3.0;
					
					break;
				case XK_Right:
					y_speed += 0.1;
					// if(y_speed > 3.0)
						// y_speed = 3.0;
					break;
				case XK_w:
					z_speed -= 0.1;
					// if(z_speed < -3.0)
						// z_speed = -3.0;
					break;
				case XK_q:
					z_speed += 0.1;
					// if(z_speed > 3.0)
						// z_speed = 3.0;
					break;
				case XK_Return:
					x_speed = 0.0;
					y_speed = 0.0;  // 0.5
					z_speed = 0.0;
					break;					
				case XK_equal:
					setup_camera();
					break;
				case XK_space:
					modeCycle++;
					break;
				case XK_BackSpace:
					modeCycle--;
					break;
				case XK_Shift_R:
					modeCycle += 4;
					break;
				case XK_Shift_L:
					modeCycle -= 4;
					break;
				}

				break;
			case ClientMessage:
				if ((Atom)event.xclient.data.l[0] == wm_delete_window_atom)
				{
					GDK_THREADS_ENTER();
					gtk_idle_add(disable_func, NULL);
					GDK_THREADS_LEAVE();
				}
				break;
			}
		}
		if(configured)
		{
			x_angle += x_speed;
			if(x_angle >= 360.0)
				x_angle -= 360.0;
			
			y_angle += y_speed;
			if(y_angle >= 360.0)
				y_angle -= 360.0;

			z_angle += z_speed;
			if(z_angle >= 360.0)
				z_angle -= 360.0;

// #define SPEED_DAMPENING 0.99

#ifdef SPEED_DAMPENING
			x_speed *= SPEED_DAMPENING;
			y_speed *= SPEED_DAMPENING;
			z_speed *= SPEED_DAMPENING;
#endif

			draw_bars();
		}
	}

	if (glxcontext)
	{
		glXMakeCurrent(dpy, 0, NULL);
		glXDestroyContext(dpy, glxcontext);
		glxcontext = NULL;
	}
	if (window)
	{
		if (grabbed_pointer)
		{
			XUngrabPointer(dpy, CurrentTime);
			grabbed_pointer = FALSE;
		}

		XDestroyWindow(dpy, window);
		window = 0;
	}

	pthread_exit(NULL);
}

static void start_display(void)
{
	int x, y;

	if(oglspectrum_cfg.tdfx_mode)
		putenv("MESA_GLX_FX=fullscreen");
	else
		putenv("MESA_GLX_FX=""");
	
	for(x = 0; x < WIDTH; x++)
	{
		for(y = 0; y < LENGTH; y++)
		{
			heights[y][x] = 0.0;
		}
	}
	scale = 1.0 / log(256.0);

	setup_camera();

	going = TRUE;
	pthread_create(&draw_thread, NULL, draw_thread_func, NULL);
}

static void stop_display(void)
{
	if (going)
	{
		going = FALSE;
		pthread_join(draw_thread, NULL);
	}

	if (colormap)
	{
		XFreeColormap(dpy, colormap);
		colormap = 0;
	}
	if (dpy)
	{
		XCloseDisplay(dpy);
		dpy = NULL;
	}
}

static void oglspectrum_init(void)
{
	if (dpy) return;

	oglspectrum_read_config();

	if(!oglspectrum_cfg.tdfx_mode)
		start_display();
}	

static void oglspectrum_cleanup(void)
{
	stop_display();
}

static void oglspectrum_playback_start(void)
{
	if(oglspectrum_cfg.tdfx_mode)
	{
		if(window)
			stop_display();

		start_display();
	}
}

static void oglspectrum_playback_stop(void)
{
	if(oglspectrum_cfg.tdfx_mode)
	{
		stop_display();
	}
}

static void oglspectrum_render_freq(gint16 data[2][256])
{
	gint i,c;
	gint y;
	GLfloat val;

#if NUM_BANDS <= 16
	gint xscale[] = {0, 1, 2, 3, 5, 7, 10, 14, 20, 28, 40, 54, 74, 101, 137, 187, 255};
#elif NUM_BANDS <= 32
	gint xscale[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 17, 20, 24, 28, 34, 40, 47, 54, 64, 74, 87, 101, 119, 137, 162, 187, 221, 255};
#else
	// #error We have not defined xscale for >32 bands!
	gint xscale[NUM_BANDS];
	for (i=0; i<NUM_BANDS; i++) {
		// xscale[i] = 255*i/(NUM_BANDS-1);
		xscale[i] = i;
	}
#endif

	// It occurs to me, if it was a dynamically allocated array, we could just
	// switch the rows, and not every cell in their contents!
	for(y = LENGTH-1; y > 0; y--) { for(i = 0; i < WIDTH; i++)
		{
			heights[y][i] = heights[y - 1][i];
		}
	}
	// But cell access is probably a lot faster in a static array, so gains in
	// switching rows would have to compete against losses for cell access,
	// which is probably larger and likely to grow in parallel.

	for(i = 0; i < NUM_BANDS; i++)
	{
		// Find the max data[0][c] in the range, store in y.
		for(c = xscale[i], y = 0; c < xscale[i + 1]; c++)
		{
			if(data[0][c] > y)
				y = data[0][c];
		}
		y >>= 7;
		if(y > 0)
			val = (log(y) * scale);
		else
			val = 0;

				
		heights[0][i] = val;
   
		
	}

// #define CFC compensateForCurveX
#define CFC(x) 1.0
	// BUG: Does not look right.
	//      Previously long streams appear too short.
	//      (Admittedly perhaps because they have been silenced by a peaking neighbour.)
#ifdef SHOW_PEAKS_ONLY
	int x;
	if (modeCycle < 128) {
		// Gah!  Since we are plotting *CFC, we must compare*CFC to be accurate.
		// But this is too extreme.
		for(x = 0; x < WIDTH-1; x++) {
			if (CFC(x)*heights[0][x] < CFC(x+1)*heights[0][x+1]) {
#ifdef BUILD_UP_PEAK_ENERGY
				// Don't throw away the energy.  Put some of it into the final peak value.
				heights[0][x+1] += heights[0][x]*0.5;
				heights[0][x] *= 0.5;
#else
				heights[0][x] = 0;
#endif
			}
		}
		for(x = WIDTH-1; x >= 1; x--) {
			if (CFC(x)*heights[0][x] < CFC(x-1)*heights[0][x-1]) {
#ifdef BUILD_UP_PEAK_ENERGY
				heights[0][x-1] += heights[0][x]*0.5;
				heights[0][x] *= 0.5;
#else
				heights[0][x] = 0;
#endif
			}
		}
	}
#endif

#ifdef HIDE_TROUGHS
	int x;
	if (modeCycle < 128) {
		// Gah!  Since we are plotting *CFC, we must compare*CFC to be accurate.
		// But this is too extreme.
		for(x = 1; x < WIDTH-1; x++) {
			if ( CFC(x)*heights[0][x] < CFC(x+1)*heights[0][x+1]
					&& CFC(x)*heights[0][x] < CFC(x-1)*heights[0][x-1]
			) {
				heights[0][x] = 0;
			}
		}
	}
#endif

}

