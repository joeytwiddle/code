// Perspective projection
// Optimised (not fully)

// You get a box [-1,1]x[-1,1]x[plane-1,plane+1] in the screen.

#define tooclose 0.5

extern int PPhscrwid; // Exists
extern int PPhscrhei; // Exists
extern float PPplane; // Exists
extern float PPindent; // Exists
extern V3d PPlefteye; // Exists
extern V3d PPrighteye; // Exists
extern float PPaspect; // Exists

extern float PPangx; // Variable initialised in .c file
extern float PPangy; // Variable initialised in .c file

// #ifndef PPclipdist
// #define PPclipdist 200000
// #endif
extern float PPclipdist; // Variable initialised in .c file

extern JBmp PPscr; // Exists

extern Viewpoint PPvp; // Exists

void PPsetup(int scrwid,int scrhei,float planedist,float indent,float eyewid); // Method


void PPsetup(int scrwid,int scrhei,float planedist,float indent); // Method


void PPsetup(int scrwid,int scrhei,float planedist); // Method


void PPgetscrpos(V3d far,V3d eye,int *x,int *y); // Method


bool PPgetscrposnoadd(V3d far,V3d eye,int *x,int *y); // Method


bool PPgetscrposnoadd(V3d *far,V3d *eye,int *x,int *y); // Method


bool PPgetscrpos(V3d far,int *x,int *y); // Method


bool PPgetscrposnoadd(V3d far,int *x,int *y); // Method


void PPgetfishscrpos(V3d far,int *x,int *y); // Method


void PPgetflatscrpos(V3d far,int *x,int *y); // Method


void PPgetspherescrpos(V3d far,int *x,int *y); // Method


float PPgetunit(V3d far); // Method


float PPgetunitnoadd(V3d far); // Method


void PPline(JBmp *j,V3d a,V3d b,int c); // Method


void PPplotline(V3d a,V3d b); // Method


void PPline(JBmp *j,V3d eye,V3d a,V3d b,int c); // Method


void PPlinenoadd(JBmp *j,V3d eye,V3d a,V3d b,int c); // Method


void PPlinenoadd(JBmp *j,V3d a,V3d b,int c); // Method


void PPlinenoadd(JBmp *j,V3d eye,V3d a,V3d b,float w,int c); // Method


void PPsphere(JBmp *j,V3d eye,V3d cen,float rad,int c); // Method


void PPspherenoadd(JBmp *j,V3d eye,V3d cen,float rad,int c); // Method


void PPplotsphere(V3d cen,float rad); // Method


V3d PPgetright(); // Method


V3d PPgetleft(); // Method


V3d PPgettop(); // Method


V3d PPgetbottom(); // Method


void PPclear(); // Method


void PPshow(); // Method

