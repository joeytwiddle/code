/** 
 * TODO:
 * - Framebuffer.
 * - Integer maths.
 * - Intercept boot messages and display.
 * - Use time instead of frame count.
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

void cls() {
	for (int x=0;x<COLS;x++) {
		for (int y=0;y<LINES;y++) {
			mvaddch(y,x,32);
		}
	}
}
void main() {

	#define PALSIZE 8
	char *palette=" ::+O###_";
	
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

	int frames=0;

	cls();

	while (true) {
		
		// doSummat(&lonewriteToPoint);

		int cenx = COLS / 2;
		int ceny = LINES / 2;
		float spacePerSwirl = 0.08;
		float swirlang = (float)frames * M_PI / 800.0;

		move(0,0);

		for (int y = 0;y<LINES;y++) {
			for (int x = 0;x<COLS;x++) {

				float X = ((float)x-(float)cenx)/(float)COLS*1.3;
				float Y = ((float)y-(float)ceny)/(float)LINES;
				float ang = atan2(Y,X)-swirlang;
				if (y<0)
					ang=ang+M_PI;
				float rad = sqrt(X*X+Y*Y) + 0.02*sin(2.0*ang);
				float swirlHeight = sin(rad/spacePerSwirl+ang-swirlang);

				float extra=-0.4+0.5*sin(2.493-rad*5.6+(float)frames*0.0026);
				if ( swirlHeight > - extra ) {
				// if ( swirlHeight > - 0.7 ) {
					int c = (((int)((swirlHeight+extra)/(1.0+extra)*(float)PALSIZE)) % PALSIZE);
					char ch = palette[c];
					c=1;
					attrset(COLOR_PAIR(c+1) | A_BOLD);
					// vaddch(y,x,ch);
					addch(ch);
				} else {
					// mvaddch(y,x,32);
					addch(32);
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

		frames += 2;

	}

	cls();

	// Needed
	endwin();
}
