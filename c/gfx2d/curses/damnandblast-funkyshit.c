/** damnandblast.c
 *  by joey
 *  No interpolation!
**/

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

	char *palette=".\"=+o%#";
	
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

		move(0,0);

			for (int y=0;y<LINES;y++) {
		for (int x=0;x<COLS;x++) {
				// x=x*2; y=y*2;
				// char ch=rand() % 256;
				float f=(float)frames;
				float X=(float)x+20.0*(sin((float)y+f/30.0));
				float Y=(float)y;
				float z=(2.0+2.0*cos(Y/12.0+3.0*sin(f*0.0039)))*sin(10.0*sin(4.0*sin(f/300.0)+X/25.0))+(2.0+2.0*cos(Y/11.0+0.012*sin(f/17.0)))*cos(2+X*0.14+Y/7.0+4.0*cos((f*0.29+y/15.0)/23.0));
				// x=x/2; y=y/2;
				if (z>-0.5) {
					z+=0.5;
					if (z>6.0)
						z=6.0;
					// char ch=32+(rand() % (127-32));
					// int c=(rand() % 7);
					int c=(((int)(z*1.0)) % 7);
					char ch=palette[c];
					attrset(COLOR_PAIR(c+1) | A_BOLD);
					// vaddch(y,x,ch);
					addch(ch);
				} else
					// mvaddch(y,x,32);
					addch(32);
			}
		}

		// int x=frames%COLS;
		// int y=(frames/COLS)%LINES;
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

		frames+=2;

	}

	cls();

	// Needed
	endwin();
}
