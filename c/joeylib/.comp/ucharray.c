#ifndef ucharray_C
  #define ucharray_C

  #include <ucharray.h>

/* 
   image processing mini library
   
   uchar_array - allocate an unsigned char array
   
   adc 11/94
   
   [rehacked 09/02 by stefan for colour support]
   [and ANSIfied it !!                         ]
   
*/
#include <math.h>
#include <stdio.h>
#include "imagproc.h"

/* -----------------------------------------------*/
unsigned char **uchar_array(int width,int height) {
  
  unsigned char **a;
  int i,j;
  
  if(!(a=(unsigned char **)calloc(height,sizeof(unsigned char *)))) {
    fprintf(stderr,"uchar_array: calloc error 1\n");
    exit(0);
  }
  for(i=0; i<height; i++) {
    if(!(a[i]=(unsigned char *)calloc(width,sizeof(unsigned char)))) {
      fprintf(stderr,"uchar_array: calloc error 2\n");
      exit(0);
    }
  }
  
  return a;
}

/* -----------------------------------------------*/
unsigned char ***uchar_array3D(int width, int height)
/* Allocate 3D array [height][width][3] for colour purposes */ {
  
  unsigned char ***a;
  int x,y;
  
  if(!(a=(unsigned char ***)calloc(height,sizeof(unsigned char *)))) {
    fprintf(stderr,"[uchar_array3D] can't malloc memory (1)\n");
    exit(0);
  }
  for(y=0; y<height; y++) {
    if(!(a[y]=(unsigned char **)calloc(width,sizeof(unsigned char *)))) {
      fprintf(stderr,"[uchar_array3D] can't malloc memory (1)\n"); 
      exit(1);
    }
  }
  
  for(y=0; y<height; y++) 
  for(x=0; x<width; x++) 
  if(!(a[y][x]=(unsigned char *)calloc(3,sizeof(unsigned char)))) {
    fprintf(stderr,"[uchar_array3D] can't malloc memory (1)\n");
    exit(1);
  }
  
  return a;
}

#endif
