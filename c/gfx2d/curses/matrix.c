/** matrix.c
 *  by joey
**/

/** TODO:
 *  Resizing: Use cached copies of COLS and LINES, and re-allocate array when they change.
 *  Block white things (processes?):
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
 *  Timing: if machine does not approach our optimal FPS, then it will have its own based on how many columns are sliding.  This should be kept constant!  ATM it will vary wrt that #.
**/

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

#define BOTHER_CLOCKING ON
#define WHITE_BITS ON
// For fast machines:
// #define sparsenessLengthOn 1
// #define sparsenessLengthOff 1
// #define sparsenessMovementOn 1
// #define sparsenessMovementOff 1
// For slow machines (eg. 486): less faithful, but animation appears much clearer
#define sparsenessLengthOn 1
#define sparsenessLengthOff 2
#define sparsenessMovementOn 1
#define sparsenessMovementOff 4

// For single/double line spacing:
// #define sparsenessSkipCols 1
#define sparsenessSkipCols 2

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

// returns true on average once every prob calls
bool prob(int prob) {
	return ( ( rand() % prob ) == 0 );
}

template<class Type>
Type max(Type x,Type y) {
	return ( x > y ? x : y );
}

/* // Thou needest this if sparseness is a float
int max(int x,double y) {
	return max(x,(int)y);
}
*/

#define mbit char

mbit* palette = "^~+zovq/\\*kAY}0&$%@#";
int paletteSize = strlen(palette);
mbit BLOCKHEAD_PROCESS    = 'B';
mbit WRITING_PROCESS      = 'W';
mbit STATIC_PROCESS_FULL  = 'S';
mbit STATIC_PROCESS_EMPTY = 'E';

int AVALTLET;
int averageLengthOfSlide;
int averageLengthBetweenSlides;
int averageLengthOfBlock;
int averageLengthBetweenBlocks;
int slidingProcessDies;
int newSlidingProcess;

mbit **thematrix;
bool *sliding;
bool *adding;

void setupProbabilities() {

	// TODO: Base some on area instead of width?
	averageLengthOfSlide       = max(2, LINES * 2/3 / sparsenessMovementOn );
	averageLengthBetweenSlides = max(2, averageLengthOfSlide * 2 * sparsenessMovementOff );
	averageLengthOfBlock       = max(2, LINES * 2/3 / sparsenessLengthOn );
	averageLengthBetweenBlocks = max(2, averageLengthOfBlock * 3/2 * sparsenessLengthOff );
	slidingProcessDies         = max(2, averageLengthOfSlide * 2/3 );
	newSlidingProcess          = max(2, averageLengthOfSlide );

}

mbit randSymbol() {
	return palette[ rand() % paletteSize ];
}

void matrix_set(int x,int y,mbit symbol) {
	// debug: catch oob?
	move(y,x);
	addch(symbol);
	thematrix[x][y] = symbol;
}

void matrix_set_process(int x,int y) {
	// debug: catch oob?
	mbit symbol = randSymbol();
	attrset( COLOR_PAIR(7) | ( prob(2) ? A_BOLD : 0 ) );
	move(y,x);
	addch(symbol);
	thematrix[x][y] = BLOCKHEAD_PROCESS;
	attrset(COLOR_PAIR(2));
}

void slideColumn(int x,bool lastSlide) {

	attrset(COLOR_PAIR(2));

	mbit last = 'L';

	for (int y=LINES-1;y>0;y--) {
		mbit src = thematrix[x][y-1];
#ifdef WHITE_BITS
		if (src == BLOCKHEAD_PROCESS) {
			if (lastSlide || prob(slidingProcessDies)) {
				matrix_set(x,y,randSymbol());
			} else {
				matrix_set_process(x,y);
			}
		} else if (src != ' ' && last == ' ' && !lastSlide && prob(newSlidingProcess)) {
			matrix_set_process(x,y);
		} else {
			if (last == src)
				thematrix[x][y]=src;
			else
				matrix_set(x,y,src);
		}
#else
		matrix_set(x,y,src);
#endif
		last = src;
	}
	thematrix[x][0] = ' ';
	move(0,x);
	addch(' ');

}


void main() {
	
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

	setupProbabilities();

#ifdef BOTHER_CLOCKING
	clock_t clocksPerFrame;
	clock_t lastframe,thisframe;
 	clocksPerFrame = CLOCKS_PER_SEC / ( LINES < 30 ? 30 : 60 );
	lastframe = clock();
#endif

	while (true) {

		// Slide: slide a whole column down n spaces

		for (int x=0;x<COLS;x+=sparsenessSkipCols) {

			// Consider changing state
			int probadd = ( adding[x] ? averageLengthOfBlock : averageLengthBetweenBlocks );
			if ( (rand() % probadd) == 0 ) {
				adding[x] = ! adding[x];
			}

			// Act upon state
			if ( sliding[x] ) {
				if ( prob(averageLengthOfSlide) ) {
					sliding[x] = false;
				}
				slideColumn(x,!sliding[x]);
				if (adding[x]) {
					mbit symbol = palette[ rand() % paletteSize ];
					matrix_set(x,0,symbol);
				}
			} else {
				if ( prob(averageLengthBetweenSlides) ) {
					sliding[x] = true;
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
