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
// #include <audacious/ui_main.h>
// #include <grand.h>

#include "logo.xpm"

// DONE dirtily: interpolation when bar is >= 2 pixels
// TODO: smooth (fade to smoke?) the tops of lines?  (May not be needed after interpolation.)
// TODO: flame is too boring on soft tracks, and maybe a little bit too excited on some crazy tracks
//       we don't want to fully normalise this, since the tracks do deserve it, we just want to reduce the inter-track variance
//       the spikes are desirable on very soft tracks
// TODO: unsmoothed tiny spikes are ok but unrealistic along the whole length of the flame
//       but well smoothed spikes are equally unrealistic.  we need a mixture!
//       maybe vary the smoothing as we cross the flame, creating some regions of spikiness, others smooth.

/* WIDTH should be kept 256, this is the hardwired resolution of the
   spectrum given by XMMS */
#define WIDTH 256

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

/* Parameters and functions for fire colouring. */
#define WINWIDTH 550
// #define INTERPOLATE_CHEAP
#define INTERPOLATE_SMOOTH
//// XSCALE() Converts WINWIDTH to WIDTH (maps window x onto bar_heights[]):
// #define XSCALE(i) (int)(i*(float)WIDTH/(float)WINWIDTH)
// #define XSCALE(i) (int)(i*(float)WIDTH/(float)WINWIDTH*0.7)
#define XSCALE(i) (int)((float)WIDTH*dropEnds(doLog((float)(i)/(float)WINWIDTH)))
//// Modify the x scale?
#define doLog(x) (x)
// #define doLog(x) pow(x,1.2)
#define dropEnds(f) (f*0.7)
// #define dropEnds(f) (f)
// #define dropEnds(f) (0.2+0.6*(float)(f))

static GtkWidget *window = NULL,*area;
static GdkPixmap *bg_pixmap = NULL, *draw_pixmap = NULL, *bar = NULL;
static GdkGC *gc = NULL;
static gint16 bar_heights[WIDTH];
/*static gint timeout_tag;*/
static gdouble scale, x00, y00;
static gdouble heatNow;

static void fsanalyzer_init(void);
static void fsanalyzer_cleanup(void);
static void fsanalyzer_playback_start(void);
static void fsanalyzer_playback_stop(void);
static void fsanalyzer_render_freq(gint16 data[2][256]);

VisPlugin fsanalyzer_vp = {
	.description = "Fiery Spectrum Analyzer",
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

static int max(int a,int b) {
	return ( a>b ? a : b );
}

static int min(int a,int b) {
	return ( a<b ? a : b );
}

static float fclamp(float val, float min, float max) {
	return ( val<min ? min : val>max ? max : val );
}

static void fsanalyzer_init(void) {
	GdkColor palette[5];
	GdkColor color;
	int i;
	int wx,wy;
	// GtkWindow *parent;
	GdkWindow *parent;

	if(window)
		return;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), _("Spectrum Analyzer"));
	gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, FALSE);
	// NEW! Joey's stuff:
	// gtk_window_set_resizable(GTK_WINDOW(window), TRUE); // worked ok, but isn't desirable until we support it!
	gtk_window_set_decorated(GTK_WINDOW(window), FALSE); // failed, the window still has a titlebar+frame.
	/*
	//// TODO:
	//// I wanted to set the initial position of the fire spectrum window to HEIGHT pixels above the main window.
	//// But I could not work out how to get the main window!
	//// Ah well I finally managed to get my window manager to remember it's position, so this is less important now.  I hope other users can achieve it that way too.
	// parent = GTK_WINDOW(window)->parent;
	// parent = gtk_widget_get_parent_window(GTK_WINDOW(window));
	// parent = gtk_window_get_transient_for(GTK_WINDOW(window));
	GdkScreen* screen = gtk_window_get_screen(GTK_WINDOW(window));
	GList* glist = gdk_screen_get_toplevel_windows(GDK_SCREEN(screen));
	parent = (GtkWindow*)g_list_first(glist);
	parent = glist->data;
	// parent = g_list_next(glist)->data;
	// parent = gdk_get_default_root_window();
	// parent = gtk_widget_get_parent_window(GTK_WIDGET(mainwin));
	if (parent==0) {
		g_printf("Failed to get parent window!\n");
	}
	// g_printf("fsanalyzer_vp.mainwin = %d\n",fsanalyzer_vp.mainwin);
	// } else {
		// gtk_window_get_position(GTK_WINDOW(parent), &wx, &wy);
		// gdk_window_get_position(GDK_WINDOW(parent), &wx, &wy);
		gdk_window_get_origin(GDK_WINDOW(parent), &wx, &wy);
		g_printf("got x=%d y=%d\n",wx,wy);
		gtk_window_move(GTK_WINDOW(window), wx, wy-HEIGHT);
	// }
	*/
	gtk_widget_realize(window);

	bg_pixmap = gdk_pixmap_create_from_xpm_d(window->window,NULL,NULL,logo_xpm);
	gdk_window_set_back_pixmap(window->window,bg_pixmap,0);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(fsanalyzer_destroy_cb),NULL);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_widget_destroyed), &window);
	gtk_widget_set_size_request(GTK_WIDGET(window), WINWIDTH, HEIGHT);
	gc = gdk_gc_new(window->window);
	draw_pixmap = gdk_pixmap_new(window->window,WINWIDTH,HEIGHT,gdk_rgb_get_visual()->depth);

	// Our new bar pixel buffer is 3x as tall as the target window.
	bar = gdk_pixmap_new(window->window,25, HEIGHT*3, gdk_rgb_get_visual()->depth);

	//// Red and orange flame
	#define stages 5
	// A hint of blue in the bright "white" makes it even brighter.  Although my eyes cannot see the blue, they actually notice a red stripe where yellow meets white.
	// palette[0].red = 0xFF44; palette[0].green = 0xFF44; palette[0].blue = 0xFFFF;
	palette[0].red = 0xFF77; palette[0].green = 0xFF77; palette[0].blue = 0xCCCC;
	palette[1].red = 0xFF77; palette[1].green = 0xEEEE; palette[1].blue = 0x4444;
	palette[2].red = 0xFF77; palette[2].green = 0xBBBB; palette[2].blue = 0x0000;
	palette[3].red = 0xCCCC; palette[3].green = 0x6666; palette[3].blue = 0x0000;
	palette[4].red = 0x2222; palette[4].green = 0x0088; palette[4].blue = 0x0000;
	// The alternative to increasing MINCOL:
	#define palDelta 0.3
	// Unfortunately, now that we are using the whole range, we do not get the bright white candle areas!
	// This makes the last 0.3 of the palette static!
	#define palScale 0.9

	/*
	//// Blue flame
	//// This palette may prefer MINCOL = HEIGHT*0.4, EXPLOSION=1.0, with a lower LOOKAHEAD.
	#define stages 4
	palette[0].red = 0xFFFF; palette[0].green = 0xFFFF; palette[0].blue = 0xEEEE;
	palette[1].red = 0x0000; palette[1].green = 0x8888; palette[1].blue = 0xCCCC;
	// palette[2].red = 0x0000; palette[2].green = 0x4444; palette[2].blue = 0x8888;
	palette[2].red = 0x0077; palette[2].green = 0x2222; palette[2].blue = 0x7777;
	palette[3].red = 0x0000; palette[3].green = 0x0000; palette[3].blue = 0x1111;
	#define palDelta 0.4
	*/

	/*
	//// blue-red-yellow-white - doesn't quite look right, especially the purple!
	#define stages 5
	palette[0].red = 0xFFFF; palette[0].green = 0xFFFF; palette[0].blue = 0xFFFF;
	palette[1].red = 0xEEEE; palette[1].green = 0xBBBB; palette[1].blue = 0x0000;
	palette[2].red = 0xBBBB; palette[2].green = 0x3333; palette[2].blue = 0x0000;
	palette[3].red = 0x5555; palette[3].green = 0x0000; palette[3].blue = 0x0000;
	palette[4].red = 0x0000; palette[4].green = 0x0000; palette[4].blue = 0x2222;
	#define palDelta 0.15
	*/

	if (1>0) {

	for(i = 0; i < 3*HEIGHT; i++) {
		float thruouter,thruinner;
		int pfrom,pto;
		//// palDelta is the proportion of the start of the palette which we drop - the rest is scaled to fit.
		//// This is because we usually don't really want very much of the first colour, just the end of its transition to the next.
		thruouter = palDelta + (1.0-palDelta)*(float)(i+1 - HEIGHT)/(float)HEIGHT / palScale; // the +1 because rather thruouter==1 than ==0!
		// if (thruouter<0) thruouter=0;
		// if (thruouter>1) thruouter=1;
		if (thruouter<=0) {
			// TODO: allocate this colour only once
			color.red = palette[stages-1].red;
			color.green = palette[stages-1].green;
			color.blue = palette[stages-1].blue;
		} else if (thruouter>=1.0) {
			color.red = palette[0].red;
			color.green = palette[0].green;
			color.blue = palette[0].blue;
		} else {
			thruouter = 1.0 - thruouter;
			thruinner = thruouter*(stages-1) - (int)(thruouter*(stages-1));
			pfrom = thruouter * (stages-1);
			pto = pfrom + 1;
			color.red = palette[pfrom].red + thruinner*(float)(palette[pto].red - palette[pfrom].red);
			color.green = palette[pfrom].green + thruinner*(float)(palette[pto].green - palette[pfrom].green);
			color.blue = palette[pfrom].blue + thruinner*(float)(palette[pto].blue - palette[pfrom].blue);
		}
		gdk_color_alloc(gdk_colormap_get_system(),&color);
		gdk_gc_set_foreground(gc,&color);
		gdk_draw_line(bar,gc,0,i,24,i);
	}

	} else {

	// The first HEIGHT pixels are "above" the flame.  They are rendered when the top of the flame is darker than 0!
	// color.red = 0xFFFF/3; // TODO: If we are gonna have extra red, keep fading it.
	// color.blue = 0xFFFF/*/3*/; // TODO: This should only be for TESTING!
	for(i = 0; i < HEIGHT; i++) {
		// color.red = 0xFFFF/3 * i/HEIGHT;
		// color.green = color.red/3;
		color.red = 0xFFFF*0.2;
		color.green = (0xDDDD-0xFFFF/3)*0.2*0.2;
		color.blue = 0x0000;
		// color.blue = color.red;   color.red = 0;
		/*
		//// Produce a yellow "lick" above the flame.  Did not work well.
		float t = HEIGHT-i;
		if (t<8) {
			color.red = 0xFFFF/3 + 0xFFFF/3*t/8;   color.green = color.red*t/8;
		}
		*/
		gdk_color_alloc(gdk_colormap_get_system(),&color);
		gdk_gc_set_foreground(gc,&color);
		gdk_draw_line(bar,gc,0,i,24,i);
	}
	// i = 0;
	// gdk_draw_line(bar,gc,0,i,0,HEIGHT-1);

	// The second HEIGHT pixels are the fire, ranging from dark to light.  We could consider increasing the size of this part.
	for(i = 0; i < HEIGHT; i++) {
		float thruouter,thruinner;
		thruouter = 1.0 - (float)i/(float)HEIGHT;
		thruinner = thruouter*5.0 - (int)(thruouter*5.0);
		if (thruinner == 0)
			thruinner = 1.0;
		thruinner *= 0xFFFF;
		if (thruouter<0.2) {
			//// bunsen blue -> white -> yellow -> red
			// color.red = 0xbbbb+thruinner/4;
			// color.green = 0xbbbb+thruinner/4;
			// color.blue = 0xFFFF;
			//// do 1st step of white -> yellow
			color.red = 0xFFFF;
			color.green = 0xEEEE - thruinner/32;
			color.blue = 0xEEEE - thruinner*7/16;
		} else if (thruouter<0.4) {
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
			color.green = 0xEEEE - 0x1111/2 - thruinner/32;
			color.blue = 0xFFFF/2 - thruinner/2;
		} else if (thruouter<0.6) {
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
			// yellow -> red:
			color.red = 0xFFFF;
			// color.green = 0xDDDD - thruinner/3;
			color.green = 0xDDDD - thruinner/3;
			color.blue = 0;
			/*
		} else if (thruouter<0.8) {
			color.red = 0xFFFF;
			color.green = max(0xDDDD - 0xFFFF/3 - thruinner/3,0);
			// color.green = color.red/3;
			color.blue = 0;
		} else {
			// color.red = 0xFFFF;
			// color.green = 0xFFFF - thruinner;
			// color.blue = 0;
			//
			//
			//
			// color.red = 0xFFFF - thruinner*0.75; // go down to 25% red, not black
			// color.green = 0;
			color.red = 0xFFFF - thruinner*2/3; // go down to 33% red, not black
			color.green = max(0xDDDD - 0xFFFF*2/3 - thruinner/3,0);
			// color.green = color.red/3;
			color.blue = 0;
			*/
		} else {
			thruinner = (thruouter-0.6)/0.4;
			#define loss (1.0-thruinner*0.8)
			// color.red = 0xFFFF*(1.0-thruouter*0.5);
			color.red = 0xFFFF*loss;
			color.green = (0xDDDD - 0xFFFF/3)*loss*loss;
			color.blue = 0;
		}
		gdk_color_alloc(gdk_colormap_get_system(),&color);
		gdk_gc_set_foreground(gc,&color);
		gdk_draw_line(bar,gc,0,HEIGHT+i,24,HEIGHT+i);
	}

	// The third HEIGHT pixels are full fire, max colour no change.
	color.red = 0xFFFF;
	color.green = 0xEEEE;
	color.blue = 0xEEEE;
	gdk_color_alloc(gdk_colormap_get_system(),&color);
	gdk_gc_set_foreground(gc,&color);
	for(i = 0; i < HEIGHT; i++) {
		gdk_draw_line(bar,gc,0,2*HEIGHT+i,24,2*HEIGHT+i);
	}

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

	heatNow = 0.0;

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

static gint draw_func(gpointer data) {
	gint i;
	gdouble heatHere;
	gint lasty;

	/* FIXME: should allow spare redrawing like the vis. in the main window */
	if(!window) {
/*		timeout_tag = 0;*/
		return FALSE;
	}

	GDK_THREADS_ENTER();
	gdk_draw_rectangle(draw_pixmap, gc, TRUE, 0, 0, WINWIDTH, HEIGHT);

	// heatHere = HEIGHT/4;
	// heatHere = (bar_heights[0] + bar_heights[4] + bar_heights[8] + bar_heights[12]) / 4;
	heatHere = ( // cheeky initial "average" from the first half of the spectrum
			+ bar_heights[WIDTH*0/16] + bar_heights[WIDTH*1/16] + bar_heights[WIDTH*2/16] + bar_heights[WIDTH*3/16]
			+ bar_heights[WIDTH*4/16] + bar_heights[WIDTH*5/16] + bar_heights[WIDTH*6/16] + bar_heights[WIDTH*7/16]
		) / 8;
	heatNow = heatNow*0.99 + (-HEIGHT/32+1.5*heatHere)*0.01; // around 3 seconds to update
	// heatHere = 0;
	// heatHere = HEIGHT/16; // When using true heatHere mean method
	lasty = HEIGHT-1;
	for(i = 0; i < WINWIDTH; i++) {

		// Black vertical stripes:
		// if ((i%8) == 0)
			// continue;

		int y,cy;
		// y = max(0.0,HEIGHT-1 - bar_heights[XSCALE(i)] - 3);
		// this clip >=0 is done later, so not really needed here.
		y = HEIGHT-1 - bar_heights[XSCALE(i)] - 3;
		// The extra -3 gives a constant 3-pixel significant flat blob of fire at the bottom, so that the fire never disappears or flickers down to 1 pixel.
		// Use 0 if you don't mind the fire disappearing.
		// Hmm even with this, we still get a 1-pixel height flicker at very quiet parts.

		#ifdef INTERPOLATE_CHEAP
		//// Cheap interpolation trick:
		// y = 0.2*y + 0.8*lasty; // smoother
		// y = 0.3*y + 0.7*lasty; // smoother
		y = 0.5*y + 0.5*lasty; // Finer+sharper.  It keeps the details but also hides the bars (at the current width settings).
		lasty = y;
		#endif
		#ifdef INTERPOLATE_SMOOTH
		// Quite fun to play with.
		// This is actually no more expensive.  It is tuned to produce a smoother result.
		// It can move surprisingly like real fire with a suitable song.  (e.g. Cold Storage - Onyx.)
		// lasty = lasty*0.9 + y*0.1;
		// The downside is that it drops information, the user can't see the details of the spectrum.
		// It works well on spectra with fractal/busy/wiggling peaks, but only
		// flattens curves further if they were already gentle, in which case we
		// prefer the details.
		// lasty = lasty*0.93 + y*0.07; // Too smooth
		// lasty = lasty*0.8 + y*0.2;
		//// Vary spikiness according to (lack of) heat.
		float spikiness;
		// spikiness = 0.2 + 0.65*abs(fclamp(heatHere/(float)HEIGHT,0,1)-0.5)*2.0;
		//// This smooths the high flames.  But it doesn't look good.  :P
		// spikiness = 0.6 - fclamp(1.2*heatHere/(float)HEIGHT,0,0.5);
		// spikiness = 0.6 - fclamp(2.0*heatHere/(float)HEIGHT,0,0.5);
		// Good value range: Min: 0.2, Max: 0.5
		// spikiness = 0.2; // Min recommended. Offers a little smoothing, but doesn't lose too much information.
		// spikiness = 0.45; // Seems ugly
		spikiness = 0.5; // Keep spike information (any higher and you end up showing the flat bars!)
		// I think I prefer full spikiness because it gives more visual information, so I can see the gentler frequencies between the strong ones.
		/*
		spikiness += 0.01*g_random_double()-0.5;
		spikiness = 0.99*spikiness + 0.01*0.25;
		*/
		// But damn the spikes are good, we should never smooth more than 50% of them.
		y = lasty*(1.0-spikiness) + y*spikiness;
		lasty = y;
		#endif
		//// TODO:
		//// Attempts to fiddle around with interpolation have a nasty habit of exhibiting artefacts resulting from the face that the input is a set of bars (not interpolated yet).
		//// Again we are using the dirty lop-sided averaging, and this time with heights not colours.
		//// Maybe better to calculate the true mean here.

		//// Update heatHere:

		//// This is a cheap way to approximate the heatHere mean, but it produces good results (localised and spread):
		//// If you increase LOOKAHEAD, you should also reduce GAIN accordingly, to calibrate phase on the x-axis.
		// #define LOOKAHEAD 12
		// #define GAIN 0.01
		// #define LOOKAHEAD 10
		// #define GAIN 0.02
		// #define LOOKAHEAD 8
		// #define GAIN 0.03
		// #define LOOKAHEAD 8
		// #define GAIN 0.04
		#define LOOKAHEAD 7
		#define GAIN 0.05
		// #define LOOKAHEAD 6
		// #define GAIN 0.05
		// #define LOOKAHEAD 5
		// #define GAIN 0.07
		// #define LOOKAHEAD 1
		// #define GAIN 0.10
		if (i+LOOKAHEAD<WINWIDTH)
			heatHere = heatHere*(1.0-GAIN) + GAIN*(float)bar_heights[XSCALE(i+LOOKAHEAD)];
		// CONSIDER: Occasionally (with strong contrast colours like blue and cyan) you can actually see
		// that the bar_heights[] have flat tops over i=n..n+2.  We could fix this by interpolating like we did with y.
		// #define MINCOL (HEIGHT/3)
		// #define MINCOL (HEIGHT/4)
		// #define MINCOL (HEIGHT/12)
		// #define MINCOL (HEIGHT/16)
		#define MINCOL 0
		// #define MINCOL (HEIGHT*0.4)
		// #define MINCOL (HEIGHT/7)
		// #define EXPLOSION 1.1
		// #define EXPLOSION 1.2
		#define EXPLOSION 1.3

		/*
		// This is a more accurate way to calculate the heatHere mean, but the results are not so good visually!
		// BUG: Sometimes rising spikes were getting their tops clipped by some rising curve.  Maybe cy was < HEIGHT ?
		// #define LOOKAHEAD 64
		#define LOOKAHEAD 10
		//// Doing them independently causes fadeouts at the edges.  Maybe a good thing, making the outer flames cooler.
		// if (i+LOOKAHEAD < WINWIDTH)
			// heatHere += (float)bar_heights[XSCALE(i+LOOKAHEAD)] / (LOOKAHEAD*2+1);
		// if (i-LOOKAHEAD > =0)
			// heatHere -= (float)bar_heights[XSCALE(i-LOOKAHEAD)] / (LOOKAHEAD*2+1);
		//// Doing them together is more correct:
		//// I don't know why DELTA is needed.  It's a horizontal shift.
		#define DELTA 8
		if (i+LOOKAHEAD<WINWIDTH && i-LOOKAHEAD-DELTA>=0) {
			heatHere += (float)bar_heights[XSCALE(i+LOOKAHEAD)] / (LOOKAHEAD*2+1+DELTA);
			heatHere -= (float)bar_heights[XSCALE(i-LOOKAHEAD-DELTA)] / (LOOKAHEAD*2+1+DELTA);
		}
		//// Very gently, move the global mean towards current state:
		//// Without this, our average average is dependent on the initial value of heatHere.
		// #define MINCOL (HEIGHT/4)
		// if (i+LOOKAHEAD<WINWIDTH)
			// heatHere = heatHere*0.999 + 0.001*(float)bar_heights[XSCALE(i+LOOKAHEAD)];
		#define MINCOL (HEIGHT/3)
		#define EXPLOSION 0.8
		*/

		//// Choose the colour intensity of this flame:

		/*
		if (bar_heights[XSCALE(i)]<HEIGHT/2)
			cy = HEIGHT*0.4 + 0.4*bar_heights[XSCALE(i)];
		else
			cy = max(1,HEIGHT*1.0 - 1.2*bar_heights[XSCALE(i)]);
		*/

		// cy = (cy + heatHere/2) / 2;
		// if (cy > HEIGHT*0.75)
			// cy = HEIGHT*0.75;

		// cy = 1;

		/*
		if (heatHere<HEIGHT/2)
			cy = heatHere * 0.3;
		else
			cy = (HEIGHT-heatHere) * 0.3;
		*/

		// heatHere = 64;

		cy = HEIGHT + MINCOL + heatHere*EXPLOSION - (HEIGHT-y);
		// cy = HEIGHT + MINCOL + (0.75*heatHere+0.25*heatNow)*EXPLOSION - (HEIGHT-y);
		// cy = HEIGHT + MINCOL + heatNow*EXPLOSION - (HEIGHT-y);
		//// heatNow varies at a gentle rate over time
		//// It was intended to stabilise the overall brightness when the spectrum is bouncing up and down rapidly.
		//// But it doesn't work, we haven't eliminated the bounce, only dampened it.
		//// We should be trying to normalise the average targetCol?
		//// Or with a better buffer, we could copy a stretch bar to fix the lower col.
		//// As it was, this acted too strongly on phat spectrums, and flattened the desirable colour spikes (could be fixed by tweaking other values).

		// cy = y;

		// cy = y * heatHere/HEIGHT;

		//// We should not need to check any bounds now that we are using 3 buffers.
		/*
		//// This actually manipulated the spectrum (the height of the flame) to fix the colours:
		if (cy < 1) {
			y += (1 - cy);
			cy = 1;
		}
		if (cy-2*HEIGHT > y) {
			y += (cy-2*HEIGHT - y);
			// cy = y;
		}
		*/
		/*
		// if (cy > y)
			// cy = y;
		//// I think printf's were causing segfaults!
		if (cy < HEIGHT+1) {
			// fprintf("Warning: cy = %i < HEIGHT !\n",&cy);
			cy = HEIGHT+1;
		}
		*/
		// if (cy < 1) {
			// // fprintf("Warning: cy = %i < 1 !\n",&cy);
			// cy = 1;
		// }

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
