#include <dos.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#define MAXITERATIONS   15
#define SCREENWIDTH     320

  typedef unsigned char byte;
  typedef unsigned short word;
  typedef unsigned long dword;


byte far *scr=(byte far *)MK_FP(0xA000,0);
unsigned int u=319,v=199;
float i,j;
unsigned int k;
unsigned int x,y;
int m;
float zr,zi;
float zr2,zi2;
float cr[628];
float ci[628];
byte *scrb,*scrt;
float jinc,iinc;

void AlgorithmBasic(void);
void AlgorithmBasicQuarter(void);
void AlgorithmGuess2(void);
void AlgorithmGuess4(void);

void main(void)
{
  clock_t startTime,endTime;
  int frames;
  clock_t fastestFrame=CLK_TCK,slowestFrame=0;
  clock_t thisFrameStart,thisFrameEnd;

  asm mov ax,0013h              // Use assembler to switch to mode
  asm int 10h                   // 13h

  for(m=0;m<628;m++) {
	cr[m]=3*(cos(3*(float)m/100))*(sin(2*(float)m/100));
	ci[m]=2*(sin(3*(float)m/100))*(cos(7*(float)m/100));
  }

  startTime=clock();
  frames=0;
  m=0;
  while(!kbhit()) {
	frames++;
	m++;
	//if(m>=628) m=0;
	if(m>=628) break;
	//plot graph
	//scr[(100+(int)(25*ci[m]))*SCREENWIDTH+(230+(int)(25*cr[m]))]=15;

	scrt=scr; scrb=scr+v*SCREENWIDTH+u;
	thisFrameStart=clock();
	AlgorithmBasic();
	thisFrameEnd=clock()-thisFrameStart;
	if(thisFrameEnd<fastestFrame) fastestFrame=thisFrameEnd;
	if(thisFrameEnd>slowestFrame) slowestFrame=thisFrameEnd;
  }
  endTime=clock();
  if(kbhit()) getch();  //clear keyboard stack
  asm mov ax,0003h
  asm int 10h
  cprintf("\n\r%.2f fps\n\r",(float)CLK_TCK*(float)frames/(float)(endTime-startTime));
  cprintf("Fastest frame: %.2fs\n\r",(float)fastestFrame/(float)CLK_TCK);
  cprintf("Slowest frame: %.2fs\n\r",(float)slowestFrame/(float)CLK_TCK);
}

void AlgorithmBasic(void)   //~16.3fps
{
  float cim=ci[m],crm=cr[m];

  j=-1.5;
  jinc=3/(float)v;
  iinc=4/(float)u;
  for(y=0;y<=v/2;y++) {
	i=-2;
	for(x=0;x<=u;x++) {
	  zr=i; zi=j;
	  zr2=i*i; zi2=j*j;
	  k=0;
	  do {
	zi=2*zr*zi+cim;
	zr=zr2-zi2+crm;
	zr2=zr*zr; zi2=zi*zi;
	k++;
	  } while(zr2+zi2<=4 && k<MAXITERATIONS);


	  // Set value of k according to:
	  // 0 < y < v/2 and 0 < x < u


	  *scrt=k; *scrb=k;
	  scrt++; scrb--;
	  i+=iinc;
	}
	scrt+=319-u;
	scrb-=319-u;
	j+=jinc;
  }
}

void AlgorithmBasicQuarter(void)    //~53.3fps
{
  float cim=ci[m],crm=cr[m];

  j=-1.5;
  jinc=6/(float)v;
  iinc=8/(float)u;
  for(y=0;y<=v/2;y+=2) {
	i=-2;
	for(x=0;x<=u;x+=2) {
	  zr=i; zi=j;
	  zr2=i*i; zi2=j*j;
	  k=0;
	  do {
	zi=2*zr*zi+cim;
		zr=zr2-zi2+crm;
		zr2=zr*zr; zi2=zi*zi;
		k++;
	  } while(zr2+zi2<=4 && k<MAXITERATIONS);
	  *scrt=k; *scrb=k;
	  scrt[1]=k; scrt[320]=k; scrt[321]=k;
	  scrb[-1]=k; scrb[-320]=k; scrb[-321]=k;
	  scrt+=2; scrb-=2;
	  i+=iinc;
	}
	scrt+=639-u;
	scrb-=639-u;
	j+=jinc;
  }
}

void AlgorithmGuess2(void)    //~12fps
{
  byte *oscrt,*oscrb;

  j=-1.5;
  jinc=3/(float)v;
  iinc=8/(float)u;
  for(y=0;y<=v/2;y++) {
	i=-2;
	oscrt=scrt; oscrb=scrb;
	for(x=0;x<=u;x+=2) {
	  zr=i; zi=j;
	  zr2=i*i; zi2=j*j;
	   k=0;
	  do {
		zi=2*zr*zi+ci[m];
		zr=zr2-zi2+cr[m];
	zr2=zr*zr; zi2=zi*zi;
	k++;
	  } while(zr2+zi2<=4 && k<MAXITERATIONS);
	  *scrt=k; *scrb=k;
	  scrt+=2; scrb-=2;
	  i+=iinc;
	}
	scrt=oscrt+1; scrb=oscrb-1;
	 
	i=iinc/2-2;
	for(x=1;x<=u;x+=2) {
	  if(scrt[1]==scrt[-1]) {
		*scrt=scrt[1];
		*scrb=scrt[1];
	  }
	  else {
		zr=i; zi=j;
		zr2=i*i; zi2=j*j;
		 k=0;
		do {
		  zi=2*zr*zi+ci[m];
		  zr=zr2-zi2+cr[m];
		  zr2=zr*zr; zi2=zi*zi;
		  k++;
		} while(zr2+zi2<=4 && k<MAXITERATIONS);
		*scrt=k; *scrb=k;
	  }
	  scrt+=2; scrb-=2;
	  i+=iinc;
	}
	scrt+=318-u;
	scrb-=318-u;
	j+=jinc;
  }
}

void AlgorithmGuess4(void)   //~10.7fps
{
  byte *oscrt,*oscrb;
  float iinch;

  j=-1.5;
  jinc=3/(float)v;
  iinc=16/(float)u;
  iinch=iinc/2;
  for(y=0;y<=v/2;y++) {
	i=-2;
	oscrt=scrt; oscrb=scrb;
	for(x=0;x<=u;x+=4) {
	  zr=i; zi=j;
	  zr2=i*i; zi2=j*j;
	  k=0;
	  do {
		zi=2*zr*zi+ci[m];
		zr=zr2-zi2+cr[m];
		zr2=zr*zr; zi2=zi*zi;
		k++;
	  } while(zr2+zi2<=4 && k<MAXITERATIONS);
	  *scrt=k; *scrb=k;
	  scrt+=4; scrb-=4;
	  i+=iinc;
	}

	scrt=oscrt+2; scrb=oscrb-2;
	i=iinch-2;
	for(x=2;x<=u;x+=4) {
	  if(scrt[2]==scrt[-2]) {
		*scrt=scrt[2];
		*scrb=scrt[2];
	  }
	  else {
		zr=i; zi=j;
		zr2=i*i; zi2=j*j;
		k=0;
		do {
		  zi=2*zr*zi+ci[m];
		  zr=zr2-zi2+cr[m];
		  zr2=zr*zr; zi2=zi*zi;
		  k++;
		} while(zr2+zi2<=4 && k<MAXITERATIONS);
		*scrt=k; *scrb=k;
	  }
	  scrt+=4; scrb-=4;
	  i+=iinc;
	}

	scrt=oscrt+1; scrb=oscrb-1;
	i=iinch/2-2;
	for(x=1;x<=u;x+=2) {
	  if(scrt[1]==scrt[-1]) {
		*scrt=scrt[1];
		*scrb=scrt[1];
	  }
	  else {
		zr=i; zi=j;
		zr2=i*i; zi2=j*j;
		k=0;
		do {
		  zi=2*zr*zi+ci[m];
		  zr=zr2-zi2+cr[m];
		  zr2=zr*zr; zi2=zi*zi;
		  k++;
		} while(zr2+zi2<=4 && k<MAXITERATIONS);
		*scrt=k; *scrb=k;
	  }
	  scrt+=2; scrb-=2;
	  i+=iinch;
	}
		
	scrt=oscrt+SCREENWIDTH;
	scrb=oscrb-SCREENWIDTH;
	j+=jinc;
  }
}
