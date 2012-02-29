/*  Spectrum Analyzer Visualization Plugin for Audacious
 *
 *  Copyright (C) 2006 William Pitcock
 *  Copyright (C) 1998-2001 V�gv�lgyi Attila, Peter Alm, Mikael Alm,
 *    Olle Hallnas, Thomas Nilsson and 4Front Technologies
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

#include "config.h"

#include <gtk/gtk.h>
#include <math.h>
#include <audacious/plugin.h>
#include <audacious/i18n.h>

#include "logo.xpm"

/* WIDTH should be kept 256, this is the hardwired resolution of the
   spectrum given by XMMS */
#define WIDTH 256
#define WINWIDTH 550

// #define XSCALE(i) (int)(i*(float)WIDTH/(float)WINWIDTH)
// #define XSCALE(i) (int)(i*(float)WIDTH/(float)WINWIDTH*0.7)
#define XSCALE(i) (int)((float)WIDTH*dropEnds(doLog((float)i/(float)WINWIDTH)))
#define doLog(x) (x)
// #define doLog(x) pow(x,1.2)
#define dropEnds(f) (f*0.7)
// #define dropEnds(f) (f)
// #define dropEnds(f) (0.2+0.6*(float)(f))

/* HEIGHT can be modified at your pleasure */
#define HEIGHT 128

/* Linearity of the amplitude scale (0.5 for linear, keep in [0.1, 0.9]) */
#define d 0.33

/* Time factor of the band dinamics. 3 means that the coefficient of the
   last value is half of the current one's. (see source) */
#define tau 3

/* Factor used for the diffusion. 4 means that half of the height is
   added to the neighbouring bars */
#define dif 4

static GtkWidget *window = NULL,*area;
static GdkPixmap *bg_pixmap = NULL, *draw_pixmap = NULL, *bar = NULL;
static GdkGC *gc = NULL;
static gint16 bar_heights[WIDTH];
/*static gint timeout_tag;*/
static gdouble scale, x00, y00;

static void fsanalyzer_init(void);
static void fsanalyzer_cleanup(void);
static void fsanalyzer_playback_start(void);
static void fsanalyzer_playback_stop(void);
static void fsanalyzer_render_freq(gint16 data[2][256]);

VisPlugin fsanalyzer_vp = {
	.description = "Spectrum Analyzer",
	.num_pcm_chs_wanted = 0,
	.num_freq_chs_wanted = 1,
	.init = fsanalyzer_init, /* init */
	.cleanup = fsanalyzer_cleanup, /* cleanup */
	.playback_start = fsanalyzer_playback_start, /* playback_start */
	.playback_stop = fsanalyzer_playback_stop, /* playback_stop */
	.render_freq = fsanalyzer_render_freq  /* render_freq */
};

VisPlugin *spectrum_vplist[] = { &fsanalyzer_vp, NULL };

DECLARE_PLUGIN(spectrum, NULL, NULL, NULL, NULL, NULL, NULL, spectrum_vplist,NULL);

static void fsanalyzer_destroy_cb(GtkWidget *w,gpointer data) {
	fsanalyzer_vp.disable_plugin(&fsanalyzer_vp);
}

static void fsanalyzer_init(void) {
	GdkColor color;
	int i;

	if(window)
		return;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), _("Spectrum Analyzer"));
	gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, FALSE);
	gtk_widget_realize(window);
	bg_pixmap = gdk_pixmap_create_from_xpm_d(window->window,NULL,NULL,logo_xpm);
	gdk_window_set_back_pixmap(window->window,bg_pixmap,0);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(fsanalyzer_destroy_cb),NULL);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_widget_destroyed), &window);
	gtk_widget_set_size_request(GTK_WIDGET(window), WINWIDTH, HEIGHT);
	gc = gdk_gc_new(window->window);
	draw_pixmap = gdk_pixmap_new(window->window,WINWIDTH,HEIGHT,gdk_rgb_get_visual()->depth);

	bar = gdk_pixmap_new(window->window,25, HEIGHT, gdk_rgb_get_visual()->depth);

	for(i = 0; i < HEIGHT; i++) {
		float thruouter,thruinner;
		thruouter = 1.0 - (float)i/(float)HEIGHT;
		thruinner = thruouter*4.0 - (int)(thruouter*4.0);
		thruinner *= 0xFFFF;
		if (thruouter<0.25) {
			//// bunsen blue -> white -> yellow -> red
			// color.red = 0xbbbb+thruinner/4;
			// color.green = 0xbbbb+thruinner/4;
			// color.blue = 0xFFFF;
			//// do 1st step of white -> yellow
			color.red = 0xFFFF;
			color.green = 0xFFFF;
			color.blue = 0xFFFF - thruinner/2;
		} else if (thruouter<0.5) {
			//// blue -> white -> yellow -> red
			// color.red = thruinner;
			// color.green = thruinner;
			// color.blue = 0xFFFF;
			//// blue -> black -> yellow -> red
			// color.red = 0;
			// color.green = 0;
			// color.blue = 0xFFFF - thruinner;
			//// yellow -> red -> darkred
			// color.red = 0xFFFF;
			// color.green = 0xFFFF - thruinner;
			// color.blue = 0;
			//// bunsen blue -> white -> yellow -> red
			// color.red = 0xFFFF;
			// color.green = 0xFFFF;
			// color.blue = 0xFFFF - thruinner;
			//// do 2nd step of white -> yellow
			color.red = 0xFFFF;
			color.green = 0xFFFF;
			color.blue = 0xFFFF/2 - thruinner/2;
		} else if (thruouter<0.75) {
			// color.red = 0xFFFF;
			// color.green = 0xFFFF;
			// color.blue = 0xFFFF - thruinner;
			//
			//
			//
			// color.red = thruinner;
			// color.green = thruinner;
			// color.blue = 0;
			// color.red = 0xFFFF - thruinner/2;
			// color.green = 0;
			// color.blue = 0;
			color.red = 0xFFFF;
			color.green = 0xFFFF - thruinner;
			color.blue = 0;
		} else {
			// color.red = 0xFFFF;
			// color.green = 0xFFFF - thruinner;
			// color.blue = 0;
			//
			//
			//
			color.red = 0xFFFF - thruinner*0.75; // go down to 25% red, not black
			color.green = 0;
			color.blue = 0;
		}
		gdk_color_alloc(gdk_colormap_get_system(),&color);
		gdk_gc_set_foreground(gc,&color);
		gdk_draw_line(bar,gc,0,i,24,i);
	}

	/*
	for(i = 0; i < HEIGHT / 2; i++) {
		color.red = 0xFFFF;
		color.green = ((i * 255) / (HEIGHT / 2)) << 8;
		color.blue = 0;

		gdk_color_alloc(gdk_colormap_get_system(),&color);
		gdk_gc_set_foreground(gc,&color);
		gdk_draw_line(bar,gc,0,i,24,i);
	}
	for(i = 0; i < HEIGHT / 2; i++) {
		color.red = (255 - ((i * 255) / (HEIGHT / 2))) <<8;
		color.green = 0xFFFF;
		color.blue = 0;
		// color.blue = (127*i*2/HEIGHT) <<8;
		// color.blue = (255*i*2/HEIGHT) <<8;

		gdk_color_alloc(gdk_colormap_get_system(),&color);
		gdk_gc_set_foreground(gc,&color);
		gdk_draw_line(bar,gc,0,i + (HEIGHT / 2),24,i + (HEIGHT / 2));
	}
	*/

	scale = 1.0 * HEIGHT / ( log((1 - d) / d) * 2 );
	x00 = d*d*32768.0/(2 * d - 1);
	y00 = -log(-x00) * scale;

/* d=0.2, HEIGHT=128
	scale = 46.1662413084;
	x00 = -2184.53333333;
	y00 = -354.979500941;
*/

	gdk_color_black(gdk_colormap_get_system(),&color);
	gdk_gc_set_foreground(gc,&color);

	area = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window),area);
	gtk_widget_realize(area);
	gdk_window_set_back_pixmap(area->window,bg_pixmap,0);

	gtk_widget_show(area);
	gtk_widget_show(window);
	gdk_window_clear(window->window);
	gdk_window_clear(area->window);
}

static void fsanalyzer_cleanup(void) {
	if(window) {
		gtk_widget_destroy(window);
	}
	if(gc) {
		gdk_gc_unref(gc);
		gc = NULL;
	}
	if(bg_pixmap) {
		gdk_pixmap_unref(bg_pixmap);
		bg_pixmap = NULL;
	}
	if(draw_pixmap) {
		gdk_pixmap_unref(draw_pixmap);
		draw_pixmap = NULL;
	}
	if(bar) {
		gdk_pixmap_unref(bar);
		bar = NULL;
	}
}

static int max(int a,int b) {
	return ( a>b ? a : b );
}

static int min(int a,int b) {
	return ( a<b ? a : b );
}

static gint draw_func(gpointer data) {
	gint i;
	float local;

	/* FIXME: should allow spare redrawing like the vis. in the main window */
	if(!window) {
/*		timeout_tag = 0;*/
		return FALSE;
	}

	GDK_THREADS_ENTER();
	gdk_draw_rectangle(draw_pixmap, gc, TRUE, 0, 0, WINWIDTH, HEIGHT);

	local = 0;
	for(i = 0; i < WINWIDTH; i++) {
		// gdk_draw_pixmap(draw_pixmap, gc, bar, 0, HEIGHT-1-bar_heights[XSCALE(i)], i, HEIGHT-1-bar_heights[XSCALE(i)], 1, bar_heights[XSCALE(i)]);
		// gdk_draw_pixmap(draw_pixmap, gc, bar, 0, HEIGHT-1-bar_heights[XSCALE(i)], i, HEIGHT-1-bar_heights[XSCALE(i)], 1, bar_heights[XSCALE(i)]);
		// gdk_draw_pixmap(draw_pixmap, gc, bar, 0, 1+bar_heights[XSCALE(i)]/4, i, HEIGHT-1-bar_heights[XSCALE(i)], 1, bar_heights[XSCALE(i)]);
		// gdk_draw_pixmap(draw_pixmap, gc, bar, 0, 1, i, HEIGHT-1-bar_heights[XSCALE(i)], 1, bar_heights[XSCALE(i)]);
		// gdk_draw_pixmap(draw_pixmap, gc, bar, 0, max(0,0.8*(HEIGHT-1-bar_heights[XSCALE(i)])), i, HEIGHT-1-bar_heights[XSCALE(i)], 1, min(HEIGHT,bar_heights[XSCALE(i)]));
		// gdk_draw_pixmap(draw_pixmap, gc, bar, 0, 0.75*(HEIGHT-1-bar_heights[XSCALE(i)]), i, HEIGHT-1-bar_heights[XSCALE(i)], 1, bar_heights[XSCALE(i)]);
		// gdk_draw_pixmap(draw_pixmap, gc, bar, 0, max(0.0,0.7*(HEIGHT-1-bar_heights[XSCALE(i)])), i, max(0.0,HEIGHT-1-bar_heights[XSCALE(i)]), 1, min(HEIGHT-1,bar_heights[XSCALE(i)]));
		// gdk_draw_pixmap(draw_pixmap, gc, bar, 0, max(1,HEIGHT*0.4-0.2*bar_heights[XSCALE(i)]), i, max(0.0,HEIGHT-1-bar_heights[XSCALE(i)]), 1, min(HEIGHT-1,bar_heights[XSCALE(i)]));
		int y,cy;
		y = max(0.0,HEIGHT-1-bar_heights[XSCALE(i)]);
		if (bar_heights[XSCALE(i)]<HEIGHT/2)
			cy = HEIGHT*0.35 + 0.15*bar_heights[XSCALE(i)];
		else
			cy = max(1,HEIGHT*1.0 - 1.2*bar_heights[XSCALE(i)]);
		/* cy = local/2;
		if (cy<0)
			cy=0;
		if (cy>HEIGHT*0.75)
			cy=HEIGHT*0.75;
		local = local*0.9 + 0.1*(float)bar_heights[XSCALE(i)]; */
		gdk_draw_pixmap(draw_pixmap, gc, bar, 0, cy, i, y, 1, HEIGHT-y-1);
	}

	gdk_window_clear(area->window);
	GDK_THREADS_LEAVE();

	return TRUE;
}

static void fsanalyzer_playback_start(void) {
	if(window) {
		gdk_window_set_back_pixmap(area->window,draw_pixmap,0);
		gdk_window_clear(area->window);
	}
}


static void fsanalyzer_playback_stop(void) {
	if(GTK_WIDGET_REALIZED(area)) {
		gdk_window_set_back_pixmap(area->window,bg_pixmap,0);
		gdk_window_clear(area->window);
	}
}

static void fsanalyzer_render_freq(gint16 data[2][256]) {
	gint i;
	gdouble y;

	if(!window)
		return;

	/* FIXME: can anything taken out of the main thread? */
	for (i = 0; i < WIDTH; i++) {
		y = (gdouble)data[0][i] * (i + 1); /* Compensating the energy */
		y = ( log(y - x00) * scale + y00 ); /* Logarithmic amplitude */

		y = ( (dif-2)*y + /* FIXME: conditionals should be rolled out of the loop */
			(i==0       ? y : bar_heights[i-1]) +
			(i==WIDTH-1 ? y : bar_heights[i+1])) / dif; /* Add some diffusion */
		y = ((tau-1)*bar_heights[i] + y) / tau; /* Add some dynamics */
		bar_heights[i] = (gint16)y;
	}
	draw_func(NULL);
	return;
}
