/** 
 * TODO:
 * - Framebuffer.
 * - Integer maths.
 * - Intercept boot messages and display.
 * - Use time instead of frame count.
 * - Proper antialiasing (current method squishes!)
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
	float wobblemag = 0.06+0.06*sin((float)frames*0.00315);
	float rad = sqrt(X*X+Y*Y) * (1.0+ wobblemag*sin(2.0*ang));
	return sin(rad/spacePerSwirl+ang-swirlang);

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
	char *palette=" ::+O####_";
	
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
		spacePerSwirl = 0.08+0.02*sin(0.001*(float)frames);
		swirlang = (float)frames * M_PI / 800.0;

		move(0,0);

		for (int y = 0;y<LINES;y++) {
			for (int x = 0;x<COLS;x++) {

				float swirlHeight=getSwirlHeight(x,y);
				// float extra=-0.4+0.5*sin(2.493-rad*5.6+(float)frames*0.00184);
				float extra=0;
				{
					int c=
							( getSwirlHeight(x+1,y+1) > 0 ? 1 : 0 )
						+	( getSwirlHeight(x,y+1) > 0 ? 1 : 0 )
						+	( getSwirlHeight(x+1,y) > 0 ? 1 : 0 )
						+	( getSwirlHeight(x,y) > 0 ? 1 : 0 )
						+	( getSwirlHeight((float)x+0.5,(float)y+0.5) > 0 ? 1 : 0 );
					char ch = palette[c];
					attrset(COLOR_PAIR(2) | A_BOLD);
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

		frames += 2;

	}

	cls();

	// Needed
	endwin();
}
