#include <stdio.h>
#include "graphics.h"

static void delay (void);
static void button_press (float x, float y);
static void drawscreen (void);
static void new_button_func (void (*drawscreen_ptr) (void));


int main(int argc, char **argv) {

 int i;

/* initialize display */
 init_graphics("Some Example Graphics");

/* still picture drawing allows user to zoom, etc. */
 init_world (0.,0.,1000.,1000.);
 update_message("Interactive graphics example.");
 event_loop(button_press, drawscreen);   

/* animation section */
 clearscreen();
 update_message("Non-interactive (animation) graphics example.");
 setcolor (RED);
 setlinewidth(0);
 setlinestyle (DASHED);
 init_world (0.,0.,1000.,1000.);
 for (i=0;i<50;i++) {
    drawline (i,10.*i,i+500.,10.*i+10.);
    flushinput();
    delay(); 
 }

/* Draw a still picture again.  I'm also creating one new button. */

 init_world (0.,0.,1000.,1000.);
 update_message("Interactive graphics example number 2!.");
 create_button ("Window", "New Button", new_button_func);
 drawscreen(); 
 event_loop(button_press, drawscreen);

 return (0);
}


static void drawscreen (void) {

int i;
				
/* redrawing routine for still pictures.  Graphics package calls  *
 * this routine to do redrawing after the user changes the window *
 * in any way.                                                    */

 t_point polypts[3] = {{500.,400.},{450.,480.},{550.,480.}};
 t_point polypts2[4] = {{700.,400.},{650.,480.},{750.,480.}, {800.,400.}};
 
 clearscreen();  /* Should be first line of all drawscreens */
 setfontsize (10);
 setlinestyle (SOLID);
 setlinewidth (0);
 setcolor (BLACK);

 drawtext (110.,55.,"colors",150.);
 setcolor (LIGHTGREY);
 fillrect (150.,30.,200.,80.);
 setcolor (DARKGREY);
 fillrect (200.,30.,250.,80.);
 setcolor (WHITE);
 fillrect (250.,30.,300.,80.);
 setcolor (BLACK);
 fillrect (300.,30.,350.,80.);
 setcolor (BLUE);
 fillrect (350.,30.,400.,80.);
 setcolor (GREEN);
 fillrect (400.,30.,450.,80.);
 setcolor (YELLOW);
 fillrect (450.,30.,500.,80.);
 setcolor (CYAN);
 fillrect (500.,30.,550.,80.);
 setcolor (RED);
 fillrect (550.,30.,600.,80.);
 setcolor (DARKGREEN);
 fillrect (600.,30.,650.,80.);
 setcolor (MAGENTA);
 fillrect (650.,30.,700.,80.);

 setcolor (WHITE);
 drawtext (400.,55.,"fillrect",150.);
 setcolor (BLACK);
 drawtext (500.,150.,"drawline",150.);
 setlinestyle (SOLID);
 drawline (440.,120.,440.,200.);
 setlinestyle (DASHED);
 drawline (560.,120.,560.,200.);
 setcolor (BLUE);
 drawtext (190.,300.,"drawarc",150.);
 drawarc (190.,300.,50.,0.,270.);
 drawarc (300.,300.,50.,0.,-180.);
 fillarc (410.,300.,50.,90., -90.);
 fillarc (520.,300.,50.,0.,360.);
 setcolor (BLACK);
 drawtext (520.,300.,"fillarc",150.);
 setcolor (BLUE);
 fillarc (630.,300.,50.,90.,180.);
 fillarc (740.,300.,50.,90.,270.);
 fillarc (850.,300.,50.,90.,30.);
 setcolor (RED);
 fillpoly(polypts,3);
 fillpoly(polypts2,4);
 setcolor (BLACK);
 drawtext (500.,450.,"fillpoly",150.);
 setcolor (DARKGREEN);
 drawtext (500.,610.,"drawrect",150.);
 drawrect (350.,550.,650.,670.); 

 setcolor (BLACK);
 setfontsize (8);
 drawtext (100.,770.,"8 Point Text",800.);
 setfontsize (12);
 drawtext (400.,770.,"12 Point Text",800.);
 setfontsize (15);
 drawtext (700.,770.,"18 Point Text",800.);
 setfontsize (24);
 drawtext (300.,830.,"24 Point Text",800.);
 setfontsize (32);
 drawtext (700.,830.,"32 Point Text",800.);
 setfontsize (10);

 setlinestyle (SOLID);
 drawtext (200.,900.,"Thin line (width 0)",800.);
 setlinewidth (0);
 drawline (100.,920.,300.,920.);
 drawtext (500.,900.,"Width 3 Line",800.);
 setlinewidth (3);
 drawline (400.,920.,600.,920.);
 drawtext (800.,900.,"Width 6 Line",800.);
 setlinewidth (6);
 drawline (700.,920.,900.,920.);

	for (i=0;i<width*height*2;i++) {
				buffer_16bpp[i]=5;
	}

	XPutImage(display,toplevel,gc,ximage,0,0,0,0,width,height);


 
}

static void delay (void) {

/* A simple delay routine for animation. */

 int i, j, k, sum;

 sum = 0;
 for (i=0;i<100;i++) 
    for (j=0;j<i;j++)
       for (k=0;k<30;k++) 
          sum = sum + i+j-k; 
}

static void button_press (float x, float y) {

/* Called whenever event_loop gets a button press in the graphics *
 * area.  Allows the user to do whatever he/she wants with button *
 * clicks.                                                        */

 printf("User clicked a button at coordinates (%f, %f)\n", x, y);
}


static void new_button_func (void (*drawscreen_ptr) (void)) {

 printf ("You pressed the new button!\n");
 setcolor (MAGENTA);
 setfontsize (12);
 drawtext (500., 500., "You pressed the new button!", 10000.);
}
