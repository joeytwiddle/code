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

#define mbit char

void main() {

	mbit** thematrix;
	
	mbit* palette = "^+oiumq/\\*0$%@#";
	int paletteSize = strlen(palette);
	
	printf("hello\n");
	srand(time(NULL));
	// Needed
	initscr(); cbreak(); noecho();
	// Optional
	nonl(); intrflush(stdscr,FALSE); keypad(stdscr,FALSE);
	printf("%i x %i\n",COLS,LINES);

	if (has_colors()) {
		start_color();
		init_pair(0,COLOR_BLACK,COLOR_BLACK);
		init_pair(1,COLOR_RED,COLOR_BLACK);
		init_pair(2,COLOR_GREEN,COLOR_BLACK);
		init_pair(3,COLOR_YELLOW,COLOR_BLACK);
		init_pair(4,COLOR_BLUE,COLOR_BLACK);
		init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(6,COLOR_CYAN,COLOR_BLACK);
		init_pair(7,COLOR_WHITE,COLOR_BLACK);
	}

	cls();

	thematrix = new mbit*[COLS];
	for (int x=0;x<COLS;x++) {
		thematrix[x] = new mbit[LINES];
	}

	move(0,0);
	for (int x=0;x<COLS;x++) {
		for (int y=0;y<LINES;y++) {
			thematrix[x][y] = ' ';
			move(y,x);
			addch(' ');
		}
	}

	while (true) {
		
		// doSummat(&lonewriteToPoint);

		int action = rand() % 100;

		if (action < 60) {

			// Alter

			int x = rand() % COLS;
			int y = rand() % LINES;
			
			attrset(COLOR_PAIR(2));
			while ( (rand() % 12) != 0 && y < LINES ) {

				move(y,x);
				// attrset(COLOR_PAIR(7) | A_BOLD);
				// vaddch(y,x,ch);
				addch('%');

				if (y>0) {
					mbit symbol = palette[ rand() % paletteSize ];
					move(y-1,x);
					// attrset(COLOR_PAIR(2));
					// addch('*');
					addch(symbol);
					thematrix[x][y-1] = symbol;
				}

				y++;

			}

			if (y<LINES) {
				move(y,x);
				attrset(COLOR_PAIR(7) | A_BOLD);
				addch('%');
			}

			
		} else if (action < 100) {

			// Slide
			
			int x = rand() % COLS;

			attrset(COLOR_PAIR(2));
			while ( (rand() % 3) != 0 ) {

				for (int y=LINES-1;y>=0;y--) {
					mbit src = ( y>0 ? thematrix[x][y-1] : ' ' );
					thematrix[x][y] = src;
					move(y,x);
					addch(src);
				}

			}

		}

		wrefresh(stdscr);

	}

	cls();

	// Needed
	endwin();

}
