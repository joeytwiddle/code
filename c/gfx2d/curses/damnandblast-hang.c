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

float myrnd() {
	return (float)(rand() % 12345678)/12345678.0;
}

float hang(double *d,double c,double dmp,double r) {
	*d=c+(*d-c)*dmp+r*(myrnd()-0.5)*2.0;
}

void main() {

	char *palette=".-=+\"o%*@#";
	
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
		init_pair(1,COLOR_RED,COLOR_BLACK);
		init_pair(2,COLOR_GREEN,COLOR_BLACK);
		init_pair(3,COLOR_YELLOW,COLOR_BLACK);
		init_pair(4,COLOR_BLUE,COLOR_BLACK);
		init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(6,COLOR_CYAN,COLOR_BLACK);
		init_pair(7,COLOR_WHITE,COLOR_BLACK);
	}

	int frames=0;

	cls();

				double floata=2.0,floatb=1.0,floatc=0.05,floatd=0.0,floate=0.1,
				  floatf=0.1,floatg=1.0,floath=0.01,floati=0.02,floatj=2.0,floatk=2.0,
					floatl=0.1,floatm=0.01,floatn=0.01,floato=0.01,
					floatp=0.3,floatq=0.01,floatr=0.0,floats=0.05,floatt=0.05;

	while (true) {
		
		// doSummat(&lonewriteToPoint);

		move(0,0);

			for (int y=0;y<LINES;y++) {
		for (int x=0;x<COLS;x++) {
				// char ch=rand() % 256;
				// float f=(2.0+1.0*cos((double)y/12.0+(double)frames/27.0))*sin((double)frames*0.1+(double)y/13.0*10.0*sin((double)frames/90.0+(double)x/15.0))+(2.0+2.0*cos((double)y/11.0+(double)frames/17.0))*cos(2+(double)frames*0.015+(double)x*0.14+(double)y/7.0+cos(((double)frames*0.3+y/17.0)/23.0));
				// float f=(2.0+1.0*cos((double)y/12.0+(double)frames/27.0))*sin((double)frames*0.1+(double)y/3.0+10.0*sin((double)frames/90.0+(double)x/15.0))+(2.0+2.0*cos((double)y/11.0+(double)frames/17.0))*cos(2+(double)frames*0.015+(double)x*0.14+(double)y/7.0+cos(((double)frames*0.3+y/17.0)/23.0));

				float f=(floata+floatb*cos((double)y*floatc+(double)frames*floatd))*sin((double)frames*floate+(double)y*floatf+floatg*sin((double)frames*floath+(double)x*floati))+(floatj+floatk*cos((double)y*floatl+(double)frames*floatm))*cos(floatn+(double)frames*floato+(double)x*floatp+(double)y*floatq+cos(((double)frames*floatr+y*floats)*floatt));
				if (f>-0.5) {
					f+=0.5;
					// char ch=32+(rand() % (127-32));
					// int c=(rand() % 7);
					int c=(((int)(f*2.0)) % 7);
					char ch=palette[c];
					attrset(COLOR_PAIR(c) | A_BOLD);
					// vaddch(y,x,ch);
					addch(ch);
				} else
					// mvaddch(y,x,32);
					addch(32);
			}
		}
		
			wrefresh(stdscr);

				hang(&floata,2.0,0.9999999999,0.00001);
				hang(&floatb,1.0,0.9999999999,0.00001);
				hang(&floatc,0.05,0.9999999999,0.00001);
				hang(&floatd,0.0,0.9999999999,0.00001);
				hang(&floate,0.1,0.9999999999,0.00001);
				hang(&floatf,0.1,0.9999999999,0.00001);
				hang(&floatg,0.0,0.9999999999,0.00001);
				hang(&floath,0.01,0.9999999999,0.00001);
				hang(&floati,0.02,0.9999999999,0.00001);
				hang(&floatj,2.0,0.9999999999,0.00001);
				hang(&floatk,2.0,0.9999999999,0.00001);
				hang(&floatl,0.0,0.9999999999,0.00001);
				hang(&floatm,0.01,0.9999999999,0.00001);
				hang(&floatn,0.01,0.9999999999,0.00001);
				hang(&floato,0.01,0.9999999999,0.00001);
				hang(&floatp,0.3,0.9999999999,0.00001);
				hang(&floatq,0.01,0.9999999999,0.00001);
				hang(&floatr,0.0,0.99999999,0.00001);
				hang(&floats,0.05,0.999999999,0.000001);
				hang(&floatt,0.05,0.999999999,0.000001);

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

		frames++;

	}

	cls();

	// Needed
	endwin();
}
