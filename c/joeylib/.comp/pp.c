#ifndef pp_C
  #define pp_C

  #include <pp.h>

// Perspective projection
// Optimised (not fully)

// You get a box [-1,1]x[-1,1]x[plane-1,plane+1] in the screen.

#define tooclose 0.5

int PPhscrwid;
int PPhscrhei;
float PPplane;
float PPindent;
V3d PPlefteye;
V3d PPrighteye;
float PPaspect;

float PPangx=45;
float PPangy=30;

JBmp PPscr;

Viewpoint PPvp;

void PPsetup(int scrwid,int scrhei,float planedist,float indent,float eyewid) {
  PPhscrwid=scrwid/2;
	PPhscrhei=scrhei/2;
  PPplane=planedist;
  PPindent=indent;
  PPlefteye=V3d(-eyewid,0,0);
  PPrighteye=V3d(eyewid,0,0);
  PPaspect=(float)PPhscrwid/(float)PPhscrhei;
  #ifdef ALLEGRO
  if (allegrostarted==0) // doesn't change depth if already defined
    allegrosetup(scrwid,scrhei);
  #endif
  PPscr=JBmp(scrwid,scrhei);
}

void PPsetup(int scrwid,int scrhei,float planedist,float indent) {
  PPsetup(scrwid,scrhei,planedist,indent,0.2);
}

void PPsetup(int scrwid,int scrhei,float planedist) {
  PPsetup(scrwid,scrhei,planedist,0);
}

void PPgetscrpos(V3d far,V3d eye,int *x,int *y) {
  // (y-eyey)/(plane-eyez)=(fary-eyey)/(farz-eyez)
  *x=(int)(PPhscrwid+PPhscrhei*(eye.x+PPplane*(far.x-eye.x)/(far.z-eye.z+PPindent)));
  *y=(int)(PPhscrhei*(1-eye.y+PPplane*(far.y-eye.y)/(far.z-eye.z+PPindent)));
}

bool PPgetscrposnoadd(V3d far,V3d eye,int *x,int *y) {
  // (y-eyey)/(plane-eyez)=(fary-eyey)/(farz-eyez)
  if (V3d::normdot(far,V3d::k)>0.2) {
    *x=(int)(PPhscrwid+PPhscrhei*(eye.x+PPplane*(far.x-eye.x)/(far.z-eye.z)));
    *y=(int)(PPhscrhei*(1-eye.y+PPplane*(far.y-eye.y)/(far.z-eye.z)));
    return true;
  } else
  return false;
}

bool PPgetscrposnoadd(V3d *far,V3d *eye,int *x,int *y) {
  // (y-eyey)/(plane-eyez)=(fary-eyey)/(farz-eyez)
  if (V3d::normdot(far,&V3d::k)>0.2) {
    *x=(int)(PPhscrwid+PPhscrhei*(eye->x+PPplane*(far->x-eye->x)/(far->z-eye->z)));
    *y=(int)(PPhscrhei*(1-eye->y+PPplane*(far->y-eye->y)/(far->z-eye->z)));
    return true;
  } else
  return false;
}

bool PPgetscrpos(V3d far,int *x,int *y) { // eye assumed at (0,0,0)
  V3d tmp=far+V3d::k*PPindent;
  // float div=tmp.z; // div>tooclose
  if (V3d::normdot(tmp,V3d::k)>0.2) {
    *x=(int)(PPhscrwid+PPhscrhei*(PPplane*far.x/tmp.z));
    *y=(int)(PPhscrhei*(1-PPplane*far.y/tmp.z));
    return true;
  } else
  return false;
}

bool PPgetscrposnoadd(V3d far,int *x,int *y) { // eye assumed at (0,0,0)
  V3d tmp=far;
  // float div=tmp.z; // div>tooclose
  if (V3d::normdot(tmp,V3d::k)>0.2) {
    *x=(int)(PPhscrwid+PPhscrhei*(PPplane*far.x/tmp.z));
    *y=(int)(PPhscrhei*(1-PPplane*far.y/tmp.z));
    return true;
  } else
  return false;
}

void PPgetfishscrpos(V3d far,int *x,int *y) { // eye assumed at (0,0,0)
  if (far.z>0.1) {
    *x=PPhscrwid+PPhscrwid*asin(far.x/far.mag())/deg2rad(PPangx);
    *y=PPhscrhei+PPhscrhei*asin(far.y/far.mag())/deg2rad(PPangy);
  }
}

void PPgetflatscrpos(V3d far,int *x,int *y) { // eye assumed at (0,0,0)
  if (far.z>0.1) {
    *x=PPhscrwid+PPhscrwid*far.x/far.z/tan(PPangx);
    *y=PPhscrhei+PPhscrhei*far.y/far.z/tan(PPangy);
  }
}

void PPgetspherescrpos(V3d far,int *x,int *y) { // eye assumed at (0,0,0)
  if (far.z>0.1) {
    
    far.normalise();
    *x=PPhscrwid+PPhscrwid*far.x/sin(PPangx);
    *y=PPhscrhei+PPhscrhei*far.y/sin(PPangy);
  }
}

float PPgetunit(V3d far) {
  float div=far.z+PPindent;
  if (div>tooclose)
    return PPhscrhei*PPplane*1.0/div;
  else
  return 0;
}

float PPgetunitnoadd(V3d far) {
  if (far.z>0.01 && V3d::normdot(V3d::k,far)>0.1)
    return PPhscrhei*PPplane*1.0/far.z;
  else
  return 0;
}

void PPline(JBmp *j,V3d a,V3d b,int c) {
  int xa,ya,xb,yb;
  PPgetscrpos(a,&xa,&ya);
  PPgetscrpos(b,&xb,&yb);
  j->line(xa,ya,xb,yb,c);
}

void PPplotline(V3d a,V3d b) {
  PPline(&PPscr,a,b,255);
}

void PPline(JBmp *j,V3d eye,V3d a,V3d b,int c) {
  int xa,ya,xb,yb;
  PPgetscrpos(a,eye,&xa,&ya);
  PPgetscrpos(b,eye,&xb,&yb);
  j->line(xa,ya,xb,yb,c);
}

void PPlinenoadd(JBmp *j,V3d eye,V3d a,V3d b,int c) {
  int xa,ya,xb,yb;
  if (PPgetscrposnoadd(a,eye,&xa,&ya) && PPgetscrposnoadd(b,eye,&xb,&yb))
    j->line(xa,ya,xb,yb,c);
}

void PPlinenoadd(JBmp *j,V3d a,V3d b,int c) {
  int xa,ya,xb,yb;
  if (PPgetscrposnoadd(a,V3d::o,&xa,&ya) && PPgetscrposnoadd(b,V3d::o,&xb,&yb))
    j->line(xa,ya,xb,yb,c);
}

void PPlinenoadd(JBmp *j,V3d eye,V3d a,V3d b,float w,int c) {
  int xa,ya,xb,yb;
  float p=PPgetunitnoadd(a);
  if (PPgetscrposnoadd(a,eye,&xa,&ya) && PPgetscrposnoadd(b,eye,&xb,&yb))
    j->thickline(xa,ya,xb,yb,w*p,c);
}

void PPsphere(JBmp *j,V3d eye,V3d cen,float rad,int c) {
    int x,y;
    float p=PPgetunit(cen);
    if (p) {
      float r=rad*p;
      PPgetscrpos(cen,&x,&y);
      j->filledcircle(x,y,r,c);
    }
}

void PPspherenoadd(JBmp *j,V3d eye,V3d cen,float rad,int c) {
    int x,y;
    float p=PPgetunitnoadd(PPvp.bring(cen));
    if (p) {
      float r=rad*p;
      PPgetscrposnoadd(PPvp.bring(cen),&x,&y);
      j->filledcircle(x,y,r,c);
    }
}

void PPplotsphere(V3d cen,float rad) {
  PPsphere(&PPscr,V3d::o,cen,rad,255);
}

V3d PPgetright() {
  // x=PPhscrwid+PPhscrhei*(PPplane*far.x/(far.z+PPindent));
  // far.z=1
  // PPplane*far.x/(1+PPindent)=PPhscrwid/PPhscrhei
  // far.x=(1+PPindent)/PPplane*PPhscrwid/PPhscrhei
  return V3d((1+PPindent)/PPplane*PPaspect,0,1);
}

V3d PPgetleft() {
  return V3d(-(1+PPindent)/PPplane*PPaspect,0,1);
}

V3d PPgettop() {
  return V3d(0,(1+PPindent)/PPplane,1);
}

V3d PPgetbottom() {
  return V3d(0,-(1+PPindent)/PPplane,1);
}

void PPclear() {
  PPscr.clear();
}

void PPshow() {
  PPscr.writetoscreen();
}

#endif
