/** 
 * TODO:
 * - Framebuffer.
 * - Integer maths, and other optimisation.
 * - Intercept boot messages and display.
 * - Use time instead of frame count.
 * - Could implement more efficient anti-aliasing:
 *   eg. - retain neighbouring heights
 *    or - work out distance to spiral edge (ie. treat is as a 2D shape, not a thresholded heightmap)
**/

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <curses.h>
	
void writeToPoint(int x,int y) {
	mvaddch(y,x,'.');
}

void doSummat(void writeFn(int,int)) {
	// ...
	writeFn(12,16);
	// ...
}


int frames;
int cenx;
int ceny;
float spacePerSwirl;
float swirlang;

float getSwirlHeight(float x,float y) {

	float X = ((float)x-(float)cenx)/(float)COLS*1.3;
	float Y = ((float)y-(float)ceny)/(float)LINES;
	float ang = atan2(Y,X)-swirlang;
	if (y<0)
		ang=ang+M_PI;
	// Squishing otherwise circular spiral
	float wobblemag = 0.07+0.07*sin((float)frames*0.00115);
	float rad = sqrt(X*X+Y*Y) * (1.0+ wobblemag*cos(2.0*(M_PI/4.0*cos((float)frames*0.000243)+ang+swirlang)));
	// Changing width of spiral
	float extra=-0.6+0.35*sin(1.493-rad*4.5+(float)frames*0.00084);
	return sin(rad/spacePerSwirl+ang-swirlang)+extra;

}

void cls() {
	for (int x=0;x<COLS;x++) {
		for (int y=0;y<LINES;y++) {
			mvaddch(y,x,32);
		}
	}
}

void main() {

	#define PALSIZE 8
	char *palette=" -=*O@@@@_";
	
	printf("Hello\n");
	srand(time(NULL));
	// Needed
	initscr(); cbreak(); noecho();
	// Optional
	nonl(); intrflush(stdscr,FALSE); keypad(stdscr,FALSE);
	printf("%i x %i\n",COLS,LINES);

	if (has_colors()) {
		start_color();
		init_pair(0,COLOR_BLACK,COLOR_BLACK);
		init_pair(1,COLOR_BLUE,COLOR_BLACK);
		init_pair(2,COLOR_RED,COLOR_BLACK);
		init_pair(3,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(4,COLOR_GREEN,COLOR_BLACK);
		init_pair(5,COLOR_CYAN,COLOR_BLACK);
		init_pair(6,COLOR_YELLOW,COLOR_BLACK);
		init_pair(7,COLOR_WHITE,COLOR_BLACK);
	}

	frames=0;

	cls();

	while (true) {
		
		// doSummat(&lonewriteToPoint);

		cenx = COLS / 2;
		ceny = LINES / 2;
		swirlang = (float)frames * M_PI / 800.0;
		// Zooming in and out over time
		spacePerSwirl = 0.08+0.03*cos(0.000638*(float)frames);

		move(0,0);
		attrset(COLOR_PAIR(2) | A_BOLD);

		for (int y = 0;y<LINES;y++) {
			for (int x = 0;x<COLS;x++) {

				float swirlHeight=getSwirlHeight(x,y);
				{
					int c=
							( getSwirlHeight(x+1,y+1) > 0 ? 1 : 0 )
						+	( getSwirlHeight(x,y+1) > 0 ? 1 : 0 )
						+	( getSwirlHeight(x+1,y) > 0 ? 1 : 0 )
						+	( getSwirlHeight(x,y) > 0 ? 1 : 0 )
						+	( getSwirlHeight((float)x+0.5,(float)y+0.5) > 0 ? 1 : 0 );
					char ch = palette[c];
					// mvaddch(y,x,32);
					addch(ch);
				}

			}
		}

		// int x = frames%COLS;
		// int y = (frames/COLS)%LINES;
		// move(y,x);
		// attrset(COLOR_PAIR(7));
		// char *str;
		// sprintf(str," (- %i -) ",frames);
		// addstr(str);
		// printf("%i ",frames);
		// move(1,0);
		// attrset(COLOR_PAIR(0));
		// printf("%i ",frames);
		wrefresh(stdscr);

		frames += 10;

	}

	cls();

	// Needed
	endwin();
}
