/** matrix.c
 *  by joey
**/

/** TODO:
 *  Resizing: Use cached copies of COLS and LINES, and re-allocate array when they change.
 *  Add white things (processes?):
 *    A process can appear at the bottom end of a sliding block, and disappear, randomly.  (ie. sometimes when sliding, the front symbols turns white)
 *    A process can appear (on a non-sliding line?) and write symbols onto the line (1 per frame).
 *    A static process can appear on a sliding or non-sliding line, presumably on the latter changing symbols which pass through it (to/from ' '?)
 *    Processes are white and change symbol once per frame.
 *  Try this to help observation: /usr/lib/xscreensaver/xmatrix -small -density 50 -geometry 10x768
 *  Turn cursor off.
 *  Exit cleanly on keypress.
 *  Putty compatability?
 *  Turn it into a terminal locker.  Proposed method (security audit please!):
 *    Block SIGINT etc calls (for CTRL+C/Z), on keypress ask for password, check with "su - $USER /bin/true" before allowing exit.
**/

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

#define BOTHER_CLOCKING ON

void homeAndWrefresh() {
	move(0,0);
	wrefresh(stdscr);
}

#define wrefresh(x) homeAndWrefresh()

// void writeToPoint(int x,int y) {
	// mvaddch(y,x,'.');
// }

void cls() {
	for (int x=0;x<COLS;x++) {
		for (int y=0;y<LINES;y++) {
			mvaddch(y,x,32);
		}
	}
}

#define mbit char

mbit* palette;
int paletteSize;

int AVALTLET;
int averageLengthOfSlide;
int averageLengthBetweenSlides;
int averageLengthOfAdd;
int averageLengthBetweenAdds;

mbit **thematrix;
bool *sliding;
bool *adding;

void matrix_set(int x,int y,mbit symbol) {
	// debug: catch oob?
	move(y,x);
	addch(symbol);
	thematrix[x][y]=symbol;
}

void slideRow(int x) {

	attrset(COLOR_PAIR(2));

	for (int y=LINES-1;y>0;y--) {
		mbit src = thematrix[x][y-1];
		thematrix[x][y] = src;
		move(y,x);
		/* if (src == '%') {
			if ( (rand() % 6*averageLengthOfSlide) == 0 ) {
				thematrix[x][y] = palette[ rand() % paletteSize ];
			} else {
				attrset(COLOR_PAIR(7) | A_BOLD );
				addch(thematrix[x][y]);
				attrset(COLOR_PAIR(2));
			}
		} else { */
			addch(thematrix[x][y]);
		// }
	}
	thematrix[x][0]=' ';
	move(0,x);
	addch(' ');

}


void main() {

	palette = "^+ouq/\\*}0&$%@#";
	paletteSize = strlen(palette);
	// mbit* altPalette = "%@#";
	// int altPaletteSize = strlen(altPalette);
	
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
	sliding = new bool[COLS];
	adding = new bool[COLS];
	for (int x=0;x<COLS;x++) {
		thematrix[x] = new mbit[LINES];
	}

	move(0,0);
	for (int x=0;x<COLS;x++) {
		sliding[x] = true;
		adding[x] = false;
		for (int y=0;y<LINES;y++) {
			thematrix[x][y] = ' ';
			move(y,x);
			addch(' ');
		}
	}

	// base it on area instead of width
	// averageLengthOfSlide       = ( LINES>30 ? 8 : 6 );
	averageLengthOfSlide       = LINES/2;
	if (averageLengthOfSlide<1)
		averageLengthOfSlide=1;
	// averageLengthBetweenSlides = ( LINES>30 ? 64 : 64 );
	// averageLengthBetweenSlides = averageLengthOfSlide * 6;
	averageLengthBetweenSlides = averageLengthOfSlide*2;
	averageLengthOfAdd = LINES / 2;
	averageLengthBetweenAdds = averageLengthOfAdd * 2;
	// AVALTLET = ( LINES>30 ? 4 : 2 );

#ifdef BOTHER_CLOCKING
	clock_t clocksPerFrame = CLOCKS_PER_SEC/100;
	clock_t lastframe,thisframe;
	lastframe = clock();
#endif

	while (true) {

		int action = rand() % 100;

		/*
		if (action < 100) {

			// Alter: add some new symbols and a white "active" symbol (goes too fast atm)

			int x = rand() % COLS;
			// int y = rand() % (int)((double)LINES * 4);
			int y = 0;
			if (y>=LINES)
				y=0;

			attrset(COLOR_PAIR(2));
			do {

				move(y,x);
				attrset(COLOR_PAIR(7) | A_BOLD);
				addch('%');

				wrefresh(stdscr);

				if (y>0) {
					mbit symbol = palette[ rand() % paletteSize ];
					move(y-1,x);
					attrset(COLOR_PAIR(2));
					addch(symbol);
					thematrix[x][y-1] = symbol;

				}

				wrefresh(stdscr);

				y++;

			} while ( (rand() % AVALTLET) != 0 && y < LINES );

			if ( y<LINES && (rand() % 2)==0 ) {
				move(y,x);
				attrset(COLOR_PAIR(7) | A_BOLD);
				addch('%');
				thematrix[x][y] = '%';
			}

		}
		*/

			// Slide: slide a whole column down n spaces

			for (int x=0;x<COLS;x++) {

				// Consider changing state
				int prob = ( sliding[x] ? averageLengthOfSlide : averageLengthBetweenSlides );
				if ( (rand() % prob) == 0 ) {
					sliding[x] = ! sliding[x];
				}
				int probadd = ( adding[x] ? averageLengthOfAdd : averageLengthBetweenAdds );
				if ( (rand() % probadd) == 0 ) {
					adding[x] = ! adding[x];
				}

				// Act upon state
				if ( sliding[x] ) {
					slideRow(x);
					if (adding[x]) {
						mbit symbol = palette[ rand() % paletteSize ];
						matrix_set(x,0,symbol);
					}
				}

			}

#ifdef BOTHER_CLOCKING
		while (true) {
			thisframe = clock();
			if (thisframe < lastframe+clocksPerFrame) {
				continue;
			} else {
				break;
			}
		}
		lastframe = thisframe;
#endif

		wrefresh(stdscr);

	}

	cls();

	// Needed
	endwin();

}
