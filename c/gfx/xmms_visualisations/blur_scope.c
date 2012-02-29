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
 *  w
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "config.h"

#include <gtk/gtk.h>
#include <string.h>
#include "xmms/plugin.h"
#include "libxmms/util.h"
#include "libxmms/configfile.h"
#include "xmms_logo.xpm"
#include "blur_scope.h"
#include "xmms/i18n.h"

static GtkWidget *window = NULL,*area;
static GdkPixmap *bg_pixmap = NULL;
static gboolean config_read = FALSE;

static void bscope_init(void);
static void bscope_cleanup(void);
static void bscope_playback_stop(void);
static void bscope_render_pcm(gint16 data[2][512]);

BlurScopeConfig bscope_cfg;

VisPlugin bscope_vp =
{
	NULL,
	NULL,
	0, /* XMMS Session ID, filled in by XMMS */
	NULL, /* description */
	1, /* Number of PCM channels wanted */
	0, /* Number of freq channels wanted */
	bscope_init, /* init */
	bscope_cleanup, /* cleanup */
	NULL, /* about */
	bscope_configure, /* configure */
	NULL, /* disable_plugin */
	NULL, /* playback_start */
	bscope_playback_stop, /* playback_stop */
	bscope_render_pcm, /* render_pcm */
	NULL  /* render_freq */
};

VisPlugin *get_vplugin_info(void)
{
	bscope_vp.description = g_strdup_printf(_("Blur Scope %s"), VERSION);
	return &bscope_vp;
}

// TODO BUG: bottom line of window does not decay properly.  Test with /stuff/would_like/mp3s/kahvi_new/ftp.scene.org/pub/music/groups/kahvicollective/kahvi101f_vizion-itistheskythatiscrying.ogg

// #define WIDTH 256
// #define WIDTH 640
// #define WIDTH 720
#define WIDTH 800
// #define WIDTH 960
// #define WIDTH 1024
#define HEIGHT 128
#define min(x,y) ((x)<(y)?(x):(y))
#define BPL	((WIDTH + 2))
// #define DECAY_RATE 8
#define DECAY_RATE 7
// #define SKIP_FRAMES 2
// The human eye may see many white lines even when only 1 is renderered, due to the high framerate.  SKIP_FRAMES can make only 1 white line visible, but the oscilloscope will also appear more flickery / less smooth.

#ifdef SKIP_FRAMES
static gint frameCount;
#endif

static guchar rgb_buf[(WIDTH + 2) * (HEIGHT + 2)];
static guchar rgb_buf2[(WIDTH + 2) * (HEIGHT + 2)];
static guchar *target_rgb_buf;
static guchar *other_rgb_buf;
static guchar *temp_rgb_buf;
static GdkRgbCmap *cmap = NULL; 
	
static void inline draw_pixel_8(guchar *buffer,gint x, gint y, guchar c)
{
	buffer[((y + 1) * BPL) + (x + 1)] = c;
}


void bscope_read_config(void)
{
	ConfigFile *cfg;
	gchar *filename;

	if(!config_read)
	{
		// bscope_cfg.color = 0xFF0000;  // red
		// bscope_cfg.color = 0xFF3F7F;  // pink
		// bscope_cfg.color = 0x00BFFF;  // cyan lightning
		bscope_cfg.color = 0x008FFF;  // electric cyan
		// bscope_cfg.color = 0x007FFF;  // electric blue
		filename = g_strconcat(g_get_home_dir(), "/.xmms/config", NULL);
		cfg = xmms_cfg_open_file(filename);
		
		if (cfg)
		{
			xmms_cfg_read_int(cfg, "BlurScope", "color", &bscope_cfg.color);
			xmms_cfg_free(cfg);
		}
		g_free(filename);
		config_read = TRUE;
	}
}


// #define fadeRate 0.9961
// #define blurTao 0.9961
// #define fadeRate 0.97
// #define blurTao 0.94
#define fadeRate 0.94
#define blurTao 0.8
#define blurTao2 0.8

// #ifndef I386_ASSEM
void bscope_blur_8_no_asm(guchar *srcptr, guchar *ptr,gint w, gint h, gint bpl)
{
	register guint i,sum;
	register guchar *iptr;
	register guchar *optr;
	
	iptr = srcptr + bpl + 1;
	optr = ptr + bpl + 1;
	i = bpl * h;
	while(i--)
	{
		// Blurring:
		sum = (iptr[-bpl] + iptr[-1] + iptr[1] + iptr[bpl]) >> 2;  // Simple 4-neighbour blur
		// #define BLUR_DIST 3
		// sum = (iptr[-BLUR_DIST*bpl] + iptr[-BLUR_DIST] + iptr[BLUR_DIST] + iptr[BLUR_DIST*bpl]) >> 2;  // Simple 4-neighbour blur
		// sum = iptr[0]; // Do not blur

		// if (sum > 0)
			// sum = sum * 0.90;
		/*
		if(sum > DECAY_RATE)
			sum -= DECAY_RATE;
		else
			sum = 0;
		*/
// #define max(a,b) (a>b?a:b)
		// sum = max(max(iptr[-bpl],iptr[bpl]),max(iptr[-1],iptr[+1])) * blurTao2;

		// Retain self with blurTao:
		// if (iptr[0] > sum)
		if (sum > iptr[0])
			sum = sum*blurTao + iptr[0]*(1.0-blurTao);
		else
			sum = iptr[0];

		if (i < bpl) {
			sum = sum / 4; // Fix for non-decaying bottom line
			if (sum > 0)
				sum -= 1;
		}

		/*
		// I made the intensities decay non-linearly.  This could alternatively
		// be achieved by using a linear decay over a non-linear cmap.
		if (sum <= 0)
			sum = 0;
		// else if (sum > 64)
			// sum = sum - 8; // Fast initial decay
		// else
			// sum = sum - 5; // Slow overall decay
		else if (sum > DECAY_RATE)
			sum = sum - DECAY_RATE; // Slow overall decay
		else
			sum = 0;
		*/

		if (sum > 32)
			sum = sum * fadeRate;
		else
			if (sum > 0)
				sum--;

		// else if (sum > 16)
			// sum = sum - 0; // Slow middle decay (in fact blur only)
		// else
			// sum = sum - 1; // Final fixed decay

		// At low intensity slow down the decay as much as possible
		// else if (sum==2 || sum==4 || sum==8 || sum==16 || sum==32 || sum==64)
			// sum = sum - 1;
		// else
			// sum = sum - 0;
		// else
			// sum = sum - 1;

		// @requires gint sum;
		// if (sum < 0)
			// sum = 0;

		// sum = 0; // Immediate total decay!  We only see the last plot.

		// sum = iptr[0];  if (sum > DECAY_RATE*4) { sum -= DECAY_RATE*4; } else { sum = 0; } // Rapid decay without blurring

		*(optr) = sum;
		iptr++;
		optr++;
	}
	
	
}
// #else
// extern void bscope_blur_8(guchar *ptr,gint w, gint h, gint bpl);
// #endif

void generate_cmap(void)
{
	guint32 colors[256],i,red,blue,green;
	if(window)
	{
		red = (guint32)(bscope_cfg.color / 0x10000);
		green = (guint32)((bscope_cfg.color % 0x10000)/0x100);
		blue = (guint32)(bscope_cfg.color % 0x100);
		for(i = 255; i > 0; i--)
		{
			if (i == 255)
				colors[i] = 0xFFFFFF;
			else
				colors[i] = (((guint32)(i*red/256) << 16) | ((guint32)(i*green/256) << 8) | ((guint32)(i*blue/256)));
		}
		colors[0]=0;
		if(cmap)
		{
			gdk_rgb_cmap_free(cmap);
		}
		cmap = gdk_rgb_cmap_new(colors,256);
	}
}

static void bscope_destroy_cb(GtkWidget *w,gpointer data)
{
	bscope_vp.disable_plugin(&bscope_vp);
}

static void bscope_init(void)
{
	if(window)
		return;
	bscope_read_config();

	window = gtk_window_new(GTK_WINDOW_DIALOG);
	gtk_window_set_title(GTK_WINDOW(window),_("Blur scope"));
	gtk_window_set_policy(GTK_WINDOW(window), FALSE, FALSE, FALSE);
	gtk_widget_realize(window);
	bg_pixmap = gdk_pixmap_create_from_xpm_d(window->window,NULL,NULL,bscope_xmms_logo_xpm);
	gdk_window_set_back_pixmap(window->window,bg_pixmap,0);
	gtk_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(bscope_destroy_cb),NULL);
	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(gtk_widget_destroyed), &window);
	gtk_widget_set_usize(window, WIDTH, HEIGHT);
	
	area = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window),area);
	gtk_widget_realize(area);
	gdk_window_set_back_pixmap(area->window,bg_pixmap,0);
	generate_cmap();
	memset(rgb_buf,0,(WIDTH + 2) * (HEIGHT + 2));
	memset(rgb_buf2,0,(WIDTH + 2) * (HEIGHT + 2));
	target_rgb_buf = rgb_buf;
	other_rgb_buf = rgb_buf2;
		
	gtk_widget_show(area);
	gtk_widget_show(window);
	gdk_window_clear(window->window);
	gdk_window_clear(area->window);
}

static void bscope_cleanup(void)
{
	if(window)
		gtk_widget_destroy(window);
	if(bg_pixmap)
	{
		gdk_pixmap_unref(bg_pixmap);
		bg_pixmap = NULL;
	}
	if(cmap)
	{
		gdk_rgb_cmap_free(cmap);
		cmap = NULL;
	}
}

static void bscope_playback_stop(void)
{
	if(GTK_WIDGET_REALIZED(area))
		gdk_window_clear(area->window);
}

static inline void draw_vert_line(guchar *buffer, gint x, gint y1, gint y2)
{
	int y;
	if(y1 < y2)
	{
		for(y = y1; y <= y2; y++)
		{
			draw_pixel_8(buffer,x,y,0xFF);
			draw_pixel_8(buffer,x+1,y,0xFF);
		}
	}
	else if(y2 < y1)
	{
		for(y = y2; y <= y1; y++)
		{
			draw_pixel_8(buffer,x,y,0xFF);
			draw_pixel_8(buffer,x+1,y,0xFF);
		}
	}
	else
	{
		draw_pixel_8(buffer,x,y1,0xFF);
		draw_pixel_8(buffer,x+1,y1,0xFF);
		draw_pixel_8(buffer,x,y1+1,0xFF);
		draw_pixel_8(buffer,x+1,y1+1,0xFF);
	}
}

static void bscope_render_pcm(gint16 data[2][512])
{
	gint i,y, prev_y;
	
	if(!window)
		return;

#ifdef SKIP_FRAMES
	frameCount++;
	if (frameCount%SKIP_FRAMES > 0) {
		return;
	}
#endif

	bscope_blur_8_no_asm(other_rgb_buf,target_rgb_buf, WIDTH, HEIGHT, BPL);
	prev_y = y = (HEIGHT / 2) + (data[0][0] >> 9);
	for(i = 0; i < WIDTH; i++)
	{
		y = (HEIGHT / 2) + (data[0][i >> 1] >> 9);
		// Since we are reading only 1 sample for 2 pixels, we interpolate the second pixel:
		if (i%2 == 1)
			y = (y + (HEIGHT / 2) + (data[0][(i+2) >> 1] >> 9))/2;
		// However, that does not produce an even volume spread for the display
		// (unless we add anti-aliasing).
		if(y < 0)
			y = 0;
		if(y >= HEIGHT)
			y = HEIGHT - 1;
		draw_vert_line(target_rgb_buf,i,prev_y,y);
		prev_y = y;
	}
				
	GDK_THREADS_ENTER();
	gdk_draw_indexed_image(area->window,area->style->white_gc,0,0,WIDTH,HEIGHT,GDK_RGB_DITHER_NONE,target_rgb_buf + BPL + 1,(WIDTH + 2),cmap);
	GDK_THREADS_LEAVE();
	temp_rgb_buf = target_rgb_buf;
	target_rgb_buf = other_rgb_buf;
	other_rgb_buf = temp_rgb_buf;
	return;			
}
