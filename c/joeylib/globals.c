// Which operating system are we under?

#include <whichos.c>
#define LINUX
// #define DOS
// #define UNIX
// #define CYGWIN

#ifdef CYGWIN
	// // This is a global variable.
	/*special c*/
	struct _reent *_impure_ptr;
	/*end special*/
	// extern struct _reent *__imp_reent_data;
	// int entry (HINSTANT hinst, DWORD reason, LPVOID reserved) {
		// _impure_ptr = __imp_reent_data;
	// }
#endif

// For SG machines, without time and difftime functions:
// #define STUPIDUNIX

// Anything special for this particular compile?
// Don't use the graphics library
// #define NOGRAPHICS
// Output debugging info
// #define DEBUG
// Don't print usual information about what's happening
// #define QUIET
// Loads a global JFont object
// #define USINGJFONT
// Force segmentation fault when error occurs - program should stop running
// #define FORCESEGFAULT
// My curses library (probably not working anyway)
// #define USINGCURSES

// Are we using the Allegro library for DJGPP (graphics under DOS)?
#ifdef DOS
 // #define ALLEGRO
#endif

#ifdef UNIX
  #define POSIX
#endif
#ifdef LINUX
  #define POSIX
#endif

// I needed these for porting - maybe still do!
// #ifdef LINUX
  // #define static const static
// #endif
// These are for class constants (of the same type as the class)
// #ifdef UNIX
  // #define fillin 
// #else
  // #define fillin static
// #endif
#define DOFILLIN
#define fillin static
#define fillup

#define uchar unsigned char
#define bool uchar
#define true (bool)1
#define false (bool)0
#define boolean bool
#define destroy(i) if (i!=NULL) { (i)->freedom(); free(i); i=NULL; }
#define destroystruct(i) if (i!=NULL) { (i)->freestruct(); free(i); i=NULL; }

#ifdef DOS
  // #include <dir.h>
#endif

#ifdef ALLEGRO
  #include <allegro.h>
#endif
