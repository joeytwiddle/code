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

/* NUM_BANDS should be either 32 or 16.  See declaration of xscale. */
// #define NUM_BANDS 16
#define NUM_BANDS 32

/* The original LENGTH was 16 */
#define LENGTH 128

#if LENGTH < 32
	#define SCALEBACK (16.0/LENGTH)
#else
	// After 32 we give up normalising the size, and we let the trail lengthen rather than compress:
	#define SCALEBACK (32.0/LENGTH)
#endif

#define WIDTH NUM_BANDS

OGLSpectrumConfig oglspectrum_cfg;

static Display *dpy = NULL;
static Colormap colormap = 0;
static GLXContext glxcontext = NULL;
static Window window = 0;
static GLfloat y_angle = 45.0, y_speed = 0.01; // 0.5;
static GLfloat x_angle = 20.0, x_speed = 0.0;
static GLfloat z_angle = 0.0, z_speed = 0.0;
static GLfloat heights[LENGTH][WIDTH], scale;
static gboolean going = FALSE, grabbed_pointer = FALSE;
static Atom wm_delete_window_atom;
static pthread_t draw_thread;

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

static void draw_bar(GLfloat x_offset, GLfloat z_offset, GLfloat height, GLfloat red, GLfloat green, GLfloat blue, float W )
{
	// default spacing in both directions is 0.2 before scaling
	// original widths and lengths were 0.1 before scaling

	GLfloat width = 0.10 * 15/(WIDTH-1);
	// GLfloat length = 0.1 * 16/LENGTH;
	GLfloat length = 0.10 * SCALEBACK;

	// float whiteness = red*red;
	// float notwhiteness = 1.0 - whiteness;

	//  red = 1.0*whiteness +  red*notwhiteness;
	// green = 1.0*whiteness + green*notwhiteness;
	// blue = 1.0*whiteness +  blue*notwhiteness;

	// float W = red*red;
	float NW = 1.0 - W;

	#define glColor3f_with_scale_then_whiteness(r,g,b,s) glColor3f(W+r*s*NW,W+g*s*NW,W+b*s*NW)

	// Flat (horizontal)
	// glColor3f(red,green,blue);
	glColor3f_with_scale_then_whiteness(red,green,blue,1.0);
	draw_rectangle(x_offset, height, z_offset, x_offset + width, height, z_offset + length);
	draw_rectangle(x_offset,      0, z_offset, x_offset + width,      0, z_offset + length);

	// In width plane
	// glColor3f(0.5 * red, 0.5 * green, 0.5 * blue);
	glColor3f_with_scale_then_whiteness(red,green,blue,0.5);
	draw_rectangle(x_offset, 0.0, z_offset + length, x_offset + width, height, z_offset + length);
	draw_rectangle(x_offset, 0.0, z_offset         , x_offset + width, height, z_offset         );
	/*
	*/

	// In depth plane
	// glColor3f(0.25 * red, 0.25 * green, 0.25 * blue);
	glColor3f_with_scale_then_whiteness(red,green,blue,0.25);
	draw_rectangle(x_offset        , 0.0, z_offset , x_offset        , height, z_offset + length);	
	draw_rectangle(x_offset + width, 0.0, z_offset , x_offset + width, height, z_offset + length);

	/*
	*/

	
}

static void draw_bars(void)
{
	gint x,y;
	GLfloat x_offset, z_offset, r_base, b_base;

	

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0,-0.5,-5.0);	      
	glRotatef(x_angle,1.0,0.0,0.0);
	glRotatef(y_angle,0.0,1.0,0.0);
	glRotatef(z_angle,0.0,0.0,1.0);

	glBegin(GL_TRIANGLES);
	for(y = 0; y < LENGTH; y++)
	{
		// z_offset = -1.6 + ((LENGTH - y)*SCALEBACK * 0.2);
		z_offset = +1.4 - ((y)*SCALEBACK * 0.2);

		// b_base = y * (1.0 / (LENGTH-1));
		b_base = (float)y / (float)(LENGTH-1);
		r_base = 1.0 - b_base;
			
		for(x = 0; x < WIDTH; x++)
		{
			x_offset = -1.6 + (x * 0.2*15.0/(WIDTH-1));
				
			draw_bar(x_offset, z_offset, heights[y][x], r_base - (x * (r_base / (WIDTH-1))), x * (1.0 / (WIDTH-1)), b_base, r_base*r_base*r_base*r_base*r_base);
		}
	}
	glEnd();

	glPopMatrix();

	glXSwapBuffers(dpy,window);
}

static gint disable_func(gpointer data)
{
	oglspectrum_vp.disable_plugin(&oglspectrum_vp);
	return FALSE;
}


void *draw_thread_func(void *arg)
{
	Bool configured = FALSE;

	if ((window = create_window(640, 480)) == 0)
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
					if(x_speed < -3.0)
						x_speed = -3.0;
					break;
				case XK_Down:					
					x_speed += 0.1;
					if(x_speed > 3.0)
						x_speed = 3.0;
					break;
				case XK_Left:
					y_speed += 0.1;
					if(y_speed < -3.0)
						y_speed = -3.0;
					
					break;
				case XK_Right:
					y_speed -= 0.1;
					if(y_speed > 3.0)
						y_speed = 3.0;
					break;
				case XK_w:
					z_speed -= 0.1;
					if(z_speed < -3.0)
						z_speed = -3.0;
					break;
				case XK_q:
					z_speed += 0.1;
					if(z_speed > 3.0)
						z_speed = 3.0;
					break;
				case XK_Return:
					x_speed = 0.0;
					y_speed = 0.5;
					z_speed = 0.0;
					x_angle = 20.0;
					y_angle = 45.0;
					z_angle = 0.0;
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

	x_speed = 0.0;
	y_speed = 0.5;
	z_speed = 0.0;
	x_angle = 20.0;
	y_angle = 45.0;
	z_angle = 0.0;

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
	gint xscale[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 20, 24, 28, 34, 40, 47, 54, 64, 74, 87, 101, 119, 137, 162, 187, 221, 255};
#else
  #warn "We have not defined xscale for >32 bands!"
#endif

	for(y = LENGTH-1; y > 0; y--)
	{
		for(i = 0; i < WIDTH; i++)
		{
			heights[y][i] = heights[y - 1][i];
		}
	}
	
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
}

