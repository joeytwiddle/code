#ifndef quad_H
  #define quad_H

// A way of hierarchical interpolation, ineficcient if eval is not memo-ed

void quadify(int l,int t,int r,int b,float (*eval)(int,int),void (*fulfil)(int,int,int,int,float,float,float,float),float tol,int res); // Method


//

extern JBmp *Quadjbmp; // Exists
extern Map2d<int> *Quadmap; // Exists
extern int (*Quadfn)(int,int); // Exists

float Quadgetpos(int i,int j); // Method


void Quadfulfil(int l,int t,int r,int b,float nw,float ne,float sw,float se); // Method


void quadify(JBmp *j,int (*colourat)(int,int),int tol,int res); // Method


void quadify(JBmp *j,int (*colourat)(int,int),int tol,int res,int size); // Method


#endif
