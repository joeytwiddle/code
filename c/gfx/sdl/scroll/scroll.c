#include "../stripped-useful.c"

#define TXTWID (10*7)
#define TXTHEI 23
char text[TXTHEI][TXTWID] =
{
	"  0      0      O      0      0      O      O      0      0     O     ",
	"  0      0      O      0      0      O      O      0      0     O     ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"0   O  00000  0   0  000      0    0      O   O  00000  00000         ",
	"0   O  0      0   0  0  O    O O   0      O   O    0    0             ",
	"0O  0  0      0   0  0   0  0   0  0      O   O    0    0             ",
	"00  0  0      0   0  0  O   0   0  0      O   O    0    0             ",
	"0 O 0  000    0   0  OOO    00000  0       O O     0    000           ",
	"0  00  0      0   0  0  O   0   0  0        O      0    0             ",
	"0  O0  0      0   0  0   0  0   0  0        O      0    0             ",
	"0   0  0      0   0  0   0  0   0  0        O      0    0             ",
	"0   0  00000   000   0   0  0   0  00000    O      0    00000         ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"                                                                      ",
	"  0      0      O      0      0      O      O      0      0     O     ",
	"  0      0      O      0      0      O      O      0      0     O     "
};

#define SCALECONST ((float)SCRWID/32.0/82.0)
#define PADDING 2
#define cr (SCRHEI/(TXTHEI+PADDING*2))

// SDL_Surface *screen;
// int frames=0;

SDL_Rect screenrect;
SDL_Rect dstrect;
int blackPixel;
int whitePixel;

float cen[TXTHEI];
float mag[TXTHEI];
float freq[TXTHEI];
float off[TXTHEI];
float speed[TXTHEI];
float space[TXTHEI];

void setSpeed(int i) {
	float mess=(1.0+cos((float)frames*0.004))/2.0;
	// speed[i]=9.0+2.0*sin(freq[i]*M_PI*frames);
	speed[i]=0.2*SCALECONST*square((cen[i]+mess*mag[i]*sin(off[i]+freq[i]*M_PI*(float)frames))/3.0);
	space[i]=speed[i]*(float)cr/19.0;
}

void init() {
	int i;
	// srand(time(NULL));
	for (i=0;i<TXTHEI;i++) {
		// speed[i]=12+(i%2);
		// speed[i]=7.0+3.0*frand();
		// space[i]=speed[i]*(float)cr/8.0;
		// cen[i]=12.0+0.5*frand();
		// mag[i]=5.0+20.0*sin(M_PI*(float)i/(float)TXTHEI);
		cen[i]=50.0; // +2.0*frand();
		mag[i]=35.0*frand();
		off[i]=2.0*M_PI*frand();
		freq[i]=0.002*frand();
		// freq[i]=0.01*frand();
		// freq[i]=(float)i/(float)TXTHEI/100.0;
		setSpeed(i);
	}
	// dstrect.w=cr/2;
	// dstrect.h=cr/2;
	blackPixel=SDL_MapRGB(screen->format,0,0,0);
	whitePixel=SDL_MapRGB(screen->format,255,60,40);
	screenrect.x=0;
	screenrect.y=0;
	screenrect.w=SCRWID;
	screenrect.h=SCRHEI;
	frames=0;
}

void plotBlob(int x,int y,int w,int h,int c) {
#define RECTSHRINK 2
	dstrect.x=x+RECTSHRINK;
	dstrect.w=(w<RECTSHRINK*2+1?1:w-RECTSHRINK*2);
	dstrect.y=y+RECTSHRINK;
	dstrect.h=h-RECTSHRINK*2;
	// dstrect.x=x;
	// dstrect.x=x+w/4;
	// dstrect.w=(w<4?1:w/2);
	// dstrect.y=y+h/4;
	// dstrect.h=h/2;
	// dstrect.x=x;
	// dstrect.w=w+1;
	// dstrect.y=y;
	// dstrect.h=h;
	SDL_FillRect(screen,&dstrect,c);
}

void doframe() {
	int row;
	SDL_FillRect(screen,&screenrect,blackPixel);
	for (row=0;row<TXTHEI;row++) {
		int cy=(PADDING+row)*cr;
		int leftat=speed[row]*frames-SCRWID/2;
		int i0=leftat/space[row];
		int diff=space[row]-(leftat-i0*space[row]);
		int col;
		// i0--; diff-=space[row];
		// printf("%i %i\n",i0,diff);
		for (col=0;col<2+SCRWID/space[row];col++) {
			int i=(i0+col)%TXTWID;
			// int c=( text[row][i] == ' ' ? blackPixel : whitePixel );
			if (text[row][i]!=' ')
			plotBlob(diff+col*space[row],cy,space[row],cr,whitePixel);
			// plotBlob(diff+col*space[row],cy,cr,cr,whitePixel);
			// printf("%s\n",text[row]);
		}
		setSpeed(row);
		// speed[row]+=0.2*(frand()-0.5);
		// space[row]=speed[row]*(float)cr/8.0;
	}
	plotBlob(SCRWID/2-cr,2*SCRHEI/8-cr,2*cr,2*cr,whitePixel);
	plotBlob(SCRWID/2-cr,6*SCRHEI/8-cr,2*cr,2*cr,whitePixel);
	// plotBlob(SCRWID/2-cr/4,0,cr/2,SCRHEI,whitePixel);
}
