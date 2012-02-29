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

#define XMMS

#include "config.h"

#include <gtk/gtk.h>
#include <math.h>

#ifdef AUDACIOUS
#include <audacious/plugin.h>
#include <audacious/i18n.h>
#include "logo.xpm"
#endif

#ifdef XMMS
#include "xmms/plugin.h"
#include "libxmms/util.h"
#include "xmms_logo.xpm"
#include "xmms/i18n.h"
#define logo_xpm sanalyzer_xmms_logo_xpm
#endif

/* This isn't just pretty.  It is still a spectrum analyser.  It retains the
 * same values as the original plugin, adding colour. */

// DONE dirtily: interpolation when bar is >= 2 pixels
// TODO: smooth (fade to smoke?) the tops of lines?  (May not be needed after interpolation.)
// TODO: flame is too boring on soft tracks, and maybe a little bit too excited on some crazy tracks
//       we don't want to fully normalise this, since the tracks do deserve it, we just want to reduce the inter-track variance
//       the spikes are desirable on very soft tracks
// TODO: unsmoothed tiny spikes are ok but unrealistic along the whole length of the flame
//       but well smoothed spikes are equally unrealistic.  we need a mixture!
//       maybe vary the smoothing as we cross the flame, creating some regions of spikiness, others smooth.
//       But if there is very little going on, we really want to display any spikes that do exist at full resolution
// TODO: make palette, and window size, user configurable

/* SPECWIDTH should be kept 256, this is the hardwired resolution of the
   spectrum given by XMMS. */
#define SPECWIDTH 256

/* FLAMEHEIGHT scales the height of the flames, and the colour buffer.
   I have seen flames actually reach 1.7*FLAMEHEIGHT pixels in height.
   If you increase FLAMEHEIGHT without increasing WINHEIGHT, the tops of noisy
   flames may get clipped. */
#define FLAMEHEIGHT 128
// #define FLAMEHEIGHT (WINHEIGHT/1.75)

/* Width 550 fits nicely over a double-size amp.  274 over normal size amp.
   TODO: Make this user configurable, either in preferences or by resizing the
   window manually, so they can place the flame over a single size-amp, or
   elsewhere on their desktop at their desired width.  If we do this, we should
   also disable ORGANIC_INTERPOLATION because it fails at high-res (until we
   add interpolation to it!  We should do something because hi-res looks too
   ... interpolated. ;P)
   If we increase WINWIDTH we should probably increase LOOKAHEAD also, and the
   way heatHere is calculated. */
// #define WINWIDTH 256
// #define WINWIDTH 274
#define WINWIDTH 550
// #define WINWIDTH 1024
// #define WINWIDTH 1200

/* Height 224 should prevent clipping of the tops of flames when FLAMEHEIGHT=128.
   TODO: Users may wish to change the window's height.  If they do I'm guessing
   they would like us to scale FLAMEHEIGHT for them also. */
#define WINHEIGHT 224

/* Linearity of the amplitude scale (0.5 for linear, keep in [0.1, 0.9]) */
#define d 0.33

/* Time factor of the band dinamics. 3 means that the coefficient of the
   last value is half of the current one's. (see source) */
#define tau 3

/* Factor used for the diffusion. 4 means that half of the height is
   added to the neighbouring bars */
#define dif 4

/* Parameters and functions for fire colouring. */
//// I actually prefer the non-true interpolation because it feels more organic, but it does not work at higher resolutions.  (At low-res it puts a slight curve on the flames, but as high-res it exhibits the flat graph underneath, unless we interpolate that input.)
#define ORGANIC_INTERPOLATION
//// XSCALE() Converts WINWIDTH to SPECWIDTH (maps window x onto bar_heights[]):
// #define XSCALE(i) (int)(i*(float)SPECWIDTH/(float)WINWIDTH)
// #define XSCALE(i) (int)(i*(float)SPECWIDTH/(float)WINWIDTH*0.7)
#define XSCALE(i) (int)((float)SPECWIDTH*dropEnds(doLog((float)(i)/(float)WINWIDTH)))
//// Modify the x scale?
#define doLog(x) (x)
// #define doLog(x) pow(x,1.2)
#define dropEnds(f) (f*0.7)
// #define dropEnds(f) (f)
// #define dropEnds(f) (0.2+0.6*(float)(f))

static GtkWidget *window = NULL,*area;
static GdkPixmap *bg_pixmap = NULL, *draw_pixmap = NULL, *bar = NULL;
static GdkGC *gc = NULL;
static gint16 bar_heights[SPECWIDTH];
/*static gint timeout_tag;*/
static gdouble scale, x00, y00;
static gdouble heatNow;

#ifdef XMMS
#define fsanalyzer_vp              sanalyzer_vp
#define fsanalyzer_init            sanalyzer_init
#define fsanalyzer_cleanup         sanalyzer_cleanup
#define fsanalyzer_playback_start  sanalyzer_playback_start
#define fsanalyzer_playback_stop   sanalyzer_playback_stop
#define fsanalyzer_render_freq     sanalyzer_render_freq
#define fsanalyzer_destroy_cb      sanalyzer_destroy_cb
#endif

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

#ifdef XMMS
VisPlugin *get_vplugin_info(void)
{
	sanalyzer_vp.description =
		g_strdup_printf(_("Fiery spectrum analyzer %s"), VERSION);
	return &sanalyzer_vp;
}
#endif

VisPlugin *spectrum_vplist[] = { &fsanalyzer_vp, NULL };

#ifdef AUDACIOUS
DECLARE_PLUGIN(spectrum, NULL, NULL, NULL, NULL, NULL, NULL, spectrum_vplist,NULL);
#endif

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
	// gtk_window_set_decorated(GTK_WINDOW(window), FALSE); // failed, the window still has a titlebar+frame.
	/*
	//// TODO:
	//// I wanted to set the initial position of the fire spectrum window to WINHEIGHT pixels above the main window.
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
		gtk_window_move(GTK_WINDOW(window), wx, wy-WINHEIGHT);
	// }
	*/
	gtk_widget_realize(window);

	bg_pixmap = gdk_pixmap_create_from_xpm_d(window->window,NULL,NULL,logo_xpm);
	gdk_window_set_back_pixmap(window->window,bg_pixmap,0);
#ifdef XMMS
	gtk_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(sanalyzer_destroy_cb),NULL);
	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(gtk_widget_destroyed), &window);
	gtk_widget_set_usize(window, WINWIDTH, WINHEIGHT);
#else
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(fsanalyzer_destroy_cb),NULL);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_widget_destroyed), &window);
	gtk_widget_set_size_request(GTK_WIDGET(window), WINWIDTH, WINHEIGHT);
#endif
	gc = gdk_gc_new(window->window);
	draw_pixmap = gdk_pixmap_new(window->window,WINWIDTH,WINHEIGHT,gdk_rgb_get_visual()->depth);

	// Our new bar pixel buffer is 3.5x as tall as the target window.
   // The first FLAMEHEIGHT pixels are black, and probably no longer accessed.
   // The second FLAMEHEIGHT pixels are our flame palette.
   // The third FLAMEHEIGHT*1.5 pixels are buffer pixels beyond the end of the
   // palette, when the flame is full white.
	bar = gdk_pixmap_new(window->window,25, FLAMEHEIGHT*3.5, gdk_rgb_get_visual()->depth);

	//// Red and orange flame
	#define stages 5
	// A hint of blue in the bright "white" makes it even brighter.  Although my eyes cannot see the blue, they actually notice a red stripe where yellow meets white.
	// palette[0].red = 0xFF44; palette[0].green = 0xFF44; palette[0].blue = 0xFFFF;
	palette[0].red = 0xFF77; palette[0].green = 0xFF77; palette[0].blue = 0xCCCC;
	palette[1].red = 0xFF77; palette[1].green = 0xEEEE; palette[1].blue = 0x4444;
	palette[2].red = 0xFF77; palette[2].green = 0xBBBB; palette[2].blue = 0x0000;
	palette[3].red = 0xDDDD; palette[3].green = 0x5555; palette[3].blue = 0x0000;
	palette[4].red = 0x8888; palette[4].green = 0x0888; palette[4].blue = 0x0000;
	// We want a lick of red, then orange quickly moving to a strong yellow
	// But I think I have the scales wrong, I always have a significant band of dark orange.
	// The alternative to increasing MINCOL:
	#define palDelta 0.1
	// Unfortunately, now that we are using the whole range, we do not get the bright white candle areas!
	// This makes the last 0.3 of the palette static!
	#define palScale 0.9

	/*
	//// Blue flame
	//// This palette may prefer MINCOL = FLAMEHEIGHT*0.4, EXPLOSION=1.0, with a lower LOOKAHEAD.
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

	for(i = 0; i < 3.5*FLAMEHEIGHT; i++) {
		float thruouter,thruinner;
		int pfrom,pto;
		//// palDelta is the proportion of the start of the palette which we drop - the rest is scaled to fit.
		//// This is because we usually don't really want very much of the first colour, just the end of its transition to the next.
		thruouter = palDelta + (1.0-palDelta)*(float)(i+1 - FLAMEHEIGHT)/(float)FLAMEHEIGHT / palScale; // the +1 because rather thruouter==1 than ==0!
		// if (thruouter<0) thruouter=0;
		// if (thruouter>1) thruouter=1;
		if (thruouter<=0) {
			// TODO: allocate these repeated colours only once - or will X handle this gracefully anyway?
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

	scale = FLAMEHEIGHT / ( log((1 - d) / d) * 2 );
	x00 = d*d*32768.0/(2 * d - 1);
	y00 = -log(-x00) * scale;

/* d=0.2, FLAMEHEIGHT=128
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
	gdk_draw_rectangle(draw_pixmap, gc, TRUE, 0, 0, WINWIDTH, WINHEIGHT);

	// heatHere = FLAMEHEIGHT/4;
	// heatHere = (bar_heights[0] + bar_heights[4] + bar_heights[8] + bar_heights[12]) / 4;
	heatHere = ( // cheeky initial "average" from the first half of the spectrum
			+ bar_heights[SPECWIDTH*0/16] + bar_heights[SPECWIDTH*1/16] + bar_heights[SPECWIDTH*2/16] + bar_heights[SPECWIDTH*3/16]
			+ bar_heights[SPECWIDTH*4/16] + bar_heights[SPECWIDTH*5/16] + bar_heights[SPECWIDTH*6/16] + bar_heights[SPECWIDTH*7/16]
		) / 8;
	heatNow = heatNow*0.99 + (-FLAMEHEIGHT/32+1.5*heatHere)*0.01; // around 3 seconds to update
	// heatHere = 0;
	// heatHere = FLAMEHEIGHT/16; // When using true heatHere mean method
	lasty = WINHEIGHT-1;
	for(i = 0; i < WINWIDTH; i++) {

		// Black vertical stripes:
		// if ((i%8) == 0)
			// continue;

		int y,cy;
		// y = max(0.0,WINHEIGHT-1 - bar_heights[XSCALE(i)] - 3);
		// this clip >=0 is done later, so not really needed here.
		y = WINHEIGHT-1 - bar_heights[XSCALE(i)] - 3;
		// The extra -3 gives a constant 3-pixel significant flat blob of fire at the bottom, so that the fire never disappears or flickers down to 1 pixel.
		// Use 0 if you don't mind the fire disappearing.
		// Hmm even with this, we still get a 1-pixel height flicker at very quiet parts.

		#ifdef ORGANIC_INTERPOLATION

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
		// spikiness = 0.2 + 0.65*abs(fclamp(heatHere/(float)FLAMEHEIGHT,0,1)-0.5)*2.0;
		//// This smooths the high flames.  But it doesn't look good.  :P
		// spikiness = 0.6 - fclamp(1.2*heatHere/(float)FLAMEHEIGHT,0,0.5);
		// spikiness = 0.6 - fclamp(2.0*heatHere/(float)FLAMEHEIGHT,0,0.5);
		// Good value range: Min: 0.2, Max: 0.5
		// spikiness = 0.2; // Min recommended. Offers a little smoothing, but doesn't lose too much information.
		// spikiness = 0.45; // Seems ugly
		spikiness = 0.5; // Keep spike information (any higher and you end up showing the flat bars!)
		// I think I prefer full spikiness because it gives more visual information, so I can see the gentler frequencies between the strong ones.
		/*
		spikiness += 0.01*g_random_double()-0.5;
		spikiness = 0.99*spikiness + 0.01*0.25;
		*/

		/*
		// Normalise in case of different window width
		spikiness /= ((float)WINWIDTH/550.0);
		spikiness /= ((float)WINWIDTH/550.0); // Square it in fact.
		spikiness /= ((float)WINWIDTH/550.0); // Still no good =/ cube it :P
		// those attempts failed
		*/

		y = lasty*(1.0-spikiness) + y*spikiness;
		lasty = y;
		//// TODO:
		//// Attempts to fiddle around with interpolation have a nasty habit of exhibiting artefacts resulting from the face that the input is a set of bars (not interpolated yet).
		//// Again we are using the dirty lop-sided averaging, and this time with heights not colours.
		//// Maybe better to calculate the true mean here.

		#else

		// True interpolation (albeit inefficient :P )
		int left = (int)((float)SPECWIDTH*0.7*(float)(i)/(float)WINWIDTH);
		int right = left + 1;
		float thru = ((float)SPECWIDTH*0.7*(float)(i)/(float)WINWIDTH) - left;
		float yleft = WINHEIGHT-1 - bar_heights[left] - 3;
		float yright = WINHEIGHT-1 - bar_heights[right] - 3;
		y = yleft*(1.0-thru) + yright*thru;

		#endif

		//// Update heatHere:

		//// This is a cheap way to approximate the heatHere mean, but it produces good results (localised and spread):
		//// If you increase LOOKAHEAD, you should also reduce GAIN accordingly, to calibrate phase on the x-axis.
		#define LOOKAHEAD 12
		#define GAIN 0.01
		// #define LOOKAHEAD 10
		// #define GAIN 0.02
		// #define LOOKAHEAD 8
		// #define GAIN 0.03
		// #define LOOKAHEAD 6
		// #define GAIN 0.04
		// #define LOOKAHEAD 6
		// #define GAIN 0.05
		// #define LOOKAHEAD 3
		// #define GAIN 0.07
		// #define LOOKAHEAD 1
		// #define GAIN 0.10
		if (i+LOOKAHEAD<WINWIDTH)
			heatHere = heatHere*(1.0-GAIN) + GAIN*(float)bar_heights[XSCALE(i+LOOKAHEAD)];
		// CONSIDER: Occasionally (with strong contrast colours like blue and cyan) you can actually see
		// that the bar_heights[] have flat tops over i=n..n+2.  We could fix this by interpolating like we did with y.
		// #define MINCOL (FLAMEHEIGHT/3)
		// #define MINCOL (FLAMEHEIGHT/4)
		// #define MINCOL (FLAMEHEIGHT/12)
		// #define MINCOL (FLAMEHEIGHT/16)
		#define MINCOL 0
		// #define MINCOL (FLAMEHEIGHT*0.4)
		// #define MINCOL (FLAMEHEIGHT/7)
		// #define EXPLOSION 1.1
		// #define EXPLOSION 1.2
		// #define EXPLOSION 1.3
		#define EXPLOSION 1.4
		// #define EXPLOSION 1.5

		/*
		// This is a more accurate way to calculate the heatHere mean, but the results are not so good visually!
		// BUG: Sometimes rising spikes were getting their tops clipped by some rising curve.  Maybe cy was < FLAMEHEIGHT ?
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
		// #define MINCOL (FLAMEHEIGHT/4)
		// if (i+LOOKAHEAD<WINWIDTH)
			// heatHere = heatHere*0.999 + 0.001*(float)bar_heights[XSCALE(i+LOOKAHEAD)];
		#define MINCOL (FLAMEHEIGHT/3)
		#define EXPLOSION 0.8
		*/

		//// Choose the colour intensity of this flame:

		/*
		if (bar_heights[XSCALE(i)]<FLAMEHEIGHT/2)
			cy = FLAMEHEIGHT*0.4 + 0.4*bar_heights[XSCALE(i)];
		else
			cy = max(1,FLAMEHEIGHT*1.0 - 1.2*bar_heights[XSCALE(i)]);
		*/

		// cy = (cy + heatHere/2) / 2;
		// if (cy > FLAMEHEIGHT*0.75)
			// cy = FLAMEHEIGHT*0.75;

		// cy = 1;

		/*
		if (heatHere<FLAMEHEIGHT/2)
			cy = heatHere * 0.3;
		else
			cy = (FLAMEHEIGHT-heatHere) * 0.3;
		*/

		// heatHere = 64;

		cy = FLAMEHEIGHT - 32 + y/4 /*MINCOL*/ + heatHere*EXPLOSION/4;
		// cy = FLAMEHEIGHT + MINCOL + (0.75*heatHere+0.25*heatNow)*EXPLOSION - (WINHEIGHT-y);
		// cy = FLAMEHEIGHT + MINCOL + heatNow*EXPLOSION - (WINHEIGHT-y);
		//// heatNow varies at a gentle rate over time
		//// It was intended to stabilise the overall brightness when the spectrum is bouncing up and down rapidly.
		//// But it doesn't work, we haven't eliminated the bounce, only dampened it.
		//// We should be trying to normalise the average targetCol?
		//// Or with a better buffer, we could copy a stretch bar to fix the lower col.
		//// As it was, this acted too strongly on phat spectrums, and flattened the desirable colour spikes (could be fixed by tweaking other values).

		// cy = y;

		// cy = y * heatHere/FLAMEHEIGHT;

		//// We should not need to check any bounds now that we are using 3 buffers.
		/*
		//// This actually manipulated the spectrum (the height of the flame) to fix the colours:
		if (cy < 1) {
			y += (1 - cy);
			cy = 1;
		}
		if (cy-2*FLAMEHEIGHT > y) {
			y += (cy-2*FLAMEHEIGHT - y);
			// cy = y;
		}
		*/
		/*
		// if (cy > y)
			// cy = y;
		//// I think printf's were causing segfaults!
		if (cy < FLAMEHEIGHT+1) {
			// fprintf("Warning: cy = %i < FLAMEHEIGHT !\n",&cy);
			cy = FLAMEHEIGHT+1;
		}
		*/
		// if (cy < 1) {
			// // fprintf("Warning: cy = %i < 1 !\n",&cy);
			// cy = 1;
		// }

		gdk_draw_pixmap(draw_pixmap, gc, bar, 0, cy, i, y, 1, WINHEIGHT-y-1);

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
	for (i = 0; i < SPECWIDTH; i++) {
		y = (gdouble)data[0][i] * (i + 1); /* Compensating the energy */
		y = ( log(y - x00) * scale + y00 ); /* Logarithmic amplitude */

		y = ( (dif-2)*y + /* FIXME: conditionals should be rolled out of the loop */
			(i==0       ? y : bar_heights[i-1]) +
			(i==SPECWIDTH-1 ? y : bar_heights[i+1])) / dif; /* Add some diffusion */
		y = ((tau-1)*bar_heights[i] + y) / tau; /* Add some dynamics */
		bar_heights[i] = (gint16)y;
	}
	draw_func(NULL);
	return;
}
